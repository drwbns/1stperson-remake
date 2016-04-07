/*
-----------------------------------------------------------------------------
Filename:    mainApplication.h
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
#ifndef __mainApplication_h_
#define __mainApplication_h_



#include "BaseApplication.h"
//#include "Sample_TileMesh.h"
//#include "Character.h"


class SampleApplication : public BaseApplication
 {
 public:
     SampleApplication()
     {
     }
     ~SampleApplication()
     {
     }
 protected:
     // Just override the mandatory create scene method
	void createScene(void) override;

	void destroyScene(void) override
     {
     }

 };


#endif // #ifndef __mainApplication_h_
