#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

#ifndef  __NATIVE_HELPER_WP_H_
#define  __NATIVE_HELPER_WP_H_

#include "NativeHelper.h"
#include "WPUtils.h"

void WP8NativeEventHelper::CallShowBanner(int position){
    if (m_CSharpShowBannerDelegate)
    {
        m_CSharpShowBannerDelegate->Invoke(position);
    }
}

void WP8NativeEventHelper::CallHideBanner(){
    if (m_CSharpHideBannerDelegate)
    {
        m_CSharpHideBannerDelegate->Invoke();
    }
}

void WP8NativeEventHelper::CallShowInterstitial(Platform::String^ adSdk){
    if (m_CSharpShowInterstitialDelegate)
    {
        m_CSharpShowInterstitialDelegate->Invoke(adSdk);
    }
}

void WP8NativeEventHelper::CallReportScreenChange(Platform::String^ name){
    if (m_CSharpReportScreenChangeDelegate)
    {
        m_CSharpReportScreenChangeDelegate->Invoke(name);
    }
}

void WP8NativeEventHelper::CallPurchaseProduct(Platform::String^ product){
    if (m_CSharpPurchaseProductDelegate)
    {
        m_CSharpPurchaseProductDelegate->Invoke(product);
    }
}

void WP8NativeEventHelper::CallRestorePurchases(){
    if (m_CSharpRestorePurchasesDelegate)
    {
        m_CSharpRestorePurchasesDelegate->Invoke();
    }
}

void WP8NativeEventHelper::CallSendEmail(Platform::String^ subject, Platform::String^ body, Platform::String^ attachmentPath){
    if (m_CSharpSendEmailDelegate)
    {
        m_CSharpSendEmailDelegate->Invoke(subject, body, attachmentPath);
    }
}

void WP8NativeEventHelper::CallGetProductPrice(Platform::String^ product){
    if (m_CSharpGetProductPriceDelegate)
    {
        m_CSharpGetProductPriceDelegate->Invoke(product);
    }
}

void WP8NativeEventHelper::CallShowImagePicker(){
    if (m_CSharpShowImagePickerDelegate)
    {
        m_CSharpShowImagePickerDelegate->Invoke();
    }
}

Platform::String^ WP8NativeEventHelper::CallGetDeviceId(){
    if (m_CSharpGetDeviceIdDelegate)
    {
        Platform::String^ result = m_CSharpGetDeviceIdDelegate->Invoke();
        return result;
    }
    return nullptr;
}

Platform::String^ WP8NativeEventHelper::CallGetAppVersion(){
    if (m_CSharpGetAppVersionDelegate)
    {
        Platform::String^ result = m_CSharpGetAppVersionDelegate->Invoke();
        return result;
    }
    return nullptr;
}

Platform::String^ WP8NativeEventHelper::CallGetTempPath(){
    if (m_CSharpGetTempPathDelegate)
    {
        Platform::String^ result = m_CSharpGetTempPathDelegate->Invoke();
        return result;
    }
    return nullptr;
}

Platform::String^ WP8NativeEventHelper::CallGetStoragePath(){
    if (m_CSharpGetStoragePathDelegate)
    {
        Platform::String^ result = m_CSharpGetStoragePathDelegate->Invoke();
        return result;
    }
    return nullptr;
}

void WP8NativeEventHelper::CallTakePhoto(){
    if (m_CSharpTakePhotoDelegate)
    {
        m_CSharpTakePhotoDelegate->Invoke();
    }
}

Platform::String^ WP8NativeEventHelper::CallGetRateLink(){
    if (m_CSharpGetRateLinkDelegate)
    {
        Platform::String^ result = m_CSharpGetRateLinkDelegate->Invoke();
        return result;
    }
    return nullptr;
}

bool WP8NativeEventHelper::CallIsFull(){
    if (m_CSharpIsFullDelegate)
    {
        bool result = m_CSharpIsFullDelegate->Invoke();
        return result;
    }
    return false;
}

void WP8NativeEventHelper::CallGoToVideos(){
    if (m_CSharpGoToVideosDelegate)
    {
        m_CSharpGoToVideosDelegate->Invoke();
    }
}

float WP8NativeEventHelper::CallGetDeviceScaleFactor(){
    if (m_CSharpGetDeviceScaleFactorDelegate){
        float result = m_CSharpGetDeviceScaleFactorDelegate->Invoke();
        return result;
    }
    return 1.0f;
}

void WP8NativeEventHelper::PurchaseCallback(Platform::String^ product)
{
    vector<std::string > *vproducts = new vector<std::string >();
    vproducts->push_back(PlatformStringToString(product));
    
    auto helper = NativeHelper::getInstance();
    if (helper->purchaseCallback != nullptr){
        helper->purchaseCallback(vproducts);
    }
}

void WP8NativeEventHelper::PurchaseCancelledCallback()
{
    auto helper = NativeHelper::getInstance();
    if (helper->purchaseCancelledCallback != nullptr){
        helper->purchaseCancelledCallback();
    }
}

