//
//  BlocksScene.h
//  Wyzwania
//
//  Created by Iwona Gorska on 05/01/2019.
//

#ifndef BlocksScene_h
#define BlocksScene_h
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

class BlocksScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(BlocksScene);
    
    virtual bool init() override;
    
    int touchID;
    Node* root;
    Node* start;
    ui::Button* btnBack;
    Node* dragged;
    int draggedLocalZOrder; //do osi Z ukladu wspolrzednych
    Vec2 draggedStartPos;
    Vec2 draggedLastPos;
    
    Sprite* animal;
    Sprite* shelf;
    Sprite* armchair;
    Sprite* table;
    Sprite* shelf_1;
    Sprite* shelf_2;
    Sprite* cupboard;
    Sprite* bigDog;
    Sprite* dog;
    int addHeight;
    int currentRoom;
    int numberOfParts;
    int counter;
    Node* node1;
    Node* node;
    vector<Sprite* > parts;
    vector<Sprite* > correctParts;
    bool dogHiden;
    
    bool finishedLevel;
    
    int avatar;
    string avatarString;
    
    int nameNr;
    vector<string > names;
    
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) override;
    void onBackPressed();
    void bigDogShow();
    void animalShow();
    
    ~BlocksScene();
};

#endif /* BlocksScene_h */
