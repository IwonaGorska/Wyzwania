//
//  BasketScene.cpp
//  Wyzwania
//
//  Created by Iwona Gorska on 05/01/2019.
//
#include <stdio.h>
#include "BasketScene.h"

USING_NS_CC;

Scene* BasketScene::createScene()
{
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = BasketScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    return scene;
}

bool BasketScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    root = CSLoader::createNode("BasketScene.csb");
    
    addChild(root);
    
    uStopBackgroundMusic();
    
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopAllEffects();
#else
    cocos2d::experimental::AudioEngine::stopAll();
#endif
    
    uPreloadEffect("BasketBackground");
    uPlayEffect("BasketBackground", true);
    uPreloadEffect("BadChoiceBasket");
    uPreloadEffect("BeeBasket");
    uPreloadEffect("GoodChoiceBasket");
    uPreloadEffect("Fanfary");
    uPreloadEffect("PandaSwingBasket");
    uPlayEffect("PandaSwingBasket", true);
    uPreloadEffect("birds");
    uPlayEffect("birds", true);
    uPreloadEffect("Click");
    
    ended = false;
    
    if(basketPointsTime < 6) // od 1 do 5
    {
        basketPointsTime++;
    }
    else
    {
        basketPointsTime = 1;
    }
    
    avatar = UserDefault::getInstance()->getIntegerForKey("avatar", 1);
    avatarString = "points" + to_str(avatar);
    
    //MUSIC PRELOAD
    //    int varMusic = uPlayEffect("MUSIC", false);
    //    uStopEffect(varMusic);
    //    varMusic=-1;
    
    UserDefault::getInstance()->setIntegerForKey("old", 10);
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
    
    for(int i = 1; i <= 9; i++)
    {
        auto konf = root->getChildByName<Sprite* >("konf" + to_str(i));
        konf->setLocalZOrder(20);
        konfs.push_back(konf);
    }
    
    categoryTitle = root->getChildByName<Sprite*>("categoryTitle");
    
    categoryTitleNr = UserDefault::getInstance()->getIntegerForKey("categoryTitleNr", 1);
    if(categoryTitleNr >= 3)
        UserDefault::getInstance()->setIntegerForKey("categoryTitleNr", 1);
    else
        UserDefault::getInstance()->setIntegerForKey("categoryTitleNr", categoryTitleNr + 1);
    
    categoryTitle->setTexture("category_" + to_str(categoryTitleNr) + (".png"));
    
    counterAll = 10;
    
    category = random(1, 3);
    CCLOG("CATEGORY = %i", category);
    
    int max = 12;//liczba elementow w kategorii, numerowane sa od  1 do 12
    
    if(category == 1)
    {
        goodName = "fruit";
    }
    
    if(category == 2)
    {
        goodName = "vegetable";
    }
    
    if(category == 3)
    {
        goodName = "mushroom";
    }
    
    int numbersMax[max];
    int numbersPosNodes[18];
    
    counterGood = 3;
    counterBad = 7;
    
    for(int i = 0; i < max; i++)
    {
        numbersMax[i]=i+1;
    }
    
    for(int i=0; i < 18; i++)
    {
        numbersPosNodes[i]=i+1;
    }
    
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::shuffle(numbersMax, numbersMax + max, generator);//trzeba wymieszac elementy, zeby nie brac zawsze pierwszych siedmiu
    std::shuffle(numbersPosNodes, numbersPosNodes + 18, generator);//trzeba wymieszac pozycje, zeby nie ustawiac zawsze w tych samych miejscach
    //elementow
    
    background= root->getChildByName<Sprite* >("background");
    background->setLocalZOrder(-1);
    konfetti_1 = root->getChildByName<Sprite* >("konfetti_1");
    konfetti_1->setOpacity(0);
    correctRegion = root->getChildByName<ui::Button* >("correctRegion");
    basket = root->getChildByName<Sprite* >("basket");
    basket->setLocalZOrder(2);
    bee = root->getChildByName<Sprite* >("bee");
    bee->setLocalZOrder(2);
    beePos = bee->getPosition();
    camomile = root->getChildByName<Sprite* >("camomile");
    camomile->setLocalZOrder(3);
