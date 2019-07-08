/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"

#import "RootViewController.h"

#import <FBSDKCoreKit/FBSDKCoreKit.h>

const NSString* rateLink = @"itms://itunes.apple.com/us/app/apple-store/id460365428?mt=8";
const NSString* fullLink = @"itms://itunes.apple.com/us/app/apple-store/id999351878?mt=8"; //FIXME
const NSString* FIREBASE_APP_NAME = @"hide_and_seek_free_ios";

@implementation AppController

//######### PARSE PUSH #########
long int statusParsePush=0;
//######### PARSE PUSH #########


#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [application setStatusBarHidden:YES];
    
    self.czyOpenCamera = NO;
    
    //######### PARSE PUSH  - SPRAWDZANIE USTAWIĆ NA POCZĄTKU ########################
//    NSMutableDictionary *plistDict = [[NSMutableDictionary alloc] initWithContentsOfFile:[self statusPath]];
//    statusParsePush = [[plistDict valueForKey:@"statusToken"] integerValue];
    //######### PARSE PUSH  - SPRAWDZANIE USTAWIĆ NA POCZĄTKU ########################
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Override point for customization after application launch.
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the CCEAGLView
    
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    //    _viewController.wantsFullScreenLayout = YES;
    
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:YES];
    
    // Use RootViewController manage CCEAGLView
    
    
    _viewController.view = eaglView;
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    app->run();
    
#pragma mark SettingRosZakup
//    [SettingsRosReklama settingsRosReklama];
//    [SettingsRosZakup settingsRosZakup];
    
#pragma mark GTM
//    [[GoogleTag sharedGoogleTag] uruchomGoogleTagAnaliticsWith:launchOptions];
    
    
#pragma mark SettingsRosOurReklama
//    [SettingsRosOurReklama settingsRosOurReklama];
    
//    [[GoogleTag sharedGoogleTag] performSelector:@selector(refreshContainer) withObject:nil afterDelay:2.0];
    //    [[GoogleTag sharedGoogleTag] refreshContainer];
    
#pragma mark OneSignal
    [OneSignal initWithLaunchOptions:launchOptions appId:ONE_SIGNAL_APP_ID handleNotificationAction:^(OSNotificationOpenedResult *result) {
        
        // This block gets called when the user reacts to a notification received
        OSNotificationPayload* payload = result.notification.payload;
        NSDictionary* additionalData = payload.additionalData;
        
        NSString* message = [payload.title copy];
        
        NSLog(@"OneSignal Notification opened:\nMessage: %@", message);
        
        if (additionalData) {
            NSLog(@"additionalData: %@", additionalData);
            
            NSString *linkApp = [additionalData valueForKey:@"a1"];
            if(!linkApp) linkApp = @"";
            
            NSString *payloadTitle = [additionalData valueForKey:@"pT"];
            if(!payloadTitle) payloadTitle = @"Download and enjoy!";
            
            NSString *payloadAlert = [additionalData valueForKey:@"pA"];
            if(!payloadAlert) payloadAlert = @"Open iTunes";
            
            NSString *promo = [additionalData valueForKey:@"pR"];
            if(!promo) promo = @"YES";
            
            NSLog(@"linkApp %@", linkApp);
            NSLog(@"payloadTitle %@", payloadTitle);
            NSLog(@"payloadAlert %@", payloadAlert);
            NSLog(@"promo %@", promo);
            
            if(![linkApp isEqualToString:@""])//OTWIERAMY APLIKACJE JAKO BIORACA UDZIAL W PROMOCJI
            {
                //zapisanie do plista linku
                NSString *filePath = [self statusPath];
                NSMutableDictionary* plistDict = [[NSMutableDictionary alloc] initWithContentsOfFile:filePath];
                [plistDict setValue:linkApp forKey:@"linkOpen"];
                [plistDict writeToFile:filePath atomically:YES];
                
                UIAlertView *av = [[UIAlertView alloc] initWithTitle:payloadTitle  message:payloadAlert delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:promo, nil];
                [av show];
                
            }
            
        }
        
    }];
    
