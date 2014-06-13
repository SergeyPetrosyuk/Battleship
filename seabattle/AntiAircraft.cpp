#include "AntiAircraft.h"


AntiAircraft::AntiAircraft(void) : BaseMapObj(eTypeObj::ANTIAIRCRAFT)
{
	mSize = 1;
	mBullets = 1;
	mDirection = HORISONTAL;
	mStatus = LIFE;
	mType = ANTIAIRCRAFT;
}


AntiAircraft::~AntiAircraft(void)
{
}