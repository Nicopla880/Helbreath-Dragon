#include "combat.h"
#include "..\HG.h"
#include "..\map\map.h"
#include "skill.h"
#include "../DelayedEvents.h"

extern class CGame *   g_game;
extern class CClient ** g_clientList;
extern class CMap	**	g_mapList;
extern class CNpc **	g_npcList;


extern char g_cTxt[512];

extern char _tmp_cTmpDirX[9];
extern char _tmp_cTmpDirY[9];


extern int * g_skillSSNpoint;

bool CheckResistingMagicSuccess(char cAttackerDir, Unit *target, int iHitRatio)
{
	double dTmp1, dTmp2, dTmp3;
	int    iTarGetMagicResistRatio, iDestHitRatio, iResult;		
	if(!target) return false;
	if(target->IsPlayer()) {
		CClient *client = (CClient*) target;
		if (client->IsInvincible()) return true;
		if (client->AfflictedBy(STATUS_REDSLATE)) return TRUE;
	}
	iTarGetMagicResistRatio = target->GetMagicResistRatio();
	if (target->HasAbsoluteMagicProtection()) return TRUE;

	if (iHitRatio < 1000 && target->HasProtectionFromMagic()) return TRUE;
	if (iHitRatio >= 10000) iHitRatio -= 10000;

	if (iTarGetMagicResistRatio < 1) iTarGetMagicResistRatio = 1;

	dTmp1 = (double)(iHitRatio);
	dTmp2 = (double)(iTarGetMagicResistRatio);

	dTmp3 = (dTmp1 / dTmp2) * 50.0f;
	iDestHitRatio = (int)(dTmp3); 

	if (iDestHitRatio < MINIMUMHITRATIO) iDestHitRatio = MINIMUMHITRATIO;
	if (iDestHitRatio > MAXIMUMHITRATIO) iDestHitRatio = MAXIMUMHITRATIO;

	if (iDestHitRatio >= 100) return FALSE;

	iResult = dice(1, 100);
	if (iResult <= iDestHitRatio) return FALSE;

	// Resisting Magic
	target->SkillUp(SKILL_MAGICRES,1);

	return TRUE;
}

bool checkResistingPoisonSuccess(Unit *target)
{
	int iResist, iResult;
	if(!target) return false;
	iResist = target->GetPoisonResistRatio();
	
	iResult = dice(1, 100);
	if (iResult >= iResist) 		return FALSE;

	// resisting poison
	target->SkillUp(SKILL_POISONRES, 1);

	return TRUE;
}

bool checkResistingIceSuccess(char cAttackerDir, Unit *target, int iHitRatio)
{

	int    iTargetIceResistRatio, iResult;
	if(!target) return false;
	iTargetIceResistRatio = target->GetFrostResistRatio();

	if (iTargetIceResistRatio < 1) iTargetIceResistRatio = 1;

	iResult = dice(1, 100);
	if (iResult <= iTargetIceResistRatio) return true;

	return false;
}

