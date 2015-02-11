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
	atkType = ATKTYPE_NORMAL;
	movType = MOVETYPE_INFANTRY;
	defType = DEFTYPE_NORMAL;
	faction = FACTION_HUMAN;
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

	if (ret->init(tileSize, x, y, op))
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
	this->unitsprite->setPosition(Vec2(64, 64));
	this->addChild(this->unitsprite);

	return true;
};

//list<string> getActions;
//list<string> uniqueActions;

void Unit::takeDmg(int damage, AttackType dmgType)
{
	//Look up damage influence
	int dmgMul = 1; //Place holder - damage type table will return this value
	damage *= dmgMul;
	damage -= defPow;	//How defPow affects incoming damage needs testing
	if (damage < 1) damage = 1;
	health -= (int)(damage);
}

int Unit::dealDmg()
{
	//Algorithm subject to change - how health affects damage needs testing
	int damage = (int)(atkPow * (health/MAX_HEALTH));
	if (damage < 1) damage = 1;
	return damage;
}

void Unit::heal(int amount)
{
	health += amount;
	if (health >MAX_HEALTH) health = MAX_HEALTH;
}

/*void kill()
{

} Kill from outside?
*/

int Unit::getHealth() { return health; }
int Unit::getAtkPow() { return atkPow; }
int Unit::getDefPow() { return defPow; }
AttackType Unit::getAtkType() { return atkType; }
DefenseType Unit::getDefType() { return defType; }
MoveType Unit::getMovType() { return movType; }
int Unit::getAtkRange() { return atkRange; }
int Unit::getMovSpd() { return movSpd; }
int Unit::getVisionRange() { return visionRange; }
int Unit::getCost() { return cost; }

bool Unit::canAttack()
{
	return health; //If health is 0, it cannot attack; otherwise, swing away
}


bool Unit::canCombine()
{
	if (health < 10 && health >1) return true;
	else return false; //Only living, damaged units can be combined
}


