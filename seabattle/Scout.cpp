#include "Scout.h"
#include "BaseMapObj.h"
#include <iostream>
using namespace std;


Scout::Scout(void) : BaseMapObj(eTypeObj::SCOUT)
{
	mSize = 1;
	mBullets = 1;
	mDirection = HORISONTAL;
	mStatus = LIFE;
	mType = SCOUT;
}


Scout::~Scout(void)
{
}
