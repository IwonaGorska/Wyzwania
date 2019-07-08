//
//  HensScene.h
//  Wyzwania
//
//  Created by Iwona Gorska on 05/01/2019.
//

#ifndef HensScene_h
#define HensScene_h
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

class HensScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(HensScene);
    
    virtual bool init() override;
    
    int touchID;
    Node* root;
    
    Node* rightNode;
    Node* leftNode;

//    Node* startNode;
    ui::Button* btnBack;
    Node* dragged;
    int draggedLocalZOrder; //do osi Z ukladu wspolrzednych
    Vec2 draggedStartPos;
    Vec2 draggedLastPos;
    
    int speed;
    int counterBad;
    int counterGood;
    int numberGoodMax;

//    Sprite* background;
//    Sprite* seat_1;
//    Sprite* seat_2;
    
    Sprite* redHood;
    Sprite* board;
    ui::Button* basket;
    ui::Button* floor;
    ui::Button* leftArrow;
    ui::Button* rightArrow;
    int previousPos;
    bool isClickBusy;
    int clickTime;
    bool rotation;
    float boardYMove;
    int maxScore;
    
    int avatar;
    string avatarString;
    
    cocos2d::ui::TextField* textGood;
    cocos2d::ui::TextField* textBad;
    
    Sprite* spider0;
    Sprite* spider1;
    Sprite* spider2;
    Sprite* spider3;
    Vec2 spider1StartPos;
    Vec2 spider2StartPos;
    Vec2 spider3StartPos;
    vector <Sprite*> hens;
    
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) override;
    void onBackPressed();
    void functionMoveEgg();
    
    ~HensScene();
};

#endif /* HensScene_h */