int calculateAttackEffect(Unit *_target, Unit *_attacker, int tdX, int tdY, int iAttackMode, bool bNearAttack, bool bIsDash)
{
	int    iAP_SM, iAP_L, iAttackerHitRatio, iTargetDefenseRatio, iDestHitRatio, iResult, iAP_Abs_Armor, iAP_Abs_Shield;
	char   cAttackerName[21], cAttackerDir, cAttackerSide, cTargetDir, cProtect;
	short  sWeaponIndex, sAttackerWeapon, dX, dY, sX, sY, sAtkX, sAtkY, sTgtX, sTgtY;
	DWORD  dwTime = timeGetTime();
	WORD   wWeaponType;
	double dTmp1, dTmp2, dTmp3;
	bool   bKilled = FALSE;
	bool   bNormalMissileAttack = FALSE;
	bool   bIsAttackerBerserk;
	bool   bIsAttackerRage; // Revan Rage 16/05/2016
	int    iKilledDice, iDamage, iExp, iWepLifeOff, iSideCondition, iMaxSuperAttack, iWeaponSkill, iComboBonus, iTemp;
	int    iAttackerHP, iMoveDamage;
	char   cAttackerSA;
	int    iAttackerSAvalue, iHitPoint;
	char   cDamageMoveDir;
	int    iPartyID, iConstructionPoint, iWarContribution, tX, tY, iDst1, iDst2;
	int	iSkillLV,iCropLimit;

	iExp = 0;
	iPartyID = 0;

	ZeroMemory(cAttackerName, sizeof(cAttackerName));
	cAttackerSA      = NULL;
	iAttackerSAvalue = NULL;
	wWeaponType      = NULL;
	
	if(_attacker->IsPlayer()) {
		CClient *client_attacker = (CClient*) _attacker;
		if (g_mapList[ client_attacker->GetMapID() ]->m_bIsAttackEnabled == FALSE) return 0;


		if (client_attacker->IsInvisible()) {
			client_attacker->RemoveInvisibility();
		}

		if ((client_attacker->m_sAppr2 & 0xF000) == 0) return 0;

		iAP_SM = 0;
		iAP_L  = 0;


		wWeaponType = ((client_attacker->m_sAppr2 & 0x0FF0) >> 4);


		cAttackerSide = client_attacker->GetFaction();

		if (wWeaponType == 0) {
			iAP_SM = iAP_L    = dice(1, (client_attacker->GetStr() / 12));
			if (iAP_SM <= 0) iAP_SM = 1;
			if (iAP_L  <= 0) iAP_L  = 1;
			iAttackerHitRatio = client_attacker->m_iHitRatio + client_attacker->m_cSkillMastery[SKILL_HANDATTACK];

			client_attacker->m_sUsingWeaponSkill = SKILL_HANDATTACK;

		}
		else if ((wWeaponType >= 1) && (wWeaponType < 40)) {

			iAP_SM = dice(client_attacker->m_cAttackDiceThrow_SM, client_attacker->m_cAttackDiceRange_SM);
			iAP_L  = dice(client_attacker->m_cAttackDiceThrow_L, client_attacker->m_cAttackDiceRange_L);


			iAP_SM += client_attacker->m_cAttackBonus_SM;
			iAP_L  += client_attacker->m_cAttackBonus_L;

			iAttackerHitRatio = client_attacker->m_iHitRatio;


			dTmp1 = (double)iAP_SM;
			if (client_attacker->GetStr() <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double)client_attacker->GetStr();

			dTmp2 = dTmp2 / 5.0f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iAP_SM = (int)(dTmp3 +0.5f);

			dTmp1 = (double)iAP_L;
			if (client_attacker->GetStr() <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double)client_attacker->GetStr();

			dTmp2 = dTmp2 / 5.0f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iAP_L = (int)(dTmp3 +0.5f);
			//
		}
		else if (wWeaponType >= 40) {
			iAP_SM = dice(client_attacker->m_cAttackDiceThrow_SM, client_attacker->m_cAttackDiceRange_SM);
			iAP_L  = dice(client_attacker->m_cAttackDiceThrow_L, client_attacker->m_cAttackDiceRange_L);


			iAP_SM += client_attacker->m_cAttackBonus_SM;
			iAP_L  += client_attacker->m_cAttackBonus_L;

			iAttackerHitRatio = client_attacker->m_iHitRatio;
			bNormalMissileAttack = TRUE;


			iAP_SM += dice(1, (client_attacker->GetStr() / 20));
			iAP_L  += dice(1, (client_attacker->GetStr() / 20));
			//
		}


		if (client_attacker->m_iCustomItemValue_Attack != 0) {

			if ((client_attacker->m_iMinAP_SM != 0) && (iAP_SM < client_attacker->m_iMinAP_SM)) {
				iAP_SM = client_attacker->m_iMinAP_SM;
			}
			if ((client_attacker->m_iMinAP_L != 0) && (iAP_L < client_attacker->m_iMinAP_L)) {
				iAP_L = client_attacker->m_iMinAP_L;
			}

			if ((client_attacker->m_iMaxAP_SM != 0) && (iAP_SM > client_attacker->m_iMaxAP_SM)) {
				iAP_SM = client_attacker->m_iMaxAP_SM;
			}
			if ((client_attacker->m_iMaxAP_L != 0) && (iAP_L > client_attacker->m_iMaxAP_L)) {
				iAP_L = client_attacker->m_iMaxAP_L;
			}
		}

		if (client_attacker->m_heroArmourBonus == 1) {
			iAttackerHitRatio += 100;
			iAP_SM += 5;
			iAP_L += 5;
		}

		cAttackerDir = client_attacker->GetFacingDirection();
		strcpy(cAttackerName, client_attacker->m_cCharName);

		bIsAttackerBerserk = client_attacker->IsBerserked();
		bIsAttackerRage = client_attacker->IsRaged(); // Revan Rage 16/05/2016

		if ((client_attacker->m_iSuperAttackLeft > 0) && (iAttackMode >= 20)) {

			dTmp1 = (double)iAP_SM;
			dTmp2 = (double)client_attacker->m_iLevel;
			dTmp3 = dTmp2 / 100.0f;
			dTmp2 = dTmp1 * dTmp3;
			iTemp = (int)(dTmp2 +0.5f);
			iAP_SM += iTemp;

			dTmp1 = (double)iAP_L;
			dTmp2 = (double)client_attacker->m_iLevel;
			dTmp3 = dTmp2 / 100.0f;
			dTmp2 = dTmp1 * dTmp3;
			iTemp = (int)(dTmp2 +0.5f);
			iAP_L += iTemp;

			switch (client_attacker->m_sUsingWeaponSkill) 
			{
			case SKILL_ARCHERY:
				iAP_SM += (iAP_SM/10); iAP_L += (iAP_L/10);
				iAttackerHitRatio  += 30;
				break;
			case SKILL_SHORTSWORD:
				iAP_SM *= 2; iAP_L *= 2;
				break;
			case SKILL_LONGSWORD:
				iAP_SM += (iAP_SM/10); iAP_L += (iAP_L/10);
				iAttackerHitRatio += 30;
				break;
			case SKILL_AXE:
				iAP_SM += (iAP_SM/5); iAP_L += (iAP_L/5);
				break;
			case SKILL_HAMMER:
				iAP_SM += (iAP_SM/5); iAP_L += (iAP_L/5);
				iAttackerHitRatio += 20;
				break;   
			case SKILL_STAFF:  
				iAP_SM += (iAP_SM/5); iAP_L += (iAP_L/5);
				iAttackerHitRatio +=  50;
				break;
			}

			iAttackerHitRatio += 100;
			iAttackerHitRatio += client_attacker->m_iCustomItemValue_Attack;
		}


		if (bIsDash == TRUE) {

			iAttackerHitRatio += 20;

			switch (client_attacker->m_sUsingWeaponSkill) {
			case SKILL_LONGSWORD:  
				iAP_SM += (iAP_SM/10); iAP_L += (iAP_L/10); 
				break;
			case SKILL_AXE: 
				iAP_SM += (iAP_SM/5); iAP_L += (iAP_L/5);
				break;
			case SKILL_HAMMER:
				iAP_SM += (iAP_SM/5); iAP_L += (iAP_L/5);
				break;
			}
		}

		iAttackerHP = client_attacker->GetHP();

		iAttackerHitRatio += client_attacker->m_iAddAR;

		sAtkX = client_attacker->m_sX;
		sAtkY = client_attacker->m_sY;

		iPartyID = client_attacker->m_iPartyID;

	}
	if(_attacker->IsNPC()) {
		CNpc *npc_attacker = (CNpc*) _attacker;
		if (g_mapList[ npc_attacker->m_cMapIndex ]->m_bIsAttackEnabled == FALSE) return 0;

		if (npc_attacker->IsInvisible()) {
			npc_attacker->RemoveInvisibility();
		}


		cAttackerSide = npc_attacker->GetFaction();

		iAP_SM = 0;
		iAP_L  = 0;

		// Crusade
		if (npc_attacker->m_cAttackDiceThrow != 0) 
			iAP_L = iAP_SM = dice(npc_attacker->m_cAttackDiceThrow, npc_attacker->m_cAttackDiceRange);

		iAttackerHitRatio = npc_attacker->m_iHitRatio;

		cAttackerDir = npc_attacker->m_cDir;
		memcpy(cAttackerName, npc_attacker->m_cNpcName, 20);

		if (npc_attacker->IsBerserked())
			bIsAttackerBerserk = TRUE;
		else bIsAttackerBerserk = FALSE;


		iAttackerHP = npc_attacker->m_iHP;


		cAttackerSA = npc_attacker->m_cSpecialAbility;

		sAtkX = npc_attacker->m_sX;
		sAtkY = npc_attacker->m_sY;
	}

	if(_target->IsPlayer()) {
		CClient *client_target = (CClient*) _target;
		if (client_target->m_bIsKilled == TRUE) return 0;

#ifdef SAMESIDETOWNPROTECT

		if ((target->m_iPKCount == 0) && (attacker->IsPlayer()) && (target->GetFaction() == cAttackerSide) && (target->m_iIsOnTown == NONPK)) return 0;
#endif 
#ifdef BEGINNERTOWNPROTECT
		if ((_attacker->IsPlayer()) && (client_target->m_bIsNeutral == TRUE)
			&& (client_target->m_iPKCount == 0) && (client_target->m_iIsOnTown == NONPK)) return 0;
#endif

		if ((client_target->m_sX != tdX) || (client_target->m_sY != tdY)) return 0;

		if (client_target->IsInvincible()) return 0;

		if (_attacker->IsPlayer()) {
			CClient *client_attacker = (CClient*) _attacker;
			if((client_attacker->IsNeutral() == TRUE) && (client_target->m_iPKCount == 0)) return 0;
		}

		if ((client_target->m_iPartyID != NULL) && (iPartyID == client_target->m_iPartyID)) return 0;

		if ((client_target->m_iStatus & STATUS_REDSLATE) != 0) return 0;

		cTargetDir = client_target->m_cDir;
		iTargetDefenseRatio = client_target->m_iDefenseRatio;

		if ((_attacker->IsPlayer()) && (((CClient*)_attacker)->m_bIsSafeAttackMode == TRUE)) {
			iSideCondition = ((CClient*)_attacker)->GetPlayerRelationship(client_target->m_handle);
			if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6)) {

				iAP_SM = iAP_SM / 2;
				iAP_L  = iAP_L / 2;
			}
			else {

				if (g_mapList[((CClient*)_attacker)->m_cMapIndex]->m_bIsFightZone == TRUE) {
					if (((CClient*)_attacker)->m_iGuildGUID == _target->m_iGuildGUID) return 0;
					else {

						iAP_SM = iAP_SM / 2;
						iAP_L  = iAP_L / 2;
					}
				}
				else return 0;
			}
		}

		iTargetDefenseRatio += client_target->m_iAddDR;

		sTgtX = _target->m_sX;
		sTgtY = _target->m_sY;
	}
	if(_target->IsNPC()) {
		CNpc *npc_target = (CNpc*) _target;
		if (npc_target->m_iHP <= 0) return 0;

		if ((npc_target->m_sX != tdX) || (npc_target->m_sY != tdY)) return 0;

		cTargetDir = npc_target->m_cDir;
		iTargetDefenseRatio = npc_target->m_iDefenseRatio;


		if (_attacker->IsPlayer()) {
			CClient *client_attacker = (CClient*) _attacker;
			switch (npc_target->m_sType) 
			{
			case NPC_ESG:
			case NPC_GMG:
				if ((_attacker->m_side == NEUTRAL) || (_target->m_side == _attacker->m_side)) return 0;
				break;
			}

			if ((wWeaponType == 25) && (npc_target->m_cActionLimit == 5) && (npc_target->m_iBuildCount > 0)) {

				if (client_attacker->m_iCrusadeDuty != 2 && !g_game->m_astoria.get() && (client_attacker->m_iAdminUserLevel == 0)) {
					goto TAG_41025;
				}

				switch (_target->m_sType) {
				case NPC_AGT:
				case NPC_CGT:
				case NPC_MS:
				case NPC_DT:
					if(client_attacker->IsGM()) npc_target->m_iBuildCount = 1;
					switch (npc_target->m_iBuildCount) {
					case 1: 		
						npc_target->m_sAppr2 = 0;
						g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTNULLACTION, NULL, NULL, NULL);

						if(g_game->m_bIsCrusadeMode)
						{
							switch (_target->m_sType) 
							{
							case NPC_AGT: iConstructionPoint = 700; iWarContribution = 700; break;
							case NPC_CGT: iConstructionPoint = 700; iWarContribution = 700; break;
							case NPC_MS: iConstructionPoint = 500; iWarContribution = 500; break;		
							case NPC_DT: iConstructionPoint = 500; iWarContribution = 500; break;
							}

							client_attacker->m_iWarContribution   += iWarContribution;
							if (client_attacker->m_iWarContribution > MAXWARCONTRIBUTION)
								client_attacker->m_iWarContribution = MAXWARCONTRIBUTION;

							g_game->SendNotifyMsg(NULL, _attacker->m_handle, NOTIFY_CONSTRUCTIONPOINT, client_attacker->m_iConstructionPoint, client_attacker->m_iWarContribution, NULL, NULL);
						}
						break;
					case 5: 		
						npc_target->m_sAppr2 = 1;
						g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTNULLACTION, NULL, NULL, NULL);
						break;
					case 10: 
						npc_target->m_sAppr2 = 2;
						g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTNULLACTION, NULL, NULL, NULL);
						break;
					}
					break;
				}

				npc_target->m_iBuildCount--;
				if (npc_target->m_iBuildCount <= 0) {
					npc_target->m_iBuildCount = 0;
				}
				return 0;
			}


			if ((wWeaponType == 27) && (npc_target->m_iNpcCrops != 0) && (npc_target->m_cActionLimit == 5) && (npc_target->m_iBuildCount > 0)) {

				iSkillLV = client_attacker->m_cSkillMastery[SKILL_FARMING];
				iCropLimit = npc_target->m_iCropsSkillLV;

				if(20 > iSkillLV) return 0;  

				if(client_attacker->m_iLevel <  20 ) return 0 ; 


				switch (npc_target->m_sType) {
				case NPC_CROPS:
					switch (npc_target->m_iBuildCount) {
					case 1: 
						npc_target->m_sAppr2 = npc_target->m_iNpcCrops << 8 | 3;
						//						target->m_sAppr2 = (char)0;

						if(iSkillLV <= iCropLimit + 10)
							_attacker->SkillUp(SKILL_FARMING, 1);

						g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTNULLACTION, NULL, NULL, NULL);


						if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						{
							g_game->bCropsItemDrop(_attacker->m_handle,_target->m_handle,TRUE);
						}
						g_game->DeleteNpc(_target->m_handle);

						break;
					case 8: 		
						npc_target->m_sAppr2 = npc_target->m_iNpcCrops << 8 | 3;
						//						target->m_sAppr2 = (char)3;

						if(iSkillLV <= iCropLimit + 10)
							_attacker->SkillUp(SKILL_FARMING, 1);

						g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTNULLACTION, NULL, NULL, NULL);


						if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						{
							if(iSkillLV <= iCropLimit + 10)
								_attacker->SkillUp(SKILL_FARMING, 1);
							g_game->bCropsItemDrop(_attacker->m_handle,_target->m_handle);
						}
						break;

					case 18: 
						npc_target->m_sAppr2 = npc_target->m_iNpcCrops << 8 | 2;
						//						target->m_sAppr2 = (char)2;

						if(iSkillLV <= iCropLimit + 10)
							_attacker->SkillUp(SKILL_FARMING, 1);

						g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTNULLACTION, NULL, NULL, NULL);


						if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						{
							if(iSkillLV <= iCropLimit + 10)
								_attacker->SkillUp(SKILL_FARMING, 1);
							g_game->bCropsItemDrop(_attacker->m_handle,_target->m_handle);
						}
						break;
					}
					break;
				}

				if(_target == NULL)
				{
					return 0;
				}

				if(probabilityTable(iSkillLV,iCropLimit,1) > 0 
					|| npc_target->m_iBuildCount == 1
					|| npc_target->m_iBuildCount == 8
					|| npc_target->m_iBuildCount == 18) {

						iTemp = client_attacker->m_sItemEquipmentStatus[EQUIPPOS_RHAND];

						if ((iTemp != -1) || (client_attacker->m_pItemList[iTemp] != NULL)) {

							if (client_attacker->m_pItemList[iTemp]->m_wCurLifeSpan > 0)
								client_attacker->m_pItemList[iTemp]->m_wCurLifeSpan--;

							if (client_attacker->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {

								g_game->SendNotifyMsg(NULL, _attacker->m_handle, NOTIFY_ITEMLIFESPANEND, client_attacker->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);

								g_game->ReleaseItemHandler(_attacker->m_handle, iTemp, TRUE);  
							}
						}


						npc_target->m_iBuildCount--;
				}

				if (npc_target->m_iBuildCount <= 0) {
					npc_target->m_iBuildCount = 0;
				}

				return 0;
			}
			if (client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1 &&
				client_attacker->m_pItemList[ client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ] != NULL &&
				client_attacker->m_pItemList[ client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ]->m_sIDnum == ITEM_DEMONSLAYER &&
				npc_target->m_sType == NPC_DEMON) {
					iAP_L += 5;
			}
		}

		sTgtX = _target->m_sX;
		sTgtY = _target->m_sY;
		
	}
