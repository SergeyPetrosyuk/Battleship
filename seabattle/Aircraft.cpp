#include "Aircraft.h"


Aircraft::Aircraft(void) : BaseMapObj(eTypeObj::AIRCRAFT)
{
	mSize = 4;
	mBullets = 3;
	mDirection = HORISONTAL;
	mStatus = LIFE;
	mType = AIRCRAFT;
}


Aircraft::~Aircraft(void)
{
}