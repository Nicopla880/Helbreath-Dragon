#pragma once
enum skillIndexes
{
	SKILL_MINING,			// 0
	SKILL_FISHING,			// 1
	SKILL_FARMING,			// 2
	SKILL_MAGICRES,			// 3
	SKILL_MAGIC,			// 4
	SKILL_HANDATTACK,		// 5
	SKILL_ARCHERY,			// 6
	SKILL_SHORTSWORD,		// 7
	SKILL_LONGSWORD,		// 8
	SKILL_FENCING,			// 9
	SKILL_AXE,				// 10
	SKILL_SHIELD,			// 11
	SKILL_ALCHEMY,			// 12
	SKILL_MANUFACTURING,	// 13
	SKILL_HAMMER,			// 14
	SKILL_15,				
	SKILL_16,				
	SKILL_17,				
	SKILL_18,
	SKILL_PRETENDCORPSE,	// 19
	SKILL_20,
	SKILL_STAFF,			// 21
	SKILL_22,
	SKILL_POISONRES			// 23
};

static int GetSkillMultiplier(skillIndexes index) {
	switch (index) {
	case SKILL_MAGIC:
	case SKILL_HANDATTACK:
	case SKILL_ARCHERY:
	case SKILL_SHORTSWORD:
	case SKILL_LONGSWORD:
	case SKILL_FENCING:
	case SKILL_AXE:
	case SKILL_HAMMER:
	case SKILL_STAFF:
	case SKILL_PRETENDCORPSE:
	case SKILL_MAGICRES:
	case SKILL_SHIELD:
	case SKILL_POISONRES:
		return 8;
		break;
	case SKILL_FARMING:
	case SKILL_MANUFACTURING:
	case SKILL_ALCHEMY:
	case SKILL_MINING:
	case SKILL_FISHING:
		return 2;
		break;
	default:
		return 1;
	}
}