//    int rCam = random(1, 4);
//    camomile->setTexture("flowers" + to_str(rCam) + ".png");
//    if(rCam < 4)
//        camomile->setScale(0.3);
    camomile->setTexture("flowers" + to_str(basketTime) + ".png");
    
    if(basketTime < 4)
    {
        camomile->setScale(0.3);
        basketTime++;
    }
    else
    {
        basketTime = 1;
    }
    
    categoryTitle = root->getChildByName<Sprite* >("categoryTitle");
    categoryTitle->setLocalZOrder(15);
    fence = root->getChildByName<Sprite* >("fence");
    fence->setLocalZOrder(1);//0
    tree_2 = root->getChildByName<Sprite* >("tree_2");
    tree_2->setLocalZOrder(0);
    tree_1 = root->getChildByName<Sprite* >("tree_1");
    tree_1->setLocalZOrder(0);
    tree_3 = root->getChildByName<Sprite* >("tree_3");
    tree_3->setLocalZOrder(-1);
    tree_4 = root->getChildByName<Sprite* >("tree_4");
    tree_4->setLocalZOrder(-1);
    panda = root->getChildByName<Sprite* >("panda");
    panda->setLocalZOrder(0);
    title = root->getChildByName<Sprite* >("title");
    title->setLocalZOrder(15);
    title->setTexture(goodName + "sTitle.png");
    forest1 = root->getChildByName<Sprite* >("forest1");
    forest1->setLocalZOrder(10);
    forest2 = root->getChildByName<Sprite* >("forest2");
    forest2->setLocalZOrder(10);
    
