#define HITRATIOFACTOR		80.0f
#define MAXSKILLPOINTS		900
class Unit;
class CClient;
bool CheckResistingMagicSuccess(char cAttackerDir, Unit *target, int iHitRatio);
bool checkResistingPoisonSuccess(Unit *target);
bool checkResistingIceSuccess(char cAttackerDir, Unit *target, int iHitRatio);
int calculateAttackEffect(Unit *target, Unit *attacker, int tdX, int tdY, int iAttackMode, bool bNearAttack = false, bool bIsDash = false);
void EnduStrippingDamage(Unit *target, Unit *attacker, int item, int higherStripEndu);
