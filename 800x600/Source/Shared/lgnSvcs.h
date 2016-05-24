#ifndef LGNSVCS_H
#define LGNSVCS_H
#include "NetMessages.h"

const enum lgnSvcsCmds{
	CMD_LGNSVC_MAGEHAT,
	CMD_LGNSVC_BMAGEHAT,
	CMD_LGNSVC_WARRIORHELM,
	CMD_LGNSVC_BMAGEHELM,
	CMD_LGNSVC_TOARE,
	CMD_LGNSVC_TOELV,
	//CMD_LGNSVC_TOIST,
	CMD_LGNSVC_MAJ2,
	CMD_LGNSVC_MAJ20,
	CMD_LGNSVC_GOLD10,
	CMD_LGNSVC_GOLD100,
	CMD_LGNSVC_REP10,
	CMD_LGNSVC_REP100,
	CMD_LGNSVC_XPSLATE,
	CMD_LGNSVC_ZERKSLATE,
	CMD_LGNSVC_MPSLATE,
	CMD_LGNSVC_HPSLATE,
	CMD_LGNSVC_ZEM,
	CMD_LGNSVC_SOM,
	CMD_LGNSVC_SOX,
	CMD_LGNSVC_MAX
};

const struct lgnPtsSvc{
	char * desc;
	char * name;
	unsigned long price;
	unsigned short cmd;
} lgnPtsSvcs[] = {
	{"MP14 Indestr. Golden Wizard Hat", "LegionHat MP14", 25, CMD_LGNSVC_MAGEHAT},
	{"HP14 Indestr. Golden Wizard Hat", "LegionHat HP14", 25, CMD_LGNSVC_BMAGEHAT},
	{"HP14 Indestr. Golden Winged Helm", "LegionHelm HP14", 25, CMD_LGNSVC_WARRIORHELM},
	{"MP14 Indestr. Golden Winged Helm", "LegionHelm MP14", 25, CMD_LGNSVC_BMAGEHELM},
	{"Town Change (Aresden)", "TC(Are)", 10, CMD_LGNSVC_TOARE},
	{"Town Change (Elvine)", "TC(Elv)", 10, CMD_LGNSVC_TOELV},
	//{"Town Change (Istria)", "TC(Ist)", 10, CMD_LGNSVC_TOIST},
	{"2 Majestic points", "2 Maj", 1, CMD_LGNSVC_MAJ2},
	{"20 Majestic points", "20 Maj", 10, CMD_LGNSVC_MAJ20},
	{"10,000 Gold", "10k Gold", 1, CMD_LGNSVC_GOLD10},
	{"100,000 Gold", "100k Gold", 10, CMD_LGNSVC_GOLD100},
	{"10 Reputations points", "10 Rep", 1, CMD_LGNSVC_REP10},
	{"100 Reputations points", "100 Rep", 10, CMD_LGNSVC_REP100},
	{"Experience Slate", "XP Slate", 2, CMD_LGNSVC_XPSLATE},
	{"Berserk Slate", "Zerk Slate", 2, CMD_LGNSVC_ZERKSLATE},
	{"Mana Slate", "MP Slate", 4, CMD_LGNSVC_MPSLATE},
	{"Health Slate", "HP Slate", 6, CMD_LGNSVC_HPSLATE},
	{"Zemstone of Sacrifice", "Zem", 2, CMD_LGNSVC_ZEM},
	{"Stone of Merien", "SoM", 2, CMD_LGNSVC_SOM},
	{"Stone of Xelima", "SoX", 2, CMD_LGNSVC_SOX},
	{"","",0,0}
};

#endif // LGNSVCS_H
