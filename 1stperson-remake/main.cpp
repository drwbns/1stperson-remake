#ifdef aa
// fix : v0.1 stupid 0.0 .....
//#include "main.h"
#include "stdafx.h"
#include <OgreException.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include "Application.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
// Create application object
	Application app;

	try {
		app.Initialize();
	}
	catch (Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(nullptr, (wchar_t*)e.getFullDescription().c_str(), L"An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		fprintf(stderr, "An exception has occured: %s\n",
			e.getFullDescription().c_str());
#endif
	}

	return 0;
}
#endif