TAG_41025:

	if (_attacker->IsPlayer() && _target->IsPlayer()) {
		CClient *client_attacker = (CClient*) _attacker;
		CClient *client_target = (CClient*) _target;

		sX = _attacker->m_sX;
		sY = _attacker->m_sY;

		dX = _target->m_sX;
		dY = _target->m_sY;

		if (g_mapList[_target->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000006) != 0) return 0;
		if (g_mapList[_target->m_cMapIndex]->iGetAttribute(dX, dY, 0x00000006) != 0) return 0;
	}


	if (_attacker->IsPlayer()) {
		CClient *client_attacker = (CClient*) _attacker;
		if (client_attacker->GetDex() > 50) {
			iAttackerHitRatio += (client_attacker->GetDex() - 50); 
		}
	}

	if (wWeaponType >= 40) {
		switch (g_mapList[_attacker->m_cMapIndex]->m_cWhetherStatus) {
		case 0:	break;
		case 1:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 20); break;
		case 2:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 10); break;
		case 3:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 4);  break;
		}
	}
	if (iAttackerHitRatio < 0)   iAttackerHitRatio = 0;    

	cProtect = _target->m_cMagicEffectStatus[MAGICTYPE_PROTECT];

	if (_attacker->IsPlayer()) {
		CClient *client_attacker = (CClient*) _attacker;
		// BUG POINT! Item == NULL
		if (client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1) {
			//#ERROR POINT!
			if (client_attacker->m_pItemList[ client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ] == NULL) {

				client_attacker->m_bIsItemEquipped[client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND]] = FALSE;
				g_game->DeleteClient(_attacker->m_handle, TRUE, TRUE);
				return 0;
			}

			if (client_attacker->m_pItemList[ client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ]->m_sItemEffectType == ITEMEFFECTTYPE_ATTACK_ARROW) {
				if (client_attacker->m_cArrowIndex == -1) {
					return 0;
				}
				else {
					if (client_attacker->m_pItemList[ client_attacker->m_cArrowIndex ] == NULL) 
						return 0;

					client_attacker->m_pItemList[ client_attacker->m_cArrowIndex ]->m_dwCount--;
					if (client_attacker->m_pItemList[ client_attacker->m_cArrowIndex ]->m_dwCount <= 0) {

						g_game->ItemDepleteHandler(_attacker->m_handle, client_attacker->m_cArrowIndex, FALSE);

						client_attacker->m_cArrowIndex = g_game->_iGetArrowItemIndex(_attacker->m_handle);
					}
					else {
						g_game->SendNotifyMsg(NULL, _attacker->m_handle, NOTIFY_SETITEMCOUNT, client_attacker->m_cArrowIndex, client_attacker->m_pItemList[ client_attacker->m_cArrowIndex ]->m_dwCount, (char)FALSE, NULL);
						g_game->iCalcTotalWeight(_attacker->m_handle);
					}
				}
				if (cProtect == MAGICPROTECT_PFA) return 0;
			}
			else {
				switch (cProtect) 
				{
				case MAGICPROTECT_DS:
					iTargetDefenseRatio += 40;
					break;
				case MAGICPROTECT_GDS:
					iTargetDefenseRatio += 100;
					break;
				}
				if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
			}
		}
	}
	else {
		CNpc *npc_attacker = (CNpc*)_attacker;
		switch (cProtect) {
		case MAGICPROTECT_PFA: 
			switch (npc_attacker->m_sType) 
			{
			case NPC_DARK_ELF: 
			case NPC_AGT:
				if(abs(sTgtX - npc_attacker->m_sX) >= 1 || abs(sTgtY - npc_attacker->m_sY) >= 1) 
					return 0; 
				break;
			}
			break;
		case MAGICPROTECT_DS: iTargetDefenseRatio += 40;  break;
		case MAGICPROTECT_GDS: iTargetDefenseRatio += 100; break;
		}
		if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
	}

	if (cAttackerDir == cTargetDir) iTargetDefenseRatio = iTargetDefenseRatio / 2;

	if (iTargetDefenseRatio < 1)   iTargetDefenseRatio = 1;

	dTmp1 = (double)(iAttackerHitRatio);
	dTmp2 = (double)(iTargetDefenseRatio);

	dTmp3 = (dTmp1 / dTmp2) * HITRATIOFACTOR;
	iDestHitRatio = (int)(dTmp3); 

	if (iDestHitRatio < MINIMUMHITRATIO) iDestHitRatio = MINIMUMHITRATIO;

	if (iDestHitRatio > MAXIMUMHITRATIO) iDestHitRatio = MAXIMUMHITRATIO;

	if ((bIsAttackerBerserk == TRUE) && (iAttackMode < 20)) {
		iAP_SM = iAP_SM * 2;
		iAP_L  = iAP_L  * 2;
	}
	// Revan Rage 16/05/2016
	if ((bIsAttackerRage == TRUE) && (iAttackMode < 20)) {
		iAP_SM = iAP_SM * 7;
		iAP_L = iAP_L * 7;
	}

	if(_attacker->IsPlayer()) {
		CClient *client_attacker = (CClient*) _attacker;
		iAP_SM += client_attacker->m_iAddPhysicalDamage;
		iAP_L  += client_attacker->m_iAddPhysicalDamage;
	}

	if (bNearAttack == TRUE) {
		iAP_SM = iAP_SM / 2;
		iAP_L  = iAP_L / 2;
	}

	if(_target->IsPlayer()) {
		CClient *client_target = (CClient*) _target;
		iAP_SM -= (dice(1, client_target->GetVit()/10) - 1);
		iAP_L  -= (dice(1, client_target->GetVit()/10) - 1);
	}


	if(_attacker->IsPlayer()) {
		CClient *client_attacker = (CClient*) _attacker;
		if (client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1 &&
			client_attacker->m_pItemList[ client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ] != NULL){
				if((g_game->m_cDayOrNight == 2 && client_attacker->m_pItemList[ client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ]->m_sIDnum == ITEM_DARKEXECUTOR) ||
					(g_game->m_cDayOrNight == 1 && client_attacker->m_pItemList[ client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ]->m_sIDnum == ITEM_LIGHTINGBLADE)){
						iAP_SM += 10;
						iAP_L  += 10;
				}
		}
	}

	if (_attacker->IsPlayer()) {
		if (iAP_SM <= 1) iAP_SM = 1;
		if (iAP_L  <= 1) iAP_L  = 1;
	}
	else {
		if (iAP_SM <= 0) iAP_SM = 0;
		if (iAP_L  <= 0) iAP_L  = 0;
	}

	iResult = dice(1, 100);

	if (iResult <= iDestHitRatio) {
		if(_attacker->IsPlayer()) {
			CClient *client_attacker = (CClient*) _attacker;

			if ( ((client_attacker->m_iHungerStatus <= 10) || (client_attacker->m_iSP <= 0)) && 
				(dice(1,10) == 5) ) return FALSE;

			client_attacker->m_iComboAttackCount++;
			if (client_attacker->m_iComboAttackCount < 0) client_attacker->m_iComboAttackCount = 0;
			if (client_attacker->m_iComboAttackCount > 4) client_attacker->m_iComboAttackCount = 1;

			iWeaponSkill = g_game->_iGetWeaponSkillType(_attacker->m_handle);
			iComboBonus = g_game->iGetComboAttackBonus(iWeaponSkill, client_attacker->m_iComboAttackCount);


			if ((client_attacker->m_iComboAttackCount > 1) && (client_attacker->m_iAddCD != NULL))
				iComboBonus += client_attacker->m_iAddCD;

			iAP_SM += iComboBonus;
			iAP_L  += iComboBonus;


			switch (client_attacker->m_iSpecialWeaponEffectType) 
			{		
			case ITEMSTAT_CRITICAL: 				
				if ((client_attacker->m_iSuperAttackLeft > 0) && (iAttackMode >= 20)) {
					iAP_SM += client_attacker->m_iSpecialWeaponEffectValue;
					iAP_L  += client_attacker->m_iSpecialWeaponEffectValue;
				}
				break;

			case ITEMSTAT_POISONING: 
				cAttackerSA = 61; 
				iAttackerSAvalue = client_attacker->m_iSpecialWeaponEffectValue*5; 
				break;

			case ITEMSTAT_RIGHTEOUS: 
				cAttackerSA = 62;
				break;
			}

			if (g_mapList[_attacker->m_cMapIndex]->m_bIsFightZone == TRUE) 
			{
				iAP_SM += iAP_SM/3;
				iAP_L  += iAP_L/3;
			}

			if (g_mapList[_attacker->m_cMapIndex]->m_bIsApocalypseMap) 
			{
				iAP_SM *= 1.2;
				iAP_L *= 1.2;
			}

			if(client_attacker->CheckNearbyFlags())
			{
				iAP_SM *= 1.3;
				iAP_L *= 1.3;
			}

			if ((_target->IsPlayer()) && (g_game->m_bIsCrusadeMode == TRUE)) {
				CClient *client_target = (CClient*) _target;
				if(client_attacker->m_iCrusadeDuty == 1) {

					if (client_attacker->m_iLevel <= 80) 
					{
						iAP_SM += iAP_SM ;
						iAP_L += iAP_L ;
					}

					else if (client_attacker->m_iLevel <= 100)
					{
						iAP_SM += (iAP_SM* 7)/10 ;
						iAP_L += (iAP_L* 7)/10 ;
					} else 
					{
						iAP_SM += iAP_SM/3; ;
						iAP_L += iAP_L/3 ;
					}
				}
			}
		}

		if(_target->IsPlayer()) {
			CClient *client_target = (CClient*) _target;
			g_game->ClearSkillUsingStatus(_target->m_handle);

			if ((dwTime - client_target->m_dwTime) > RAGPROTECTIONTIME) {

				return 0;
			}
			else {

				switch (cAttackerSA) {
				case 62: 
					if (client_target->m_reputation < 0) {

						iTemp = abs(client_target->m_reputation) / 10;
						if (iTemp > 10) iTemp = 10;
						iAP_SM += iTemp;
					}
					break;
				}


				iAP_Abs_Armor  = 0;
				iAP_Abs_Shield = 0;

				iTemp = dice(1,10000);
				if ((iTemp >= 1) && (iTemp < 5000))           iHitPoint = 1; // BODY
				else if ((iTemp >= 5000) && (iTemp < 7500))   iHitPoint = 2; // PANTS + LEGGINGS
				else if ((iTemp >= 7500) && (iTemp < 9000))   iHitPoint = 3; // ARMS
				else if ((iTemp >= 9000) && (iTemp <= 10000)) iHitPoint = 4; // HEAD

				switch (iHitPoint) {
				case 1:
					if (client_target->m_iDamageAbsorption_Armor[EQUIPPOS_BODY] > 0) {

						if (client_target->m_iDamageAbsorption_Armor[EQUIPPOS_BODY] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)client_target->m_iDamageAbsorption_Armor[EQUIPPOS_BODY];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 2:
					if ((client_target->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] +
						client_target->m_iDamageAbsorption_Armor[EQUIPPOS_LEGGINGS]) > 0) {

							if ((client_target->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] +
								client_target->m_iDamageAbsorption_Armor[EQUIPPOS_LEGGINGS]) >= 80)
								dTmp1 = 80.0f;
							else dTmp1 = (double)(client_target->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] + client_target->m_iDamageAbsorption_Armor[EQUIPPOS_LEGGINGS]);
							dTmp2 = (double)iAP_SM;
							dTmp3 = (dTmp1 / 100.0f) * dTmp2;

							iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 3: 
					if (client_target->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS] > 0) {

						if (client_target->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)client_target->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 4:
					if (client_target->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD] > 0) {

						if (client_target->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)client_target->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;
				}

				if(cAttackerDir != cTargetDir && client_target->m_iDamageAbsorption_Shield > 0) {
					if (dice(1,100) <= (client_target->m_cSkillMastery[SKILL_SHIELD])) {

						_target->SkillUp(SKILL_SHIELD, 1);

						if (client_target->m_iDamageAbsorption_Shield >= 80) 
							dTmp1 = 80.0f;
						else dTmp1 = (double)client_target->m_iDamageAbsorption_Shield;
						dTmp2 = (double)iAP_SM - iAP_Abs_Armor;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Shield = (int)dTmp3;


						iTemp = client_target->m_sItemEquipmentStatus[EQUIPPOS_LHAND];
						if ((iTemp != -1) && (client_target->m_pItemList[iTemp] != NULL)) {


							if (!_target->IsNeutral() && (client_target->m_pItemList[iTemp]->m_wCurLifeSpan > 0))
								client_target->m_pItemList[iTemp]->m_wCurLifeSpan--;

							if (client_target->m_pItemList[iTemp]->m_wCurLifeSpan == 0) {

								g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_ITEMLIFESPANEND, client_target->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);

								g_game->ReleaseItemHandler(_target->m_handle, iTemp, TRUE);  
							}
						}
					}
				}

				iAP_SM = iAP_SM - (iAP_Abs_Armor + iAP_Abs_Shield);
				if (iAP_SM <= 0) iAP_SM = 1;


				if ((_attacker->IsPlayer())) {
					CClient *client_attacker = (CClient*)_attacker;
					if((client_attacker != NULL) && (client_attacker->m_bIsSpecialAbilityEnabled == TRUE)) {
						switch (client_attacker->m_iSpecialAbilityType) {
						case 0: break;
						case 1: 
							iTemp = (_target->m_iHP / 2);
							if (iTemp > iAP_SM) iAP_SM = iTemp;
							if (iAP_SM <= 0) iAP_SM = 1;
							break;

						case 2: 
							if (client_target->m_cMagicEffectStatus[ MAGICTYPE_ICE ] == 0) {
								client_target->m_cMagicEffectStatus[ MAGICTYPE_ICE ] = 1;
								client_target->SetStatusFlag(STATUS_FROZEN, TRUE);
								RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_ICE, dwTime + (30*1000), 
									_target, NULL, NULL, NULL, 1, NULL, NULL);

								g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_MAGICEFFECTON, MAGICTYPE_ICE, 1, NULL, NULL);
							}
							break;

						case 3: 
							if (_target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ] == 0) {
								_target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ] = 2;

								RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_HOLDOBJECT, dwTime + (10*1000), 
									_target, NULL, NULL, NULL, 10, NULL, NULL);

								g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_MAGICEFFECTON, MAGICTYPE_HOLDOBJECT, 10, NULL, NULL);
							}
							break;

						case 4: 			
							iAP_SM = (_target->m_iHP);
							break;

						case 5: 			
							client_attacker->m_iHP += iAP_SM;
							if (client_attacker->GetMaxHP() < client_attacker->m_iHP) 
								client_attacker->m_iHP = client_attacker->GetMaxHP();
							g_game->SendNotifyMsg(NULL, client_attacker->m_handle, NOTIFY_HP, NULL, NULL, NULL, NULL);
							break;
						}
					}
				}


				if (_attacker->IsPlayer()) {
					CClient *client_attacker = (CClient*) _attacker;
					if((client_attacker != NULL) && (client_target->m_bIsSpecialAbilityEnabled == TRUE)) {
						switch (client_target->m_iSpecialAbilityType) {
						case 50: 
							if (client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1)
								sWeaponIndex = client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
							else sWeaponIndex = client_attacker->m_sItemEquipmentStatus[EQUIPPOS_RHAND];
							if (sWeaponIndex != -1)	client_attacker->m_pItemList[sWeaponIndex]->m_wCurLifeSpan = 0;
							break;

						case 51: 			
							if (iHitPoint == client_target->m_iSpecialAbilityEquipPos)
								iAP_SM = 0;
							break;

						case 52: 			
							iAP_SM = 0;
							break;
						}
					}
				}

				if ((client_target->m_bIsLuckyEffect == TRUE) && 
					(dice(1,10) < 5) && (client_target->m_iHP <= iAP_SM)) {
						iAP_SM = 1;
				}

				switch (iHitPoint) {
				case 1:
					iTemp = client_target->m_sItemEquipmentStatus[EQUIPPOS_BODY];
					if(iTemp == -1) 
						iTemp = client_target->m_sItemEquipmentStatus[EQUIPPOS_FULLBODY];

					EnduStrippingDamage(_target, _attacker, iTemp, 2000);
					break;

				case 2:
					iTemp = client_target->m_sItemEquipmentStatus[EQUIPPOS_PANTS];
					EnduStrippingDamage(_target, _attacker, iTemp, 2000);

					iTemp = client_target->m_sItemEquipmentStatus[EQUIPPOS_LEGGINGS];
					EnduStrippingDamage(_target, _attacker, iTemp, 2000);
					break;

				case 3:
					iTemp = client_target->m_sItemEquipmentStatus[EQUIPPOS_ARMS];
					EnduStrippingDamage(_target, _attacker, iTemp, 2000);
					break;

				case 4:
					iTemp = client_target->m_sItemEquipmentStatus[EQUIPPOS_HEAD];
					EnduStrippingDamage(_target, _attacker, iTemp, 3000);
					break;
				}


				if (cAttackerSA == 2 && client_target->m_cMagicEffectStatus[MAGICTYPE_PROTECT]) {
					g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_MAGICEFFECTOFF, MAGICTYPE_PROTECT, _target->m_cMagicEffectStatus[MAGICTYPE_PROTECT], NULL, NULL);
					switch(_target->m_cMagicEffectStatus[MAGICTYPE_PROTECT]) 
					{
					case MAGICPROTECT_PFA:
						_target->SetStatusFlag(STATUS_PFA, FALSE);
						break;
					case MAGICPROTECT_PFM:
					case MAGICPROTECT_AMP:
						_target->SetStatusFlag(STATUS_PFM, FALSE);
						break;
					case MAGICPROTECT_DS:
					case MAGICPROTECT_GDS:
						_target->SetStatusFlag(STATUS_DEFENSESHIELD, FALSE);
						break;
					}
					_target->m_cMagicEffectStatus[MAGICTYPE_PROTECT] = NULL;
					RemoveFromDelayEventList(_target, MAGICTYPE_PROTECT);					  
				}


				if ( (client_target->m_bIsPoisoned == FALSE) && 
					((cAttackerSA == 5) || (cAttackerSA == 6) || (cAttackerSA == 61)) ) {
						if (checkResistingPoisonSuccess(_target) == FALSE) {

							client_target->m_bIsPoisoned  = TRUE;
							if (cAttackerSA == 5)		client_target->m_iPoisonLevel = 15;
							else if (cAttackerSA == 6)  client_target->m_iPoisonLevel = 40;
							else if (cAttackerSA == 61) client_target->m_iPoisonLevel = iAttackerSAvalue; 

							client_target->m_dwPoisonTime = dwTime;

							client_target->SetStatusFlag(STATUS_POISON, TRUE);
							g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_MAGICEFFECTON, MAGICTYPE_POISON, client_target->m_iPoisonLevel, NULL, NULL);
#ifdef TAIWANLOG
							_bItemLog(ITEMLOG_POISONED,_target->m_handle,(char *) NULL,NULL) ;
#endif
						}
				}

				_target->m_iHP -= iAP_SM;
				client_target->m_lastDamageTime = dwTime;
				if (_target->m_iHP <= 0) {

					if (_attacker->IsPlayer())
						g_game->bAnalyzeCriminalAction(_attacker->m_handle, _target->m_sX, _target->m_sY);

					client_target->KilledHandler( _attacker->m_handle, _attacker->m_ownerType, iAP_SM);
					bKilled     = TRUE;
					iKilledDice = client_target->m_iLevel;

				}
				else {
					if (iAP_SM > 0) {

						if (client_target->m_iAddTransMana > 0) {
							dTmp1 = (double)client_target->m_iAddTransMana;
							dTmp2 = (double)iAP_SM;
							dTmp3 = (dTmp1/100.0f)*dTmp2;


							iTemp = (2*client_target->GetMag()) + (2*client_target->m_iLevel) + (client_target->GetInt()/2);
							client_target->m_iMP += (int)dTmp3;
							if (client_target->m_iMP > iTemp) client_target->m_iMP = iTemp;
						}


						if (client_target->m_iAddChargeCritical > 0) {
							if (dice(1,100) < (client_target->m_iAddChargeCritical)) {
								iMaxSuperAttack = (client_target->m_iLevel / 10);
								if (client_target->m_iSuperAttackLeft < iMaxSuperAttack) client_target->m_iSuperAttackLeft++;

								g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_SUPERATTACKLEFT, NULL, NULL, NULL, NULL);
							}
						}

						g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_HP, NULL, NULL, NULL, NULL);

						if (_attacker->IsPlayer()) 
							sAttackerWeapon = ((((CClient*)_attacker)->m_sAppr2 & 0x0FF0) >> 4);
						else sAttackerWeapon = 1;

						if (_attacker->IsPlayer()) {
							if ((g_mapList[((CClient*)_attacker)->m_cMapIndex]->m_bIsFightZone == TRUE)) 
								iMoveDamage = 60;
							else iMoveDamage = 50;
						}
						//Flybug 
						iMoveDamage = 50;
						if (iAP_SM >= iMoveDamage) {

							if (sTgtX == sAtkX) {
								if (sTgtY == sAtkY)     goto CAE_SKIPDAMAGEMOVE;
								else if (sTgtY > sAtkY) cDamageMoveDir = 5;
								else if (sTgtY < sAtkY) cDamageMoveDir = 1;
							}
							else if (sTgtX > sAtkX) {
								if (sTgtY == sAtkY)     cDamageMoveDir = 3;
								else if (sTgtY > sAtkY) cDamageMoveDir = 4;
								else if (sTgtY < sAtkY) cDamageMoveDir = 2;
							}
							else if (sTgtX < sAtkX) {
								if (sTgtY == sAtkY)     cDamageMoveDir = 7;
								else if (sTgtY > sAtkY) cDamageMoveDir = 6;
								else if (sTgtY < sAtkY) cDamageMoveDir = 8;
							}


							client_target->m_iLastDamage = iAP_SM;

							g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_DAMAGEMOVE, cDamageMoveDir, iAP_SM, sAttackerWeapon, NULL);
						}
						else {
CAE_SKIPDAMAGEMOVE:;
							int iProb;


							if (_attacker->IsPlayer()) {
								CClient *client_attacker = (CClient*) _attacker;
								switch (client_attacker->m_sUsingWeaponSkill) {
								case SKILL_ARCHERY: iProb = 3500; break;
								case SKILL_LONGSWORD: iProb = 1000; break;
								case SKILL_FENCING: iProb = 2900; break;
								case SKILL_AXE: iProb = 2500; break;
								case SKILL_HAMMER: iProb = 2000; break;
								case SKILL_STAFF: iProb = 2000; break;
								default: iProb = 1; break;
								}
							}
							else iProb = 1;

							if (dice(1,10000) >= iProb) 
								g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, OBJECTDAMAGE, iAP_SM, sAttackerWeapon, NULL);
						}

						if (_target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ] == 1) {

							// 1: Hold-Person 
							// 2: Paralize
							g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_MAGICEFFECTOFF, MAGICTYPE_HOLDOBJECT, _target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ], NULL, NULL);

							_target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ] = NULL;
							RemoveFromDelayEventList(_target, MAGICTYPE_HOLDOBJECT);
						}

						client_target->m_iSuperAttackCount++;
						if (client_target->m_iSuperAttackCount > 14) {

							client_target->m_iSuperAttackCount = 0;
							iMaxSuperAttack = (client_target->m_iLevel / 10);
							if (client_target->m_iSuperAttackLeft < iMaxSuperAttack) client_target->m_iSuperAttackLeft++;

							g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_SUPERATTACKLEFT, NULL, NULL, NULL, NULL);
						}
					}
				}
			}
		}			   
		if(_target->IsNPC()) {
			CNpc *npc_target = (CNpc*)_target;
			if (npc_target->m_cBehavior == BEHAVIOR_DEAD) return 0;
			if (npc_target->m_bIsKilled == TRUE) return 0;

			if (g_game->m_bIsCrusadeMode == TRUE) {
				if (cAttackerSide == _target->m_side) {
					switch (npc_target->m_sType) {
					case NPC_ESG:
					case NPC_GMG:
					case NPC_LWB:
					case NPC_GHK:
					case NPC_GHKABS:
					case NPC_TK:
					case NPC_BG: 
					case NPC_CP:
						return 0;

					default: break;
					}
				}
				else {
					switch (npc_target->m_sType) {
					case NPC_GMG:
						if (cAttackerSide != 0) {
							npc_target->m_iV1 += iAP_L;
							if (npc_target->m_iV1 > 500) {

								npc_target->m_iV1 = 0;
								g_game->m_mana[npc_target->m_side]--;
								if (g_game->m_mana[npc_target->m_side] <= 0) g_game->m_mana[npc_target->m_side] = 0;
								//testcode
								wsprintf(g_cTxt, "ManaStock down: %d", g_game->m_mana[npc_target->m_side]);
								PutLogList(g_cTxt);
							}
						}
						break;
					}
				}
			}

			switch (npc_target->m_cActionLimit) 
			{
			case 1:
			case 2:
			case 6: 
				return 0;
			}

			if (npc_target->m_cSize == 0)
				iDamage = iAP_SM;
			else iDamage = iAP_L;

			if (npc_target->m_iAbsDamage < 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)(abs(npc_target->m_iAbsDamage))/100.0f;
				dTmp3 = dTmp1 * dTmp2;
				dTmp2 = dTmp1 - dTmp3;
				iDamage = (int)dTmp2;
				if (iDamage < 0) iDamage = 1;
			}


			if (cAttackerSA == 2 && npc_target->m_cMagicEffectStatus[MAGICTYPE_PROTECT]) {
				switch(npc_target->m_cMagicEffectStatus[MAGICTYPE_PROTECT]) 
				{
				case MAGICPROTECT_PFA:
					npc_target->SetStatusFlag(STATUS_PFA, FALSE);
					break;
				case MAGICPROTECT_PFM:
				case MAGICPROTECT_AMP:
					npc_target->SetStatusFlag(STATUS_PFM, FALSE);
					break;
				case MAGICPROTECT_DS:
				case MAGICPROTECT_GDS:
					npc_target->SetStatusFlag(STATUS_DEFENSESHIELD, FALSE);
					break;
				}
				npc_target->m_cMagicEffectStatus[MAGICTYPE_PROTECT] = NULL;
				RemoveFromDelayEventList(npc_target, MAGICTYPE_PROTECT);					  
			}


			switch (npc_target->m_cActionLimit) 
			{
			case 0: 		
			case 3: 
			case 5: 
				npc_target->m_iHP -= iDamage;
				break;
			}

			if (npc_target->m_iHP <= 0) {

				g_game->NpcKilledHandler(_attacker->m_handle, _attacker->m_ownerType, _target->m_handle, iDamage);

				bKilled = TRUE;
				iKilledDice = npc_target->m_iHitDice;

			}
			else {

				if ((npc_target->m_sType != 21) && (npc_target->m_sType != 55) && (npc_target->m_sType != 56)
					&& (npc_target->m_side == cAttackerSide)) goto CAE_SKIPCOUNTERATTACK;


				if (npc_target->m_cActionLimit != 0) goto CAE_SKIPCOUNTERATTACK;
				if (npc_target->m_bIsPermAttackMode == TRUE) goto CAE_SKIPCOUNTERATTACK;
				if ((npc_target->m_bIsSummoned == TRUE) && 
					(npc_target->m_iSummonControlMode == 1)) goto CAE_SKIPCOUNTERATTACK;

				if (npc_target->m_sType == NPC_CP) goto CAE_SKIPCOUNTERATTACK;

				if (dice(1,3) == 2) {

					if (npc_target->m_cBehavior == BEHAVIOR_ATTACK) {
						tX = tY = 0;
						switch (npc_target->m_cTargetType) {
						case OWNERTYPE_PLAYER:
							if (g_clientList[npc_target->m_iTargetIndex] != NULL) {
								tX = g_clientList[npc_target->m_iTargetIndex]->m_sX;
								tY = g_clientList[npc_target->m_iTargetIndex]->m_sY;
							}
							break;

						case OWNERTYPE_NPC:
							if (g_npcList[npc_target->m_iTargetIndex] != NULL) {
								tX = g_npcList[npc_target->m_iTargetIndex]->m_sX;
								tY = g_npcList[npc_target->m_iTargetIndex]->m_sY;
							}
							break;
						}
						iDst1 = (_target->m_sX - tX)*(_target->m_sX - tX) + (_target->m_sY - tY)*(_target->m_sY - tY);

						tX = tY = 0;

						tX = _attacker->m_sX;
						tY = _attacker->m_sY;



						iDst2 = (_target->m_sX - tX)*(_target->m_sX - tX) + (_target->m_sY - tY)*(_target->m_sY - tY);

						if (iDst2 <= iDst1) {

							npc_target->m_cBehavior          = BEHAVIOR_ATTACK;
							npc_target->m_sBehaviorTurnCount = 0;
							npc_target->m_iTargetIndex = _attacker->m_handle;
							npc_target->m_cTargetType  = _attacker->m_ownerType;
						}
					}
					else {
						npc_target->m_cBehavior          = BEHAVIOR_ATTACK;
						npc_target->m_sBehaviorTurnCount = 0;
						npc_target->m_iTargetIndex = _attacker->m_handle;
						npc_target->m_cTargetType  = _attacker->m_ownerType;
					}
				}

CAE_SKIPCOUNTERATTACK:;


				// Stun
				if ((dice(1, (npc_target->dwGoldDropValue / 55) + 3 ) == 1) && 
					(npc_target->m_cActionLimit == 0))
					npc_target->m_dwTime = dwTime;


				if (_attacker->m_ownerType == OWNERTYPE_PLAYER) 
					sAttackerWeapon = ((((CClient*)_attacker)->m_sAppr2 & 0x0FF0) >> 4);
				else sAttackerWeapon = 1;

				if ((wWeaponType < 40) && (npc_target->m_cActionLimit == 4)) {

					if (sTgtX == sAtkX) {
						if (sTgtY == sAtkY)     goto CAE_SKIPDAMAGEMOVE2;
						else if (sTgtY > sAtkY) cDamageMoveDir = 5;
						else if (sTgtY < sAtkY) cDamageMoveDir = 1;
					}
					else if (sTgtX > sAtkX) {
						if (sTgtY == sAtkY)     cDamageMoveDir = 3;
						else if (sTgtY > sAtkY) cDamageMoveDir = 4;
						else if (sTgtY < sAtkY) cDamageMoveDir = 2;
					}
					else if (sTgtX < sAtkX) {
						if (sTgtY == sAtkY)     cDamageMoveDir = 7;
						else if (sTgtY > sAtkY) cDamageMoveDir = 6;
						else if (sTgtY < sAtkY) cDamageMoveDir = 8;
					}

					dX = _target->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = _target->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

					if (g_mapList[_target->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == FALSE) {

						cDamageMoveDir = dice(1,8);
						dX = _target->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = _target->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

						if (g_mapList[_target->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == FALSE) goto CAE_SKIPDAMAGEMOVE2;
					}

					g_mapList[_target->m_cMapIndex]->ClearOwner(/*5,*/ _target->m_handle, OWNERTYPE_NPC, _target->m_sX, _target->m_sY);

					g_mapList[_target->m_cMapIndex]->SetOwner(_target->m_handle, OWNERTYPE_NPC, dX, dY);
					_target->m_sX   = dX;
					_target->m_sY   = dY;
					_target->m_cDir = cDamageMoveDir;

					g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTMOVE, NULL, NULL, NULL);

					dX = _target->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = _target->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

					if (g_mapList[_target->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == FALSE) {

						cDamageMoveDir = dice(1,8);
						dX = _target->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = _target->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

						if (g_mapList[_target->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == FALSE) goto CAE_SKIPDAMAGEMOVE2;
					}

					g_mapList[_target->m_cMapIndex]->ClearOwner(/*5,*/ _target->m_handle, OWNERTYPE_NPC, _target->m_sX, _target->m_sY);

					g_mapList[_target->m_cMapIndex]->SetOwner(_target->m_handle, OWNERTYPE_NPC, dX, dY);
					_target->m_sX   = dX;
					_target->m_sY   = dY;
					_target->m_cDir = cDamageMoveDir;

					g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTMOVE, NULL, NULL, NULL);

					if (g_game->bCheckEnergySphereDestination(_target->m_handle, _attacker->m_handle, _attacker->m_ownerType) == TRUE) {
						if (_attacker->m_ownerType == OWNERTYPE_PLAYER) {
							iExp = (_target->m_iExp/3);
							if (npc_target->m_iNoDieRemainExp > 0)
								iExp += npc_target->m_iNoDieRemainExp;


							if (((CClient*)_attacker)->m_iAddExp != NULL) {
								dTmp1 = (double)((CClient*)_attacker)->m_iAddExp;
								dTmp2 = (double)iExp;
								dTmp3 = (dTmp1/100.0f)*dTmp2;
								iExp += (int)dTmp3;
							}

							// Crusade
							if ((g_game->m_bIsCrusadeMode == TRUE) && (iExp > 10)) iExp = 10;

							//attacker->m_iExpStock += iExp;
							g_game->GetExp(_attacker->m_handle, iExp);


							g_game->DeleteNpc(_target->m_handle);
							return FALSE;
						}
					}

CAE_SKIPDAMAGEMOVE2:;
				}
				else {
					g_game->SendEventToNearClient_TypeA(_target->m_handle, OWNERTYPE_NPC, MSGID_EVENT_MOTION, OBJECTDAMAGE, iDamage, sAttackerWeapon, NULL);
				}

				if (_target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ] == 1) {

					// 1: Hold-Person 
					_target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ] = NULL;
					RemoveFromDelayEventList(_target, MAGICTYPE_HOLDOBJECT);
				}
				else if (_target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ] == 2) {
					// 2: Paralize
					if ((npc_target->m_iHitDice > 50) && (dice(1, 10) == 5)) {
						_target->m_cMagicEffectStatus[ MAGICTYPE_HOLDOBJECT ] = NULL;
						RemoveFromDelayEventList(_target, MAGICTYPE_HOLDOBJECT);
					}
				}


				if ( (npc_target->m_iNoDieRemainExp > 0) && (npc_target->m_bIsSummoned != TRUE) && 
					(_attacker->m_ownerType == OWNERTYPE_PLAYER) && (_attacker != NULL) ) {
						CClient *client_attacker = (CClient*) _attacker;

						if (npc_target->m_iNoDieRemainExp > iDamage) {
							iExp = iDamage;
							npc_target->m_iNoDieRemainExp -= iDamage;
						}
						else {
							iExp = npc_target->m_iNoDieRemainExp;
							npc_target->m_iNoDieRemainExp = 0;
						}


						if (client_attacker->m_iAddExp != NULL) {
							dTmp1 = (double)(client_attacker)->m_iAddExp;
							dTmp2 = (double)iExp;
							dTmp3 = (dTmp1/100.0f)*dTmp2;
							iExp += (int)dTmp3;
						}

						if (g_game->m_bIsCrusadeMode == TRUE) iExp = iExp/3;


						if ((client_attacker)->m_iLevel > 100 ) {
							switch (npc_target->m_sType) {
							case 55:
							case 56:
								iExp = 0 ;
								break ;
							default: break;
							}
						}

				}
			}
		}

		//CAE_SKIPCOUNTERATTACK:;

		if (_attacker->m_ownerType == OWNERTYPE_PLAYER) {
			CClient *client_attacker = (CClient*) _attacker;
			if (client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1)
				sWeaponIndex = client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
			else sWeaponIndex = client_attacker->m_sItemEquipmentStatus[EQUIPPOS_RHAND];

			if (sWeaponIndex != -1) {
				if ( (client_attacker->m_pItemList[sWeaponIndex] != NULL) &&
					(client_attacker->m_pItemList[sWeaponIndex]->m_sIDnum != ITEM_PICKAXE) ) {
						if (bKilled == FALSE)
							_attacker->WeaponSkillUp(sWeaponIndex, 1);
						else {
							if (client_attacker->m_iHP <= 3) 
								client_attacker->WeaponSkillUp(sWeaponIndex, dice(1, iKilledDice)*2);
							else client_attacker->WeaponSkillUp(sWeaponIndex, dice(1, iKilledDice));
						}
				}

				if ( (client_attacker->m_pItemList[sWeaponIndex] != NULL) && 
					(client_attacker->m_pItemList[sWeaponIndex]->m_wMaxLifeSpan != 0) ) 
				{
					iWepLifeOff = 1;
					if ((wWeaponType >= 1) && (wWeaponType < 40)) {
						switch (g_mapList[_attacker->m_cMapIndex]->m_cWhetherStatus) {
						case 0:	break;
						case 1:	if (dice(1,3) == 1) iWepLifeOff++; break;
						case 2:	if (dice(1,2) == 1) iWepLifeOff += dice(1,2); break;
						case 3:	if (dice(1,2) == 1) iWepLifeOff += dice(1,3); break;
						}
					}

					if (!_attacker->IsNeutral()) {
						if (client_attacker->m_pItemList[sWeaponIndex]->m_wCurLifeSpan < iWepLifeOff) 
							client_attacker->m_pItemList[sWeaponIndex]->m_wCurLifeSpan = 0;
						else client_attacker->m_pItemList[sWeaponIndex]->m_wCurLifeSpan -= iWepLifeOff;
					}

					if (client_attacker->m_pItemList[sWeaponIndex]->m_wCurLifeSpan == 0) {

						g_game->SendNotifyMsg(NULL, _attacker->m_handle, NOTIFY_ITEMLIFESPANEND, client_attacker->m_pItemList[sWeaponIndex]->m_cEquipPos, sWeaponIndex, NULL, NULL);

						g_game->ReleaseItemHandler(_attacker->m_handle, sWeaponIndex, TRUE);  
					}
				}
			}
			else {
				if (wWeaponType == 0) {
					_attacker->SkillUp(SKILL_HANDATTACK, 1);
				}
			}
		}
	}
	else {
		if (_attacker->m_ownerType == OWNERTYPE_PLAYER) {
			CClient *client_attacker = (CClient*) _attacker;
			client_attacker->m_iComboAttackCount = 0;
		}
	}

	return iExp;
}

