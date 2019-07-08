package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.CursorLoader;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.content.ServiceConnection;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.provider.MediaStore;
import android.provider.Settings.Secure;
import android.text.InputType;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

import com.android.vending.billing.IInAppBillingService;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class AppActivity extends Cocos2dxActivity {

    private static final String TAG = "AppActivity";
    public final static String FIREBASE_APP_NAME = "hide_and_seek_free_android";
	
//	private static final String ADMOB_BANNER = "UNUSED";
	private static final String ADMOB_INTERSTITIAL = "ca-app-pub-2575683230512628/5833084223";
	
//	private static final String ANALYTICS_ID = "UA-57578456-28";
	private static final String ANALYTICS_PREFIX = "123 Kids Fun Hide and Seek Free Android - ";
	
//	private static final String CHARTBOOST_APP_ID = "5892ff2104b01672fc3625fa";
//	private static final String CHARTBOOST_APP_SIGNATURE = "6f257dc1547f0390cebbf568a61573db0e4d9bc2";
	
	private static final int SELECT_PHOTO = 100;
	private static final int TAKE_PHOTO = 101;
	
	private InterstitialAd admobInterstitial;
	private static AppActivity _appActivity;
	private AdView adView;
//	private Tracker tracker;
//	private GoogleAnalytics analytics;
	private IInAppBillingService mService;
	private ServiceConnection mServiceConn;
	private SecureRandom secRandom;
	private String token = "";
	private FirebaseAnalytics mFirebaseAnalytics;
    private FirebaseDatabase firebase;
    private Map<String, Object> settings = new HashMap<>();
	
	public static native void purchaseCallback(String[] products);
	public static native void purchaseCancelledCallback();
	public static native void imagePickedCallback(String filename);
	public static native void photoTakenCallback(String filename);
	public static native void productPriceCallback(String product, String price);
	public static native void videosCallback();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		_appActivity = this;
		
		System.out.println("onCreate");
		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		// * * * ADMOB BANNER * * *
		
//		RelativeLayout layout = new RelativeLayout(this);
//		RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
//		RelativeLayout.LayoutParams adParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
//		adParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
//
//		adView = new AdView(this);
//		adView.setAdSize(AdSize.SMART_BANNER);
//		adView.setAdUnitId(ADMOB_BANNER);
//		adView.loadAd(newAdRequest());
//		adView.setBackgroundColor(0);
//
//		layout.addView(adView, adParams);
//		addContentView(layout, layoutParams);
		
		// * * * ADMOB INTERSTITIAL * * *
		
		admobInterstitial = new InterstitialAd(this);
		admobInterstitial.setAdUnitId(ADMOB_INTERSTITIAL);
		admobInterstitial.setAdListener(new AdListener() {
			@Override
			public void onAdClosed() {
				admobInterstitial.loadAd(newAdRequest());
			}

 	    });
		admobInterstitial.loadAd(newAdRequest());
		
		// * * * GOOGLE ANALYTICS * * *
		
//        analytics = GoogleAnalytics.getInstance(this);
//        analytics.getLogger().setLogLevel(LogLevel.VERBOSE);
//        tracker = analytics.newTracker(ANALYTICS_ID);

        // * * * FIREBASE * * *

        firebase = FirebaseDatabase.getInstance();
        firebaseGetSettings();

		// * * * FIREBASE ANALYTICS * * *

		mFirebaseAnalytics = FirebaseAnalytics.getInstance(this);
        
        // * * * IN-APP * * *
        
        secRandom = new SecureRandom();
        mServiceConn = new ServiceConnection() {

			@Override
			public void onServiceConnected(ComponentName name, IBinder service) {
				mService = IInAppBillingService.Stub.asInterface(service);
			}

			@Override
			public void onServiceDisconnected(ComponentName name) {
				mService = null;
			}
			
        };
        
        Intent intent = new Intent("com.android.vending.billing.InAppBillingService.BIND");
	    intent.setPackage("com.android.vending");	
	    getContext().bindService(intent, mServiceConn, Context.BIND_AUTO_CREATE);
        
        cleanUp(getTempPath());
	}
	
	private String getRealPathFromURI(Uri contentUri) {
	    String[] proj = { MediaStore.Images.Media.DATA };
	    CursorLoader loader = new CursorLoader(_appActivity, contentUri, proj, null, null, null);
	    Cursor cursor = loader.loadInBackground();
	    int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
	    cursor.moveToFirst();
	    String result = cursor.getString(column_index);
	    cursor.close();
	    return result;
	}
	
	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(requestCode == SELECT_PHOTO){
	        if(resultCode == RESULT_OK){
	            Uri selectedImage = data.getData();
	            
	            String path = getRealPathFromURI(selectedImage);	            
	            Bitmap bitmap = getBitmap(path);
	            
	            final String newPath = getTempPath() + "photo.jpg";
	            saveScaledBitmap(bitmap, newPath);
	            
	            Cocos2dxHelper.runOnGLThread(new Runnable() {
					
					@Override
					public void run() {
						imagePickedCallback(newPath);
					}
				});
	        }
	   }
	    if(requestCode == TAKE_PHOTO){
	        if (resultCode == Activity.RESULT_OK) {
	        	final String newPath = getTempPath() + "photo.jpg";
	        	Bitmap bitmap = getBitmap(newPath);
	        	saveScaledBitmap(bitmap, newPath);
	        	
	            Cocos2dxHelper.runOnGLThread(new Runnable() {
					
					@Override
					public void run() {
						photoTakenCallback(newPath);
					}
				});
	        }
	    }
       if (requestCode == 1001 && data != null) {           
          int responseCode = data.getIntExtra("RESPONSE_CODE", 0);
          String purchaseData = data.getStringExtra("INAPP_PURCHASE_DATA");
            
          if (resultCode == RESULT_OK && (responseCode == 0 || responseCode == 7)) {
        	  
             try {
                JSONObject jo = new JSONObject(purchaseData);
                final String sku = jo.getString("productId");
                String token = jo.getString("developerPayload");
                if(token.equals(this.token)){
                	Cocos2dxHelper.runOnGLThread(new Runnable() {
    					
    					@Override
    					public void run() {
    	                	purchaseCallback(new String[]{sku});
    					}
    				});
                }
                else{
                	Cocos2dxHelper.runOnGLThread(new Runnable() {
    					
    					@Override
    					public void run() {
    	                	purchaseCancelledCallback();
    					}
    				});
                	Log.e("IN-APP", "Wrong token!");
                }
              }
              catch (Exception e) {
                 e.printStackTrace();
                 Cocos2dxHelper.runOnGLThread(new Runnable() {
 					
 					@Override
 					public void run() {
 		                 purchaseCancelledCallback();
 					}
 				});
              }
          }
          else{
              Cocos2dxHelper.runOnGLThread(new Runnable() {
					
				@Override
				public void run() {
		              purchaseCancelledCallback();
				}
			});
          }
       }
    }
	
	public static void showBanner(int position) {
//		final int _position = position;
//		_appActivity.runOnUiThread(new Runnable() {
//
//			@Override
//			public void run() {
//				if(_appActivity != null){
//					_appActivity.adView.setVisibility(View.VISIBLE);
//					RelativeLayout.LayoutParams adParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
//					if(_position == 0){
//						adParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
//					}
//					else{
//						adParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
//					}
//					_appActivity.adView.setLayoutParams(adParams);
//				}
//			}
//		});
	}

	public static void hideBanner() {
//		_appActivity.runOnUiThread(new Runnable() {
//
//			@Override
//			public void run() {
//				if(_appActivity != null){
//					_appActivity.adView.setVisibility(View.INVISIBLE);
//				}
//			}
//		});
	}

	public static void showInterstitial(final String adSdk){
		Log.d("ADMOB INTERSTITIAL", "showInterstitial: " + adSdk);
		_appActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if(adSdk.equalsIgnoreCase("admob")){
					if(_appActivity.admobInterstitial != null){
						if(_appActivity.admobInterstitial.isLoaded()){
							_appActivity.admobInterstitial.show();
						}
					}
				}
				else if(adSdk.equalsIgnoreCase("chartboost")){
				}
				else if(adSdk.equalsIgnoreCase("revmob")){
				}
				else{
					Log.w("INTERSTITIAL", "unknown ad to show: " + adSdk);
				}
			}
		});
	}
	
	public static void reportScreenChange(final String name) {
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if(_appActivity != null){
					String screenName = ANALYTICS_PREFIX + name;
//					if (_appActivity.tracker != null){
//						_appActivity.tracker.setScreenName(screenName);
//						Log.d("ANALYTICS", screenName);
//						_appActivity.tracker.send(new HitBuilders.AppViewBuilder().build());
//						_appActivity.analytics.dispatchLocalHits();
//					}
					Bundle params = new Bundle(); params.putString(FirebaseAnalytics.Param.ITEM_CATEGORY, "screen");
					params.putString(FirebaseAnalytics.Param.ITEM_NAME, screenName);
					_appActivity.mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.VIEW_ITEM, params);

					_appActivity.mFirebaseAnalytics.setCurrentScreen(_appActivity, screenName, null);
				}
			}
		});
	}
	
	public static String getDeviceId(){		
		//https://www.random.org/strings/?num=2&len=10&digits=on&upperalpha=on&loweralpha=on&unique=on&format=html&rnd=new
		String salt0 = "CJpZ8mpbwi";
		String salt1 = "ihhkeL3wxI";
		String secureString = Secure.getString(_appActivity.getContentResolver(), Secure.ANDROID_ID);
		String stringToEncrypt = salt0 + secureString + salt1;
		MessageDigest messageDigest;
		try {
			messageDigest = MessageDigest.getInstance("SHA-256");
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
			return stringToEncrypt;
		}
		messageDigest.update(stringToEncrypt.getBytes());
		String encryptedString = new String(toHex(messageDigest.digest()));
		
		return encryptedString;
	}
	
	private static String toHex(byte[] array)
    {
        BigInteger bi = new BigInteger(1, array);
        String hex = bi.toString(16);
        int paddingLength = (array.length * 2) - hex.length();
        if(paddingLength > 0)
            return String.format("%0" + paddingLength + "d", 0) + hex;
        else
            return hex;
    }
	
	public static String getAppVersion(){
        try {
			return _appActivity.getPackageManager().getPackageInfo(_appActivity.getPackageName(), 0).versionName;
		} 
        catch (NameNotFoundException e) {
        	return "";
        }
	}
	
	public static void purchaseProduct(final String product){
		
		if(_appActivity == null){
			purchaseCancelledCallback();
			return;
		}
		
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
		
				if(_appActivity.mService == null){
					purchaseCancelledCallback();
					return;
				}
				
				_appActivity.token = new BigInteger(130, _appActivity.secRandom).toString(32);
				
				try {
					Bundle buyIntentBundle = _appActivity.mService.getBuyIntent(3, _appActivity.getPackageName(),
							product, "inapp", _appActivity.token);
					int RESPONSE_CODE = buyIntentBundle.getInt("RESPONSE_CODE");
					Log.d("IN-APP", "RESPONSE_CODE: " + RESPONSE_CODE + " for purchase: " + product);
					PendingIntent pendingIntent = buyIntentBundle.getParcelable("BUY_INTENT");
					try {
						if(pendingIntent == null){
							Cocos2dxHelper.runOnGLThread(new Runnable() {
								
								@Override
								public void run() {
									purchaseCancelledCallback();
								}
							});
							return;
						}
						
						_appActivity.startIntentSenderForResult(pendingIntent.getIntentSender(),
								   1001, new Intent(), Integer.valueOf(0), Integer.valueOf(0),
								   Integer.valueOf(0));
						
					} catch (SendIntentException e) {
						e.printStackTrace();
						Cocos2dxHelper.runOnGLThread(new Runnable() {
							
							@Override
							public void run() {
								purchaseCancelledCallback();
							}
						});
					}
				} catch (Exception e) {
					e.printStackTrace();
					Cocos2dxHelper.runOnGLThread(new Runnable() {
						
						@Override
						public void run() {
							purchaseCancelledCallback();
						}
					});
				}
			}
		});
	}
	
	public static void restorePurchases(){
		if(_appActivity == null){
			purchaseCancelledCallback();
			return;
		}
		
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
		
				try {
					Bundle ownedItems = _appActivity.mService.getPurchases(3, _appActivity.getPackageName(), "inapp", null);
					int response = ownedItems.getInt("RESPONSE_CODE");
					if (response == 0) {
					   ArrayList<String> ownedSkus = 
					      ownedItems.getStringArrayList("INAPP_PURCHASE_ITEM_LIST");
					   
					   final String[] skus = ownedSkus.toArray(new String[ownedSkus.size()]);
					   
					   Cocos2dxHelper.runOnGLThread(new Runnable() {
							
							@Override
							public void run() {
								purchaseCallback(skus);
							}
						});
					}
				} catch (Exception e) {
					Cocos2dxHelper.runOnGLThread(new Runnable() {
						
						@Override
						public void run() {
							purchaseCancelledCallback();
						}
					});
					e.printStackTrace();
				}
			}
		});
	}
	
	public static void getProductPrice(final String product){
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				try {
				    ArrayList<String> skuList = new ArrayList<String>();
				    skuList.add(product);
				    
				    Bundle querySkus = new Bundle();
				    querySkus.putStringArrayList("ITEM_ID_LIST", skuList);
			
				    Bundle skuDetails = _appActivity.mService.getSkuDetails(3, _appActivity.getPackageName(), "inapp", querySkus);
					
				    int response = skuDetails.getInt("RESPONSE_CODE");
				    if (response == 0) {
				       ArrayList<String> responseList = skuDetails.getStringArrayList("DETAILS_LIST");
			
				       for (String thisResponse : responseList) {
				          JSONObject object = new JSONObject(thisResponse);
				          String sku = object.getString("productId");
				          final String price = object.getString("price");
			
				          if(sku.contains(product)){
				        	  Cocos2dxHelper.runOnGLThread(new Runnable() {
									
									@Override
									public void run() {
										productPriceCallback(product, price);
									}
				        	  });
				          }
			
				       }
				    }
				} 
				catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	public static void sendEmail(final String subject, final String body, final String attachmentPath){	
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				
				File f = new File(attachmentPath);
				Log.d("MYFILE", f.getAbsolutePath() + ", exists: " + f.exists());
				Intent sendIntent;
		
				sendIntent = new Intent(Intent.ACTION_SEND);
				sendIntent.setType("message/rfc822");
				sendIntent.putExtra(Intent.EXTRA_SUBJECT, subject);
				sendIntent.putExtra(Intent.EXTRA_EMAIL, new String[]{"contact@123kidsfun.com"});
				sendIntent.putExtra(Intent.EXTRA_TEXT, body);
				if(f.exists()){
					sendIntent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(f));					
					sendIntent.setType("image/png");
					Log.d("EMAIL", "attachment " + attachmentPath + " added");
				}
				else{
					Log.d("EMAIL", "file " + attachmentPath + " does not exist!");
				}
				
				Log.d("EMAIL", "subject = " + subject + ", body = " + body);
		
				_appActivity.startActivity(Intent.createChooser(sendIntent, "Send Mail"));				
			}
			
		});
	}
	
	public static void showImagePicker(){
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Intent photoPickerIntent = new Intent(Intent.ACTION_PICK);
				photoPickerIntent.setType("image/*");
				_appActivity.startActivityForResult(photoPickerIntent, SELECT_PHOTO);
			}
			
		});
	}
	
	public static String getTempPath(){
		String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/.rosmedia/";
		File f = new File(path);
		f.mkdirs();
		Log.d("getTempPath", "path = " + path);
		return path;		
	}
	
	public static String getStoragePath(){
		String path = _appActivity.getApplicationInfo().dataDir + "/";
		Log.d("getStoragePath", "path = " + path);
		return path;		
	}
	
	public static void takePhoto(){
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Intent intent = new Intent("android.media.action.IMAGE_CAPTURE");
			    String newPath = getTempPath() + "photo.jpg";
			    intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(newPath)));
			    _appActivity.startActivityForResult(intent, TAKE_PHOTO);
			}
			
		});
	}
	
	public static String getRateLink(){
		String rateLink = "market://details?id=" + _appActivity.getPackageName();
		return rateLink;
	}
	
	public static String getFullLink(){
		String rateLink = "market://details?id=" + _appActivity.getPackageName() + "Full";
		return rateLink;
	}

	public static String getBundleId(){
		return _appActivity.getPackageName();
	}
	
	public static boolean isFull(){
		return false;
	}
	
	public static void goToVideos(){

    }

	public static void openAppUrl(final String url) {
		_appActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Random rnd = new Random();
				int r1 = rnd.nextInt(10) + 1;
				int r2 = rnd.nextInt(10) + 1;
				final int w = r1 + r2;
				String equation = "Please solve the following equation\n" + r1 + " + " + r2 + " = ?";

				AlertDialog.Builder builder = new AlertDialog.Builder(getContext());
				builder.setMessage(equation);
				builder.setTitle("Child protection");

				// Set up the input
				final EditText input = new EditText(getContext());
				input.setInputType(InputType.TYPE_CLASS_NUMBER);
				builder.setView(input);

				builder.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialogInterface, int i) {
						String result = input.getText().toString();
						try {
							int w2 = Integer.parseInt(result);
							if(w == w2) {
								Intent intent = new Intent(Intent.ACTION_VIEW);
								intent.setData(Uri.parse(url));
								_appActivity.startActivity(intent);

								return;
							}
						} catch (Exception e){
							e.printStackTrace();
						}

						new AlertDialog.Builder(getContext())
								.setTitle("Error")
								.setMessage("Answer is incorrect")
								.setPositiveButton("Ok", null)
								.show();
					}
				});

				builder.setNegativeButton("Cancel", null);

				builder.show();
			}
		});
	}
    
    public static String getAppType(){
        return "googleplay";
    }

	private Bitmap getBitmap(String path) {

		Uri uri = Uri.fromFile(new File(path));
		InputStream in = null;
		try {
		    final int IMAGE_MAX_SIZE = 1200000; // 1.2MP		    
		    in = getContentResolver().openInputStream(uri);

		    // Decode image size
		    BitmapFactory.Options o = new BitmapFactory.Options();
		    o.inJustDecodeBounds = true;
		    BitmapFactory.decodeStream(in, null, o);
		    in.close();

		    int scale = 1;
		    while ((o.outWidth * o.outHeight) * (1 / Math.pow(scale, 2)) > 
		          IMAGE_MAX_SIZE) {
		       scale++;
		    }
		    Log.d(this.getClass().getSimpleName(), "scale = " + scale + ", orig-width: " + o.outWidth + ", orig-height: " + o.outHeight);

		    Bitmap b = null;
		    in = getContentResolver().openInputStream(uri);
		    if (scale > 1) {
		        scale--;
		        // scale to max possible inSampleSize that still yields an image
		        // larger than target
		        o = new BitmapFactory.Options();
		        o.inSampleSize = scale;
		        b = BitmapFactory.decodeStream(in, null, o);

		        // resize to desired dimensions
		        int height = b.getHeight();
		        int width = b.getWidth();
		        Log.d(this.getClass().getSimpleName(), "1th scale operation dimenions - width: " + width + ", height: " + height);

		        double y = Math.sqrt(IMAGE_MAX_SIZE
		                / (((double) width) / height));
		        double x = (y / height) * width;

		        Bitmap scaledBitmap = Bitmap.createScaledBitmap(b, (int) x, 
		           (int) y, true);
		        b.recycle();
		        b = scaledBitmap;

		        System.gc();
		    } else {
		        b = BitmapFactory.decodeStream(in);
		    }
		    in.close();

		    Log.d(this.getClass().getSimpleName(), "bitmap size - width: " +b.getWidth() + ", height: " + 
		       b.getHeight());
		    return b;
		} catch (Exception e) {
		    Log.e(this.getClass().getSimpleName(), e.getMessage(),e);
		    return null;
		}
	}
	
	private void saveScaledBitmap(Bitmap bitmap, String filename){
		int imgWidth = bitmap.getWidth();
        int imgHeight = bitmap.getHeight();

        Log.d(this.getClass().getSimpleName(), "orginal size = " + imgWidth + ", " + imgHeight);

        if(imgWidth > 1024 || imgHeight > 1024)
        {
            float ratio = 1024.0f / (imgWidth > imgHeight ? imgWidth : imgHeight);
            imgWidth = (int) (imgWidth * ratio);
            imgHeight = (int) (imgHeight * ratio);
        }

        Log.d(this.getClass().getSimpleName(), "scaled size = " + imgWidth + ", " + imgHeight);
        
        Bitmap scaledBitmap = Bitmap.createScaledBitmap(bitmap, imgWidth, imgHeight, false);
        
        FileOutputStream out = null;
        try {
            out = new FileOutputStream(filename);
            scaledBitmap.compress(Bitmap.CompressFormat.JPEG, 100, out);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (out != null) {
                    out.close();
                    Log.d(this.getClass().getSimpleName(), "Image successfully saved to file: " + filename);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
	}
	
	@Override
	protected void onResume() {
		super.onResume();
//		if (adView != null) {
//			adView.resume();
//		}
	}

	@Override
	protected void onPause() {
//		if (adView != null) {
//			adView.pause();
//		}
		super.onPause();
	}

	@Override
	protected void onDestroy() {
//		adView.destroy();
		if (mServiceConn != null) {
            unbindService(mServiceConn);
		}
		super.onDestroy();
	}
	
	@Override
	public void onStart() {
	    super.onStart();
	}
	        
	@Override
	public void onStop() {
	    super.onStop();
	}
	
	@Override
	public void onBackPressed() {
	    super.onBackPressed();
	}
	
	private AdRequest newAdRequest(){
		AdRequest adRequest = new AdRequest.Builder()
				.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
				.addTestDevice("4383031028E59F004D9E94AC0642B0C9") //LENOVO
				.addTestDevice("6F0691E0EAE05BCD2A4C7E5FBEA3569B") //P7-L10
                .addTestDevice("63E05681D14C9EBC49B9014ADBED3CF5") //Nexus 6P
				.build();		
		
		return adRequest;
	}
	
	private void cleanUp(String directory)
    {
        Log.d(this.getClass().getSimpleName(), "cleanUp: " + directory);

        int fCounter = 0;
        
        File f = new File(directory);  
        if(f != null){
	        File files[] = f.listFiles();
	        
	        if(files != null){
		        for (int i = 0; i < files.length; i++)
		        {
		        	File file = files[i];
		        	if (file.getName().startsWith("image") || (file.getName().startsWith("photo") || file.getName().startsWith("screenshot")) && (file.getName().endsWith(".jpg") || file.getName().endsWith(".png"))) { 
		                file.delete();
		                Log.d(this.getClass().getSimpleName(), "Removing file: " + file.getName());
		                fCounter++;
		            }
		            else
		            {
		            	Log.d(this.getClass().getSimpleName(), "Skip file removal: " + file.getName());
		            }
		        }
		
		        Log.d(this.getClass().getSimpleName(), "Removed " + fCounter + " file(s).");
	        }
        }
    }

    public void firebaseGetSettings(){
        String path = "settings/" + FIREBASE_APP_NAME;
        DatabaseReference ref = _appActivity.firebase.getReference(path);
        ValueEventListener listener = new ValueEventListener() {

            @Override
            public void onDataChange(final DataSnapshot snapshot) {
                _appActivity.runOnUiThread(new Runnable() {

                    @Override
                    public void run() {
                        try {
                            _appActivity.settings.clear();
                            _appActivity.settings.putAll((HashMap<String, Object>) snapshot.getValue());
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.e(TAG, databaseError.getMessage());
            }
        };
        ref.addValueEventListener(listener);
    }

    public static String getSetting(String propertyName){
        Object value = _appActivity.settings.get(propertyName);
        if(value != null) {
            if (value instanceof String) {
                return (String) value;
            }
        }
        return "";
    }

}
