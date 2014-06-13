#include "Linkor.h"


Linkor::Linkor(void) : BaseMapObj(eTypeObj::LINKOR)
{
	mSize = 3;
	mBullets = 3;
	mDirection = HORISONTAL;
	mStatus = LIFE;
	mType = LINKOR;
}


Linkor::~Linkor(void)
{
}
