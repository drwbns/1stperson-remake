// fix : v0.1 stupid 0.0 .....
#include "stdafx.h"
#include "main.h"
#include <OgreSceneNode.h>
#include "shareddata.h"

//#include <Application.h>


void SampleApplication::createScene()
{
	// Set ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.2, 0.2, 0.2));

	// LIGHTS!!
	// Create a point light
	Light* l = mSceneMgr->createLight("MainLight");
	// Accept default settings: point light, white diffuse, just set position
	// NB I could attach the light to a SceneNode if I wanted it to move automatically with
	//  other objects, but I don't
	l->setType(Light::LT_DIRECTIONAL);
	l->setDirection(-0.5, -0.5, 0);

	// CAMERA!!
	mCamera->setPosition(0, 0, 0); // Required or else the camera will have an offset

								   // ACTION!!!
								   // Fill the scene with some razors
//	SceneNode* razorNode;
//	Entity* razorEntity;
//	for (unsigned int i = 0; i < 30; ++i)
//	{
//		razorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(StringConverter::toString(i), Vector3(Math::RangeRandom(-1000, 1000), 0, Math::RangeRandom(-1000, 1000)));
//		razorEntity = mSceneMgr->createEntity(StringConverter::toString(i), "razor.mesh");
//		razorNode->attachObject(razorEntity);
//	}

	// creating plane
//	Plane plane(Vector3::UNIT_Y, 0);
//	MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//		plane, 12000, 12000, 1, 1, true, 1, 50, 50, Vector3::UNIT_Z);
//	// placing plane
//	Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
//
//	mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, 0, 0))->attachObject(entGround);
//
//	// texturing plane
//	entGround->setMaterialName("Examples/Rockwall");
//	entGround->setCastShadows(false);

	// Main character
	// mChar = new OgreCharacter ("Ogre 1", mSceneMgr);
	// Frame listener to manage both character and camera updating and different camera modes
	// Need to create it here as we want to change some parameters here, thus avoiding defining 
	// ogre and exCamera as member variables
//	mChar->getCameraNode()->attachObject(mCamera);

	// Initialize shared data, can use BASE instead ???
	new SharedData();
	SharedData::getSingleton().iGlobalActivate = true;
	SharedData::getSingleton().iCamera = mCamera;
	SharedData::getSingleton().iSceneMgr = mSceneMgr;
	SharedData::getSingleton().iRoot = mRoot;
	SharedData::getSingleton().iWindow = mWindow;

	//NavMesher * nv = new NavMesher();
	//Sample_TileMesh * tileMesh = new Sample_TileMesh();
	//if(!tileMesh->handleBuild())  Exception(Exception::ERR_ITEM_NOT_FOUND,"No tile nav Mesh!","TileMesh::Build");
	//if(!nv->Build()) Ogre::Exception(Exception::ERR_ITEM_NOT_FOUND,"No nav Mesh!","NavMesher::Build");
}
/*
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
 #define WIN32_LEAN_AND_MEAN
 #include "windows.h"
 
 INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
 #else
 int main(int argc, char **argv)
 #endif
 {
	 UNREFERENCED_PARAMETER(hInst);
	 UNREFERENCED_PARAMETER(strCmdLine);
    // Create application object
	// Creating the manager. This may only be called once!
	 Application * app = new Application();
	 app->go();
	 //Application app;

	 // Obtaining a reference to the manager instance
	 //Application& app = Application::getSingleton();
 
//    try {
//		//app.Initialize();
//    } catch( Exception& e ) {
// #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
//        MessageBox( nullptr, e.getFullDescription().c_str(), "An exception has occured!", MB_OK |      MB_ICONERROR | MB_TASKMODAL);
// #else
//        fprintf(stderr, "An exception has occured: %s\n",
//                e.getFullDescription().c_str());
// #endif
//    }
 
    return 0;
 }*/