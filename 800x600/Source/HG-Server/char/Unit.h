#ifndef UNIT_H
#define UNIT_H
#pragma once

#include "magic.h"
#include "../../Shared/Skilltypes.h"
#include "../../Shared/Common.h"
class CClient;

enum OwnerType{
	OWNERTYPE_NONE,
	OWNERTYPE_PLAYER,
	OWNERTYPE_NPC,
	OWNERTYPE_PLAYER_INDIRECT
};

class Unit abstract
{

public:
	enum UnitType {
		UNITTYPE_PLAYER,
		UNITTYPE_NPC
	};
private:
	UnitType m_unittype;
public:
	Unit(UnitType t);
	virtual ~Unit(void);
	void SetStatusFlag(long flag, bool enabled);
	void ToggleStatusFlag(long flag);
	bool GetStatusFlag(long flag) const;
	void SetMagicFlag(short magicType, bool enabled);
	bool AddMagicEffect(short magicType, long effectTime, char kind = 1);
	bool RemoveMagicEffect(char magicType);
	void SetSideFlag(Side side);

	bool IsDead()		{ return (m_iHP <= 0); }
	bool IsBerserked()	{ return m_cMagicEffectStatus[MAGICTYPE_BERSERK] ? TRUE : FALSE; }
	bool IsRaged()		{ return m_cMagicEffectStatus[MAGICTYPE_RAGE] ? TRUE : FALSE; } // Revan Rage 16/05/2016
	bool IsInvisible()	{ return m_cMagicEffectStatus[MAGICTYPE_INVISIBILITY] ? TRUE : FALSE; }
	virtual void RemoveInvisibility();
	bool IsPlayer()		{ return (m_ownerType == OWNERTYPE_PLAYER) ? TRUE : FALSE; }
	bool IsNPC()		{ return (m_ownerType == OWNERTYPE_NPC) ? TRUE : FALSE; }
	bool IsNeutral()	{ return (m_side == NEUTRAL) ? TRUE : FALSE; }
	bool IsAres()		{ return (m_side == ARESDEN) ? TRUE : FALSE; }
	bool IsElv()		{ return (m_side == ELVINE) ? TRUE : FALSE; }

	CClient * GetKiller() const;

	uint8 GetFaction() {
		return m_side;
	}
	virtual int GetMagicResistRatio() = 0;
	virtual int GetPoisonResistRatio() = 0;
	virtual int GetFrostResistRatio() = 0;
	virtual void WeaponSkillUp(short sWeaponIndex, int iValue){}
	virtual void SkillUp(skillIndexes index, int skillups){}
	bool HasProtectionFromMagic() {
		return m_cMagicEffectStatus[MAGICTYPE_PROTECT] == MAGICPROTECT_PFM;
	}
	bool HasAbsoluteMagicProtection() {
		return m_cMagicEffectStatus[MAGICTYPE_PROTECT] == MAGICPROTECT_AMP;
	}
	bool AfflictedBy(int flag) {
		return (m_iStatus & flag) ? TRUE : FALSE;
	}
	int32 GetMapID() {
		return m_cMapIndex;
	}
	char GetFacingDirection() {
		return m_cDir;
	}
	int GetHP() {
		return m_iHP;
	}
	int GetMP() {
		return m_iMP;
	}
	
	short m_handle;

	int  m_iHP;
	int  m_iMP;              
	int  m_iExp;
	uint8 m_side;
	uint32 m_iStatus;
	bool m_bIsKilled;
	long m_killerh;
	char  m_cMapIndex;

	char  m_cDir;
	short m_sX, m_sY;

	short m_sType;
	short m_sOriginalType;
	short m_ownerType;

	char  m_cMagicEffectStatus[MAXMAGICEFFECTS];

	int   m_iGuildGUID;
};
Unit *GetUnit(int handler, int type);
#endif