//
//  AppItem.cpp
//  NativeHelperTest
//
//  Created by Iwona on 05.01.2019.
//
//

#include "AppItem.h"

AppItem* AppItem::createAppItem(string id, string icon, string link, string name, string category, string bundleId)
{
    auto appItem = new AppItem();
    appItem->id = id;
    appItem->icon = icon;
    appItem->link = link;
    appItem->name = name;
    appItem->category = category;
    appItem->bundleId = bundleId;
    
    //    CCLOG("New item created: id = %s, icon = %s, link = %s, name = %s, category = %s", id.c_str(), icon.c_str(), link.c_str(), name.c_str(), category.c_str());
    
    return appItem;
}
