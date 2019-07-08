//
//  ScoreScene.cpp
//  Wyzwania
//
//  Created by Iwona Gorska on 23/02/2019.
//

#include <stdio.h>
#include "ScoreScene.h"

Scene* ScoreScene::createScene()
{
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = ScoreScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    return scene;
}

bool ScoreScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    root = CSLoader::createNode("ScoreScene.csb");
    addChild(root);
    
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);
    
    uPreloadEffect("Click");
    
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
    
    cup = root->getChildByName<Sprite* >("cup");
    auto actionCup = Sequence::create(
        EaseSineOut::create(ScaleTo::create(0.8f, 0.38f)),
        EaseSineIn::create(ScaleTo::create(0.8f, 0.35f)),
        nullptr);
    cup->runAction(Repeat::create((actionCup), -1));
    
    decor = root->getChildByName<Sprite* >("decor");
    auto actionDecor = Sequence::create(
      EaseSineOut::create(RotateBy::create(3.0f, 360.0f)),
      nullptr);
    decor->runAction(Repeat::create((actionDecor), -1));
    
    for(int i = 1; i <= 5; i++)
    {
        string name = "score" + to_str(i);
//        scores.push_back(UserDefault::getInstance()->getIntegerForKey(name.c_str(), 0));
//        scoresCopy.push_back(UserDefault::getInstance()->getIntegerForKey(name.c_str(), 0));
//        scores.push_back(i);
        string avatarString = "points" + to_str(i);
        int scoreInt = UserDefault::getInstance()->getIntegerForKey(avatarString.c_str(), 0);
        scores.push_back(scoreInt);
        scoresCopy.push_back(scoreInt);
        auto scoreT = root->getChildByName<cocos2d::ui::TextField* >("scoreT" + to_str(i));
        scoreT->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        scoreT->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        scoresT.push_back(scoreT);
        playersStairs.push_back(root->getChildByName<Sprite* >("playerStairs" + to_str(i)));
        players.push_back(root->getChildByName<Sprite* >("player" + to_str(i)));
        avatarsP.push_back(root->getChildByName<Sprite* >("avatarP" + to_str(i)));
        avatarsS.push_back(root->getChildByName<Sprite* >("avatarS" + to_str(i)));
    }
    
//    scores.push_back(5);
//    scores.push_back(23);
//    scores.push_back(2);
//    scores.push_back(12);
//    scores.push_back(0);
//    scoresCopy.push_back(5);
//    scoresCopy.push_back(23);
//    scoresCopy.push_back(2);
//    scoresCopy.push_back(12);
//    scoresCopy.push_back(0);
  
    
    int indexes[5] = {1, 2, 3, 4, 5};// pomocnicza tabela przechowujaca kolejnosc indexow awatarow rosnoco wedlug wynikow
    
    //SORTOWANIE PRZEZ WSTAWIANIE
    int N = 5;
    int i, j, x, y;
    for(j = N - 2; j >= 0; j--)
    {
        x = scores[j];
        y = indexes[j];
        i = j + 1;
        while((i < N) && (x > scores[i]))
        {
            scores[i - 1] = scores[i];
            indexes[i - 1] = indexes[i];
            i++;
        }
        scores[i - 1] = x;
        indexes[i - 1] = y;
    }
    
    int k = 4;
    for(int i = 0; i < 5; i++)
    {
        CCLOG("scores od %i = %i", i, scores[i]);
        scoresT[i]->setString(to_str(scores[k]));
        avatarsP[i]->setTexture("avatar" + to_str(indexes[k]) + ".png");
        avatarsS[i]->setTexture("avatar" + to_str(indexes[k]) + ".png");
        k--;
    }
    
    return true;
}

void ScoreScene::onBackPressed()
{
    auto scene = PlayScene::createScene();
    replaceSceneBackward(scene, btnBack->getPosition());
}

ScoreScene::~ScoreScene()
{

}

