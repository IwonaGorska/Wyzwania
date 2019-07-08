#include "AppDelegate.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect(APP_NAME, cocos2d::Rect(0, 0, 960, 640));
#else
        glview = GLViewImpl::create(APP_NAME);
#endif
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    //    director->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);
    
    FileUtils::getInstance()->addSearchPath("res");
    FileUtils::getInstance()->addSearchPath("anims");
    
    string soundsDir = string("sounds_") + SOUNDS_EXT;
    CCLOG("sounds dir: %s", soundsDir.c_str());
    FileUtils::getInstance()->addSearchPath(soundsDir);
    
    NativeHelper::loadSettings();
//    NativeHelper::hideBanner();
    
    Rect safeArea = Director::getInstance()->getSafeAreaRect();
    Vec2 safeAreaOrigin = safeArea.origin;
    Vec2 safeAreaSize = safeArea.size;
    
    Size size = glview->getFrameSize();
    float scaleFactor = NativeHelper::getDeviceScaleFactor();
    virtualWidth = 960;
    virtualHeight = 640;
    float screenWidth = size.width;
    float screenHeight = size.height;
    float virtualRatio = virtualWidth / virtualHeight; //1.5
    float screenRatio = screenWidth / screenHeight;
    float scale = 1;
    
    offsetX = 0;
    offsetY = 0;
    
    //sgs3: 1.777
    float widestWidth = 1385.0f;
    float widestHeight = 640.0f;
    float tallestWidth = 1024.0f;
    float tallestHeight = 768.0f;
    float widestRatio = widestWidth / widestHeight; //2.16 (old: 1.775)
    float tallestRatio = tallestWidth / tallestHeight; //1.333
    
    float sratio = (size.height / scaleFactor) / virtualHeight;
    
    if(screenRatio > virtualRatio){ //left and right letterboxing
        bool minIsScreen = screenRatio <= widestRatio;
        scale = virtualRatio / MIN(screenRatio, widestRatio);
        float swidth = (size.width / scaleFactor) / sratio;
        float owidth = minIsScreen ? swidth : widestWidth;
        offsetX = round((1 - scale) * owidth * 0.5);
    }
    else if(screenRatio < virtualRatio){ //up and bottom letterboxing
        bool maxIsScreen = screenRatio >= tallestRatio;
        scale = screenRatio / MAX(virtualRatio, tallestRatio);
        float oheight = maxIsScreen ? tallestHeight : size.height / scaleFactor;
        offsetY = round((1 - scale) * oheight * 0.5);
    }
    
    safeAreaX = round((safeAreaOrigin.x / (safeAreaSize.x + safeAreaOrigin.x)) * widestWidth);
    safeAreaY = round((safeAreaOrigin.y / (safeAreaSize.y + safeAreaOrigin.y)) * widestHeight);
    
    glview->setDesignResolutionSize(virtualWidth, virtualHeight, ResolutionPolicy::NO_BORDER);
    
    gameScale = scale;
    
    std::srand(unsigned(std::time(0)));
    
    CCLOG("device resolution: %fx%f, scale: %f, offsetX: %f, offsetY: %f", screenWidth, screenHeight, scale, offsetX, offsetY);
    
    apps = new vector<AppItem* >();
    downloadOurApps();
    
    // create a scene. it's an autorelease object
    auto scene = LoadingScene::createScene();
    
    // run
    director->runWithScene(scene);
    
    return true;
}

void drawRect(Node* root, Rect rect, Color4F color) {
    auto drawNode = DrawNode::create();
    drawNode->drawSolidRect(rect.origin, rect.origin + Vec2(rect.size), color);
    root->addChild(drawNode);
}

void AppDelegate::drawDebugAreas(Node* root) {
    //    Rect r1 = Rect(0, 0, virtualWidth, virtualHeight);
    //    Rect r2 = Rect(-offsetX, -offsetY, offsetX, virtualHeight + 2 * offsetY);
    //    Rect r3 = Rect(virtualWidth, -offsetY, offsetX, virtualHeight + 2 * offsetY);
    //    Rect r4 = Rect(-offsetX, -offsetY, virtualWidth + 2 * offsetX, offsetY);
    //    Rect r5 = Rect(-offsetX, virtualHeight, virtualWidth + 2 * offsetX, offsetY);
    //    Rect r6 = Rect(-offsetX, -offsetY, safeAreaX, virtualHeight + 2 * offsetY);
    //    Rect r7 = Rect(virtualWidth + offsetX - safeAreaX, -offsetY, safeAreaX, virtualHeight + 2 * offsetY);
    //    Rect r8 = Rect(-offsetX, -offsetY, virtualWidth + 2 * offsetX, safeAreaY);
    
    //    drawRect(root, r1, Color4F(1, 1, 0, 0.3)); //virtual area
    //    drawRect(root, r2, Color4F(1, 0, 1, 0.3)); //left bar
    //    drawRect(root, r3, Color4F(1, 0, 1, 0.3)); //right bar
    //    drawRect(root, r4, Color4F(0, 1, 1, 0.3)); //bottom bar
    //    drawRect(root, r5, Color4F(0, 1, 1, 0.3)); //top bar
    //    drawRect(root, r6, Color4F(0, 0, 1, 0.5)); //left safe area
    //    drawRect(root, r7, Color4F(0, 0, 1, 0.5)); //right safe area
    //    drawRect(root, r8, Color4F(0, 1, 0, 0.5)); //bottom safe area
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be paused
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#else
    uPauseEffect(backgroundMusicSoundId);
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#else
    uResumeEffect(backgroundMusicSoundId);
#endif
}

