//
//  PlayScene.cpp
//  NativeHelperTest
//  Wyzwania
//
//  Created by Iwona Gorska on 03/10/2018.
//
#include "LoadingScene.h"

Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = LoadingScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    return scene;
}

bool LoadingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
 
    UserDefault::getInstance()->getXMLFilePath();
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    
    NativeHelper::adsSceneEnter("Loading", 1);
    NativeHelper::reportScreenChange("Loading");
    
    auto placeholder = Sprite::create("ekran_play_przed.png");
//    auto placeholder = Sprite::create("panda1.png");
    placeholder->setAnchorPoint(Point::ZERO);
//    placeholder->setPosition(Point(-100, -64));
    placeholder->setPosition(Point(-5, -38));
    addChild(placeholder);
    
    auto label = ui::Text::create("Ładowanie...", "Arial", 20);
    label->setTextHorizontalAlignment(TextHAlignment::LEFT);
    label->setTextVerticalAlignment(TextVAlignment::CENTER);
    label->setAnchorPoint(Point::ZERO);
    label->setTextColor(Color4B::BLACK);
    label->setPosition(Point(-offsetX + 10, 10 - offsetY));
    
    addChild(label);
    scheduleUpdate();
    
    return true;
}

void LoadingScene::update(float delta)
{
    
    auto director = Director::getInstance();
    
    for (int i = 0; i < 12; i++){
        for (int j = 0; j < 3; j++){
            std::string filename = "emiter_" + to_str(i + 1) + "_" + to_str(j + 1) + ".png";
            log("File %s loaded.", filename.c_str());
            director->getTextureCache()->addImage(filename);
        }
    }
 
    unscheduleUpdate();
    
    auto scene = PlayScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

LoadingScene::~LoadingScene()
{
    NativeHelper::adsSceneExit("Loading");
}
