//
//  PlayScene.h
//  NativeHelperTest
//  Wyzwania
//
//  Created by Iwona Gorska on 03/10/2018.
//

#ifndef __NativeHelperTest__LoadingScene__
#define __NativeHelperTest__LoadingScene__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Utils.h"
#include "ui/UIButton.h"
#include "NativeHelper.h"
#include "AppDelegate.h"
#include "PlayScene.h"

USING_NS_CC;
using namespace cocostudio::timeline;

class LoadingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void update(float delta);
    CREATE_FUNC(LoadingScene);
    Sprite* background;
    bool initialized;
    
private:
    ~LoadingScene();
};

#endif /* defined(__NativeHelperTest__LoadingScene__) */
