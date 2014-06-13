#pragma once

enum eTypeObj
{
	SCOUT = 0,
	FREGAT,
	LINKOR,
	AIRCRAFT,
	ANTIAIRCRAFT
};

enum eDirect
{
	VERTICAL = 0,
	HORISONTAL
};

enum eStatus
{
	LIFE = 0,
	DESTROYED
};

enum eHeadOrBoby
{
	HEAD = 0,
	BODY
};

class BaseMapObj
{
public:
	BaseMapObj(eTypeObj mType = SCOUT);
	virtual ~BaseMapObj(void);


	// SET //////////////////////////////
	void setPositionX(int aX);
	void setPositionY(int aY);
	void setSizeObj(int aSize);
	void setBullets(int aBullets);
	void setTypeObj(eTypeObj aTypeObj);
	void setDirectObj(eDirect aDirection);
	void setStatusObj(eStatus aStatus);
	void setHeadOrBody(eHeadOrBoby aHeadOrBody);
	void setShipId(int aId);
	// END SET //////////////////////////

	// GET //////////////////////////////
	int getPositionX();
	int getPositionY();
	int getSizeObj();
	int getBullets();
	eTypeObj getTypeObj();
	eDirect getDirectObj();
	eStatus getStatusObj();
	eHeadOrBoby getHeadOrBody();
	int getShipId();
	// END GET //////////////////////////
	void shoot(BaseMapObj *enemyShip);
		//this method makes shots.


protected:
	int mId;
	int mPositionX;
	int mPositionY;
	int mSize;
		// quantity of parts of the obj.
	int mBullets;
		// quantity of bullets.
	eTypeObj mType;
	eDirect mDirection;
	eStatus mStatus;
	eHeadOrBoby mHeadOrBody;
};