#pragma mark Firebase
    [FIRApp configure];
    _firebase = [FIRDatabase database];
    _firebase.persistenceEnabled = true;
    _settings = [[NSMutableDictionary alloc] init];
    [self firebaseGetSettings];
    
    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                    didFinishLaunchingWithOptions:launchOptions];
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    
    [FBSDKAppEvents activateApp];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    
}

- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)err {
    
    
}
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    
}


- (void)dealloc {
    [window release];
    [super dealloc];
}

#pragma mark - Ustawianie Orientacji ekranu -
- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        if(self.czyOpenCamera == YES){
            return UIInterfaceOrientationMaskPortrait;
        }
        else{
            return UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
        }
    }
    else{
        return UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
    }
}

#pragma mark - PARSE PUSH pomocnicze -
- (NSString *)statusPath {
    BOOL success;
    NSError *error;
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat: @"push.plist"]];
    success = [fileManager fileExistsAtPath:filePath];
    if (!success) {
        NSString *path = [[NSBundle mainBundle] pathForResource: @"push" ofType:@"plist"];
        success = [fileManager copyItemAtPath:path toPath:filePath error:&error];
        if(!success) {
            
        }
        ///////NSLog(@"nie mozna znalezc pliku status");
    }
    return filePath;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

- (void) showInterstitial:(std::string) eventName :(std::string) adSdk {
    NSString *dev = ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) ? @"iPhone" : @"iPad";
    NSString *nseventName = [NSString stringWithCString:eventName.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *nseventName2 = [NSString stringWithFormat:@"%@_%@", nseventName, dev];
//    [[SettingsRosReklama settingsRosReklama] pokazZestawReklamPelnoekranowych:nseventName2 controller:self.viewController];
}

- (void) adsSceneEnter:(std::string) name :(int) bannerPosition {
    NSString *nsname = [NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]];
    
    NSString *dev = ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) ? @"iPhone" : @"iPad";
    nsname = [NSString stringWithFormat:@"%@_WillAppear_%@", nsname, dev];
    
//    [[SettingsRosReklama settingsRosReklama] pokazZestawReklam:nsname controller:self.viewController polozenieBannera:(PolozenieReklamy)bannerPosition];
}

- (void) adsSceneExit:(std::string) name {
    NSString *nsname = [NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *dev = ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) ? @"iPhone" : @"iPad";
    nsname = [NSString stringWithFormat:@"%@_WillDisappear_%@", nsname, dev];
//    [[SettingsRosReklama settingsRosReklama] pokazZestawReklamPelnoekranowych:nsname controller:self.viewController];
}

- (void) reportScreenChange:(std::string) name {
    NSString *nsname = [NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]];
//    [[GoogleTag sharedGoogleTag] nazwaEkranu:nsname];
}

- (void) purchaseProduct:(std::string) product {
//    [[SettingsRosZakup settingsRosZakup] windowTransaction].layer.hidden = NO;
    NSString *nsproduct = [NSString stringWithCString:product.c_str() encoding:[NSString defaultCStringEncoding]];
//    [[SettingsRosZakup settingsRosZakup] kupujeProdukt:nsproduct];
}

- (void) restorePurchases {
//    [[SettingsRosZakup settingsRosZakup] windowTransaction].layer.hidden = NO;
//    [[SettingsRosZakup settingsRosZakup] restoreAll];
    
}

- (void) getProductPrice:(std::string) product {
    NSString *nsproduct = [NSString stringWithCString:product.c_str() encoding:[NSString defaultCStringEncoding]];
//    [[SettingsRosZakup settingsRosZakup] fetchAvailableProductWithName:nsproduct];
}

