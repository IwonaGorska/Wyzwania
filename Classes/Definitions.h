//
//  Definitions.h
//  NativeHelperTest
//
//  Created by Piotr on 05.03.2015.
//
//

#ifndef NativeHelperTest_Definitions_h
#define NativeHelperTest_Definitions_h

#define APP_NAME "Zabawy dla dzieci"
#define OUR_APPS_URL "http://rosmedia.eu/apps_pl/"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

//#define IN_APP_ALL "full"
//#define IN_APP_1 "game1"
//#define IN_APP_2 "game2"
//#define IN_APP_3 "game3"
//#define IN_APP_FULL "full"
//#define APP_PLATFORM "Android"
//#define OUR_APPS_URL_SUFFIX "apps_android.plist"

#define IN_APP_ALL "com.rosapp.AllInOneLite.InApp"
#define IN_APP_1 "com.rosapp.AllInOneLite1.InApp"
#define IN_APP_2 "com.rosapp.AllInOneLite2.InApp"
#define IN_APP_3 "com.rosapp.AllInOneLite3.InApp"
#define IN_APP_FULL "full"//
#define APP_PLATFORM "Android"//
#define OUR_APPS_URL_SUFFIX "apps_android.plist"//

/*
 self.inAppAllGame = @"com.rosapp.AllInOneLite.InApp";         // -inApp Calej Aplikacji
 NSArray *tablicaInAppName = [NSArray arrayWithObjects:
 self.inAppAllGame,// -inApp odblokowujaca calosc
 //      Button "allInApp" i "restoreAllButton"
 @"com.rosapp.AllInOneLite1.InApp",
 @"com.rosapp.AllInOneLite2.InApp",    // -inApp Button "opcjaInApp1" i "opcjaRestore1"
 @"com.rosapp.AllInOneLite3.InApp",     // -inApp Button "opcjaInApp2" i "opcjaRestore2"
 nil];
 */

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define IN_APP_ALL "com.rosapp.AllInOneLite.InApp"
#define IN_APP_1 "com.rosapp.AllInOneLite1.InApp"
#define IN_APP_2 "com.rosapp.AllInOneLite2.InApp"
#define IN_APP_3 "com.rosapp.AllInOneLite3.InApp"
//#define IN_APP_ALL "com.rosapp.Numbers1"
//#define IN_APP_1 "com.rosapp.Numbers1_1"
//#define IN_APP_2 "com.rosapp.Numbers1_2"
//#define IN_APP_3 "com.rosapp.Numbers1_3"
//#define IN_APP_4 "com.rosapp.Numbers1_4"
#define APP_PLATFORM "iOS"
#define OUR_APPS_URL_SUFFIX "apps_ios.plist"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

#define IN_APP_ALL "numbersfull"
#define IN_APP_1 "numbersgame1"
#define IN_APP_2 "numbersgame2"
#define IN_APP_3 "numbersgame3"
#define APP_PLATFORM "Windows Phone"
#define OUR_APPS_URL_SUFFIX "apps_windows_phone.plist"

#else

#define IN_APP_ALL ""
#define IN_APP_1 ""
#define IN_APP_2 ""
#define IN_APP_3 ""
#define APP_PLATFORM ""
#define OUR_APPS_URL_SUFFIX ""

#endif

#endif
