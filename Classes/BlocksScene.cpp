//
//  BlocksScene.cpp
//  Wyzwania
//
//  Created by Iwona Gorska on 05/01/2019.
//

#include <stdio.h>
#include "BlocksScene.h"
#include "AppDelegate.h"
#include <algorithm>
#include <random>
#include <iostream>

USING_NS_CC;

Scene* BlocksScene::createScene()
{
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = BlocksScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    
    return scene;
}

bool BlocksScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);
    
    root = CSLoader::createNode("BlocksScene.csb");
    addChild(root);
    
    uStopBackgroundMusic();
    
    finishedLevel = false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopAllEffects();
#else
    cocos2d::experimental::AudioEngine::stopAll();
#endif
    
    uPreloadEffect("BlocksBackground");
    uPlayEffect("BlocksBackground", true);
    uPreloadEffect("Click");
    uPreloadEffect("NewBlock");
    uPreloadEffect("GoodChoiceBasket");
    
    avatar = UserDefault::getInstance()->getIntegerForKey("avatar", 1);
    avatarString = "points" + to_str(avatar);
    
    bigDog = root->getChildByName<Sprite*>("bigDog");
    bigDog->setTexture("dog_" + to_str(bigDogTime) + ".png");
    if(bigDogTime < 3)
        bigDogTime++;
    else
        bigDogTime = 1;
    
    dogHiden = true;
    
    start = root->getChildByName("start");
    animal = root->getChildByName<Sprite*>("animal");
    animal->setLocalZOrder(15);
    animal->setGlobalZOrder(3);
    animalShow();
    
    armchair = root->getChildByName<Sprite*>("armchair");
    table = root->getChildByName<Sprite*>("table");
    shelf_1 = root->getChildByName<Sprite*>("shelf_1");
    shelf_2 = root->getChildByName<Sprite*>("shelf_2");
    cupboard = root->getChildByName<Sprite*>("cupboard");
    dog = root->getChildByName<Sprite*>("dog");
    
    currentRoom = UserDefault::getInstance()->getIntegerForKey("roomNumber", 1);
    if(currentRoom >= 3)
        UserDefault::getInstance()->setIntegerForKey("roomNumber", 1);
    else
        UserDefault::getInstance()->setIntegerForKey("roomNumber", currentRoom + 1);
    
    table->setTexture("table_" + to_str(currentRoom) + (".png"));
    shelf_1->setTexture("shelf_" + to_str(currentRoom) + (".png"));
    shelf_2->setTexture("shelf_" + to_str(currentRoom) + (".png"));
    cupboard->setTexture("cupboard_" + to_str(currentRoom) + (".png"));
    armchair->setTexture("armchair_" + to_str(currentRoom) + (".png"));
    armchair->setTexture("armchair_" + to_str(currentRoom) + (".png"));
    
    addHeight = 0;
    switch(currentRoom)
    {
        case 1: addHeight = 30; break;
        case 2: addHeight = 20; break;
        case 3: addHeight = 55; break;
        default: addHeight = 40; break;
    }

    switch(blocksLevel)
    {
      case 1: {numberOfParts = 5; break;} // bedzie 5 klockow, partNumber bedzie zalezny od levelu, bedzie 5 ukladanek w kazdym levelu
          // dlatego co 5 wzrasta, 5 dodaje
      case 2: {numberOfParts = 10;  break;}
      case 3: {numberOfParts = 15;  break;}
    }

    node1 = root->getChildByName("blocksPackage");
