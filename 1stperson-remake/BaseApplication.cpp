/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "BaseApplication.h"
#include <OgreWindowEventUtilities.h>
#include <OgreConfigFile.h>
#include <OgreTextureManager.h>
#include <OgreFrameListener.h>
#include <OgreMaterialManager.h>
#include <utility>


template<> BaseApplication* Singleton<BaseApplication>::msSingleton = nullptr;

BaseApplication* BaseApplication::getSingletonPtr(void)
{
	return msSingleton;
}

BaseApplication& BaseApplication::getSingleton(void)
{  
	assert( msSingleton );  return ( *msSingleton );
}

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mMode(0), lockedCam(0), yaw(0), pitch(0), moveScale(0), camToPlayer(0), mMouseSpeed(5),
    mRotateSpeed(50),
    mZoomSpeed(50),
    fixedCam(false),
    fixedYaw(false),
    fixedPitch(false),
    camRotate(false),
    yawHasBeenReset(false),
    pitchAngle(0),
    yawAngle(0),
    guiDbg(nullptr),
    mTimeUntilNextToggle(0),
    mFiltering(),
	mAniso(0),
	mSceneDetailIndex(0),
	mRoot(nullptr), mCamera(nullptr), mSceneMgr(nullptr), mWindow(nullptr), mResourcesCfg(BLANKSTRING), mPluginsCfg(BLANKSTRING), mTrayMgr(nullptr), mDetailsPanel(nullptr), mCursorWasVisible(false), mShutDown(false)
    {
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
    if (mTrayMgr) delete mTrayMgr;

    //Remove ourself as a Window listener
    WindowEventUtilities::removeWindowEventListener(mWindow, this);
	BaseApplication::windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Vector3(0,0,80));
    // Look back along -Z
    mCamera->lookAt(Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    size_t windowHnd = 0;
    std::stringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
	



    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    WindowEventUtilities::addWindowEventListener(mWindow, this);

	OgreBites::InputContext input;
	//input.mMouse = mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, input, this);

    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // create a params panel for displaying sample details
    StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
    // Load resource paths from config file
    ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    mRoot = new Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();

    createFrameListener();

    return true;
};
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    //Need to capture/update each device
    //mKeyboard->capture();
   // mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }


/*	if(!mChar->getAnimState()->hasEnded())
		mChar->getAnimState()->addTime(evt.timeSinceLastFrame);
	 else {
		 mChar->setAnimationState("Idle1");
		 mChar->getAnimState()->setLoop(true);
	 }
	 mKeyboard->capture();

	 // How close camera is to player
	Real camDistToPlayer = mChar->getCameraNode()->getPosition().z;

	// If in 1st person set camera's direction to player's facing direction
	if(camDistToPlayer < 10) { 
		fixedYaw = true;
		fixedPitch = false;
		if(yawHasBeenReset == false) {
			Quaternion quat = mChar->getPlayerNode()->getOrientation();
			mChar->getYawNode()->setOrientation(quat);
			yawHasBeenReset = true;
		}
		
	}*/
	/*
	
	if(fixedYaw == true){
		mChar->getPlayerNode()->getOrientation();
		mChar->getYawNode()->setOrientation(quat);
	}
	*/