void AppDelegate::downloadOurApps(){
    network::HttpRequest* request = new network::HttpRequest();
    string url = OUR_APPS_URL;
    url += OUR_APPS_URL_SUFFIX;
    request->setUrl(url.c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(AppDelegate::ourAppsDownloaded, this));
    network::HttpClient::getInstance()->send(request);
    request->release();
}

void AppDelegate::ourAppsDownloaded(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response){
    string path = FileUtils::getInstance()->getWritablePath() + "apps.plist";
    
    CCLOG("ourAppsDownloaded()");
    
    if (response && response->getResponseCode() == 200 && response->getResponseData()) {
        vector<char> *data = response->getResponseData();
        string ret(&(data->front()), data->size());
        //        CCLOG("%s", ("Response message: " + ret).c_str());
        
        FILE *fp = fopen(path.c_str(), "w");
        
        if (!fp)
        {
            CCLOG("can not create file %s", path.c_str());
            return;
        }
        
        fputs(ret.c_str(), fp);
        fclose(fp);
    }
    else {
        CCLOG("%s", ("Error in request: " + to_str(response->getResponseCode())).c_str());
    }
    
    if (FileUtils::getInstance()->isFileExist(path)){
        loadOurApps();
    }
}

void AppDelegate::loadOurApps(){
    apps->clear();
    string path = FileUtils::getInstance()->getWritablePath() + "apps.plist";
    ValueVector array = FileUtils::getInstance()->getValueVectorFromFile(path);
    
    for (int i = 0; i < array.size(); i++) {
        ValueMap itemData = (array[i]).asValueMap();
        string icon = OUR_APPS_URL + itemData["AppLinkToIcon"].asString();
        AppItem* item = AppItem::createAppItem(itemData["AppID"].asString(), icon, itemData["AppLinkiTunes"].asString(), itemData["AppName"].asString(), itemData["AppCategory"].asString(), itemData["AppBundleId"].asString());
        
        apps->push_back(item);
    }
    
    string extrasIcon = OUR_APPS_URL;
    extrasIcon += "extras.png";
    AppItem* extras = AppItem::createAppItem("000000000", extrasIcon, "http://123kidsfun.com/extras/", "Extras", "Extras", "Extras");
    apps->push_back(extras);
    
    string youtubeIcon = OUR_APPS_URL;
    youtubeIcon += "youtube.png";
    AppItem* youtube = AppItem::createAppItem("000000001", youtubeIcon, "http://rosmedia.eu/youtube.php", "YouTube", "Extras", "YouTube");
    apps->push_back(youtube);
}

void AppDelegate::downloadAndLoadButton(string url, string path, ui::Button* button, Action *actionAfter){
    button->retain();
    if(actionAfter != nullptr){
        actionAfter->retain();
    }
    network::HttpRequest* request = new network::HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([&, path, button, actionAfter](cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response){
        
        if(response && response->getResponseCode() == 200 && response->getResponseData()) {
            std::vector<char> *buffer = response->getResponseData();
            
            Image *img = new Image();
            img->initWithImageData((const unsigned char *)&(buffer->front()), buffer->size());
            
            CCLOG("saving image to: %s", path.c_str());
            img->saveToFile(path.c_str(), false);
            
            CC_SAFE_DELETE(img);
        }
        else{
            CCLOG("request failed: %s", response->getErrorBuffer());
        }
        
        if(FileUtils::getInstance()->isFileExist(path)){
            button->setVisible(true);
            button->loadTextures(path, path, path);
            if(actionAfter != nullptr){
                button->runAction(actionAfter);
            }
            
        }
        button->release();
        if(actionAfter != nullptr){
            actionAfter->release();
        }
    });
    network::HttpClient::getInstance()->send(request);
    
    request->release();
}

void AppDelegate::goToNextLevel(int oldLevel)
{
    Scene* scene = nullptr;
   
    scene = MenuScene::createScene();

    if(scene != nullptr) {
        replaceSceneForward(scene);
    }
}
