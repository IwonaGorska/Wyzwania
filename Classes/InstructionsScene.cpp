//
//  InstructionsScene.cpp
//  Wyzwania
//
//  Created by Iwona Gorska on 09/03/2019.
//

#include <stdio.h>
#include "InstructionsScene.h"

Scene* InstructionsScene::createScene()
{
    auto scene = Scene::create();
    float gameScale = ((AppDelegate*) Application::getInstance())->gameScale;
    auto layer = InstructionsScene::create();
    layer->setScale(gameScale);
    scene->addChild(layer);
    return scene;
}

bool InstructionsScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    uPreloadEffect("Click");
    
    root = CSLoader::createNode("InstructionsScene.csb");
    addChild(root);
    
    auto appDelegate = (AppDelegate* )(Application::getInstance());
    float offsetX = appDelegate->offsetX;
    float offsetY = appDelegate->offsetY;
    float safeAreaX = appDelegate->safeAreaX;
    float safeAreaY = appDelegate->safeAreaY;
    appDelegate->drawDebugAreas(root);
    
    uPreloadEffect("lector");
    
    
    isSpeaking = false;
    lectorSound = -1;
    
    speaker = root->getChildByName<Sprite* >("speaker");
    
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
    
    btnRead = root->getChildByName<ui::Button* >("buttonRead");
    btnRead->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType eventType)
       {
           if (eventType != ui::Widget::TouchEventType::ENDED) return;
           uPlayEffect("Click", false);
           
           if(!isSpeaking)
           {
               isSpeaking = true;
               root->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
                {
                    // skala poczatkowa jest 0.5
                    auto scale1 = ScaleTo::create(0.3f, 0.55f);
                    auto scale2 = ScaleTo::create(0.3f, 0.5f);
                    auto sequence = Sequence::createWithTwoActions(scale1, scale2);
                    auto speakAction = Repeat::create(sequence, -1);
                    speakAction->setTag(100);
                    speaker->runAction(speakAction);
                    lectorSound = uPlayEffect("lector", false); //NA PRZYKLAD 20 SEKUND BEDZIE CZYTAL A PO 20 SEKUNDACH TEN STOP ACTION
                    // TEKST PRZESTAJE SIE PRZEWIJAC, USTAWIA U GORY POCZATKIEM I NA NOWO ZACZYNA SIE PRZEWUJAC PRZEZ TE NP. 20 SEKUND, A LECTOR CZYTA, GLOSNICZEK MA REPEAT NA TE NP. 20 SEKUND Z SCALETO
                })));
               
               root->runAction(Sequence::createWithTwoActions(DelayTime::create(50.0f), CallFunc::create([&]() // zeby opoznic na czas trwania dzwieku "2_Dzwiek_Back"
                 {
                     speaker->stopActionByTag(100);
                     speaker->setScale(0.5f);
                     isSpeaking = false;
                 })));
           }
           else
           {
               speaker->stopActionByTag(100);
               uStopEffect(lectorSound);
               lectorSound = -1;
               isSpeaking = false;
           }
           
       });
    
//    string textInstr = "KURKI /n Zbieraj jajka. Unikaj potworków. Za złapanie wszystkich prawdziwych jajek i nie złapanie ani jednego potworka otrzymasz jeden punkt.  /nKOSZYK /nWłóż do koszyka elementy należące do podpisanej kategorii. Za 5 poprawnych rozwiązań z rzędu otrzymasz 1 punkt. /nWODA /nZłów ryby, które mają na sobie zegarki z godziną nie wcześniejszą niż ta na zegarze w lewym górnym rogu. Za złowienie wszystkich dobrych i nie złowienie ani jednej złej ryby otrzymasz 1 punkt. /nKLOCKI /nOdtwórz budowlę z klocków. Za przejście 1 poziomu (5 gier) otrzymasz 1 punkt.";
//    instr = root->getChildByName<cocos2d::ui::TextField* >("text");
//    instr->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
//    instr->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
//    instr->setString(
////                     "KURKI "
////                     "Zbieraj jajka. Unikaj potworków. Za złapanie wszystkich prawdziwych jajek i nie złapanie ani jednego potworka otrzymasz jeden punkt.  "
////                     "KOSZYK "
////                     "Włóż do koszyka elementy należące do podpisanej kategorii. Za 5 poprawnych rozwiązań z rzędu otrzymasz 1 punkt. "
////                     "WODA"
////                     "Złów ryby, które mają na sobie zegarki z godziną nie wcześniejszą niż ta na zegarze w lewym górnym rogu. Za złowienie wszystkich dobrych i nie złowienie ani jednej złej ryby otrzymasz 1 punkt. "
////                     "KLOCKI"
////                     "Odtwórz budowlę z klocków. Za przejście 1 poziomu (5 gier) otrzymasz 1 punkt."
//                     textInstr
//                     );
    
    return true;
}

void InstructionsScene::onBackPressed()
{
    uStopEffect(lectorSound);
    lectorSound = -1;
    isSpeaking = false;
    auto scene = PlayScene::createScene();
    replaceSceneBackward(scene, btnBack->getPosition());
}

InstructionsScene::~InstructionsScene()
{
}
