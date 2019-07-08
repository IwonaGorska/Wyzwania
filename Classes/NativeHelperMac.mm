#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

#include "NativeHelper.h"

bool NativeHelper::instanceFlag = false;

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


void NativeHelper::adsSceneEnter(string name, int bannerPosition){}

void NativeHelper::adsSceneExit(string name){}

void NativeHelper::reportScreenChange(string name){}



void NativeHelper::sendEmail(string subject, string body, string attachmentPath){}

void NativeHelper::showImagePicker(){}


string NativeHelper::getDeviceId(string inapp){
    return "";
}

string NativeHelper::getAppVersion(){
    return "";
}

string NativeHelper::getTempPath(){
    return "";
}

string NativeHelper::getStoragePath(){
    return "";
}

void NativeHelper::goToVideos(){}

void NativeHelper::takePhoto(){}

string NativeHelper::getRateLink(){
    return "";
}

string NativeHelper::getFullLink(){
    return "";
}

string NativeHelper::getAppType(){
    return "appstore";
}

bool NativeHelper::isFull(){
    return true;
}

void NativeHelper::openAppUrl(string url){}

float NativeHelper::getDeviceScaleFactor(){
    return 1.0f;
}

string NativeHelper::getBundleId(){
    return "";
}

string NativeHelper::getSetting(string propertyName, string defaultValue){
    return "";
}



void NativeHelper::loadSettings(){
    
}

void NativeHelper::saveSettings(){
    
}



#endif
