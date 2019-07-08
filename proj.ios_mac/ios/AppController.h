#import <UIKit/UIKit.h>
#import "NativeHelper.h"
#import <CommonCrypto/CommonHMAC.h>

#pragma mark - Identyfikatory dla Reklam -
//- ADMob -
#define kAdUnitID_bannerView ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) ? @"ca-app-pub-2575683230512628/8089381820" : @"ca-app-pub-2575683230512628/1358301021" //////pierwszy numer ca-app jest dla iPada a drugi iPhone
#define kAdUnitID_interstitial ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) ? @"ca-app-pub-2575683230512628/2042848229" : @"ca-app-pub-2575683230512628/2835034227" //////pierwszy numer ca-app jest dla iPada a drugi iPhone

//- Charboost -
#define kStartWithAppId @"545357b343150f495ee5422c"
#define kAppSignature @"616307693f516aae6993a10c05ca70d27ed81a44"

//- OneSignal

#define ONE_SIGNAL_APP_ID @"5f90642-33b6-4705-9e58-b6d6e1061a51"

//- Zmienna przekazywana do mieszania klas -
#define kWartoscIntToMieszanieReklamy 300 // - Typ int

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, UIAlertViewDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;

#pragma mark - Slawek -
@property (nonatomic, strong) UIImage *imgNewApp;
@property (nonatomic, readwrite) BOOL czyOpeniTunes;
@property (nonatomic, readwrite) BOOL czyOpenCamera;
@property (strong, nonatomic) OneSignal *oneSignal;
@property (strong, nonatomic) FIRDatabase *firebase;
@property (strong, nonatomic) NSMutableDictionary *settings;
//-----
//- (BOOL)czyOblokowaneInApp:(NSString* ) name;
- (void) showInterstitial:(std::string) eventName :(std::string) adSdk;
- (void) adsSceneEnter:(std::string) name :(int) bannerPosition;
- (void) adsSceneExit:(std::string) name;
- (void) reportScreenChange:(std::string) name;
- (void) purchaseProduct:(std::string) product;
- (void) restorePurchases;
- (void) sendEmail:(std::string) subject :(std::string) body :(std::string) attachmentPath;
- (void) showImagePicker;
- (std::string) getDeviceId:(std::string) salt0 :(std::string) salt1;
- (std::string) getAppVersion;
- (std::string) getTempPath;
- (std::string) getStoragePath;
- (void) takePhoto;
- (void) goToVideos;
- (void) getProductPrice:(std::string) product;
- (void) gotProductPrice:(NSString*) product :(NSString*) price;
- (void) purchasedProducts:(NSArray*) aproducts;
- (void) purchaseCancelled;
- (bool) checkIfProductPurchased:(std::string) product;
- (bool) checkIfAnythingPurchased;
- (std::string) getRateLink;
- (std::string) getFullLink;
- (std::string) getSetting:(std::string) propertyName;
- (bool) isFull;
- (void)openiTunesInsideApp:(std::string)appId;
- (void) videosExit;
- (std::string) getBundleId;
- (UIWindow*) getWindow;
- (void)firebaseGetSettings;


@end

