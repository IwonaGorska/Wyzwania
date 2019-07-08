//
//  HensScene.cpp
//  Wyzwania
//
//  Created by Iwona Gorska on 05/01/2019.
//

#include <stdio.h>
#include "HensScene.h"

USING_NS_CC;

Scene* HensScene::createScene(){
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = HensScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    return scene;
}

bool HensScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    root = CSLoader::createNode("HensScene.csb");
    addChild(root);
    
    uStopBackgroundMusic();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopAllEffects();
    audio->setEffectsVolume(10.0f);
#else
    cocos2d::experimental::AudioEngine::stopAll();
#endif
    
    uPreloadEffect("Fanfary");
    uPreloadEffect("HensBackground");
    uPreloadEffect("RedHoodStep");
    uPreloadEffect("EggCrack");
    uPreloadEffect("EggKept");
    uPreloadEffect("dinoKept");
    uPlayEffect("HensBackground", true);
    uPreloadEffect("badScore");
    uPreloadEffect("withoutEnthusiasm");
    uPreloadEffect("Click");

    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);
    
    avatar = UserDefault::getInstance()->getIntegerForKey("avatar", 1);
    avatarString = "points" + to_str(avatar);
    
    clickTime = 0.15f;
    isClickBusy = false;
    rotation = false;
    boardYMove = 500;

    speed = UserDefault::getInstance()->getIntegerForKey("speed", 3);// tak jakby 15 poziomow szybkosci powstalo xdd
    if(speed <= 0.2)
        UserDefault::getInstance()->setIntegerForKey("speed", 3);
    else
        UserDefault::getInstance()->setIntegerForKey("speed", speed - 0.2);
    
    speed = 5.0f;
    previousPos = 0;
    
    //speed = 3; //ile sekund pomiedzy jajkami
    counterBad = 0;//ile zlych zlapano
    counterGood = 0; //ile dobrych zlapano
    numberGoodMax = 15; //jak spadnie tyle to koniec gry
    maxScore = 15;
    // za dobre roztrzaskane nie odliczam punktów, bo juz wystarczy ze nie dostal pozytywanego punktu
    
    board = root->getChildByName<Sprite* >("board");
    board->setPosition(board->getPosition() + Vec2(0, -boardYMove));
    board->setLocalZOrder(50);
    
    textGood = board->getChildByName<cocos2d::ui::TextField* >("textGood");
    textGood->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    textGood->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
    textBad = board->getChildByName<cocos2d::ui::TextField* >("textBad");
    textBad->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    textBad->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
//    background = root->getChildByName<Sprite* >("background");
//    background->setLocalZOrder(-20);
//    seat_1 = root->getChildByName<Sprite* >("seat_1");
//    seat_1->setLocalZOrder(-20);
//    seat_2 = root->getChildByName<Sprite* >("seat_2");
//    seat_2->setLocalZOrder(-20);
    
    redHood = root->getChildByName<Sprite* >("redHood");
    redHood->setLocalZOrder(1);
    
    rightNode = root->getChildByName<Node* >("rightNode");
    leftNode = root->getChildByName<Node* >("leftNode");
    basket = root->getChildByName<ui::Button* >("basket");
    floor = root->getChildByName<ui::Button* >("floor");
    
    leftArrow = root->getChildByName<ui::Button* >("leftArrow");
    leftArrow->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
        {
            if (eventType != ui::Widget::TouchEventType::ENDED) return;
            if(isClickBusy) return;
            isClickBusy = true;
            if(!rotation)
            {
                uPlayEffect("RedHoodStep", false);
            }
            root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){isClickBusy = false;}), NULL));
            redHood->runAction(RotateTo::create(0.0f, 0, 180));
            rotation = true;
            basket->setPosition(leftNode->getPosition());
        });
    
    rightArrow = root->getChildByName<ui::Button* >("rightArrow");
    rightArrow->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
        {
             if (eventType != ui::Widget::TouchEventType::ENDED) return;
             if(isClickBusy) return;
             isClickBusy = true;
            if(rotation)
            {
                uPlayEffect("RedHoodStep", false);
            }
             root->runAction(Sequence::create(DelayTime::create(clickTime), CallFunc::create([&](){isClickBusy = false;}), NULL));
             redHood->runAction(RotateTo::create(0.0f, 0, 0));
            rotation = false;
             basket->setPosition(rightNode->getPosition());
        });

    for(int i = 1; i <= 6; i++)
    {
        hens.push_back(root->getChildByName<Sprite*>("hen" + to_str(i)));
    }
    
     btnBack = root->getChildByName<ui::Button* >("btnBack");
     btnBack->setPosition(btnBack->getPosition() + Point(-offsetX + safeAreaX, offsetY));
     btnBack->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
       {
           if (eventType != ui::Widget::TouchEventType::ENDED) return;
           uPlayEffect("Click", false);
           root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
            {
                onBackPressed();
            })));
       });
    
    spider0 = root->getChildByName<Sprite* >("spider0");
    spider1 = root->getChildByName<Sprite* >("spider1");
    spider2 = root->getChildByName<Sprite* >("spider2");
    spider3 = root->getChildByName<Sprite* >("spider3");
    spider1->setOpacity(0);
    spider2->setOpacity(0);
    spider3->setOpacity(0);
    spider1StartPos = spider1->getPosition();
    spider2StartPos = spider2->getPosition();
    spider3StartPos = spider3->getPosition();
    auto seqSpider = Sequence::create(DelayTime::create(7.0f), CallFunc::create([&]()
        {
            spider1->setPosition(spider1StartPos + Vec2(-93, 77));
            spider1->setOpacity(255);
            spider1->runAction(MoveTo::create(0.5f, spider1StartPos));
        }
        ), DelayTime::create(3.0f), CallFunc::create([&]()
        {
           spider1->setOpacity(0);
           spider2->setOpacity(255);
           spider2->runAction(MoveTo::create(0.3f, spider3StartPos));
        }
        ), DelayTime::create(0.3f), CallFunc::create([&]()
        {
           spider2->setOpacity(0);
           spider2->setPosition(spider2StartPos);
           spider3->setOpacity(255);
           spider3->runAction(MoveBy::create(0.5f, Vec2(300, 0)));
        }
        ), DelayTime::create(0.5f), CallFunc::create([&]()
        {
            spider3->setOpacity(0);
            spider3->setPosition(spider3StartPos);
        }
        ), nullptr);
    root->runAction(RepeatForever::create(seqSpider));
    
    functionMoveEgg();
    
    return true;
}

