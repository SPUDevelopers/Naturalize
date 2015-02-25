#ifndef __UNIT_H__
#define __UNIT_H__

#include <list>
#include <string>
#include "cocos2d.h"

#include "NatTypes.h"

using namespace Naturalize;

#define MAX_HEALTH 10

class Unit : public cocos2d::Node
{
private:
	cocos2d::Sprite *unitsprite;
	cocos2d::Size tileSize; //Map tile size in px

	int health; //The unit's current health, ranging from MAX_HEALTH to 1. At 0, the unit dies and the instance is deleted.
	int xcord;	//The unit's current x-coordinate on the map
	int ycord;	//The unit's current y-coordinate on the map
	int atkPow;	//The unit's strength; affects damage output
	int atkRange;	//The range (in tiles) for the unit's attacks. 1 = adjacent.
	int defPow;	//The unit's raw defense; lowers damage taken
	int movSpd;	//How many tiles the unit can move. 0 = Stationary.
	int cost;	//How many resources the unit will cost.
	int visionRange;	//How many tiles the unit can see in fog of war. 0 = only the occupied tile.
	int player;	//The player controlling the unit. 0 for computer, 1-4

	//The attack type represents the kind of damage a unit deals
	//This is compared to a unit's defense type to multiply or mitigate incoming damage
	AttackType atkType;	
	DefenseType defType;
	//The movement type determines movement penalties imposed by tiles
	//A flying unit is less affected by hills than a unit on foot, for example
	MoveType movType;

	FactionType faction; //The faction/race of the unit (0 for humans, 1 for plants). Determines various affects.
	//std::list<std::string> actions; //List of all the unit's potential actions. Implement differently?

public:
	Unit(int x, int y, int op); //This will eventually take in a text file as well
	~Unit(); 

	//Initialization
	static Unit* create(const cocos2d::Size tileSize, int x, int y, int op);
	virtual bool init(const cocos2d::Size tileSize, int x, int y, int op);

	/* Implement differently?
	std::list<std::string> getActions(); //Takes in all potential actions of unit and filters out current invalid ones, 
											//then returns a list of valid actions. "Attack" is not returned if there are no enemies in range, for example.
	std::list<std::string> uniqueActions(std::list<std::string> a); //Used for determining actions unique to a given unit type (like the Lumberjack's Capture ability)
	*/

	/* Takes incoming damage, factors in the type of damage, adjusts the damage 
	using the damage type/defense type table, adjusts damage for the unit's raw
	defensive power, then subtracts the final total from the unit's health.*/
	void takeDmg(int damage, AttackType dmgType); //Add something if health hits 0?
	
	/*Uses the unit's current health and raw damage output to return an int value of damage being dealt by the unit*/
	int dealDmg();

	/*Adds the amount to the unit's total health.
	The unit's total health may not go over MAX_HEALTH.
	Used with combine action, mostly.*/
	void heal(int amount);

	/*Delete the instance of the unit from the game.*/
	//void kill(); //Better way to kill units?

	//These return private variables
	int getHealth();
	int getAtkPow();
	int getDefPow();
	AttackType getAtkType();
	DefenseType getDefType();
	MoveType getMovType();
	int getAtkRange();
	int getMovSpd();
	int getVisionRange();
	int getCost();

	/*Returns true if the unit is still alive and can deal damage, 
	false if the unit has died. Used for counterattacks mid combat.*/
	bool canAttack();

	/*Returns true if the unit is damaged, but alive, and false if otherwise.
	Units cannot be combined if they are not damaged.*/
	bool canCombine();

	// Small getters
	int getX() { return xcord; }
	int getY() { return ycord; }

	bool isActive; //The unit can be selected by the player controlling it this turn
	bool isSelected;	//The unit is currently selected
};

#endif