/*	// Zoom in / out
	if(mMouse->getMouseState().Z.rel)
	{
		Real zDist;

		zDist = (mZoomSpeed * -0.05f * mMouse->getMouseState().Z.rel);
		if (mChar->getCameraNode()->getPosition().z + zDist >= 0 )
			mChar->getCameraNode()->translate(Vector3(0.0f, 0.0f, zDist));
	}

	// Used to determine negative or positive pitch
	Real angleSign;

	// If in 1st person, reverse up/down for free look style cam
	if(camDistToPlayer < 10) {
		
		angleSign = -1;
	}
	else {
		angleSign = 1;
		yawHasBeenReset = false;
	}


	// Allow pitching if not fixed
	if (fixedPitch == false) {

		// Pitch angle
		pitchAngle = (mMouse->getMouseState().Y.rel * evt.timeSinceLastFrame);

		mChar->getPitchNode()->pitch((Radian)pitchAngle * angleSign,Node::TS_LOCAL);

		// Angle of rotation around the X-axis.
		pitchAngle = (2 * Degree(Math::ACos(mChar->getPitchNode()->getOrientation().w)).valueDegrees());

		// Just to determine the sign of the angle we pick up above, the
		// value itself does not interest us.
		Real pitchAngleSign = mChar->getPitchNode()->getOrientation().x;

		// Limit the pitch between 0 degress and +90 degrees.
		
		
		if (pitchAngle > 90.0f)
		{
			if (pitchAngleSign > 0)
				 // Set orientation to 90 degrees on X-axis.
				 mChar->getPitchNode()->setOrientation(Quaternion(Math::Sqrt(0.5f),
																		Math::Sqrt(0.5f), 0, 0));
			else if (pitchAngleSign < 0)
				 // Sets orientation to -90 degrees on X-axis.
				 mChar->getPitchNode()->setOrientation(Quaternion(Math::Sqrt(0.5f),
																		-Math::Sqrt(0.5f), 0, 0));
		}
		
		// If cam is not first person
		if ( angleSign == 1 ) {

			//Limit the pitch if not in 1st person view
			if (pitchAngle > 0.0f && pitchAngleSign > 0)
				// Set orientation to 0 degrees on X-axis.
				mChar->getPitchNode()->setOrientation(Quaternion(Math::Sqrt(0.5f),
																	0, 0, 0));
		}
		
	}*/
	/*
	// First person camera rotate
	if (camDistToPlayer < 10){
		fixedYaw = true;
		fixedPitch = false;
		
		yawAngle = (0.872 * mMouse->getMouseState().X.rel * evt.timeSinceLastFrame);
		
		mChar->getYawNode()->yaw((Radian)yawAngle * angleSign,Node::TS_LOCAL);


	}
	*/
/*	if (mKeyboard->isKeyDown (OIS::KC_LCONTROL) && camDistToPlayer > 10) {
		fixedYaw = false;
		fixedPitch = false;

		yawAngle = (mMouse->getMouseState().X.rel * evt.timeSinceLastFrame);
		
		mChar->getYawNode()->yaw((Radian)yawAngle * angleSign,Node::TS_LOCAL);
	}
	// 3rd person fixed cam
	if (!mKeyboard->isKeyDown (OIS::KC_LCONTROL) && camDistToPlayer > 10) {
		fixedYaw = true;
		fixedPitch = false;
	}
	// end cam rotate stuff

	// Only rotate player if fixed pitch and yaw are true ( don't rotate player if rotating camera )
	if (fixedYaw == true && fixedPitch == true) {
		if (camDistToPlayer > 10)
			mChar->getMainNode()->rotate(Vector3::UNIT_Y,Degree(mRotateSpeed * -mMouse->getMouseState().X.rel * evt.timeSinceLastFrame));
	}
	// Allow player rotation for 1st person view
	if (fixedYaw == true && fixedPitch == false)
		mChar->getMainNode()->rotate(Vector3::UNIT_Y,Degree(mRotateSpeed * -mMouse->getMouseState().X.rel * evt.timeSinceLastFrame));

	 mChar->update (evt.timeSinceLastFrame, mKeyboard, mMouse);

	 if (mKeyboard->isKeyDown (OIS::KC_F1) && mTimeUntilNextToggle <= 0){
		 switch (mMode) {
			 case 0: // 3rd person chase
				 mChar->getCameraNode()->setPosition(Vector3 (0, 0, 300));
				 mChar->getSightNode()->setPosition(Vector3 (0, 0, -300));
				 mTimeUntilNextToggle = 0.5;
				 mMode = 1;
				 break;
			case 1: // 3rd person far chase cam
				 mChar->getCameraNode()->setPosition(Vector3 (0, 0, 600));
				 mChar->getSightNode()->setPosition(Vector3 (0, 0, -300));
				 mTimeUntilNextToggle = 0.5;
				 mMode = 2;
				 break; 
			 case 2: // 1st person
				 mChar->getCameraNode()->setPosition(Vector3 (0, 0, 0));
				 mChar->getSightNode()->setPosition(Vector3 (0, 0, -300));
				 mTimeUntilNextToggle = 0.5;
				 mMode = 0;
				 break;
				 
		 }
	 }*/
	// end View changes


	 // Switch whether cam is locked to player or free 
	 //( **** fixed cam while not rotating with CTRL button ( fixedCam = true ) ****)
