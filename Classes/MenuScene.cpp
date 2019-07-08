//  Wyzwania
//
//  Created by Iwona Gorska on 03/10/2018.
//

#include "MenuScene.h"
#include "BlocksScene.h"
#include "WaterScene.h"
#include "BasketScene.h"
#include "HensScene.h"

#include <string>
USING_NS_CC;

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = MenuScene::create();
    layer->setScale(gameScale);
    
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    uPreloadEffect("Click");
    
    clickBusy = false;
    
    setOnEnterCallback([&]()
       {
           if (!uIsBackgroundMusicPlaying())
           {
               uPlayBackgroundMusic("MainBackground");
           }
       });
    
    root = CSLoader::createNode("MenuScene.csb");
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);
    
    NativeHelper::adsSceneEnter("Menu", 1);
    NativeHelper::reportScreenChange("Menu");
    
    waterBtn = root->getChildByName<ui::Button* >("waterBtn");
    waterBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
    {
        if (eventType != ui::Widget::TouchEventType::ENDED) return;
        
        if(clickBusy) return;
        clickBusy = true;
        root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
        
        uPlayEffect("Click", false);
        auto scene = WaterScene::createScene();
        replaceSceneBackward(scene, waterBtn->getPosition());
    });
    
    blocksBtn = root->getChildByName<ui::Button* >("blocksBtn");
    blocksBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
    {
        if (eventType != ui::Widget::TouchEventType::ENDED) return;
        
        if(clickBusy) return;
        clickBusy = true;
        root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
        
        uPlayEffect("Click", false);
        auto scene = BlocksScene::createScene();
        replaceSceneBackward(scene, blocksBtn->getPosition());
    });
    
    hensBtn = root->getChildByName<ui::Button* >("hensBtn");
    hensBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
     {
         if (eventType != ui::Widget::TouchEventType::ENDED) return;
         
         if(clickBusy) return;
         clickBusy = true;
         root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
         
         uPlayEffect("Click", false);
         auto scene = HensScene::createScene();
         replaceSceneBackward(scene, hensBtn->getPosition());
     });
    
    basketBtn = root->getChildByName<ui::Button* >("basketBtn");
    basketBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
    {
        if (eventType != ui::Widget::TouchEventType::ENDED) return;
        
        if(clickBusy) return;
        clickBusy = true;
        root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
        
        uPlayEffect("Click", false);
        auto scene = BasketScene::createScene();
        replaceSceneBackward(scene, basketBtn->getPosition());
    });
    
    btnBack = root->getChildByName<ui::Button* >("btnBack");
    btnBack->setPosition(btnBack->getPosition() + Point(-offsetX + safeAreaX, offsetY));
    btnBack->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
    {
        if (eventType != ui::Widget::TouchEventType::ENDED) return;
        
        if(clickBusy) return;
        clickBusy = true;
        root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
        
        uPlayEffect("Click", false);
        onBackPressed();
    });
    
    addChild(root);
  
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    return true;
}

void MenuScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
        onBackPressed();
    }
}

void MenuScene::onBackPressed()
{
    auto scene = PlayScene::createScene();
    replaceSceneBackward(scene, btnBack->getPosition());
}


MenuScene::~MenuScene()
{

}
