#pragma once
#include <Windows.h>
class Unit;
enum delayeventtype__t {
	DELAYEVENTTYPE_DAMAGEOBJECT=1,
	DELAYEVENTTYPE_MAGICRELEASE,
	DELAYEVENTTYPE_USEITEM_SKILL,
	DELAYEVENTTYPE_METEORSTRIKE,
	DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE,
	DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT,
	DELAYEVENTTYPE_ANCIENT_TABLET,

	DELAYEVENTTYPE_END_APOCALYPSE,
	DELAYEVENTTYPE_KILL_ABADDON
};

bool RemoveFromDelayEventList(Unit * unit, int iEffectType);
void DelayEventProcessor();
bool RegisterDelayEvent(int iDelayType, int iEffectType, DWORD dwLastTime, Unit * unit, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3);