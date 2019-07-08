//
//  AvatarsScene.h
//  Wyzwania
//
//  Created by Iwona Gorska on 23/02/2019.
//

#ifndef AvatarsScene_h
#define AvatarsScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Utils.h"
#include "ui/UIButton.h"
#include "NativeHelper.h"
#include "AppDelegate.h"
#include "PlayScene.h"
#include "MenuScene.h"

#include "HitDetectHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "SimpleAudioEngine.h"
#else
#include "AudioEngine.h"
#endif

class AvatarsScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(AvatarsScene);
    
    virtual bool init() override;
    
    Node* root;
    bool clickBusy;
    float const clickTime = 0.3f;
    int avatar;
    Sprite* backYellow;
    ui::Button* btnBack;
    vector<ui::Button*> buttons;
    void onBackPressed();
    
    ~AvatarsScene();
};

#endif /* AvatarsScene_h */
