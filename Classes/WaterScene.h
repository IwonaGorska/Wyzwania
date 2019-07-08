//
//  WaterScene.h
//  Wyzwania
//
//  Created by Iwona Gorska on 05/01/2019.
//

#ifndef WaterScene_h
#define WaterScene_h
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

class WaterScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(WaterScene);
    
    virtual bool init() override;
    
    int touchID;
    Node* root;
    ui::Button* btnBack;
    Node* dragged;
    int draggedLocalZOrder; //do osi Z ukladu wspolrzednych
    Vec2 draggedStartPos;
    Vec2 draggedLastPos;
    Sprite* decorClock;
    Sprite* stones;
    Sprite* lilies;
    Sprite* jellyfish;
    Sprite* treasure;
    Sprite* turtle;
    Sprite* nest;
    Sprite* ducks;
    Sprite* duckMother;
    Sprite* cane;
    bool front;
    
    int avatar;
    string avatarString;
    
    int counterBad;
    int counterGood;
    Sprite* glass;
    cocos2d::ui::TextField* textGood;
    cocos2d::ui::TextField* textBad;
    float timeArriveToHook;
    
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) override;
    void onBackPressed();
//    void functionMoveFish();
    Sprite* exampleClock;
    Sprite* hook;
    int counter;
    int possibleClocksNr;
    int possibleFishNr;
    int whichFish;
    vector <string> hours;
    void functionMoveFish();
    Node* startNode;
    int randClockEx;
    int maxScore;
    
    ~WaterScene();
};

#endif /* WaterScene_h */
