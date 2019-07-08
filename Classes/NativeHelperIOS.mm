#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "NativeHelper.h"
#import "AppController.h"

bool NativeHelper::instanceFlag = false;
bool NativeHelper::isBannerShowing = true;
bool NativeHelper::iapAll = true;
bool NativeHelper::iap1 = true;
bool NativeHelper::iap2 = true;
bool NativeHelper::iap3 = true;
string NativeHelper::iapIdAll = "";
string NativeHelper::iapId1 = "";
string NativeHelper::iapId2 = "";
string NativeHelper::iapId3 = "";
//string NativeHelper::iapId4 = "";
NativeHelper* NativeHelper::instance = NULL;


NativeHelper* NativeHelper::getInstance()
{
    if(!instanceFlag){
        instance = new NativeHelper();
        instanceFlag = true;
        instance->purchaseCallback = nullptr;
        instance->purchaseCancelledCallback = nullptr;
        instance->imagePickedCallback = nullptr;
        instance->photoTakenCallback = nullptr;
        instance->productPriceCallback = nullptr;
        instance->videosExitCallback = nullptr;
    }
    
    return instance;
}

bool NativeHelper::checkIfPurchased(string product)
{
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    return [appController checkIfProductPurchased:product];
    return true;
}

void NativeHelper::showBanner(int position){} //DO NOT USE

void NativeHelper::hideBanner(){} //DO NOT USE

void NativeHelper::showInterstitial(string eventName, string adSdk){
    //    if(eventName != ""){
    //        AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //        [appController showInterstitial:eventName :adSdk];
    //    }
}

void NativeHelper::adsSceneEnter(string name, int bannerPosition){ //0 - top, 1 - bottom
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    CCLOG("adsSceneEnter: %s [Banner on the %s]", name.c_str(), (bannerPosition == 0 ? "Top" : "Bottom"));
    //    [appController adsSceneEnter: name :bannerPosition];
    
}

void NativeHelper::adsSceneExit(string name){
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    CCLOG("adsSceneExit: %s", name.c_str());
    //    [appController adsSceneExit: name];
    
}

void NativeHelper::reportScreenChange(string name){
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    [appController reportScreenChange: name];
}

void NativeHelper::getProductPrice(string product){
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    [appController getProductPrice: product];
    
}

void NativeHelper::purchaseProduct(string product){
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    [appController purchaseProduct: product];
}

void NativeHelper::restorePurchases(){
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    [appController restorePurchases];
}

void NativeHelper::sendEmail(string subject, string body, string attachmentPath){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    [appController sendEmail:subject :body :attachmentPath];
}

//bool NativeHelper::checkIfPurchased(string product){
//    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
//    return [appController checkIfProductPurchased:product];
//}

void NativeHelper::showImagePicker(){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    [appController showImagePicker];
}

string NativeHelper::getDeviceId(string inapp){
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    string salt0 = "abc";
    //    string salt1 = "xyz";
    //
    //    if(inapp == IN_APP_ALL){
    //        salt0 = "Rl06MNVmPo";
    //        salt1 = "d9TQd1lVCq";
    //    }
    //    else if(inapp == IN_APP_1){
    //        salt0 = "0PVUQ6tCMA";
    //        salt1 = "X61vBVLS1V";
    //    }
    //    else if(inapp == IN_APP_2){
    //        salt0 = "dNBgmRd1pX";
    //        salt1 = "wDEhKve7q7";
    //    }
    //    else if(inapp == IN_APP_3){
    //        salt0 = "p0qWsApcE5";
    //        salt1 = "igRFavAGfy";
    //    }
    //    //    else if(inapp == IN_APP_4){
    //    //        salt0 = "2K9WvVGSIO";
    //    //        salt1 = "yJ6NXe6NMt";
    //    //    }
    //    return [appController getDeviceId:salt0 :salt1];
    return "";
}

string NativeHelper::getAppVersion(){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    return [appController getAppVersion];
//         return "";
}

string NativeHelper::getTempPath(){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    return [appController getTempPath];
//         return "";
}

string NativeHelper::getStoragePath(){
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    return [appController getStoragePath];
    return "";
}

void NativeHelper::goToVideos(){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    [appController goToVideos];
}

void NativeHelper::takePhoto(){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    [appController takePhoto];
}

string NativeHelper::getRateLink(){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    return [appController getRateLink];
    return "";
}

string NativeHelper::getFullLink(){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    return [appController getFullLink];
    return "";
}

string NativeHelper::getAppType(){
    return "appstore";
}

bool NativeHelper::isFull(){
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
//        return [appController isFull];
    return true;
//    return "";
}

void NativeHelper::openAppUrl(string url){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    [appController openiTunesInsideApp: url];
}

float NativeHelper::getDeviceScaleFactor(){
    return 1.0f;
}

string NativeHelper::getBundleId(){
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    return [appController getBundleId];
    return "";
}

string NativeHelper::getSetting(string propertyName, string defaultValue){
    return defaultValue;
    //    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    //    string str = [appController getSetting:propertyName];
    //    return str == "" ? defaultValue : str;
    //     return "";
}

//void NativeHelper::loadSettings(){
//    NativeHelper::fullId = UserDefault::getInstance()->getStringForKey("fullId", "");
//    if(NativeHelper::fullId == NativeHelper::getDeviceId()){
//        NativeHelper::full = true;
//    }
//    else{
//        NativeHelper::full = false;
//    }
//}
//
//void NativeHelper::saveSettings(){
//    UserDefault::getInstance()->setStringForKey("fullId", NativeHelper::fullId);
//    UserDefault::getInstance()->flush();
//}
void NativeHelper::loadSettings(){
    //    NativeHelper::iapIdAll = UserDefault::getInstance()->getStringForKey("iapIdAll", "");
    //    NativeHelper::iapId1 = UserDefault::getInstance()->getStringForKey("iapId1", "");
    //    NativeHelper::iapId2 = UserDefault::getInstance()->getStringForKey("iapId2", "");
    //    NativeHelper::iapId3 = UserDefault::getInstance()->getStringForKey("iapId3", "");
    //    //    NativeHelper::iapId4 = UserDefault::getInstance()->getStringForKey("iapId4", "");
    //
    //    NativeHelper::iapAll = NativeHelper::iapIdAll == NativeHelper::getDeviceId(IN_APP_ALL);
    //    NativeHelper::iap1 = NativeHelper::iapId1 == NativeHelper::getDeviceId(IN_APP_1);
    //    NativeHelper::iap2 = NativeHelper::iapId2 == NativeHelper::getDeviceId(IN_APP_2);
    //    NativeHelper::iap3 = NativeHelper::iapId3 == NativeHelper::getDeviceId(IN_APP_3);
    //    //    NativeHelper::iap4 = NativeHelper::iapId4 == NativeHelper::getDeviceId(IN_APP_4);
}

void NativeHelper::saveSettings(){
    //    UserDefault::getInstance()->setStringForKey("iapIdAll", NativeHelper::iapIdAll);
    //    UserDefault::getInstance()->setStringForKey("iapId1", NativeHelper::iapId1);
    //    UserDefault::getInstance()->setStringForKey("iapId2", NativeHelper::iapId2);
    //    UserDefault::getInstance()->setStringForKey("iapId3", NativeHelper::iapId3);
    //    //    UserDefault::getInstance()->setStringForKey("iapId4", NativeHelper::iapId4);
    //
    //    UserDefault::getInstance()->flush();
}
#endif
