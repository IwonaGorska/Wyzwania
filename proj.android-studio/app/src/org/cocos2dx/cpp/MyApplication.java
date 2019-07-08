package org.cocos2dx.cpp;

import com.facebook.FacebookSdk;
import com.facebook.appevents.AppEventsLogger;
import com.google.firebase.database.FirebaseDatabase;
import com.onesignal.OneSignal;

import android.app.Application;
import android.content.Context;
import android.support.multidex.MultiDex;
import android.support.multidex.MultiDexApplication;

public class MyApplication extends MultiDexApplication {

    @Override
    protected void attachBaseContext(Context context) {
        super.attachBaseContext(context);
        MultiDex.install(this);
    }
	
	@Override
    public void onCreate() {
        super.onCreate();
        OneSignal.startInit(this).init();
//        OneSignal.setLogLevel(OneSignal.LOG_LEVEL.VERBOSE, OneSignal.LOG_LEVEL.VERBOSE);
//        FacebookSdk.sdkInitialize(getApplicationContext());
//        AppEventsLogger.activateApp(this);
        FirebaseDatabase.getInstance().setPersistenceEnabled(true);
    }
	
}