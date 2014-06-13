#include "BaseMapObj.h"


BaseMapObj::BaseMapObj(eTypeObj mType):
	mPositionX(0),
	mPositionY(0),
	mSize(1),
	mBullets(1),
	mType(mType),
	mStatus(LIFE),
	mDirection(HORISONTAL),
	mHeadOrBody(HEAD),
	mId(0)
{
}


BaseMapObj::~BaseMapObj(void)
{
}

// SET //////////////////////////////////////////

void BaseMapObj::setPositionX(int aX)
{
	mPositionX = aX;
}

void BaseMapObj::setPositionY(int aY)
{
	mPositionY = aY;
}

void BaseMapObj::setSizeObj(int aSize)
{
	mSize = aSize;
}

void BaseMapObj::setBullets(int aBullets)
{
	mBullets = aBullets;
}

void BaseMapObj::setTypeObj(eTypeObj aTypeObj)
{
	mType = aTypeObj;
}

void BaseMapObj::setDirectObj(eDirect aDirection)
{
	mDirection = aDirection;
}

void BaseMapObj::setStatusObj(eStatus aStatus)
{
	mStatus = aStatus;
}

void BaseMapObj::setHeadOrBody(eHeadOrBoby aHeadOrBody)
{
	mHeadOrBody = aHeadOrBody;
}

void BaseMapObj::setShipId(int aId)
{
	mId = aId;
}

// END SET //////////////////////////////////////

// GET //////////////////////////////////////////
int BaseMapObj::getPositionX()
{
	return mPositionX;
}

int BaseMapObj::getPositionY()
{
	return mPositionY;
}

int BaseMapObj::getSizeObj()
{
	return mSize;
}

int BaseMapObj::getBullets()
{
	return mBullets;
}

eTypeObj BaseMapObj::getTypeObj()
{
	return mType;
}

eDirect BaseMapObj::getDirectObj()
{
	return mDirection;
}

eStatus BaseMapObj::getStatusObj()
{
	return mStatus;
}

eHeadOrBoby BaseMapObj::getHeadOrBody()
{
	return mHeadOrBody;
}

int BaseMapObj::getShipId()
{
	return mId;
}

// END GET //////////////////////////////////////

void BaseMapObj::shoot(BaseMapObj *enemyShip)
{
	enemyShip->setStatusObj(DESTROYED);
}// shooting