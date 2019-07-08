#ifndef  __NATIVE_HELPER_H_
#define  __NATIVE_HELPER_H_

#include <string>
#include <functional>
#include "cocos2d.h"
#include "Definitions.h"

using namespace std;
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
namespace cocos2d
{
    public delegate void CSharpShowBannerDelegate(int position);
    public delegate void CSharpHideBannerDelegate();
    public delegate void CSharpShowInterstitialDelegate(Platform::String^ adSdk);
    public delegate void CSharpReportScreenChangeDelegate(Platform::String^ name);
    public delegate void CSharpPurchaseProductDelegate(Platform::String^ product);
    public delegate void CSharpRestorePurchasesDelegate();
    public delegate void CSharpGetProductPriceDelegate(Platform::String^ product);
    public delegate void CSharpSendEmailDelegate(Platform::String^ subject, Platform::String^ body, Platform::String^ attachmentPath);
    public delegate void CSharpShowImagePickerDelegate();
    public delegate Platform::String^ CSharpGetDeviceIdDelegate();
    public delegate Platform::String^ CSharpGetAppVersionDelegate();
    public delegate Platform::String^ CSharpGetTempPathDelegate();
    public delegate Platform::String^ CSharpGetStoragePathDelegate();
    public delegate void CSharpTakePhotoDelegate();
    public delegate void CSharpGoToVideosDelegate();
    public delegate Platform::String^ CSharpGetRateLinkDelegate();
    public delegate bool CSharpIsFullDelegate();
    public delegate float CSharpGetDeviceScaleFactorDelegate();
    public delegate Platform::String^ CSharpGetBundleIdDelegate();
    
