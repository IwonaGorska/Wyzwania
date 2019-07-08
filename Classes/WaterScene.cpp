//  WaterScene.cpp
//  Wyzwania
//
//  Created by Iwona Gorska on 05/01/2019.
//

#include <stdio.h>
#include "WaterScene.h"

USING_NS_CC;

Scene* WaterScene::createScene(){
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = WaterScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    return scene;
}

bool WaterScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    root = CSLoader::createNode("WaterScene.csb");
    addChild(root);
    
    uStopBackgroundMusic();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopAllEffects();
#else
    cocos2d::experimental::AudioEngine::stopAll();
#endif
    
    uPreloadEffect("BadClock");
    uPreloadEffect("GoodClock");
    uPreloadEffect("Fanfary");
    uPreloadEffect("WaterBackground1");
    uPlayEffect("WaterBackground1", true);
    uPreloadEffect("WaterBackground2");
    uPlayEffect("WaterBackground2", true);
    uPreloadEffect("badScore");
    uPreloadEffect("withoutEnthusiasm");
    uPreloadEffect("Click");
    
    //ZNAJDZ 15 RYB Z GODZINA POZNIEJSZA LUB ROWNA TEJ NA PRZYKLADOWYM ZEGARKU, TP "LUB ROWNA" TO ZABEZPIECZENIE NA WYPADEK GDYBGY NA PRZYKLADOWYM WYLOSOWALA SIE TA NAJPOZNIEJSZA MOZLIWA
    whichFish = 0;
    timeArriveToHook = 4.2f;
    front = true;
    
    avatar = UserDefault::getInstance()->getIntegerForKey("avatar", 1);
    avatarString = "points" + to_str(avatar);
    
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);

    btnBack = root->getChildByName<ui::Button* >("btnBack");
    btnBack->setLocalZOrder(15);
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
    
    glass = root->getChildByName<Sprite* >("glass");
    glass->setOpacity(0);
    
    textGood = glass->getChildByName<cocos2d::ui::TextField* >("textGood");
    textGood->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    textGood->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
    textBad = glass->getChildByName<cocos2d::ui::TextField* >("textBad");
    textBad->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    textBad->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
    hours.push_back("100");
    hours.push_back("130");
    int nr = 230; // nie ma 200
    for(int i = 0; i < 21; i++)
    {
        CCLOG("nr = %i", nr);
        hours.push_back(to_str(nr));
        if(i%2)
            nr += 30;
        else
            nr += 70;
    }
//    hours.push_back("230");
//    hours.push_back("300");
//    hours.push_back("330");
//    hours.push_back("400");
//    hours.push_back("430");
//    hours.push_back("500");
//    hours.push_back("530");
//    hours.push_back("600");
//    hours.push_back("630");
//    hours.push_back("700");
//    hours.push_back("730");
//    hours.push_back("800");
//    hours.push_back("830");
//    hours.push_back("900");
//    hours.push_back("930");
//    hours.push_back("1000");
//    hours.push_back("1030");
//    hours.push_back("1100");
//    hours.push_back("1130");
//    hours.push_back("1200");
//    hours.push_back("1230");
    
//    auto fish = Sprite::create("fish_1.png");
//    fish->setPosition(570, 270);
//    root->addChild(fish);
//    Rect rect1 = fish->getBoundingBox();
//    auto drawNode1 = DrawNode::create();
//    root->addChild(drawNode1);
//    drawNode1->setOpacity(200);
//    drawNode1->drawSolidRect(rect1.origin, rect1.origin + rect1.size, Color4F::YELLOW);//
    
    counterBad = 0;//ile zlych zlapano
    counterGood = 0; //ile dobrych zlapano
    maxScore = 10;
    possibleClocksNr = 23;
    possibleFishNr = 9;
    randClockEx = random(1, possibleClocksNr - 6); // -3 zeby nie losowac calkiem najpozniejszych godzin, bo dziecko sie zanudzi zanim dotrwa do tylu ryb z taka sama lub pozniejsza
    // 1 zeby nie wylosowac calkiem pierwszej, bo wszystkie beda poprawne wtedy
    
    stones = root->getChildByName<Sprite* >("stones");
    decorClock = root->getChildByName<Sprite* >("decorClock");
    treasure = root->getChildByName<Sprite* >("treasure");
    if(waterTime == 1)
        waterTime = 0;
    else
    {
        waterTime = 1;
        decorClock->setTexture("decoration_clock_scene_2.png");
        decorClock->setRotation(-30.0f);
    }
    
    cane = root->getChildByName<Sprite* >("cane");
    cane->setLocalZOrder(14);
    jellyfish = root->getChildByName<Sprite* >("jellyfish");
    lilies = root->getChildByName<Sprite* >("lilies");
    lilies->runAction(Repeat::create(Sequence::createWithTwoActions(MoveBy::create(2.5f, Vec2(5, 10)), MoveBy::create(2.5f, Vec2(-5, -10))), -1));
    nest = root->getChildByName<Sprite* >("nest");
