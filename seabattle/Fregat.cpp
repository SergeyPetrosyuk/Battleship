#include "Fregat.h"


Fregat::Fregat(void) : BaseMapObj(eTypeObj::FREGAT)
{
	mSize = 2;
	mBullets = 2;
	mDirection = HORISONTAL;
	mStatus = LIFE;
	mType = FREGAT;
}


Fregat::~Fregat(void)
{
}