void WP8NativeEventHelper::ImagePickedCallback(Platform::String^ path)
{
    auto helper = NativeHelper::getInstance();
    if (helper->imagePickedCallback != nullptr){
        helper->imagePickedCallback(PlatformStringToString(path));
    }
}

void WP8NativeEventHelper::PhotoTakenCallback(Platform::String^ path)
{
    auto helper = NativeHelper::getInstance();
    if (helper->photoTakenCallback != nullptr){
        helper->photoTakenCallback(PlatformStringToString(path));
    }
}

void WP8NativeEventHelper::ProductPriceCallback(Platform::String^ product, Platform::String^ price)
{
    auto helper = NativeHelper::getInstance();
    if (helper->productPriceCallback != nullptr){
        string _product = PlatformStringToString(product);
        string _price = PlatformStringToString(price);
        helper->productPriceCallback(_product, _price);
    }
}

void WP8NativeEventHelper::VideosExitCallback()
{
    auto helper = NativeHelper::getInstance();
    if (helper->videosExitCallback != nullptr){
        helper->videosExitCallback();
    }
}

bool NativeHelper::instanceFlag = false;
bool NativeHelper::isBannerShowing = true;
bool NativeHelper::full = false;
string NativeHelper::fullId = "";
NativeHelper* NativeHelper::instance = NULL;

NativeHelper* NativeHelper::getInstance()
{
    if(!instanceFlag){
        instance = new NativeHelper();
        instanceFlag = true;
        instance->wp8helper = ref new WP8NativeEventHelper();
    }
    
    return instance;
}

void NativeHelper::showBanner(int position){
    NativeHelper::getInstance()->wp8helper->CallShowBanner(position);
}

void NativeHelper::hideBanner(){
    NativeHelper::getInstance()->wp8helper->CallHideBanner();
}

void NativeHelper::showInterstitial(string eventName, string adSdk){
    NativeHelper::getInstance()->wp8helper->CallShowInterstitial(PlatformStringFromString(adSdk));
}

void NativeHelper::reportScreenChange(string name){
    NativeHelper::getInstance()->wp8helper->CallReportScreenChange(PlatformStringFromString(name));
}

void NativeHelper::purchaseProduct(string product){
    NativeHelper::getInstance()->wp8helper->CallPurchaseProduct(PlatformStringFromString(product));
}

void NativeHelper::restorePurchases(){
    NativeHelper::getInstance()->wp8helper->CallRestorePurchases();
}

void NativeHelper::getProductPrice(string product){
    NativeHelper::getInstance()->wp8helper->CallGetProductPrice(PlatformStringFromString(product));
}

string NativeHelper::getDeviceId(){
    Platform::String^ result = NativeHelper::getInstance()->wp8helper->CallGetDeviceId();
    return PlatformStringToString(result);
}

string NativeHelper::getAppVersion(){
    Platform::String^ result = NativeHelper::getInstance()->wp8helper->CallGetAppVersion();
    return PlatformStringToString(result);
}

string NativeHelper::getTempPath(){
    Platform::String^ result = NativeHelper::getInstance()->wp8helper->CallGetTempPath();
    return PlatformStringToString(result);
}

void NativeHelper::sendEmail(string subject, string body, string attachmentPath){
    NativeHelper::getInstance()->wp8helper->CallSendEmail(PlatformStringFromString(subject), PlatformStringFromString(body), PlatformStringFromString(attachmentPath));
}

void NativeHelper::showImagePicker(){
    NativeHelper::getInstance()->wp8helper->CallShowImagePicker();
}

string NativeHelper::getStoragePath(){
    Platform::String^ result = NativeHelper::getInstance()->wp8helper->CallGetStoragePath();
    return PlatformStringToString(result);
}

void NativeHelper::takePhoto(){
    NativeHelper::getInstance()->wp8helper->CallTakePhoto();
}

string NativeHelper::getRateLink(){
    Platform::String^ result = NativeHelper::getInstance()->wp8helper->CallGetRateLink();
    return PlatformStringToString(result);
}

bool NativeHelper::isFull(){
    bool result = NativeHelper::getInstance()->wp8helper->CallIsFull();
    return result;
}

float NativeHelper::getDeviceScaleFactor(){
    float result = NativeHelper::getInstance()->wp8helper->CallGetDeviceScaleFactor();
    return result;
}

void NativeHelper::openAppUrl(string url){} //ios only

void NativeHelper::goToVideos(){
    NativeHelper::getInstance()->wp8helper->CallGoToVideos();
}

void NativeHelper::adsSceneEnter(string name, int bannerPosition){} //ios only
void NativeHelper::adsSceneExit(string name){} //ios only

void NativeHelper::loadSettings(){
    NativeHelper::fullId = UserDefault::getInstance()->getStringForKey("fullId", "");
    if(NativeHelper::fullId == NativeHelper::getDeviceId()){
        NativeHelper::full = true;
    }
    else{
        NativeHelper::full = false;
    }
}

void NativeHelper::saveSettings(){
    UserDefault::getInstance()->setStringForKey("fullId", NativeHelper::fullId);
    UserDefault::getInstance()->flush();
}

#endif

#endif
