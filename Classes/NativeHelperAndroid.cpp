#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "NativeHelper.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";

bool NativeHelper::instanceFlag = false;
bool NativeHelper::isBannerShowing = false;
//bool NativeHelper::iapAll = true;
//bool NativeHelper::iap1 = true;
//bool NativeHelper::iap2 = true;
//bool NativeHelper::iap3 = true;
//string NativeHelper::iapIdAll = "";
//string NativeHelper::iapId1 = "";
//string NativeHelper::iapId2 = "";
//string NativeHelper::iapId3 = "";

NativeHelper* NativeHelper::instance = NULL;

NativeHelper* NativeHelper::getInstance()
{
    if(!instanceFlag){
        instance = new NativeHelper();
        instanceFlag = true;
    }
    
    return instance;
}

//void NativeHelper::showBanner(int position){
//    if(NativeHelper::iapAll || NativeHelper::iap1 || NativeHelper::iap2 || NativeHelper::iap3 ) return;
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showBanner", "(I)V")){
//
//        t.env->CallStaticVoidMethod(t.classID, t.methodID, position);
//
//        t.env->DeleteLocalRef(t.classID);
//
//        isBannerShowing = true;
//    }
//}

//void NativeHelper::hideBanner(){
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hideBanner", "()V")){
//
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//
//        t.env->DeleteLocalRef(t.classID);
//
//        isBannerShowing = false;
//    }
//}
//
//void NativeHelper::showInterstitial(string eventName, string adSdk){
//    if(NativeHelper::iapAll || NativeHelper::iap1 || NativeHelper::iap2 || NativeHelper::iap3 ) return;
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showInterstitial", "(Ljava/lang/String;)V")){
//
//        jstring stringArg = t.env->NewStringUTF(adSdk.c_str());
//        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
//
//        t.env->DeleteLocalRef(t.classID);
//        t.env->DeleteLocalRef(stringArg);
//    }
//}

void NativeHelper::reportScreenChange(string name){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "reportScreenChange", "(Ljava/lang/String;)V")){
        
        jstring stringArg = t.env->NewStringUTF(name.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(stringArg);
    }
}

//void NativeHelper::purchaseProduct(string product){
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "purchaseProduct", "(Ljava/lang/String;)V")){
//
//        jstring stringArg = t.env->NewStringUTF(product.c_str());
//        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
//
//        t.env->DeleteLocalRef(t.classID);
//        t.env->DeleteLocalRef(stringArg);
//    }
//}
//
//void NativeHelper::restorePurchases(){
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "restorePurchases", "()V")){
//
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//
//        t.env->DeleteLocalRef(t.classID);
//    }
//}

//void NativeHelper::getProductPrice(string product){
//    cocos2d::JniMethodInfo t;
//    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getProductPrice", "(Ljava/lang/String;)V")){
//
//        jstring stringArg = t.env->NewStringUTF(product.c_str());
//        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
//
//        t.env->DeleteLocalRef(t.classID);
//        t.env->DeleteLocalRef(stringArg);
//    }
//}

//bool NativeHelper::checkIfPurchased(string product){
//    if(product == IN_APP_ALL){
//        NativeHelper::iapAll = NativeHelper::iapIdAll == NativeHelper::getDeviceId(IN_APP_ALL);
//        return NativeHelper::iapAll;
//    }
//    else if(product == IN_APP_1){
//        NativeHelper::iap1 = NativeHelper::iapId1 == NativeHelper::getDeviceId(IN_APP_1);
//        return NativeHelper::iap1;
//    }
//    else if(product == IN_APP_2){
//        NativeHelper::iap2 = NativeHelper::iapId2 == NativeHelper::getDeviceId(IN_APP_2);
//        return NativeHelper::iap2;
//        return true;
//    }
//    else if(product == IN_APP_3){
//        NativeHelper::iap3 = NativeHelper::iapId3 == NativeHelper::getDeviceId(IN_APP_3);
//        return NativeHelper::iap3;
//    }
//
//
//    return false;
//}

//---------
string NativeHelper::getDeviceId(string inapp){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getDeviceId", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")){
        string salt0 = "abc";
        string salt1 = "xyz";
        
//        if(inapp == IN_APP_ALL){
//            salt0 = "W5aq4Tu8QE";
//            salt1 = "9l72VunBVt";
//        }
//        else if(inapp == IN_APP_1){
//            salt0 = "xuJPBtEMRa";
//            salt1 = "2Cy5Jyq0kt";
//        }
//        else if(inapp == IN_APP_2){
//            salt0 = "trv4lU48RA";
//            salt1 = "i9DEfv5wMF";
//        }
//        else if(inapp == IN_APP_3){
//            salt0 = "pskwpM4be8";
//            salt1 = "u9v4w80bPH";
//        }
        
        jstring salt0Arg = t.env->NewStringUTF(salt0.c_str());
        jstring salt1Arg = t.env->NewStringUTF(salt1.c_str());
        
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID, salt0Arg, salt1Arg);
        std::string str = JniHelper::jstring2string(s);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(s);
        t.env->DeleteLocalRef(salt0Arg);
        t.env->DeleteLocalRef(salt1Arg);
        return str;
    }
    return "";
}