//    nest->runAction(RotateBy::create(0.0f, 0,180)); TO ZROBILAM W COCOS STUDIO
    nest->setPosition(nest->getPosition() + Point(offsetX - safeAreaX, 0));
    hook = root->getChildByName<Sprite* >("hook");
    hook->setLocalZOrder(15);
    exampleClock = root->getChildByName<Sprite* >("exampleClock");
    exampleClock->setPosition(exampleClock->getPosition() + Point(-offsetX + safeAreaX, 0));
    string randomTexture = hours[randClockEx];
    exampleClock->setTexture(randomTexture + "a.png");
    exampleClock->setLocalZOrder(15);
    CCLOG("%s", randomTexture.c_str());
    
    float timeSwim = 11.0f;
    ducks = root->getChildByName<Sprite* >("ducks");
    duckMother = root->getChildByName<Sprite* >("duckMother");
    ducks->setLocalZOrder(1);
    duckMother->setLocalZOrder(1);
    ;//    auto duckNode = Node::create();
//    root->addChild(duckNode);
//    ducks->removeFromParentAndCleanup(false);
//    duckMother->removeFromParentAndCleanup(false);
//    duckNode->addChild(ducks);
//    duckNode->addChild(duckMother);
    
    auto swim = Repeat::create(Sequence::create(
       EaseSineInOut::create(MoveBy::create(timeSwim, Vec2(500, 0))),
       RotateTo::create(0.0f, 0, 180.0f),
       EaseSineInOut::create(MoveBy::create(timeSwim, Vec2(-500, 0))),
       RotateTo::create(0.0f, 0, 0),
       nullptr), -1);
    ducks->runAction(swim);
    duckMother->runAction(swim->clone());