//    blocksNodeNumber = 2;
    CCLOG("NODENUMBER = %i", blocksNodeNumber);
    node = CSLoader::createNode("BlocksNode" + to_str(blocksNodeNumber) + ".csb");// tu zamiast partNumber jakis numer, ale zalezny od levelu
    node1->addChild(node); // tego node'a musisz wybrac ktoregos z kolei w obecnym level i ktory z kolei sie dowiesz dzieki partNumber zapisanym w utilsach

    counter = 0;
    
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

    for(int i = 1; i <= numberOfParts; i++)
    {
      auto part = node->getChildByName<Sprite* >("part" + to_str(i) + "_0"  + "_0"); // parts to klocki wskakujace na stolik
      part->setTag(i);
        part->setGlobalZOrder(2);
        part->setLocalZOrder(10);
      parts.push_back(part);
      auto correctPart= node->getChildByName<Sprite* >("part" + to_str(i) + "_0"); // correctParts to nieruchome docelowe klocki na lewej polce
      correctPart->setTag(i);
      correctPart->setOpacity(0);
      correctParts.push_back(correctPart);
        // klockow z nazwami bez zadnego zera z tylu nie pobieram, bo nie beda uzywane w kodzie, sa tylko podgladem dla gracza
    }
    
    for(int i = 0; i < numberOfParts; i++)
    {
        for(int j = numberOfParts - 1; j >= 0; j--)
        {
            if(parts[i]->getTexture() == parts[j]->getTexture())
            {
                parts[i]->setTag(parts[j]->getTag());
                correctParts[i]->setTag(parts[j]->getTag());
            }
        }
    }
    
    for(int j = 0; j < numberOfParts; j++)
    {
        CCLOG("PARTS[%i] TAG = %i", j, parts[j]->getTag());
        CCLOG("CORRECTPARTS[%i] TAG = %i", j, correctParts[j]->getTag());
    }
    
    auto moveAction = MoveTo::create(1.0f, table->getPosition() - Vec2(480, 320) + Vec2(0, addHeight)+ Vec2(0, parts[counter]->getContentSize().height/2));
    // pierwszy Vec2 od przesuniecia node'a w roocie, drugi to podniesienie z anchorpointa na blat stolu, trzeci to dopasowanie do wysokosci klocka
    moveAction->setTag(300);
    parts[counter]->runAction(moveAction);
    uPlayEffect("NewBlock", false);
    auto rotateAction = RotateTo::create(1.0f, 360.0f);
    parts[counter]->runAction(rotateAction);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    touchID = -1;

    listener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        if(touchID == -1)
        {
            Vec2 locationInNode = root->convertToNodeSpace(touch->getLocation());
            
            for(int i = 0; i < parts.size(); i++)
            {
                auto part = parts.at(i);
                if(HitDetectHelper::hitTestJustBounds(part, touch) && part->getTag() >= 0 && !part->getActionByTag(100) && !part->getActionByTag(200) && !part->getActionByTag(300))//tu moze byc hittestjustbounds, bo jest zawsze tylko jeden klikalny, nie zaslaniaja sie
                {
                    touchID = touch->getID();
                    dragged = part;
//                    dragged->setGlobalZOrder(2); // JUZ JEST W DEKLARACJI Z ORDER POWIEKSZANA
//                    draggedStartPos = part->getPosition();
                    draggedStartPos = table->getPosition() - Vec2(480, 320) + Vec2(0, addHeight)+ Vec2(0, parts[counter]->getContentSize().height/2);
                    draggedLastPos = locationInNode;
                    return true;
                }
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
            Vec2 newPosition = dragged->getPosition() + delta;
            dragged->setPosition(newPosition);
            draggedLastPos = touchPoint;
            int matching = -1;
            
            bool isClose = false;
            
            for(int j = 0; j < correctParts.size(); j++)
            {
                if(dragged->getTag() == correctParts[j]->getTag())
                {
                    // nie wystarczy sprawdzic, czy odpowiadaja tagi, bo gdy tylko to sprawdzam, to matching ustawia na pierwszy lepszy correctpart z tym tagiem, a moze gracz jest blikso drugiego z odpowiadajacych akurat
                    if(dragged->getPosition().distance(correctParts[j]->getPosition()) < 20)
                        matching = j;
                }
            }
            
            CCLOG("DRAGGED TAG = %i", dragged->getTag());
            
            if(matching != -1 && dragged->getPosition().distance(correctParts[matching]->getPosition()) < 20 && counter != numberOfParts  && !dragged->getActionByTag(100) && !dragged->getActionByTag(200) && !dragged->getActionByTag(300) )
            {
               
                auto moving = EaseSineOut::create(MoveTo::create(0.5f, correctParts[matching]->getPosition()));
                uPlayEffect("GoodChoiceBasket", false);
                moving->setTag(100);
                counter++;
                correctParts[matching]->setTag(-1);// odhaczam, zeby juz tu nie probowac nakladac, gdzie juz jest cos
  
                dragged->runAction(
                   Sequence::createWithTwoActions(
                      moving,
                      CallFunc::create([&]()
                                       {
                                           if(counter < numberOfParts)
                                           {
                                               auto moveAction = MoveTo::create(1.0f, table->getPosition() - Vec2(480, 320) + Vec2(0, addHeight)+ Vec2(0, parts[counter]->getContentSize().height/2));
                                               moveAction->setTag(300);
                                               uPlayEffect("NewBlock", false);
                                               parts[counter]->runAction(moveAction);
                                               auto rotateAction = RotateTo::create(1.0f, 360.0f);
                                               parts[counter]->runAction(rotateAction);
                                           }
                                       }
                                       )));
                
                dragged->setTag(-1);
                dragged = nullptr;
                touchID = -1;
                
                if(counter == numberOfParts)
                {
                    if((blocksNodeNumber == 5) || (blocksNodeNumber == 10) || (blocksNodeNumber == 15))
                    {
                        finishedLevel = true;
                    }
                    
                    if(blocksNodeNumber < 15)
                        blocksNodeNumber++;
                    else
                        blocksNodeNumber = 1;
                    
                    if(blocksNodeNumber == 6)
                    {
                        blocksLevel = 2;
                        
                    }
                    if(blocksNodeNumber == 11)
                    {
                        blocksLevel = 3;
                    }
                    if(blocksNodeNumber == 1)
                    {
                        blocksLevel = 1;
                    }
                    
                    CCLOG("BLOCKSLEVEL = %i", blocksLevel);
                    
                    if(finishedLevel)
                    {
                        auto seqRoot = Sequence::create(
                            DelayTime::create(2.0f),
                            CallFunc::create([&]()
                             {
                                 auto plus = Sprite::create("plus.png");
//                                 plus->setScale(0.4f);
                                 plus->setLocalZOrder(20);
                                 root->addChild(plus);
                                 plus->setPosition(Vec2(800, 320));
                             }),
                            DelayTime::create(3.0f),
                            CallFunc::create([&]()
                             {
                                #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
                                 auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
                                 audio->stopAllEffects();
                                #else
                                 cocos2d::experimental::AudioEngine::stopAll();
                                #endif
                                 UserDefault::getInstance()->setIntegerForKey(avatarString.c_str(), UserDefault::getInstance()->getIntegerForKey(avatarString.c_str(), 0) + 1);
                                 auto scene = MenuScene::createScene();
                                 replaceSceneBackward(scene);
                             }), nullptr
                            );
                        root->runAction(seqRoot);
                    }
                    else
                    {
                        auto seqRoot = Sequence::create(
                            DelayTime::create(2.0f),
                            CallFunc::create([&]()
                                             {
                                                 auto scene = BlocksScene::createScene();
                                                 replaceSceneBackward(scene);
                                             }), nullptr
                            );
                        root->runAction(seqRoot);
                    }
                }
            }
        }
    };
    

    listener->onTouchEnded = [&](Touch* touch, Event* event)
    {
      if(touch->getID() == touchID)
      {
          if(dragged != nullptr)
          {
              auto movingBack = MoveTo::create(0.4f, draggedStartPos);
              bigDogShow();
              movingBack->setTag(200);
              auto seqDrag = Sequence::create(movingBack, DelayTime::create(0.4f), CallFunc::create([&](){/*dragged->setGlobalZOrder(2); */dragged = nullptr; touchID = -1; }), NULL);
              dragged->runAction(seqDrag);
          }
      }
    };

    listener->onTouchCancelled = [&](Touch* touch, Event* event)
    {
      if(touch->getID() == touchID)
      {
          if(dragged != nullptr)
          {
              auto movingBack = MoveTo::create(0.4f, draggedStartPos);
              bigDogShow();
              movingBack->setTag(200);
              auto seqDrag = Sequence::create(movingBack, DelayTime::create(0.4f), CallFunc::create([&](){/*dragged->setGlobalZOrder(2); */dragged = nullptr; touchID = -1; }), NULL);
              dragged->runAction(seqDrag);
          }
      }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
    }

