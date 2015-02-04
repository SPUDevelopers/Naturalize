#include "Unit.h"

USING_NS_CC;

Unit::Unit(int x, int y, int op)
{
	//This will be changed to pull values from file; hardcode now for testing
	health = MAX_HEALTH;
	xcord = x;
	ycord = y;
	atkPow = 5;
	atkRange = 1;
	defPow = 1;
	movSpd = 3;
	cost = 5;
	visionRange = 3;
	player = op;
	atkType = normal;
	movType = foot;
	defType = normal;
	faction = 0;
	//list<string> actions; Not sure how we want to implement this yet
	isActive = false;
	isSelected = false;
}

Unit::~Unit()
{
}

Unit* Unit::create(const cocos2d::Size tileSize, int x, int y, int op)
{
	Unit *ret = new (std::nothrow) Unit(x, y, op);

	if (ret->init(tileSize))
	{
		//Enable garbage collection
		ret->autorelease();
		return ret;
	}
	else
	{
		//Delete the instance
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool Unit::init(const cocos2d::Size tileSize, int x, int y, int op)
{
	//Super init
	if (!cocos2d::Node::init())
		return false;

	log("*** CURSOR INIT ***");

	//Cache map grid size
	this->tileSize = tileSize;

	//Cre3ate sprite and attach
	this->unitsprite = cocos2d::Sprite::create("maps/test/lumberjack.png");
	this->unitsprite->setPosition(Vec21(64, 64));
	this->addChild(this->unitsprite);

	return true;
};

//list<string> getActions;
//list<string> uniqueActions;

void takeDmg(int damage, attackType dmgType)
{
	//Look up damage influence
	dmgMul = 1; //Place holder - damage type table will return this value
	damage *= dmgMul;
	damage -= defPow;	//How defPow affects incoming damage needs testing
	if (damage < 1) damage = 1;
	health -= (int)(damage);
}

int dealDmg()
{
	//Algorithm subject to change - how health affects damage needs testing
	int damage = (int)(atkPow * (health/MAX_HEALTH));
	if (damage < 1) damage = 1;
	return damage;
}

void heal(int amount)
{
	health += amount;
	if (health >MAX_HEALTH) health = MAX_HEALTH;
}

/*void kill()
{

} Kill from outside?
*/

int getHealth() { return health; }
int getAtkPow() { return atkPow; }
int getDefPow() { return defPow; }
attackType getAtkType() { return atkType(); }
defenseType getDefType() { return defType(); }
movementType getMovType() { return movType(); }
int getAtkRange() { return atkRange; }
int getMovSpd() { return movSpd; }
int getVisionRange() { return visionRange; }
int getCost() { return getCost; }

bool canAttack()
{
	return health; //If health is 0, it cannot attack; otherwise, swing away
}


bool canCombine()
{
	if (health < 10 && health >1) return true;
	else return false; //Only living, damaged units can be combined
}