    public ref class WP8NativeEventHelper sealed
    {
    public:
        
        void WP8NativeEventHelper::SetCSharpShowBannerDelegate(CSharpShowBannerDelegate^ delegate){ //sample part 2
            m_CSharpShowBannerDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpHideBannerBannerDelegate(CSharpHideBannerDelegate^ delegate){
            m_CSharpHideBannerDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpShowInterstitialDelegate(CSharpShowInterstitialDelegate^ delegate){
            m_CSharpShowInterstitialDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpReportScreenChangeDelegate(CSharpReportScreenChangeDelegate^ delegate){
            m_CSharpReportScreenChangeDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpPurchaseProductDelegate(CSharpPurchaseProductDelegate^ delegate){
            m_CSharpPurchaseProductDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpRestorePurchasesDelegate(CSharpRestorePurchasesDelegate^ delegate){
            m_CSharpRestorePurchasesDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpGetProductPriceDelegate(CSharpGetProductPriceDelegate^ delegate){
            m_CSharpGetProductPriceDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpSendEmailDelegate(CSharpSendEmailDelegate^ delegate){
            m_CSharpSendEmailDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpShowImagePickerDelegate(CSharpShowImagePickerDelegate^ delegate){
            m_CSharpShowImagePickerDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpGetDeviceIdDelegate(CSharpGetDeviceIdDelegate^ delegate){
            m_CSharpGetDeviceIdDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpGetAppVersionDelegate(CSharpGetAppVersionDelegate^ delegate){
            m_CSharpGetAppVersionDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpGetTempPathDelegate(CSharpGetTempPathDelegate^ delegate){
            m_CSharpGetTempPathDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpGetStoragePathDelegate(CSharpGetStoragePathDelegate^ delegate){
            m_CSharpGetStoragePathDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpTakePhotoDelegate(CSharpTakePhotoDelegate^ delegate){
            m_CSharpTakePhotoDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpGoToVideosDelegate(CSharpGoToVideosDelegate^ delegate){
            m_CSharpGoToVideosDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpGetRateLinkDelegate(CSharpGetRateLinkDelegate^ delegate){
            m_CSharpGetRateLinkDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpIsFullDelegate(CSharpIsFullDelegate^ delegate){
            m_CSharpIsFullDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpGetDeviceScaleFactor(CSharpGetDeviceScaleFactorDelegate^ delegate){
            m_CSharpGetDeviceScaleFactorDelegate = delegate;
        }
        
        void WP8NativeEventHelper::SetCSharpBundleIdDelegate(CSharpGetBundleIdDelegate^ delegate){
            m_CSharpGetBundleIdDelegate = delegate;
        }
        
        void CallShowBanner(int position);
        void CallHideBanner();
        void CallShowInterstitial(Platform::String^ adSdk);
        void CallReportScreenChange(Platform::String^ name);
        void CallPurchaseProduct(Platform::String^ product);
        void CallRestorePurchases();
        void CallGetProductPrice(Platform::String^ product);
        void CallSendEmail(Platform::String^ subject, Platform::String^ body, Platform::String^ attachmentPath);
        void CallShowImagePicker();
        Platform::String^ CallGetDeviceId();
        Platform::String^ CallGetAppVersion();
        Platform::String^ CallGetTempPath();
        Platform::String^ CallGetStoragePath();
        void CallTakePhoto();
        void CallGoToVideos();
        Platform::String^ CallGetRateLink();
        bool CallIsFull();
        float CallGetDeviceScaleFactor();
        Platform::String^ CallGetBundleId();
        
        void PurchaseCallback(Platform::String^ product);
        void PurchaseCancelledCallback();
        void ImagePickedCallback(Platform::String^ path);
        void PhotoTakenCallback(Platform::String^ filename);
        void ProductPriceCallback(Platform::String^ product, Platform::String^ price);
        void VideosExitCallback();
        
    private:
        property static CSharpShowBannerDelegate^ m_CSharpShowBannerDelegate;
        property static CSharpHideBannerDelegate^ m_CSharpHideBannerDelegate;
        property static CSharpShowInterstitialDelegate^ m_CSharpShowInterstitialDelegate;
        property static CSharpReportScreenChangeDelegate^ m_CSharpReportScreenChangeDelegate;
        property static CSharpPurchaseProductDelegate^ m_CSharpPurchaseProductDelegate;
        property static CSharpRestorePurchasesDelegate^ m_CSharpRestorePurchasesDelegate;
        property static CSharpGetProductPriceDelegate^ m_CSharpGetProductPriceDelegate;
        property static CSharpSendEmailDelegate^ m_CSharpSendEmailDelegate;
        property static CSharpShowImagePickerDelegate^ m_CSharpShowImagePickerDelegate;
        property static CSharpGetDeviceIdDelegate^ m_CSharpGetDeviceIdDelegate;
        property static CSharpGetAppVersionDelegate^ m_CSharpGetAppVersionDelegate;
        property static CSharpGetTempPathDelegate^ m_CSharpGetTempPathDelegate;
        property static CSharpGetStoragePathDelegate^ m_CSharpGetStoragePathDelegate;
        property static CSharpTakePhotoDelegate^ m_CSharpTakePhotoDelegate;
        property static CSharpGoToVideosDelegate^ m_CSharpGoToVideosDelegate;
        property static CSharpGetRateLinkDelegate^ m_CSharpGetRateLinkDelegate;
        property static CSharpIsFullDelegate^ m_CSharpIsFullDelegate;
        property static CSharpGetDeviceScaleFactorDelegate ^ m_CSharpGetDeviceScaleFactorDelegate;
        property static CSharpGetBundleIdDelegate ^ m_CSharpGetBundleIdDelegate;
    };
    
}
#endif

class NativeHelper
{
public:
    
    
    //properties
    static bool isBannerShowing;
    static bool iapAll;
    static bool iap1;
    static bool iap2;
    static bool iap3;
    static bool full;
    //    static bool iap4;
    static string iapIdAll;
    static string iapId1;
    static string iapId2;
    static string iapId3;
    static string fullId;
    
    //    static string iapId4;
    
    //ads
    static void showBanner(int position);
    static void hideBanner();
    static void showInterstitial(string eventName, string adSdk); //eventName for ios only
    static void adsSceneEnter(string name, int bannerPosition); //ios only
    static void adsSceneExit(string name); //ios only
    
    //analytics
    static void reportScreenChange(string name);
    
    //in-app
    static void purchaseProduct(string product);
    static void restorePurchases();
    static void getProductPrice(string product);
    static bool checkIfPurchased(string product);
    //static bool checkIfAnythingPurchased(); Z NUMBERSOW PRZEPISALAM, WCZESNIEJ NIE BYLO WCALE TEGO
    
    //utils
    static void sendEmail(string subject, string body, string attachmentPath);
    static void showImagePicker();
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    static string getDeviceId(string inapp);
    //#else
    //    static string getDeviceId();/////// specjalnie dla bledow w android studio dodalam////////////////////////////////////////////////////////////////////////////////////
    //#endif
    static string getAppVersion();
    static string getTempPath();
    static string getStoragePath();
    static void takePhoto();
    static string getRateLink();
    static string getFullLink();
    static bool isFull();
    static void openAppUrl(string url); //ios only
    static float getDeviceScaleFactor();
    static string getBundleId();
    static string getSetting(string propertyName, string defaultValue);
    static string getAppType(); //for apptutti
    
    //ros
    static void goToVideos();
    
    static void loadSettings();
    static void saveSettings();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    WP8NativeEventHelper^ wp8helper;
#endif
    
    //callbacks
    std::function<void(vector<string> *products)> purchaseCallback;
    std::function<void()> purchaseCancelledCallback;
    std::function<void(string filename)> imagePickedCallback;
    std::function<void(string filename)> photoTakenCallback;
    std::function<void(string product, string price)> productPriceCallback;
    std::function<void()> videosExitCallback;
    
    //instance required only for setting callback
    static NativeHelper* getInstance();
    
    ~NativeHelper()
    {
        instanceFlag = false;
    }
    
private:
    
    static bool instanceFlag;
    static NativeHelper* instance;
    
    NativeHelper() {
        purchaseCallback = nullptr;
        purchaseCancelledCallback = nullptr;
        imagePickedCallback = nullptr;
        photoTakenCallback = nullptr;
        productPriceCallback = nullptr;
        videosExitCallback = nullptr;
    };
    
};

#endif // __NATIVE_HELPER_H_