//    duckNode->runAction(swim);
    
    turtle = root->getChildByName<Sprite* >("turtle");
    auto sequenceTurtle = Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]()
        {
           turtle->setTexture("zolw_2.png");
        }
        ), DelayTime::create(0.3f), CallFunc::create([&]()
        {
            turtle->setTexture("zolw_3.png");
        }
        ), DelayTime::create(0.3f), CallFunc::create([&]()
        {
          turtle->setTexture("zolw_4.png");
        }
        ), DelayTime::create(0.3f), CallFunc::create([&]()
        {
         turtle->setTexture("zolw_1.png");
        }
        ), DelayTime::create(1.0f), nullptr);
    turtle->runAction(RepeatForever::create(sequenceTurtle));
    
    startNode = root->getChildByName("startNode");
    functionMoveFish();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    touchID = -1;
    
    listener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        if(touchID == -1)
        {
            Vec2 locationInNode = root->convertToNodeSpace(touch->getLocation());
            auto hookMove = hook;
            if(HitDetectHelper::hitTestJustBounds(hookMove, touch) && touchID == -1)
            {
                touchID = touch->getID();
                dragged = hookMove;
                draggedStartPos = hookMove->getPosition();
                draggedLastPos = locationInNode;
                return true;
            }
        }
        return false;
    };
    
    listener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        if(touch->getID() == touchID)
        {
            Vec2 touchPoint = root->convertToNodeSpace(touch->getLocation());
            Vec2 delta = touchPoint - draggedLastPos;
            delta = Vec2(0, delta.y);
            Vec2 newPosition = dragged->getPosition() + delta;
            if(dragged->getPositionY() + delta.y < 800 && dragged->getPositionY() + delta.y > 400)
            {
                dragged->setPosition(newPosition);
                draggedLastPos = touchPoint;
            }
        }
        
    };
    
    listener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        if(touch->getID() == touchID)
        {
            if(dragged != nullptr)
            {
                touchID = -1;
            }
        }
    };
    
    listener->onTouchCancelled = [&](Touch* touch, Event* event)
    {
        if(touch->getID() == touchID)
        {
            if(dragged != nullptr)
            {
                touchID = -1;
            }
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void WaterScene::functionMoveFish()
{
    Vec2 vector;
    vector = Vec2(1600, 0);
    float time = 7.0f;
    
//    Rect rectH = hook->getBoundingBox();
//    auto drawNodeH = DrawNode::create();
//    root->addChild(drawNodeH);
//    drawNodeH->setOpacity(200);
//    drawNodeH->drawSolidRect(rectH.origin, rectH.origin + rectH.size, Color4F::BLUE);//
    
    auto sequence = Sequence::createWithTwoActions(
        CallFunc::create([&, vector, time]()
        {
            int randClock = random(0, possibleClocksNr - 1);
            int randFish = random(1, possibleFishNr);
            CCLOG(" RAND CLOCK = %i", randClock);
            
            auto fish = Sprite::create("fish_" + to_str(randFish) + ".png");
            auto clock = Sprite::create(hours[randClock] + "d.png");
            fish->setPosition(startNode->getPosition() + Vec2(0, random(-100, 100)));
            clock->setPosition(fish->getPosition());
            fish->setLocalZOrder(10);
            clock->setLocalZOrder(10);

        //            fish->setTag(randClock);

            if(randClock >= randClockEx)
                whichFish ++;

            if(whichFish == maxScore)// 10 poprawnych ryb przeplynie i koniec
            {
                startNode->stopAllActions();
                //wszystkie plywajace ryby fadeout
                auto sequenceRoot = Sequence::create(
                     DelayTime::create(timeArriveToHook + 2.0f), // 2.0f for safety xd
                     CallFunc::create([&]()
                                      {
                                          //oklaski jakies daj, moze nie gratulacje a wynik i zobaczy sie czy dobrze bylo czy zle wiekszosc
                                          textGood->setString(to_str(counterGood));
                                          textBad->setString(to_str(counterBad));
                                          glass->setLocalZOrder(50);
                                          glass->runAction(FadeIn::create(1.5f));
                                          if(counterGood == maxScore && counterBad == 0)
                                          {
                                              UserDefault::getInstance()->setIntegerForKey(avatarString.c_str(), UserDefault::getInstance()->getIntegerForKey(avatarString.c_str(), 0) + 1);
                                              uPlayEffect("Fanfary", false);
                                              
                                              auto plus = Sprite::create("plus.png");
                                              plus->setLocalZOrder(55);
//                                              plus->setScale(0.4f);
                                              root->addChild(plus);
                                              plus->setPosition(Vec2(800, 320));
                                          }
                                          else
                                          {
                                              if(counterGood > counterBad)
                                                  uPlayEffect("withoutEnthusiasm", false);
                                              if(counterGood < counterBad)
                                                  uPlayEffect("badScore", false);
                                          }
                                      }),
                     DelayTime::create(5.5f),
                     CallFunc::create([&]()
                                      {
//                                          auto scene = MenuScene::createScene();
//                                          replaceSceneBackward(scene);
                                      }), nullptr);
                root->runAction(sequenceRoot);
            }
            
            root->addChild(fish);
            root->addChild(clock); // potem sprobuj to w nodzie razem moze trzymac
            
            root->runAction(Sequence::createWithTwoActions(DelayTime::create(timeArriveToHook), CallFunc::create([&, randClock, fish, clock]()
               {
                   float fadeTime = 0.7f;
                     if(fish->getBoundingBox().intersectsRect(hook->getBoundingBox()))
                   {
                       if(randClock >= randClockEx)//moze nawet tagow nie potrzeba dla ryb
                       {
                           counterGood++;
                           uPlayEffect("GoodClock", false);
                           fish->runAction(FadeOut::create(fadeTime));
                           clock->runAction(FadeOut::create(fadeTime));
                           auto star = Sprite::create("starFish.png");
                           star->setPosition(fish->getPosition() + Vec2(50, 0));
                           star->runAction(MoveBy::create(1.0f, Vec2(0, 100)));
                           star->runAction(FadeOut::create(1.0f));
                           star->setScale(0.2);
                           root->addChild(star);
                           
                           jellyfish->setTexture("meduza_1.png");
                           
                           if(!front)
                           {
                               jellyfish->runAction(Sequence::createWithTwoActions(RotateBy::create(0.2f, -10), RotateBy::create(0.2f, 10)));
                               front = true;
                           }
                           
                           // jakies pozytywne oklaski i punkt
                           if(randClockEx == randClock)
                           {
                               //jakies specjalne pozytywne reakcje i wiecej punktow
                           }
                       }
                       else
                       {
                           counterBad++;
                           uPlayEffect("BadClock", false);
                           auto rotate1 = RotateTo::create(0.2f, 30.0f);
                           auto rotate2 = RotateTo::create(0.2f, -30.0f);
                           auto rotate3 = RotateTo::create(0.2f, 0.0f);
                           auto seqRotate = Sequence::createWithTwoActions(rotate1, rotate2);
                           fish->runAction(Sequence::create(seqRotate, seqRotate->clone(), rotate3, FadeOut::create(fadeTime), nullptr));
                           clock->runAction(Sequence::create(FadeOut::create(fadeTime), nullptr));
                           jellyfish->setTexture("meduza_2.png");
                           if(front)
                           {
                               jellyfish->runAction(Sequence::createWithTwoActions(RotateBy::create(0.2f, -10), RotateBy::create(0.2f, 10)));
                               front = false;
                           }
                       }
                   };
               }
               )));
            
            fish->runAction(Sequence::createWithTwoActions(MoveBy::create(time, vector), CallFunc::create([&]()
                {
                    fish = nullptr;
                    CCLOG("fish nie zyje");
                }
                )));

            clock->runAction(Sequence::createWithTwoActions(MoveBy::create(time, vector), CallFunc::create([&]()
                {
                    clock = nullptr;
                }
                )));
        }),
        DelayTime::create(2.0f)
        );
//    startNode->runAction(sequence);
    startNode->runAction(Repeat::create(sequence, -1));
}

void WaterScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        onBackPressed();
    }
}

void WaterScene::onBackPressed()
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

WaterScene::~WaterScene()
{
}

