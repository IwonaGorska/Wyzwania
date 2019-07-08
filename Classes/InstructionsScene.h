//
//  InstructionsScene.h
//  Wyzwania
//
//  Created by Iwona Gorska on 09/03/2019.
//

#ifndef InstructionsScene_h
#define InstructionsScene_h

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

class InstructionsScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(InstructionsScene);
    
    virtual bool init() override;
    
    Node* root;
    ui::Button* btnBack;
    ui::Button* btnRead;
    Sprite* speaker;
    
    bool isSpeaking;
    int lectorSound;
    
//    cocos2d::ui::TextField* instr;
    
    void onBackPressed();
    
    ~InstructionsScene();
};

#endif /* InstructionsScene_h */