void EnduStrippingDamage(Unit *_target, Unit *_attacker, int item, int higherStripEndu)
{
	int equiptItem, enduDamage = 1;
	float stripChance = 0;
	if(!_target) return;
	if(!_target->IsPlayer()) return;
	CClient *client_target = (CClient*) _target;
	if (_attacker == NULL) return;
	if (item == -1 || client_target->m_pItemList[item] == NULL) return;

	if (_attacker->m_ownerType	== OWNERTYPE_PLAYER && 
		(_target->m_side != _attacker->m_side || g_mapList[_attacker->m_cMapIndex]->m_bIsFightZone)){
			CClient *client_attacker = (CClient*) _attacker;
		switch (client_attacker->m_sUsingWeaponSkill) {
		case SKILL_HAMMER:
			stripChance = 0.25;
			enduDamage = 10;
			equiptItem = client_attacker->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
			if (equiptItem == -1 || client_attacker->m_pItemList[equiptItem] == NULL) break;

			switch (client_attacker->m_pItemList[equiptItem]->m_sIDnum){
			case ITEM_HAMMER:
				stripChance = 0.25;
				enduDamage = 10;
				break;
			case ITEM_BHAMMER:
				stripChance = 0.35f; 
				enduDamage = 15;
				break;
			case ITEM_BARBARIANHAMMER:
				stripChance = 0.45f;
				enduDamage = 20;
				break;
			case ITEM_GBHAMMER:
				stripChance = 0.6f;
				enduDamage = 25;
				break;
			}
			break;
		case SKILL_AXE: enduDamage = 3; break;  
		default: enduDamage = 1; break;  
		}
	}

	if (!client_target->IsNeutral() && 
		client_target->m_pItemList[item]->m_wCurLifeSpan > 0 &&
		memcmp(client_target->m_pItemList[item]->m_cName, "Legion", 6) != 0) {
		client_target->m_pItemList[item]->m_wCurLifeSpan -= enduDamage; 
	}

	if (client_target->m_pItemList[item]->m_wCurLifeSpan <= 0) {
		client_target->m_pItemList[item]->m_wCurLifeSpan = 0 ;
		g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_ITEMLIFESPANEND, client_target->m_pItemList[item]->m_cEquipPos, item, NULL, NULL);
		g_game->ReleaseItemHandler(_target->m_handle, item, TRUE);  
	}
	else if (stripChance > 0) {
		switch (client_target->m_pItemList[item]->m_sIDnum) {
		case ITEM_MERIENPLATEMAIL_M: 
		case ITEM_MERIENPLATEMAIL_W: 
			return;
		}

		if ( client_target->m_pItemList[item]->m_wMaxLifeSpan <  higherStripEndu )
			stripChance *= dice(6, (client_target->m_pItemList[item]->m_wMaxLifeSpan - client_target->m_pItemList[item]->m_wCurLifeSpan));
		else 
			stripChance *= dice(4, (client_target->m_pItemList[item]->m_wMaxLifeSpan - client_target->m_pItemList[item]->m_wCurLifeSpan));

		if (stripChance >  client_target->m_pItemList[item]->m_wCurLifeSpan) {
			g_game->ReleaseItemHandler(_target->m_handle, item, TRUE);  
			g_game->SendNotifyMsg(NULL, _target->m_handle, NOTIFY_ITEMRELEASED, client_target->m_pItemList[item]->m_cEquipPos, item, NULL, NULL);
		}
	}
}