#include "stdafx.h"
#include "shareddata.h"

template<> SharedData* Ogre::Singleton<SharedData>::msSingleton = nullptr;

SharedData& SharedData::getSingleton()
{
	assert(msSingleton);  return (*msSingleton);
}

SharedData* SharedData::getSingletonPtr()
{
	return msSingleton;
}