/*	 if (mKeyboard->isKeyDown (OIS::KC_F5) && mTimeUntilNextToggle <= 0) {
		 switch(fixedCam){
				case 0:
					fixedCam = true;
					mTimeUntilNextToggle = 0.5;
					break;
				case 1:
					fixedCam = false;
					mTimeUntilNextToggle = 0.5;
					break;
		 }
	 }

	if( mKeyboard->isKeyDown(OIS::KC_F) && mTimeUntilNextToggle <= 0 )
	{
		mTimeUntilNextToggle = 1;
	}

	if( mKeyboard->isKeyDown(OIS::KC_T) && mTimeUntilNextToggle <= 0 )
	{
		switch(mFiltering)
		{
		case TFO_BILINEAR:
			mFiltering = TFO_TRILINEAR;
			mAniso = 1;
			break;
		case TFO_TRILINEAR:
			mFiltering = TFO_ANISOTROPIC;
			mAniso = 8;
			break;
		case TFO_ANISOTROPIC:
			mFiltering = TFO_BILINEAR;
			mAniso = 1;
			break;
		default: break;
		}
		MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
		MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);

		mTimeUntilNextToggle = 1;
	}*/

//	if(mKeyboard->isKeyDown(OIS::KC_SYSRQ) && mTimeUntilNextToggle <= 0)
//	{
//		std::ostringstream ss;
//		BASE->getWindow()->writeContentsToFile(ss.str());
//		mTimeUntilNextToggle = 0.5;
//	}

//	if(mKeyboard->isKeyDown(OIS::KC_R) && mTimeUntilNextToggle <=0)
//	{
//		mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
//		switch(mSceneDetailIndex) {
//			case 0 : BASE->getCamera()->setPolygonMode(PM_SOLID); break;
//			case 1 : BASE->getCamera()->setPolygonMode(PM_WIREFRAME); break;
//			case 2 : BASE->getCamera()->setPolygonMode(PM_POINTS); break;
//		}
//		mTimeUntilNextToggle = 0.5;
//	}

	//static bool displayCameraDetails = false;
//	if(mKeyboard->isKeyDown(OIS::KC_P) && mTimeUntilNextToggle <= 0)
//	{
//		displayCameraDetails = !displayCameraDetails;
//		mTimeUntilNextToggle = 0.5;
//	}

	// Decrease time for next available keypress
	if (mTimeUntilNextToggle >= 0)
		mTimeUntilNextToggle -= evt.timeSinceLastFrame;

	 // Exit if we press Esc
//	 if(mKeyboard->isKeyDown (OIS::KC_ESCAPE))
//		 return false;

    return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        String newVal;
        TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = TFO_BILINEAR;
            aniso = 1;
        }

        MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        String newVal;
        PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case PM_SOLID:
            newVal = "Wireframe";
            pm = PM_WIREFRAME;
            break;
        case PM_WIREFRAME:
            newVal = "Points";
            pm = PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
    return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
//	if (arg.key == OIS::KC_W){
//		mChar->setAnimationState("Idle1");
//	}
    return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectPointerMove(arg)) return true;
    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectPointerDown(arg, id)) return true;
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectPointerUp(arg, id)) return true;
    return true;
}

//Adjust mouse clipping area
void BaseApplication::windowResized(RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

   // const OIS::MouseState &ms = mMouse->getMouseState();
   // ms.width = width;
    //ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
//        if( mInputManager )
//        {
//            mInputManager->destroyInputObject( mMouse );
//            mInputManager->destroyInputObject( mKeyboard );
//
//            OIS::InputManager::destroyInputSystem(mInputManager);
//            mInputManager = 0;
//        }
    }
}
