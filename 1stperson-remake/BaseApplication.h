/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
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
#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>

//#include <OIS/OISKeyboard.h>
//#include <OIS/OISMouse.h>

#include <SdkTrays.h>

//#include "Character.h"
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>

#define BASE BaseApplication::getSingletonPtr()

using namespace Ogre;

class BaseApplication : public Singleton<BaseApplication>, public FrameListener, public WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    BaseApplication(void);
    virtual ~BaseApplication(void);

	static BaseApplication& getSingleton();
	static BaseApplication* getSingletonPtr();

	RenderWindow* getWindow() { return mWindow; };
	//OIS::Mouse * getMouse() { return mMouse; };
	//OIS::Keyboard* getKeyboard() { return mKeyboard; }
	Camera* getCamera() { return mCamera; }
	SceneManager* getSceneMgr() { return mSceneMgr; }
	Root* getRoot() { return mRoot; } 
	//OIS::InputManager* getInputMgr() { return mInputManager; } 

    virtual void go(void);

		// References to the main character and the camera
	//OgreCharacter *mChar;

	// Camera mode - Now supports 1st person, 3rd person (chasing) and 3rd person (fixed)
	unsigned int mMode;

	// Whether Cam is locked or not, true at initialization
	int lockedCam;
	float yaw;
	float pitch;

	Real moveScale;
	Real camToPlayer;
	int mMouseSpeed, mRotateSpeed, mZoomSpeed;
	bool fixedCam, fixedYaw, fixedPitch;
	bool camRotate;
	bool yawHasBeenReset;
	Real pitchAngle, yawAngle;
	Quaternion qr;

	OverlayElement* guiDbg;
	float mTimeUntilNextToggle;
	TextureFilterOptions mFiltering;
	size_t mAniso;
	int mSceneDetailIndex;

protected:
     bool setup();
     bool configure(void);
     void chooseSceneManager(void);
     void createCamera(void);
	virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);

    // Ogre::FrameListener
	bool frameRenderingQueued(const FrameEvent& evt) override;

    // OIS::KeyListener
	bool keyPressed( const OIS::KeyEvent &arg ) override;
	bool keyReleased( const OIS::KeyEvent &arg ) override;
    // OIS::MouseListener
	bool mouseMoved( const OIS::MouseEvent &arg ) override;
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) override;
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) override;

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
	void windowResized(RenderWindow* rw) override;
    //Unattach OIS before window shutdown (very important under Linux)
	void windowClosed(RenderWindow* rw) override;

    Root *mRoot;
    Camera* mCamera;
    SceneManager* mSceneMgr;
    RenderWindow* mWindow;
    String mResourcesCfg;
    String mPluginsCfg;

    // OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;
};

#endif // #ifndef __BaseApplication_h_
