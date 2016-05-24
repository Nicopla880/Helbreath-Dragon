#include "DelayedEvents.h"
#include <Windows.h>
#include "char/Unit.h"
#include "HG.h"

extern class CGame *   g_game;
#define MAXDELAYEVENTS		60000
namespace {
	struct DelayEvent__t
	{
		delayeventtype__t m_iDelayType;
		int m_iEffectType;

		char m_cMapIndex;
		int m_dX, m_dY;

		Unit *m_pTarget;
		int m_iV1, m_iV2, m_iV3;

		DWORD m_dwTriggerTime;
	};

	DelayEvent__t    * DelayEventList[MAXDELAYEVENTS];
};
extern class CClient ** g_clientList;

bool RegisterDelayEvent(int iDelayType, int iEffectType, DWORD dwLastTime, Unit * unit, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3)
{
	register int i;

	for (i = 0; i < MAXDELAYEVENTS; i++) 
		if (DelayEventList[i] == NULL) {

			DelayEventList[i] = new DelayEvent__t;

			DelayEventList[i]->m_iDelayType = (delayeventtype__t)iDelayType;
			DelayEventList[i]->m_iEffectType = iEffectType;

			DelayEventList[i]->m_cMapIndex = cMapIndex;
			DelayEventList[i]->m_dX = dX;
			DelayEventList[i]->m_dY = dY;

			DelayEventList[i]->m_pTarget    = unit;
			DelayEventList[i]->m_iV1         = iV1;
			DelayEventList[i]->m_iV2         = iV2;
			DelayEventList[i]->m_iV3         = iV3; 

			DelayEventList[i]->m_dwTriggerTime = dwLastTime;

			return TRUE;
		}

		return FALSE;
}

void DelayEventProcessor()
{
	register int i, iSkillNum, iResult;
	DWORD dwTime = timeGetTime();
	int iTemp;
	Unit * unit = NULL;

	for (i = 0; i < MAXDELAYEVENTS; i++) 
		if ((DelayEventList[i] != NULL) && (DelayEventList[i]->m_dwTriggerTime < dwTime)) {

			switch (DelayEventList[i]->m_iDelayType) 
			{

			case DELAYEVENTTYPE_ANCIENT_TABLET:
				if ((DelayEventList[i]->m_pTarget->m_iStatus & STATUS_REDSLATE) != 0) {
					iTemp = 1;
				}
				else if ((DelayEventList[i]->m_pTarget->m_iStatus & STATUS_BLUESLATE) != 0) {
					iTemp = 3;
				}
				else if ((DelayEventList[i]->m_pTarget->m_iStatus & STATUS_GREENSLATE) != 0) {
					iTemp = 4;
				}

				g_game->SendNotifyMsg(NULL, DelayEventList[i]->m_pTarget->m_handle, NOTIFY_SLATE_STATUS, iTemp, NULL, NULL, NULL);
				g_game->SetSlateFlag(DelayEventList[i]->m_pTarget->m_handle, iTemp, FALSE);
				break;

			case DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT:
				g_game->CalcMeteorStrikeEffectHandler(DelayEventList[i]->m_cMapIndex);
				break;

			case DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE:
				g_game->DoMeteorStrikeDamageHandler(DelayEventList[i]->m_cMapIndex);
				break;

			case DELAYEVENTTYPE_METEORSTRIKE:
				g_game->MeteorStrikeHandler(DelayEventList[i]->m_cMapIndex);
				break;

			case DELAYEVENTTYPE_USEITEM_SKILL:
				{
					if(DelayEventList[i]->m_pTarget->IsPlayer()) {
						CClient *player = (CClient*) DelayEventList[i]->m_pTarget;
						iSkillNum = DelayEventList[i]->m_iEffectType;

						if ( player == NULL ) break;
						if ( player->m_bSkillUsingStatus[iSkillNum] == FALSE ) break;
						if ( player->m_iSkillUsingTimeID[iSkillNum] != DelayEventList[i]->m_iV2) break;

						player->m_bSkillUsingStatus[iSkillNum] = FALSE;
						player->m_iSkillUsingTimeID[iSkillNum] = NULL;

						iResult = g_game->iCalculateUseSkillItemEffect(player->m_handle, OWNERTYPE_PLAYER,
							DelayEventList[i]->m_iV1, iSkillNum, DelayEventList[i]->m_cMapIndex, DelayEventList[i]->m_dX, DelayEventList[i]->m_dY);

						g_game->SendNotifyMsg(NULL, player->m_handle, NOTIFY_SKILLUSINGEND, iResult, NULL, NULL, NULL);
					}
				} break;
		case DELAYEVENTTYPE_KILL_ABADDON:
			g_game->NpcKilledHandler(DelayEventList[i]->m_pTarget->m_handle, OWNERTYPE_NPC, DelayEventList[i]->m_pTarget->m_handle, 0);
			break;

		case DELAYEVENTTYPE_END_APOCALYPSE:
			g_game->GlobalEndApocalypseMode(0);	
			break;
		case DELAYEVENTTYPE_DAMAGEOBJECT:
			break;

		case DELAYEVENTTYPE_MAGICRELEASE:
			if(DelayEventList[i]->m_pTarget) {
				DelayEventList[i]->m_pTarget->RemoveMagicEffect(DelayEventList[i]->m_iEffectType);
			}
			break;
		}

		delete DelayEventList[i];
		DelayEventList[i] = NULL;
	}
}

bool RemoveFromDelayEventList(Unit * unit, int iEffectType)
{
	register int i;

	for (i = 0; i < MAXDELAYEVENTS; i++) 
		if (DelayEventList[i] != NULL) {

			if (iEffectType == NULL) {

				if (DelayEventList[i]->m_pTarget == unit) {
					delete DelayEventList[i];
					DelayEventList[i] = NULL;
				}
			}
			else {
				if ( (DelayEventList[i]->m_pTarget == unit) &&
					(DelayEventList[i]->m_iEffectType == iEffectType) ) {
						delete DelayEventList[i];
						DelayEventList[i] = NULL;
				}
			}
		}

	return TRUE;
}

