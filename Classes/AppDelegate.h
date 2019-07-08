#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "Definitions.h"
#include "NativeHelper.h"
#include "PlayScene.h"
#include "AppItem.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "LoadingScene.h"
#include "MenuScene.h"
#include "cocostudio/CCComExtensionData.h"
#include "Utils.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#define SOUNDS_EXT "aac"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define SOUNDS_EXT "ogg"
#else
#define SOUNDS_EXT "wav"
#endif

class AppItem;

USING_NS_CC;
using namespace std;

/**
 @brief    The cocos2d Application.
 
 Private inheritance here hides part of interface from Director.
 */
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();
    
    virtual void initGLContextAttrs();
    
    /**
     @brief    Implement Director and Scene init code here.
     @return true    Initialize success, app continue.
     @return false   Initialize failed, app terminate.
     */
    virtual bool applicationDidFinishLaunching();
    
    /**
     @brief  Called when the application moves to the background
     @param  the pointer of the application
     */
    virtual void applicationDidEnterBackground();
    
    /**
     @brief  Called when the application reenters the foreground
     @param  the pointer of the application
     */
    virtual void applicationWillEnterForeground();
    
    void downloadOurApps();
    void ourAppsDownloaded(network::HttpClient* client, network::HttpResponse* response);
    void loadOurApps();
    static void downloadAndLoadButton(string url, string path, ui::Button* button, Action *actionAfter = nullptr);
    void goToNextLevel(int oldLevel);
    
    void drawDebugAreas(Node* root);
    
    float virtualWidth;
    float virtualHeight;
    float offsetX;
    float offsetY;
    float safeAreaX;
    float safeAreaY;
    float gameScale;
    
    vector <int> placesInt;
    
    vector<AppItem* > *apps;
    
    int nodeNumber;
    
};

#endif // _APP_DELEGATE_H_