string NativeHelper::getAppVersion(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getAppVersion", "()Ljava/lang/String;")){
        
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string str = JniHelper::jstring2string(s);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(s);
        
        return str;
    }
    
    return "";
}

string NativeHelper::getTempPath(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getTempPath", "()Ljava/lang/String;")){
        
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string str = JniHelper::jstring2string(s);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(s);
        
        return str;
    }
    
    return "";
}


string NativeHelper::getAppType(){//////
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getAppType", "()Ljava/lang/String;")){
        
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string str = JniHelper::jstring2string(s);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(s);
        
        return str;
    }
    
    return "";
}

void NativeHelper::sendEmail(string subject, string body, string attachmentPath){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "sendEmail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")){
        
        jstring stringArg1 = t.env->NewStringUTF(subject.c_str());
        jstring stringArg2 = t.env->NewStringUTF(body.c_str());
        jstring stringArg3 = t.env->NewStringUTF(attachmentPath.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2, stringArg3);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(stringArg3);
    }
}

void NativeHelper::showImagePicker(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showImagePicker", "()V")){
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        
        t.env->DeleteLocalRef(t.classID);
    }
}

string NativeHelper::getStoragePath(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getStoragePath", "()Ljava/lang/String;")){
        
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string str = JniHelper::jstring2string(s);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(s);
        
        return str;
    }
    
    return "";
}

void NativeHelper::takePhoto(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "takePhoto", "()V")){
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        
        t.env->DeleteLocalRef(t.classID);
    }
}

void NativeHelper::adsSceneEnter(string name, int bannerPosition){} //ios only
void NativeHelper::adsSceneExit(string name){} //ios only

string NativeHelper::getRateLink(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getRateLink", "()Ljava/lang/String;")){
        
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string str = JniHelper::jstring2string(s);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(s);
        
        return str;
    }
    
    return "";
}

string NativeHelper::getFullLink(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getFullLink", "()Ljava/lang/String;")){
        
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string str = JniHelper::jstring2string(s);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(s);
        
        return str;
    }
    
    return "";
}

string NativeHelper::getBundleId(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getBundleId", "()Ljava/lang/String;")){
        
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string str = JniHelper::jstring2string(s);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(s);
        
        return str;
    }
    
    return "";
}

bool NativeHelper::isFull(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "isFull", "()Z")){
        
        jboolean b = (jboolean) t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        bool result = (bool) b;
        
        t.env->DeleteLocalRef(t.classID);
        
        return result;
    }
    
    return false;
}

void NativeHelper::openAppUrl(string url){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "openAppUrl", "(Ljava/lang/String;)V")){
        
        jstring stringArg = t.env->NewStringUTF(url.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(stringArg);
    }
}

void NativeHelper::goToVideos(){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "goToVideos", "()V")){
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        
        t.env->DeleteLocalRef(t.classID);
    }
}

float NativeHelper::getDeviceScaleFactor(){
    return 1.0f;
}

string NativeHelper::getSetting(string propertyName, string defaultValue){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getSetting", "(Ljava/lang/String;)Ljava/lang/String;")){
        
        jstring stringArg = t.env->NewStringUTF(propertyName.c_str());
        jstring s = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID, stringArg);
        std::string str = JniHelper::jstring2string(s);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(stringArg);
        
        return str == "" ? defaultValue : str;
    }
}


