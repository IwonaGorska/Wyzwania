//
//  ScoreScene.h
//  Wyzwania
//
//  Created by Iwona Gorska on 23/02/2019.
//
#ifndef ScoreScene_h
#define ScoreScene_h
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

class ScoreScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(ScoreScene);
    
    virtual bool init() override;

    Node* root;
    ui::Button* btnBack;
    void onBackPressed();
    
    Sprite* cup;
    Sprite* decor;
    vector <Sprite*> playersStairs;
    vector <Sprite*> players;
    vector <Sprite*> avatarsP;
    vector <Sprite*> avatarsS;
    vector <cocos2d::ui::TextField*> scoresT;
    vector <int> scores;
    vector <int> scoresCopy;
    
    ~ScoreScene();
};

#endif /* ScoreScene_h */