//    panda->setAnchorPoint(Vec2(0.5, 0)); W COCOS STUDIO USTAWILAM JUZ
    auto actionPanda = Sequence::create(
        EaseSineOut::create(RotateBy::create(1.0f, 10.0f)),
        EaseSineIn::create(RotateTo::create(0.7f, 0)),
        nullptr);
    panda->runAction(Repeat::create((actionPanda), -1));
    
    auto actionBee = Sequence::create(
          CallFunc::create([&](){bee->setLocalZOrder(15); uPlayEffect("BeeBasket", false); }),
          MoveBy::create(5.0f, Vec2(1.0f*random(-600, -100), 700.0f)),
          CallFunc::create([&](){bee->setPosition(beePos + Vec2(1.0f*random(-600, 600), -200.0f)); bee->setLocalZOrder(2);}),
          MoveTo::create(2.0f, beePos),
          DelayTime::create(25.0f),
        nullptr);
    bee->runAction(Repeat::create((actionBee), -1));
    
    for(int i = 1; i <= 18; i++)// 20 mozliwych miejsc, w ktorych moga siedziec elementy, dwa ostatnie to boki pienkow, na nich
        //moga byc tylko grzyby opienki
    {
        auto elemNode = root->getChildByName<Node* >("elemNode" + to_str(i));
        elemNodes.push_back(elemNode);
    }
    
    auto elemNode1 = root->getChildByName<Node* >("elemNode19");
    placeNodeTree1 = elemNode1->getPosition();//zapamietuje miejsca z pienkami jeszcze zanim przetasuje wszystkie miejsca
    auto elemNode2 = root->getChildByName<Node* >("elemNode20");
    placeNodeTree2 = elemNode2->getPosition();
    
    for(int i = 0; i < counterAll; i++) //liczba poprawnych elementow do wlozenia w koszyk
    {
        string elemName = "element_" + to_str(i + 1);
        auto element = root->getChildByName<Sprite* >(elemName);
        if(i < counterGood)
        {
            name = goodName;
            element->setTag(1);
        }
        else
        {
            do
            {
                randBad = random(1, 3);
            }
            while(randBad == category);
            
            if(randBad == 1)
            {
                badName = "fruit";
            }
            
            if(randBad == 2)
            {
                badName = "vegetable";
            }
            
            if(randBad == 3)
            {
                badName = "mushroom";
            }
            name = badName;
            element->setTag(0);
        }
        fullName = name + "_" + (to_str(numbersMax[i])) ;
        element->setTexture(fullName + ".png");
        
        if(fullName != "mushroom_8" && fullName != "mushroom_12")
        {
            CCLOG("numer pozycji node = %i", numbersPosNodes[i]);
            element->setPosition(elemNodes[numbersPosNodes[i] - 1]->getPosition()); //ustawiamy na wylosowanych wczesniej pozycjach wylosowane elementy
            CCLOG("fullName = %s", fullName.c_str());
            
            if(numbersPosNodes[i] <= 8)
                element->setLocalZOrder(0);
            if(numbersPosNodes[i] == 9)
                element->setLocalZOrder(14);
            if(numbersPosNodes[i] >= 10 && numbersPosNodes[i] <= 18 )
                element->setLocalZOrder(16);
//            if(numbersPosNodes[i] == 20)
//                element->setLocalZOrder(6);
        }
        else
        {
            if(fullName == "mushroom_8")
            {
                element->setPosition(placeNodeTree1);
                element->setLocalZOrder(16);
            }
            else
            {
                element->setPosition(placeNodeTree2);
                element->setLocalZOrder(6);
            }
        }
        elements.push_back(element);
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    touchID = -1;
    counter = counterGood;
    
    listener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        if(touchID == -1 && !ended)
        {
            Vec2 locationInNode = root->convertToNodeSpace(touch->getLocation());
            for(int i = 0; i < counterAll; i++)
            {
                auto element = elements.at(i);
//                if(HitDetectHelper::hitTestJustBounds(element, touch) && touchID == -1 &&  !element->getActionByTag(100) && !element->getActionByTag(200) && element->getTag() != -1 && !(element->getBoundingBox().intersectsRect(correctRegion->getBoundingBox())))
                if(HitDetectHelper::hitTestJustBounds(element, touch) && touchID == -1 &&  !element->getActionByTag(100) && !element->getActionByTag(200) && element->getTag() != -1 )
                {
                    touchID = touch->getID();
                    dragged = element;
                    draggedZOrder = element->getLocalZOrder();
                    CCLOG("ZORDER CHWYTANEGO  = %i", draggedZOrder );
                    dragged->setLocalZOrder(18);
                    CCLOG("ZORDER PO ZMIANIE  = %i", dragged->getLocalZOrder() );
                    draggedStartPos = element->getPosition();
                    draggedLastPos = locationInNode;
                    CCLOG("TAG CHWYTANEGO  = %i", dragged->getTag() );
                    //dragged->setTag(i);
                    return true;
                }
            }
        }
        return false;
    };
    
    listener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        if(touch->getID() == touchID && dragged!=nullptr && dragged->getTag() != -1 && !dragged->getActionByTag(100) && !dragged->getActionByTag(200))
        {
            Vec2 touchPoint = root->convertToNodeSpace(touch->getLocation());
            Vec2 delta = touchPoint - draggedLastPos;
            Vec2 newPosition = dragged->getPosition() + delta;
            dragged->setPosition(newPosition);
            draggedLastPos = touchPoint;
            
            
            if(dragged->getTag() == 1 && dragged->getBoundingBox().intersectsRect(correctRegion->getBoundingBox())  &&  dragged->getPositionY() > correctRegion->getPositionY())
                {
                    dragged->setTag(-1);
                    dragged->setLocalZOrder(1);
                    uPlayEffect("GoodChoiceBasket", false);
                    auto jumping = JumpTo::create(1.0f, correctRegion->getPosition(), 50, 1);
                    jumping->setTag(100);
                    auto seqDrag = Sequence::create(jumping, DelayTime::create(1.0f), CallFunc::create([&](){dragged = nullptr; touchID = -1;}), NULL);
                    dragged->runAction(seqDrag);
                    counter --;
                    if(counter == 0)
                    {
                        //zakonczenie gry
                        ended = true;
                        
                        if(basketPointsTime < 5)
                        {
                            auto seqRoot = Sequence::create(
                                DelayTime::create(2.0f),
                                CallFunc::create([&]()
                                 {
                                     uPlayEffect("Fanfary", false);
                                     
                                     konfetti_1->setLocalZOrder(20);
                                     konfetti_1->runAction(FadeIn::create(0.5f));
                                     auto action = Repeat::create(Sequence::create(
                                       EaseSineInOut::create(MoveBy::create(0.07f, Vec2(0.7, 0))),
                                       EaseSineInOut::create(MoveBy::create(0.07f, Vec2(-0.7, 0))),
                                       nullptr), -1);
                                     konfetti_1->runAction(action);
                                     for(int i = 0; i < 9; i++)
                                     {
                                         auto konf = konfs[i];
                                         int randTime = random(2, 4);
                                         int randY = random(800, 900);
                                         konf->runAction(MoveBy::create(randTime*1.0f, Vec2(0, -randY)));
                                     }
                                 }),
                                DelayTime::create(5.0f),
                                CallFunc::create([&]()
                                 {
                                     auto scene = BasketScene::createScene();
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
                                     uPlayEffect("Fanfary", false);
                                     
                                     auto plus = Sprite::create("plus.png");
                                     plus->setLocalZOrder(19);
//                                     plus->setScale(0.4f);
                                     root->addChild(plus);
                                     plus->setPosition(Vec2(800, 320));
                                     
                                     
                                     konfetti_1->setLocalZOrder(20);
                                     konfetti_1->runAction(FadeIn::create(0.5f));
                                     auto action = Repeat::create(Sequence::create(
                                       EaseSineInOut::create(MoveBy::create(0.07f, Vec2(0.7, 0))),
                                       EaseSineInOut::create(MoveBy::create(0.07f, Vec2(-0.7, 0))),
                                       nullptr), -1);
                                     konfetti_1->runAction(action);
                                     for(int i = 0; i < 9; i++)
                                     {
                                         auto konf = konfs[i];
                                         int randTime = random(2, 4);
                                         int randY = random(800, 900);
                                         konf->runAction(MoveBy::create(randTime*1.0f, Vec2(0, -randY)));
                                     }
                                 }),
                                DelayTime::create(5.0f),
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
                        
                        
                    }
                }
                else
                {
//                    dragged->setLocalZOrder(draggedZOrder);
                    //wracana swoje miejsce dzieki onTouchEnded, wiec to juz zalatwione,
                    //a tu wystarczy tylko jakis smutny okrzk dodac czy cos, moze ta krowe zalamana
                }
            }
    };
    
    listener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        if(touch->getID() == touchID)
        {
            if(dragged != nullptr  && dragged->getTag() != -1)
            {
                if(dragged->getTag() != 1)
                {
                    uPlayEffect("BadChoiceBasket", false);
                }
                auto movingBack = MoveTo::create(0.8f, draggedStartPos);
                movingBack->setTag(200);
                auto seqDrag = Sequence::create(movingBack, DelayTime::create(0.8f), CallFunc::create([&](){dragged = nullptr; touchID = -1; }), NULL);
                dragged->runAction(seqDrag);
                auto seqDrag2 = Sequence::create(DelayTime::create(0.4f), CallFunc::create([&](){dragged->setLocalZOrder(draggedZOrder);}), NULL);
                dragged->runAction(seqDrag2);
                
                //dragged->setTag(-1);
            }
        }
    };
    
    listener->onTouchCancelled = [&](Touch* touch, Event* event)
    {
        if(touch->getID() == touchID)
        {
            if(dragged != nullptr  && dragged->getTag() != -1)
            {
                uPlayEffect("BadChoiceBasket", false);
                auto movingBack = MoveTo::create(0.8f, draggedStartPos);
                movingBack->setTag(200);
                auto seqDrag = Sequence::create(movingBack, DelayTime::create(0.8f), CallFunc::create([&](){dragged = nullptr; touchID = -1; }), NULL);
                dragged->runAction(seqDrag);
                auto seqDrag2 = Sequence::create(DelayTime::create(0.4f), CallFunc::create([&](){dragged->setLocalZOrder(draggedZOrder);}), NULL);
                dragged->runAction(seqDrag2);
                
                //dragged->setTag(-1);
            }
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
    }

void BasketScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        onBackPressed();
    }
}

void BasketScene::onBackPressed()
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


BasketScene::~BasketScene()
{
}
