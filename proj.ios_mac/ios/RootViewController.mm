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

#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
 - (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
 if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
 // Custom initialization
 }
 return self;
 }
 */

/*
 // Implement loadView to create a view hierarchy programmatically, without using a nib.
 - (void)loadView {
 }
 */

/*
 // Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
 - (void)viewDidLoad {
 [super viewDidLoad];
 }
 
 */
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    if (glview)
    {
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();
        
        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


- (void)showImagePicker:(UIImagePickerControllerSourceType)sourceType
{
    if ([UIImagePickerController isSourceTypeAvailable:sourceType])
    {
        if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
        {
            //
            if(sourceType == UIImagePickerControllerSourceTypeCamera){
                //robimy zdjecie iPadem
                self.picker = [[UIImagePickerController alloc] init];
                self.picker.delegate = self;
                self.picker.sourceType = sourceType;
                
                [self presentViewController:self.picker animated:YES completion:nil];
            }
            else {
                //pokazujemy library na ipad
                if([self.popover isPopoverVisible]) {
                    [self.popover dismissPopoverAnimated:YES];
                }
                else {
                    self.picker = [[UIImagePickerController alloc] init];
                    self.picker.delegate = self;
                    self.picker.sourceType = sourceType;
                    
                    self.popover = [[UIPopoverController alloc] initWithContentViewController:self.picker];
                    //                    self.popover.delegate = self;
                    
                    [self.popover presentPopoverFromRect:CGRectMake(self.view.frame.size.width/2.f, 0, self.view.frame.size.width, self.view.frame.size.height) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
                    
                    [self.popover setPopoverContentSize:CGSizeMake(self.picker.view.frame.size.width, 1000) animated:NO];
                    
                }
                
            }
            
        }
        else{
            if(sourceType == UIImagePickerControllerSourceTypeCamera) {
                self.picker = [[UIImagePickerController alloc] init];
                self.picker.delegate = self;
                self.picker.sourceType = UIImagePickerControllerSourceTypeCamera;
                
                AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
                appController.czyOpenCamera = true;
                
                [self presentViewController:self.picker animated:YES completion:nil];
            }
            else {
                self.picker = [[UIImagePickerController alloc] init];
                self.picker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
                self.picker.delegate = self;
                self.picker.allowsEditing = NO;
                self.picker.navigationBar.hidden = YES;
                [self presentViewController:self.picker animated:YES completion:nil];
            }
        }
    }
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    appController.czyOpenCamera = false;
    
    if([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad) {
        if(picker.sourceType == UIImagePickerControllerSourceTypeCamera){
            //robimy zdjecie iPadem
            [self dismissViewControllerAnimated:NO completion:nil];
        }
        else {
            [self.popover dismissPopoverAnimated:YES];
        }
    }
    else {
        [self dismissViewControllerAnimated:NO completion:nil];
    }
    
    UIImage *image = [info objectForKey:@"UIImagePickerControllerOriginalImage"];
    
    CGSize size = [image size];
    float imgWidth = size.width;
    float imgHeight = size.height;
    
    NSLog(@"orginal size = %f, %f", imgWidth, imgHeight);
    
    if(imgWidth > 1024 || imgHeight > 1024){
        float ratio = 1024.0f / (imgWidth > imgHeight ? imgWidth : imgHeight);
        imgWidth = (int) (imgWidth * ratio);
        imgHeight = (int) (imgHeight * ratio);
    }
    
    NSLog(@"scaled size = %f, %f", imgWidth, imgHeight);
    
    UIImage *scaledImage = [RootViewController imageWithImage:image scaledToSize:CGSizeMake(imgWidth, imgHeight)];
    
    NSString* tempDir = NSTemporaryDirectory();
    if (tempDir == nil)
        tempDir = @"/tmp";
    
    NSString* filePath = [tempDir stringByAppendingString:@"photo.jpg"];
    
    std::string *stdstring = new std::string([filePath UTF8String]);
    
    [UIImageJPEGRepresentation(scaledImage, 100) writeToFile:filePath atomically:YES];
    
    NativeHelper* helper = NativeHelper::getInstance();
    if([picker sourceType ] == UIImagePickerControllerSourceTypePhotoLibrary){
        if(helper->imagePickedCallback != nullptr){
            helper->imagePickedCallback(*stdstring);
        }
    }
    else{
        if(helper->photoTakenCallback != nullptr){
            helper->photoTakenCallback(*stdstring);
        }
    }
}

- (void) imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    AppController *appController = (AppController* )[UIApplication sharedApplication].delegate;
    appController.czyOpenCamera = false;
    
    if([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad) {
        if(picker.sourceType == UIImagePickerControllerSourceTypeCamera){
            //robimy zdjecie iPadem
            [self dismissViewControllerAnimated:NO completion:nil];
        }
        else {
            [self.popover dismissPopoverAnimated:YES];
        }
    }
    else {
        [self dismissViewControllerAnimated:NO completion:nil];
    }
}

+ (UIImage *)imageWithImage:(UIImage *)image scaledToSize:(CGSize)newSize {
    //UIGraphicsBeginImageContext(newSize);
    // In next line, pass 0.0 to use the current device's pixel scaling factor (and thus account for Retina resolution).
    // Pass 1.0 to force exact pixel size.
    UIGraphicsBeginImageContextWithOptions(newSize, NO, 0.0);
    [image drawInRect:CGRectMake(0, 0, newSize.width, newSize.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}

- (void) sendEmail:(NSString*) subject :(NSString*) body :(NSString*) attachmentPath {
    
    if ([MFMailComposeViewController canSendMail])
    {
        MFMailComposeViewController *mail = [[MFMailComposeViewController alloc] init];
        mail.mailComposeDelegate = self;
        [mail setSubject:subject];
        [mail setMessageBody:body isHTML:NO];
        [mail setToRecipients:@[@"kontakt@proliberis.org"]];
        
        if(![attachmentPath isEqualToString:@""]){
            UIImage *img = [[[UIImage alloc] initWithContentsOfFile:attachmentPath] autorelease];
            NSData *data = UIImagePNGRepresentation(img);
            [mail addAttachmentData:data mimeType:@"image/png" fileName:@"screenshot.png"];
        }
        
        [self presentViewController:mail animated:YES completion:NULL];
    }
    else
    {
        NSLog(@"This device cannot send email");
        
        UIAlertView * alert =[[UIAlertView alloc ] initWithTitle:@"Warning" message:@"You don't have an email account on this device." delegate:nil cancelButtonTitle:(nil) otherButtonTitles:nil];
        [alert addButtonWithTitle:@"Ok"];
        [alert show];
    }
}

- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error
{
    switch (result) {
        case MFMailComposeResultSent:
            NSLog(@"You sent the email.");
            break;
        case MFMailComposeResultSaved:
            NSLog(@"You saved a draft of this email");
            break;
        case MFMailComposeResultCancelled:
            NSLog(@"You cancelled sending this email.");
            break;
        case MFMailComposeResultFailed:
            NSLog(@"Mail failed:  An error occurred when trying to compose this email");
            break;
        default:
            NSLog(@"An error occurred when trying to compose this email");
            break;
    }
    
    [self dismissViewControllerAnimated:YES completion:NULL];
}

//delegate - SKStoreProductViewControllerDelegate
- (void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController {
    //    AppDelegate *appDel = (AppDelegate*)[UIApplication sharedApplication].delegate;
    //    appDel.czyOpeniTunes = NO;
    
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
