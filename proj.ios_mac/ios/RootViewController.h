/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

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

#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
#include "NativeHelper.h"
#import "AppController.h"

@interface RootViewController : UIViewController <UIImagePickerControllerDelegate, UINavigationControllerDelegate, MFMailComposeViewControllerDelegate, SKStoreProductViewControllerDelegate> {
    
}

@property(nonatomic, retain) UIImagePickerController *picker;
@property(nonatomic, retain) UIPopoverController *popover;

- (BOOL) prefersStatusBarHidden;

- (void) showImagePicker:(UIImagePickerControllerSourceType)sourceType;
+ (UIImage *)imageWithImage:(UIImage *)image scaledToSize:(CGSize)newSize;
- (void) sendEmail:(NSString*) subject :(NSString*) body :(NSString*) attachmentPath;

@end