- (void) gotProductPrice:(NSString*) product :(NSString*) price {
    std::string *stdproduct = new std::string([product UTF8String]);
    std::string *stdprice = new std::string([price UTF8String]);
    NativeHelper* helper = NativeHelper::getInstance();
    if(helper->productPriceCallback != nullptr){
        helper->productPriceCallback(*stdproduct, *stdprice);
    }
}

- (void) purchasedProducts:(NSArray*) aproducts {
//    [[SettingsRosZakup settingsRosZakup] windowTransaction].layer.hidden = YES;
    NativeHelper* helper = NativeHelper::getInstance();
    if(helper->purchaseCallback != nullptr){
        
        vector<std::string> *products = new vector<std::string>();
        
        for(NSString* product in aproducts){
            std::string *stdproduct = new std::string([product UTF8String]);
            products->push_back(*stdproduct);
        }
        
        helper->purchaseCallback(products);
    }
}

- (void) purchaseCancelled {
    NativeHelper* helper = NativeHelper::getInstance();
    if(helper->purchaseCancelledCallback != nullptr){
        helper->purchaseCancelledCallback();
    }
}

- (bool) checkIfProductPurchased:(std::string) product {
    NSString *nsproduct = [NSString stringWithCString:product.c_str() encoding:[NSString defaultCStringEncoding]];
//    return [[SettingsRosZakup settingsRosZakup] czyOblokowaneInApp:nsproduct];
}

- (bool) checkIfAnythingPurchased {
//    return [[SettingsRosZakup settingsRosZakup] czyWykupioneJakiesInApp];
}

- (void) sendEmail:(std::string) subject :(std::string) body :(std::string) attachmentPath {
    
    
    NSString *nssubject = [NSString stringWithCString:subject.c_str() encoding:[NSString defaultCStringEncoding]];
    //    NSString *nsbody = [NSString stringWithCString:body.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *nsbody = [NSString stringWithUTF8String: body.c_str()];
    NSString *nsattachmentPath = [NSString stringWithCString:attachmentPath.c_str() encoding:[NSString defaultCStringEncoding]];
    [self.viewController sendEmail:nssubject :nsbody :nsattachmentPath];
}

- (void) showImagePicker {
    [self.viewController showImagePicker: UIImagePickerControllerSourceTypePhotoLibrary];
}



//- (BOOL)czyOblokowaneInApp:(NSString* ) name
//{
//    //NSLog(@"czyOblokowaneInApp: %@", name);
//    for(int i = 0; i < self.arrZakupInAppClass.count; i++)
//    {
//        ZakupInAppClass *zClass = self.arrZakupInAppClass[i];
//        MieszanieClass* mieszanie = [[MieszanieClass alloc] init:zClass.nrInAppMieszanieClass];
//        //NSLog(@"nameInApp: %@, mieszanie: %@", zClass.nameInApp, mieszanie.mieszanie);
//        if([zClass.nameInApp isEqualToString:name]){
//            if(!zClass.czyWystepujeWSettingsPlist) return YES;
//            //NSLog(@"znalazlem");
//
//            NSString* zClassNameInApp = [[self.rosDict valueForKey:@"spisInApp"] valueForKey:zClass.nameInApp];
//
//            zClass.czyWykupioneInApp = [zClassNameInApp isEqualToString:mieszanie.mieszanie];
//            //NSLog(@"zClassNameInApp: %@, mieszanie: %@", zClassNameInApp, mieszanie.mieszanie);
//            //NSLog(@"zgadza sie: %d", zClass.czyWykupioneInApp);
//            return zClass.czyWykupioneInApp;
//        }
//    }
//
//    return NO;
//}

