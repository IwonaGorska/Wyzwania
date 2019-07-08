//
//  BasketScene.h
//  Wyzwania
//
//  Created by Iwona Gorska on 05/01/2019.
//

#ifndef BasketScene_h
#define BasketScene_h
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

class BasketScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(BasketScene);
    
    virtual bool init() override;
    
    int touchID;
    Node* root;
    ui::Button* btnBack;
    ui::Button* correctRegion;
    Node* dragged;
    int draggedLocalZOrder; //do osi Z ukladu wspolrzednych
    Vec2 draggedStartPos;
    Vec2 draggedLastPos;
    string name;
    string badName;
    string goodName;
    int counterAll;
    int category;
    int counterGood;
    int counterBad;
    int randBad;
    int counter;
    bool ended;
    
    int avatar;
    string avatarString;
    
//    int numbers7[7];
    vector<Sprite* > elements;
//    vector<int > zOrders;
    int draggedZOrder;
    vector<Node* > elemNodes;
    Vec2 placeNodeTree1;
    Vec2 placeNodeTree2;
    Sprite* basket;
    Sprite* categoryTitle;
    Sprite* fence;
    Sprite* tree_2;
    Sprite* tree_1;
    Sprite* tree_3;
    Sprite* tree_4;
    Sprite* panda;
    Sprite* bee;
    Vec2 beePos;
    Sprite* forest1;
    Sprite* forest2;
    Sprite* background;
    string fullName;
    Sprite* konfetti_1;
    Sprite* title;
    Sprite* camomile;
    vector<Sprite* > konfs;
    
    int categoryTitleNr;
    
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) override;
    void onBackPressed();
        
    ~BasketScene();
};

#endif /* BasketScene_h */
