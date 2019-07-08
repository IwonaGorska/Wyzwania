//
//  AppItem.h
//  NativeHelperTest
//  Wyzwania
//
//  Created by Iwona Gorska on 03/10/2018.
//

#ifndef __NativeHelperTest__AppItem__
#define __NativeHelperTest__AppItem__

#include <string>
using namespace std;

class AppItem
{
public:
    static AppItem* createAppItem(string id, string icon, string link, string name, string category, string bundleId);
    
    string id;
    string icon;
    string link;
    string name;
    string category;
    string bundleId;
};

#endif /* defined(__NativeHelperTest__AppItem__) */