void NativeHelper::loadSettings(){
//    NativeHelper::iapIdAll = UserDefault::getInstance()->getStringForKey("iapIdAll", "");
//    NativeHelper::iapId1 = UserDefault::getInstance()->getStringForKey("iapId1", "");
//    NativeHelper::iapId2 = UserDefault::getInstance()->getStringForKey("iapId2", "");
//    NativeHelper::iapId3 = UserDefault::getInstance()->getStringForKey("iapId3", "");
//
//
//    NativeHelper::iapAll = NativeHelper::iapIdAll == NativeHelper::getDeviceId(IN_APP_ALL);
//    NativeHelper::iap1 = NativeHelper::iapId1 == NativeHelper::getDeviceId(IN_APP_1);
//    NativeHelper::iap2 = NativeHelper::iapId2 == NativeHelper::getDeviceId(IN_APP_2);
//    NativeHelper::iap3 = NativeHelper::iapId3 == NativeHelper::getDeviceId(IN_APP_3);
    
}

void NativeHelper::saveSettings(){
//    UserDefault::getInstance()->setStringForKey("iapIdAll", NativeHelper::iapIdAll);
//    UserDefault::getInstance()->setStringForKey("iapId1", NativeHelper::iapId1);
//    UserDefault::getInstance()->setStringForKey("iapId2", NativeHelper::iapId2);
//    UserDefault::getInstance()->setStringForKey("iapId3", NativeHelper::iapId3);
//    
//    
//    UserDefault::getInstance()->flush();
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseCallback(JNIEnv* env, jobject thiz, jobjectArray stringArrays);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseCancelledCallback(JNIEnv* env, jobject thiz);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_imagePickedCallback(JNIEnv* env, jobject thiz, jstring filename);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_photoTakenCallback(JNIEnv* env, jobject thiz, jstring filename);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_productPriceCallback(JNIEnv* env, jobject thiz, jstring product, jstring price);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_videosCallback(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseCallback(JNIEnv* env, jobject thiz, jobjectArray stringArrays){
    auto helper = NativeHelper::getInstance();
    if(helper->purchaseCallback != nullptr){
        
        vector<string> *products = new vector<string>();
        int size = env->GetArrayLength(stringArrays);
        
        for (int i = 0; i < size; i++){
            jstring javaString = (jstring) env->GetObjectArrayElement(stringArrays, i);
            const char* charString = env->GetStringUTFChars(javaString, 0);
            
            std::string cstring(charString);
            products->push_back(cstring);
            
            env->ReleaseStringUTFChars(javaString, charString);
            env->DeleteLocalRef(javaString);
        }
        
        helper->purchaseCallback(products);
    }
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseCancelledCallback(JNIEnv* env, jobject thiz){
    auto helper = NativeHelper::getInstance();
    if(helper->purchaseCancelledCallback != nullptr){
        helper->purchaseCancelledCallback();
    }
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_imagePickedCallback(JNIEnv* env, jobject thiz, jstring filename){
    std::string str = JniHelper::jstring2string(filename);
    auto helper = NativeHelper::getInstance();
    if(helper->imagePickedCallback != nullptr){
        helper->imagePickedCallback(str);
    }
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_photoTakenCallback(JNIEnv* env, jobject thiz, jstring filename){
    std::string str = JniHelper::jstring2string(filename);
    auto helper = NativeHelper::getInstance();
    if(helper->photoTakenCallback != nullptr){
        helper->photoTakenCallback(str);
    }
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_productPriceCallback(JNIEnv* env, jobject thiz, jstring product, jstring price){
    std::string _product = JniHelper::jstring2string(product);
    std::string _price = JniHelper::jstring2string(price);
    auto helper = NativeHelper::getInstance();
    if(helper->productPriceCallback != nullptr){
        helper->productPriceCallback(_product, _price);
    }
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_videosCallback(JNIEnv* env, jobject thiz){
    auto helper = NativeHelper::getInstance();
    if(helper->videosExitCallback != nullptr){
        helper->videosExitCallback();
    }
}

#endif
