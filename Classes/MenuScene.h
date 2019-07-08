#ifndef __MenuScene_SCENE_H__
#define __MenuScene_SCENE_H__

#include "cocos2d.h"
#include "NativeHelper.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "base/CCData.h"
#include "Utils.h"
#include <ctime>
#include "AppDelegate.h"
#include <spine/spine-cocos2dx.h>

class IndicatorNode;

USING_NS_CC;
using namespace std;
using namespace spine;

class MenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(MenuScene);
    
    virtual bool init() override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) override;
    void onBackPressed();
    
    float offsetX;
    float offsetY;
    
    bool clickBusy;
    float const clickTime = 0.3f;
    
    Node* root;
    ui::Button* btnBack;
    ui::Button* waterBtn;
    ui::Button* blocksBtn;
    ui::Button* hensBtn;
    ui::Button* basketBtn;

private:
    ~MenuScene();
};

#endif // __MenuScene_SCENE_H__
