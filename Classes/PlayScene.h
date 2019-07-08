//
//  PlayScene.h
//  NativeHelperTest
//
//  Created by Piotr on 04.03.2015.

#ifndef __NativeHelperTest__PlayScene__
#define __NativeHelperTest__PlayScene__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Utils.h"
#include "ui/UIButton.h"
#include "NativeHelper.h"
#include "AppDelegate.h"

class ParentGate;

USING_NS_CC;
using namespace cocostudio::timeline;

class PlayScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
//    virtual void update(float delta);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
    void onBackPressed();
    
    CREATE_FUNC(PlayScene);
    
    bool clickBusy;
    float const clickTime = 0.3f;
    
    Node* root;
    ui::Button* startBtn;
    ui::Button* scoreBtn;
    ui::Button* choiceBtn;
    ui::Button* instructionsBtn;
//    ui::Button* exitBtn;
    Sprite* logo;
    
private:
    ~PlayScene();
};

#endif /* defined(__NativeHelperTest__PlayScene__) */
