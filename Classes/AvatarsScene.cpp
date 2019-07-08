//
//  AvatarsScene.cpp
//  Wyzwania
//
//  Created by Iwona Gorska on 23/02/2019.
//

#include <stdio.h>
#include "AvatarsScene.h"

Scene* AvatarsScene::createScene()
{
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = AvatarsScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    return scene;
}

bool AvatarsScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    uPreloadEffect("Click");
    
    root = CSLoader::createNode("AvatarsScene.csb");
    addChild(root);
    
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);
    
    clickBusy = false;
    
    avatar = UserDefault::getInstance()->getIntegerForKey("avatar", 1);
    
    backYellow = root->getChildByName<Sprite* >("backYellow");
    auto actionBack = Sequence::create(
      DelayTime::create(2.0f),
      FadeOut::create(5.0f),
      DelayTime::create(2.0f),
      FadeIn::create(5.0f),
      nullptr);
    backYellow->runAction(Repeat::create((actionBack), -1));

    btnBack = root->getChildByName<ui::Button* >("btnBack");
    btnBack->setPosition(btnBack->getPosition() + Point(-offsetX + safeAreaX, offsetY));
    btnBack->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
       {
           if (eventType != ui::Widget::TouchEventType::ENDED) return;
           
           if(clickBusy) return;
           clickBusy = true;
           root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
           
           uPlayEffect("Click", false);
           root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
            {
                onBackPressed();
            })));
       });
    
    for(int i = 1; i <= 5; i++)
    {
        auto button = root->getChildByName<ui::Button* >("Button" + to_str(i));
        button->setTag(i);
        buttons.push_back(button);
    }
    
    for(int i = 0; i < 5; i++)
    {
        auto button = buttons.at(i);
        if(button->getTag() == avatar)
        {
            button->loadTextures("gold.png", "gold.png");
        }
            
        button->addTouchEventListener([&, i, button](Ref* ref, ui::Widget::TouchEventType eventType)
          {
              if (eventType != ui::Widget::TouchEventType::ENDED) return;
              if(clickBusy) return;
              clickBusy = true;
              uPlayEffect("Click", false);
              root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
              UserDefault::getInstance()->setIntegerForKey("avatar", button->getTag());
              avatar = button->getTag();
              for(int i = 0; i < 5; i++)
              {
                  buttons.at(i)->loadTextures("blue.png", "blue.png");
              }
              button->loadTextures("gold.png", "gold.png");
          });
    }
    
    return true;
}

void AvatarsScene::onBackPressed()
{
    auto scene = PlayScene::createScene();
    replaceSceneBackward(scene, btnBack->getPosition());
}

AvatarsScene::~AvatarsScene()
{
    
}