- (std::string) getDeviceId:(std::string) salt0 :(std::string) salt1 {
    NSString *nssalt0 = [NSString stringWithCString: salt0.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *nssalt1 = [NSString stringWithCString: salt1.c_str() encoding:[NSString defaultCStringEncoding]];
    
    NSString* nsstring = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    
    NSString* data = @"";
    data = [data stringByAppendingString:nssalt0];
    data = [data stringByAppendingString:nsstring];
    data = [data stringByAppendingString:nssalt1];
    
    //https://www.random.org/strings/?num=1&len=20&upperalpha=on&loweralpha=on&unique=on&format=html&rnd=new
    NSString* key = @"RmDhkRISnzoeRMueRiLJ";
    
    const char *cKey = [key cStringUsingEncoding:NSASCIIStringEncoding];
    const char *cData = [data cStringUsingEncoding:NSASCIIStringEncoding];
    unsigned char cHMAC[CC_SHA256_DIGEST_LENGTH];
    CCHmac(kCCHmacAlgSHA256, cKey, strlen(cKey), cData, strlen(cData), cHMAC);
    NSData *hash = [[NSData alloc] initWithBytes:cHMAC length:sizeof(cHMAC)];
    
    NSString* encrypted = [AppController base64forData:hash];
    
    std::string *stdstring = new std::string([encrypted UTF8String]);
    
    return *stdstring;
}

//- (std::string) getAppVersion {
//    NSString *nsstring = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
//
//    std::string *stdstring = new std::string([nsstring UTF8String]);
//
//    return *stdstring;
//}
//
//- (std::string) getTempPath {
//    NSString* tempDir = NSTemporaryDirectory();
//    if (tempDir == nil)
//        tempDir = @"/tmp";
//
//    std::string *stdstring = new std::string([tempDir UTF8String]);
//
//    return *stdstring;
//}

- (std::string) getAppVersion {
    NSString *nsstring = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
    
    std::string *stdstring = new std::string([nsstring UTF8String]);
    
    return *stdstring;
}

- (std::string) getTempPath {
    NSString* tempDir = NSTemporaryDirectory();
    if (tempDir == nil)
        tempDir = @"/tmp";
    
    std::string *stdstring = new std::string([tempDir UTF8String]);
    
    return *stdstring;
}

- (std::string) getStoragePath {
    NSArray *dirPaths;
    NSString *docsDir;
    dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    docsDir = dirPaths[0];
    
    std::string *stdstring = new std::string([docsDir UTF8String]);
    
    *stdstring += "/";
    
    return *stdstring;
}

- (void) takePhoto {
    [self.viewController showImagePicker: UIImagePickerControllerSourceTypeCamera];
}

- (void) goToVideos {
    
}

- (std::string) getRateLink {
    std::string *stdstring = new std::string([rateLink UTF8String]);
    return *stdstring;
}

- (std::string) getFullLink {
    std::string *stdstring = new std::string([fullLink UTF8String]);
    return *stdstring;
}

- (std::string) getBundleId {
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    std::string *stdstring = new std::string([bundleIdentifier UTF8String]);
    return *stdstring;
}

- (std::string) getSetting:(std::string) propertyName {
    NSString *key = [NSString stringWithCString:propertyName.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString* value = [_settings valueForKey:key];
    if(value != nil){
        std::string *stdstring = new std::string([value UTF8String]);
        return *stdstring;
    }
    return "";
}

- (bool) isFull {
    return false;
}

+ (NSString*)base64forData:(NSData*)theData {
    const uint8_t* input = (const uint8_t*)[theData bytes];
    NSInteger length = [theData length];
    
    static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    
    NSMutableData* data = [NSMutableData dataWithLength:((length + 2) / 3) * 4];
    uint8_t* output = (uint8_t*)data.mutableBytes;
    
    NSInteger i;
    for (i=0; i < length; i += 3) {
        NSInteger value = 0;
        NSInteger j;
        for (j = i; j < (i + 3); j++) {
            value <<= 8;
            
            if (j < length) {  value |= (0xFF & input[j]);  }  }  NSInteger theIndex = (i / 3) * 4;  output[theIndex + 0] = table[(value >> 18) & 0x3F];
        output[theIndex + 1] = table[(value >> 12) & 0x3F];
        output[theIndex + 2] = (i + 1) < length ? table[(value >> 6) & 0x3F] : '=';
        output[theIndex + 3] = (i + 2) < length ? table[(value >> 0) & 0x3F] : '=';
    }
    
    return [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
}

- (void)openiTunesInsideApp:(std::string)appId {
    
    NSString *nsid = [NSString stringWithCString:appId.c_str() encoding:[NSString defaultCStringEncoding]];
    
    NSString *model = [[UIDevice currentDevice] model];
    if ([model rangeOfString:@"Simulator"].location != NSNotFound) {
        if(nsid != nil) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Simulator Info" message:@"You are using Simulator. Please check it on real device." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
            [alert show];
        }
    } else {
        if(nsid != nil) {
            
//            if([SettingsRosZakup settingsRosZakup ].czyInternetReachable == NO && (SYSTEM_VERSION_LESS_THAN(@"7.0")))  {
//                
//                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Info" message:@"Please check your internet connection or try again later" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
//                [alert show];
//                
//                return;
//            }
            if (SYSTEM_VERSION_LESS_THAN(@"6.0") || [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
            {
                [[UIApplication sharedApplication] openURL: [NSURL URLWithString:[NSString stringWithFormat:@"https://itunes.apple.com/us/app/id%@?mt=8", nsid]]];
                
                return;
            }
            
            UIActivityIndicatorView *acIndicator = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(0, 0, self.viewController.view.frame.size.width, self.viewController.view.frame.size.height)];
            acIndicator.backgroundColor = [UIColor blackColor];
            acIndicator.alpha = 0.8;
            [self.viewController.view addSubview:acIndicator];
            [acIndicator startAnimating];
            
            SKStoreProductViewController * otherViewController = [[SKStoreProductViewController alloc] init];
            
            otherViewController.delegate = self.viewController;
            
            NSDictionary *parameters =@{SKStoreProductParameterITunesItemIdentifier:[NSNumber numberWithLong:[nsid integerValue]]};
            
            [otherViewController loadProductWithParameters:parameters
                                           completionBlock:^(BOOL result, NSError *error) {
                                               [acIndicator stopAnimating];
                                               if (result) {
                                                   [self.viewController presentViewController:otherViewController animated:NO completion:^{}];
                                                   
                                               } else {
                                                   UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Info" message:@"Please check your internet connection or try again later" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                                                   [alert show];
                                               }
                                           }];
            
            
            
        }
    }
    
}

- (void) videosExit {
    NativeHelper* helper = NativeHelper::getInstance();
    if(helper->videosExitCallback != nullptr){
        helper->videosExitCallback();
    }
}

- (UIWindow*) getWindow {
    return window;
}

- (void)alertView:(UIAlertView *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(buttonIndex == 1)
    {
        NSMutableDictionary *plistDict = [[NSMutableDictionary alloc] initWithContentsOfFile:[self statusPath]];
        NSString *link = [plistDict valueForKey:@"linkOpen"];
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%@",link]]];
    }
}

- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation {
    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                                          openURL:url
                                                sourceApplication:sourceApplication
                                                       annotation:annotation];
}

- (void) firebaseGetSettings {
    NSString* path = [NSString stringWithFormat:@"settings/%@", FIREBASE_APP_NAME];
    FIRDatabaseReference* ref = [[_firebase reference] child:path];
    [ref observeEventType:FIRDataEventTypeValue withBlock:^(FIRDataSnapshot * _Nonnull snapshot) {
        if(![snapshot.value isKindOfClass:[NSNull self]]){
            NSMutableDictionary *newSettings = snapshot.value;
            [_settings removeAllObjects];
            [_settings setValuesForKeysWithDictionary:newSettings];
        }
    }];
}

@end