void HensScene::functionMoveEgg()
{
    auto sequence = Sequence::createWithTwoActions(
       CallFunc::create([&]()
        {
            if(numberGoodMax == 0)
            {
                root->stopActionByTag(100);
                CCLOG("KONIEC GRY, WSZYSTKIE DOBRE JAJKA JUZ ZA NAMI");
                CCLOG("LICZBA DOBRYCH JAJEK = %i", counterGood);
                CCLOG("LICZBA ZLYCH JAJEK = %i", counterBad);

                auto sequenceRoot = Sequence::create(
                     DelayTime::create(2.5f),
                     CallFunc::create([&]()
                      {
                          textGood->setString(to_str(counterGood));
                          textBad->setString(to_str(counterBad));
                           board->runAction(MoveBy::create(2.0f, Vec2(0, boardYMove)));
                          if(counterGood == maxScore && counterBad == 0)
                          {
                              UserDefault::getInstance()->setIntegerForKey(avatarString.c_str(), UserDefault::getInstance()->getIntegerForKey(avatarString.c_str(), 0) + 1);
                              uPlayEffect("Fanfary", false);
                              
                              auto plus = Sprite::create("plus.png");
                              plus->setLocalZOrder(20);
//                              plus->setScale(0.4f);
                              root->addChild(plus);
                              plus->setPosition(Vec2(800, 320));
                              
                              
                          }
                          
                          if(counterGood > counterBad)
                              uPlayEffect("withoutEnthusiasm", false);
                          if(counterGood < counterBad)
                              uPlayEffect("badScore", false);
                      }),
                     DelayTime::create(5.0f),
                     CallFunc::create([&]()
                      {
//                           auto scene = MenuScene::createScene();
//                           replaceSceneBackward(scene);
                      }), nullptr);
                root->runAction(sequenceRoot);
            }
            else
            {
                Vec2 vector;
                vector = Vec2(0, -700);
                float time = speed;
                auto move = MoveBy::create(time, vector);
                int startPlace;
                startPlace = random(1, 6);
                // dla poprzedniego jajka z pozycji nie na najnizszej grzedzie trzeba przyjac dodatkowe obostrzenia losowania,
                // zeby kolejne jajko nie nalozylo sie na spadajace wczesniejsze
                if(previousPos == 1)
                {
                    do
                    {
                        startPlace = random(1, 6);
                    }
                    while(startPlace == 3 || startPlace == 5);
                }
                if(previousPos == 2)
                {
                    do
                    {
                        startPlace = random(1, 6);
                    }
//                    while(startPlace == 4 ); // nie moze powstac jajko na pozycji o jedna grzede nizej niz poprzednie, bo sie najda na siebie i zle z dobrym trzeba bd zlapac do koszyka, nie bd innego wyjscia
                    while(startPlace == 4 || startPlace == 6);
                }
                if(previousPos == 3)
                {
                    do
                    {
                        startPlace = random(1, 6);
                    }
                    while(startPlace == 5 );
                }
                if(previousPos == 4)
                {
                    startPlace = random(1, 5); // tu wystarczy zawezic zakres losowania
                }
                
                previousPos = startPlace;
                
                CCLOG("WYBRANE MIEJSCE DLA JAJKA CZYLI NUMER KURY = %i", startPlace);
                
                auto egg = Sprite::create("egg_chicken.png");
                egg->setPosition(hens[startPlace - 1]->getPosition() + Vec2(0, -15));
//                egg->setLocalZOrder(-10);
                egg->setLocalZOrder(5);
                
                string eggType = "aligator";
                int randEgg = random(0, 6);
                if(randEgg%2 == 0) // 60% szansy na stworzenie dobrego jajka
                {
                    eggType = "chicken";
                    egg->setTag(1);
                    numberGoodMax--;
                    CCLOG("DOBRE JAJKO");
                }
                else
                {
                    egg->setTag(0);
                    CCLOG("ZLE JAJKO");
                }

                egg->setTexture("egg_" + eggType + ".png");
                root->addChild(egg);

                auto checkEggPosAction = Sequence::create(
                     DelayTime::create(0.2f),
                     CallFunc::create([&, egg]()
                          {
                              CCLOG("-------sprawdzam pozycje jajka");
                              
                              if(egg->getBoundingBox().intersectsRect(basket->getBoundingBox()) && egg->getTag() != 2 && (egg->getPositionY() > basket->getPositionY()))
                              {
                                  CCLOG("-------jajko w koszyku");
                                  
                                  egg->runAction(Sequence::create(
                                        CallFunc::create([&]()
                                           {
                                               egg->setLocalZOrder(0);
                                           }
                                           ), DelayTime::create(0.2f),
                                          CallFunc::create([&]()
                                           {
                                               egg->setOpacity(0);
                                           }
                                           ),  nullptr));
                                         
                                  if(egg->getTag() == 1)
                                  {
                                      counterGood++;
                                      uPlayEffect("EggKept", false);
                                  }
                                  else
                                  {
                                      counterBad++;
                                      uPlayEffect("dinoKept", false);
                                  }
                                  egg->setTag(2); // zeby juz bylo wylaczone z gry, bo czesto sprawdzam, co 0.2 sekundy i kilka razy jedno moze tu policzyc
                              };
                              
                              if(egg->getBoundingBox().intersectsRect(floor->getBoundingBox()) && egg->getTag() != 2)
                              {
                                  CCLOG("-------jajko na podlodze");
                                  egg->setLocalZOrder(0);
                                  
                                  egg->stopAllActions();
                                  if(egg->getTag() == 1)
                                  {
                                      egg->setTexture("egg_crash.png");
                                      uPlayEffect("EggCrack", false);
//                                      counterBad++;//roztrzaskanie jajka na podlodze
                                  }
                                  float deltaPos;
                                  int minusDecision = random(0, 1);
                                  if(minusDecision)
                                      deltaPos = (-1.0)*random(0, 50);
                                  else
                                      deltaPos = random(0, 50);
                                  
                                  egg->setPosition(egg->getPosition() + Vec2(deltaPos, 0));
                                  egg->setTag(2); // zeby juz bylo wylaczone z gry, bo czesto sprawdzam, co 0.2 sekundy i kilka razy jedno moze tu policzyc
                              };
                          }), nullptr);
                
                auto eggUpdateAction = Repeat::create(checkEggPosAction, -1);
                eggUpdateAction->setTag(200);
                egg->runAction(eggUpdateAction);
                
//                root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([&, egg]()
//                     {
//                         egg->setLocalZOrder(5);
//                     }
//                     )));
                
                egg->runAction(Sequence::createWithTwoActions(move, CallFunc::create([&]()
                     {
                         CCLOG("JAJKO SKONCZYLO MOVE ACTION");
//                         egg->stopActionByTag(200);
                         egg = nullptr;
                     }
                     )));
        }
        }),
       DelayTime::create(1.5f)
       );
    
    auto mainAction = Repeat::create(sequence, -1);
    mainAction->setTag(100);
    root->runAction(mainAction);
}

void HensScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        onBackPressed();
    }
}

void HensScene::onBackPressed()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->stopAllEffects();
    #else
        cocos2d::experimental::AudioEngine::stopAll();
    #endif
    auto scene = MenuScene::createScene();
    replaceSceneBackward(scene, btnBack->getPosition());
}

HensScene::~HensScene()
{
}

