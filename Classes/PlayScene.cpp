//
//  PlayScene.cpp
//  NativeHelperTest
//  Wyzwania
//
//  Created by Iwona Gorska on 03/10/2018.
//

#include "PlayScene.h"
#include "ScoreScene.h"
#include "AvatarsScene.h"
#include "InstructionsScene.h"

Scene* PlayScene::createScene()
{
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = PlayScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    return scene;
}

bool PlayScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    root = CSLoader::createNode("PlayScene.csb");
    addChild(root);
    
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);
    
    uPreloadEffect("Click");
    
    clickBusy = false;

    setOnEnterCallback([&]()
       {
           if (!uIsBackgroundMusicPlaying())
           {
               uPlayBackgroundMusic("MainBackground");
           }
       });
  
    startBtn = root->getChildByName<ui::Button* >("startBtn");
//    startBtn->runAction(
//           RepeatForever::create(
//             EaseQuadraticActionInOut::create(
//                  Sequence::create(
//                       ScaleTo::create(.75, .9),
//                       ScaleTo::create(.75, 1),
//                       nullptr)
//                  )
//             )
//           );
    
    startBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType touchEventType)
       {
           if (touchEventType != ui::Widget::TouchEventType::ENDED) return;
           
           if(clickBusy) return;
           clickBusy = true;
           root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
           
           uPlayEffect("Click", false);
           root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
            {
                auto scene = MenuScene::createScene();
                replaceSceneForward(scene);
            })));
       });
    
    scoreBtn = root->getChildByName<ui::Button* >("scoreBtn");
//    scoreBtn->setPosition(scoreBtn->getPosition()); //PROBLEM JEST Z SAMYM PRZYCISKIEM, NIE ZE SCENA SCORESCENE JEDNAK
    scoreBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType touchEventType)
    {
        if (touchEventType != ui::Widget::TouchEventType::ENDED) return;
        
        if(clickBusy) return;
        clickBusy = true;
        root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
        
        uPlayEffect("Click", false);
        root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
         {
             auto scene = ScoreScene::createScene();
             replaceSceneForward(scene);
         })));
    });
    
    
    choiceBtn = root->getChildByName<ui::Button* >("choiceBtn");
    choiceBtn->setPosition(choiceBtn->getPosition());
    choiceBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType touchEventType)
    {
        if (touchEventType != ui::Widget::TouchEventType::ENDED) return;
        
        if(clickBusy) return;
        clickBusy = true;
        root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
        
        uPlayEffect("Click", false);
        root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
         {
             auto scene = AvatarsScene::createScene();
             replaceSceneForward(scene);
         })));
    });
    
    instructionsBtn = root->getChildByName<ui::Button* >("instructionsBtn");
    instructionsBtn->setPosition(instructionsBtn->getPosition());
    instructionsBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType touchEventType)
     {
         if (touchEventType != ui::Widget::TouchEventType::ENDED) return;
         
         if(clickBusy) return;
         clickBusy = true;
         root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
         
         uPlayEffect("Click", false);
         root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
          {
              auto scene = InstructionsScene::createScene();
              replaceSceneForward(scene);
          })));
     });
    
//    exitBtn = root->getChildByName<ui::Button* >("exitBtn");
//    exitBtn->setPosition(exitBtn->getPosition());
//    exitBtn->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType touchEventType)
//    {
//        if (touchEventType != ui::Widget::TouchEventType::ENDED) return;
//
//        if(clickBusy) return;
//        clickBusy = true;
//        root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){clickBusy = false;}), NULL));
//
//        uPlayEffect("Click", false);
//        root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
//         {
////            #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
////                exit(1); // CIEKAWE CZY NA ANDROIDZIE ZADZIALA WGL
////            #else
//             Director::getInstance()->end();
////             #endif
//         })));
//    });
 
    logo = root->getChildByName<Sprite* >("Logo");
    logo->setPosition(logo->getPosition());
    logo->setVisible(false);
    
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(PlayScene::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    setonEnterTransitionDidFinishCallback([&](){
        Vec2 p = logo->getPosition();
        logo->setPosition(p + Vec2(0, 400));
        logo->setVisible(true);
        float t = 1.0f;
        logo->runAction(
                        EaseBounceOut::create(
                                              Spawn::createWithTwoActions(
                                                                          MoveTo::create(t, p),
                                                                          Sequence::create(
                                                                                           DelayTime::create(0.7f * t),
                                                                                           ScaleTo::create(0.15f * t, 1.35f, 1.0f),
                                                                                           ScaleTo::create(0.15f * t, 1.0f, 1.0f),
                                                                                           NULL)
                                                                          )
                                              )
                        );
    });
    
    return true;
}

void PlayScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
        onBackPressed();
    }
}

void PlayScene::onBackPressed()
{
    uPlayEffect("menu");
    Director::getInstance()->end();
}

PlayScene::~PlayScene()
{
}