void BlocksScene::animalShow()
{
    nameNr = 0;
    names.push_back("miniDogRed");
    names.push_back("miniDogBlack");
    names.push_back("panda_");
    
     Vector<cocos2d::FiniteTimeAction *> array;
    
    array.pushBack(CallFunc::create([&]()
    {
        animal->setPosition(start->getPosition());
    }));
   
    for(int k = 0; k < 2; k++) // to dwa razy ma sie wydarzyc, bo bedzie 12 podskokow, a mam 6 tekstur
    {
        for(int i = 1; i <= 6; i++)
        {
            array.pushBack(CallFunc::create([&, i]()
            {
                animal->setTexture(names[nameNr] + to_str(i) + ".png");
            }));
            array.pushBack(EaseSineOut::create(JumpBy::create(1.0f, Vec2(125, 0), 75, 1)));
        }
    }
    
    array.pushBack(CallFunc::create([&]()
                                    {
                                       if(nameNr < 2)
                                           nameNr++;
                                        else
                                            nameNr = 0;
                                    }));
    
    auto action = Sequence::create(array);
    animal->runAction(Repeat::create(action, -1));
}

void BlocksScene::bigDogShow()
{
    if(!dogHiden) return;
    dogHiden = false;
    
    CCLOG("CURRENT ROOM = %i", currentRoom);
    
    float r = random(-0.5, 0.5);
    float y = 90.0;
    float time = 0.5f;
    auto rotation1 = RotateTo::create(time, r);
    
    if(currentRoom == 1)
    {
        r = 0; // bo ta szafa ma za niskie boki i widac, ze pies nie ma reszty ciala jak sie przechyla o rotację
        y = 75;
    }
    
    auto rotation2 = RotateTo::create(r, 0);
    auto move1 = MoveBy::create(time, Vec2(0, y));
    auto move2 = MoveBy::create(time, Vec2(0, -y));
    auto seqDog = Sequence::create(
       Spawn::createWithTwoActions(rotation1, move1),
       DelayTime::create(time),
       Spawn::createWithTwoActions(rotation2, move2),
       CallFunc::create([&]()
        {
            bigDog->setTexture("dog_" + to_str(bigDogTime) + ".png");
            if(bigDogTime < 3)
                bigDogTime++;
            else
                bigDogTime = 1;
            
            dogHiden = true;
        }),
       NULL);
    bigDog->runAction(seqDog);
}
                                                                                  
void BlocksScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
    {
        if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
        {
            onBackPressed();
        }
    }
                                                                                  
void BlocksScene::onBackPressed()
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
                                                                                  
BlocksScene::~BlocksScene()
    {
    }
                                                                                  
