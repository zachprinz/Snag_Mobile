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
#import "IOSNDKHelper.h"
#import <Parse/Parse.h>

static BOOL isConnected = false;

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
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
        
        /* Let the helper know that this class wants to receive data from your game */
        [IOSNDKHelper setNDKReceiver:self];
    }
    
    return self;
}

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


- (void) setConnected
{
    isConnected = YES;
}

- (void)setUnConnected
{
    isConnected = NO;
}
/* Your method must have this NSObject* parameter */
- (void)checkForUser:(NSObject *)parametersObject
{
    NSLog(@"Native has recieved a message from Snag!");
    PFUser *currentUser = [PFUser currentUser];
    if (currentUser){
        PFQuery *query = [PFUser query];
        [query whereKey:@"username" equalTo:currentUser.username];
        if(!isConnected)
            [query fromLocalDatastore];
        NSArray *user = [query findObjects];
        if ([user count] > 0) {
            NSString *success = @"true";
            NSString *responce = @"true";
            NSString *username = [PFUser currentUser].username;
            NSDictionary *params = [[NSDictionary alloc] initWithObjectsAndKeys:success, @"success", responce, @"responce", username, @"username", nil];
            [IOSNDKHelper sendMessage:@"userCheckCallback" withParameters:params];
        } else {
            NSDictionary *notFound = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
            [IOSNDKHelper sendMessage:@"userCheckCallback" withParameters:notFound];
        }
    } else {
        NSDictionary *notFound = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
        [IOSNDKHelper sendMessage:@"userCheckCallback" withParameters:notFound];
    }
}

- (void)register:(NSObject *)parametersObject
{
    NSDictionary *parameters = (NSDictionary *)parametersObject;
    if(parameters != nil){
        NSString* username = (NSString *)[parameters objectForKey:@"username"];
        NSString* password = (NSString *)[parameters objectForKey:@"password"];

        PFUser *user = [PFUser user];
        user.username = username;
        user.password = password;
        //user.email = @"";
        [user signUpInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
            if (!error) {
                logIn:(parametersObject);
            } else {
                NSString *errorString = [error userInfo][@"error"];
                NSDictionary *params = [[NSDictionary alloc] initWithObjectsAndKeys:errorString, @"responce", nil];
                [IOSNDKHelper sendMessage:@"registerCallback" withParameters:params];
            }
        }];
    }
}
//Legs you log in even if you give bad information
- (void)logIn:(NSObject *)parametersObject
{
    NSDictionary *parameters = (NSDictionary *)parametersObject;
    if(parameters != nil){
        NSString* username = (NSString *)[parameters objectForKey:@"username"];
        NSString* password = (NSString *)[parameters objectForKey:@"password"];
        [PFUser logInWithUsernameInBackground:username password:password
            block:^(PFUser *user, NSError *error) {
                if (user) {
                    NSString *success = @"true";
                    NSString *responce = @"none";
                    NSString *username = user.username;
                    NSDictionary *params = [[NSDictionary alloc] initWithObjectsAndKeys:success, @"success", responce, @"responce", username, @"username", nil];
                    [IOSNDKHelper sendMessage:@"logInCallback" withParameters:params];
                } else {
                    NSString *responce = [error userInfo][@"error"];
                    NSString *success = @"false";
                    NSDictionary *params = [[NSDictionary alloc] initWithObjectsAndKeys:success, @"success", responce, @"responce", nil];
                    [IOSNDKHelper sendMessage:@"logInCallback" withParameters:params];                }
            }];
    }
}

- (void)newLevel:(NSObject *)parametersObject
{
    NSDictionary *parameters = (NSDictionary *)parametersObject;
    if(parameters != nil){
        NSLog(@"\nNative Creating a new Level.\n");
        PFObject *level = [PFObject objectWithClassName:@"Level"];
        level[@"name"] = [parameters objectForKey:@"name"];
        NSString* author = [PFUser currentUser].username;
        level[@"author"] = author;
        level[@"entcount"] = @"0";
        level[@"status"] = @"Private";
        level[@"favorites"] = @0;
        level[@"data"] = [parameters objectForKey:@"data"];
        if(isConnected){
            [level saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
                if (succeeded) {
                    level[@"id"] = [level objectId];
                    [level saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
                        [level pinInBackground];
                        NSLog(@"\nFinished Creating a new Level.\n");
                        NSString *responce = @"responce";
                        NSDictionary *params = [[NSDictionary alloc] initWithObjectsAndKeys:responce, @"responce", nil];
                        //[IOSNDKHelper sendMessage:@"newLevelResponce" withParameters:params];
                    }];
                } else {
                    NSLog(@"%@", error);
                }
            }];
        } else {
            [level saveEventually:^(BOOL succeeded, NSError *error) {
                if (succeeded) {
                    level[@"id"] = [level objectId];
                    [level saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
                        NSLog(@"\nFinished Creating a new Level.\n");
                        NSString *responce = @"responce";
                        NSDictionary *params = [[NSDictionary alloc] initWithObjectsAndKeys:responce, @"responce", nil];
                        //[IOSNDKHelper sendMessage:@"newLevelResponce" withParameters:params];
                    }];
                } else {
                    NSLog(@"%@", error);
                }
            }];
        }
    }
}

- (void)saveLevel:(NSObject *)parametersObject
{
    NSDictionary *parameters = (NSDictionary *)parametersObject;
    if(parameters != nil){
        NSString* levelID = (NSString *)[parameters objectForKey:@"id"];
        PFQuery *query = [PFQuery queryWithClassName:@"Level"];
        if(!isConnected)
            [query fromLocalDatastore];
        [query getObjectInBackgroundWithId:levelID block:^(PFObject *level, NSError *error) {
            level[@"entcount"] = [parameters objectForKey:@"entcount"];
            level[@"name"] = [parameters objectForKey:@"name"];
            level[@"status"] = [parameters objectForKey:@"status"];
            level[@"data"] = [parameters objectForKey:@"data"];
            if(isConnected)
                [level saveInBackground];
            else
                [level saveEventually];
        }];
    }
}
- (void)deleteLevel:(NSObject *)parametersObject
{
    NSDictionary *parameters = (NSDictionary *)parametersObject;
    if(parameters != nil){
        NSString* levelID = (NSString *)[parameters objectForKey:@"id"];
        PFQuery *query = [PFQuery queryWithClassName:@"Level"];
        [query getObjectInBackgroundWithId:levelID block:^(PFObject *level, NSError *error) {
            if(level != nil){
                [level deleteInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
                        int refresh = [[parameters objectForKey:@"refresh"] intValue];
                        int refreshTest = 1;
                        if(refresh == refreshTest){
                            NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
                            [IOSNDKHelper sendMessage:@"levelDeletedCallback" withParameters:found];
                        } else {
                            NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
                            [IOSNDKHelper sendMessage:@"finishQuit" withParameters:found];
                        }
                }];
            } else {
                int refresh = [[parameters objectForKey:@"refresh"] intValue];
                int refreshTest = 1;
                if(refresh == refreshTest){
                    NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
                    [IOSNDKHelper sendMessage:@"levelDeletedCallback" withParameters:found];
                } else {
                    NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
                    [IOSNDKHelper sendMessage:@"finishQuit" withParameters:found];
                }
            }
        }];
    }
}
- (void)favorite:(NSObject *)parametersObject
{
    NSDictionary *parameters = (NSDictionary *)parametersObject;
    if(parameters != nil){
        NSString* levelID = (NSString *)[parameters objectForKey:@"id"];
        PFQuery *query = [PFQuery queryWithClassName:@"Level"];
        [query getObjectInBackgroundWithId:levelID block:^(PFObject *level, NSError *error) {
            if(level){
                NSNumber *number = [NSNumber numberWithInt:[level[@"favorites"] intValue]];
                int value = [number intValue];
                number = [NSNumber numberWithInt:value + 1];
                level[@"favorites"] = number;
                [level saveInBackground];
                PFUser *user = [PFUser currentUser];
                PFRelation *relation = [user relationForKey:@"favorites"];
                NSLog(@"About to add level as favorited.");
                //NSString* levelID = (NSString *)[parameters objectForKey:@"id"];
                [relation addObject:level];
                NSLog(@"About to save user.");
                [user saveInBackground];
                NSLog(@"About to call favorited Callback Responce");
                NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
                [IOSNDKHelper sendMessage:@"favCallback" withParameters:found];
            }
        }];
    }
}
- (void)fetchCustomLevels:(NSObject *)parametersObject
{
    NSLog(@"\nLoading Custom Levels.\n");
    if(isConnected){
        PFQuery *customQuery = [PFQuery queryWithClassName:@"Level"];
        [customQuery whereKey:@"author" equalTo:[PFUser currentUser].username];
        [customQuery findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
            if (!error) {
                PFUser *user = [PFUser currentUser];
                for(int x = 0; x < objects.count; x++){
                    [[objects objectAtIndex:x] pinInBackground];
                    PFObject* level = [objects objectAtIndex:x];
                    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
                    NSArray* allKeys = [level allKeys];
                    for(NSString* key in allKeys){
                        [dict setObject:[level objectForKey:key] forKey:key];
                    }
                    
                }
                //Search Offline//
                //////////////////
                PFQuery *customQuery2 = [PFQuery queryWithClassName:@"Level"];
                [customQuery2 fromLocalDatastore];
                [customQuery2 whereKey:@"author" equalTo:[PFUser currentUser].username];
                [customQuery2 findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
                    if (!error) {
                        PFUser *user = [PFUser currentUser];
                        for(int x = 0; x < objects.count; x++){
                            [[objects objectAtIndex:x] pinInBackground];
                            PFObject* level = [objects objectAtIndex:x];
                            NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
                            NSArray* allKeys = [level allKeys];
                            for(NSString* key in allKeys){
                                [dict setObject:[level objectForKey:key] forKey:key];
                            }
                            dict[@"favorited"] = @"false";
                            [IOSNDKHelper sendMessage:@"fetchCustomCallback" withParameters:dict];
                        }
                        NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
                        [IOSNDKHelper sendMessage:@"doneFetching" withParameters:found];
                    }
                }];
            }
        }];
    } else {
        //Search Offline//
        //////////////////
        PFQuery *customQuery2 = [PFQuery queryWithClassName:@"Level"];
        [customQuery2 fromLocalDatastore];
        [customQuery2 whereKey:@"author" equalTo:[PFUser currentUser].username];
        [customQuery2 findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
            if (!error) {
                PFUser *user = [PFUser currentUser];
                for(int x = 0; x < objects.count; x++){
                    [[objects objectAtIndex:x] pinInBackground];
                    PFObject* level = [objects objectAtIndex:x];
                    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
                    NSArray* allKeys = [level allKeys];
                    for(NSString* key in allKeys){
                        [dict setObject:[level objectForKey:key] forKey:key];
                    }
                    dict[@"favorited"] = @"false";
                    [IOSNDKHelper sendMessage:@"fetchCustomCallback" withParameters:dict];
                }
                NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
                [IOSNDKHelper sendMessage:@"doneFetching" withParameters:found];
            }
        }];
    }
}
- (void)fetchRisingLevels:(NSObject *)parametersObject
{
    PFQuery *query = [PFQuery queryWithClassName:@"Level"];
    if(!isConnected)
        [query fromLocalDatastore];
    [query whereKey:@"status" equalTo:@"Public"];
    [query addDescendingOrder:@"favorites"];
    query.limit = 12;
    [query findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
        if (!error) {
            PFUser *user = [PFUser currentUser];
            for(int x = 0; x < objects.count; x++){
                [[objects objectAtIndex:x] pinInBackground];
                PFObject* level = [objects objectAtIndex:x];
                NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
                NSArray* allKeys = [level allKeys];
                for(NSString* key in allKeys){
                    [dict setObject:[level objectForKey:key] forKey:key];
            }
                dict[@"favorited"] = @"false";
                /*for(PFObject* levelID in favs){
                    NSString* levelID1 = (NSString *)[level objectForKey:@"id"];
                    NSString* levelID2 = (NSString *)[levelID objectForKey:@"id"];
                    if([levelID1 isEqualToString:levelID2]){
                        NSLog(@"Found a favorited Level");
                        dict[@"favorited"] = @"true";
                        break;
                    }
                }*/
                [IOSNDKHelper sendMessage:@"fetchRisingCallback" withParameters:dict];
            }
            NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
            [IOSNDKHelper sendMessage:@"doneFetching" withParameters:found];
        }
    }];
}
- (void)fetchSocialLevels:(NSObject *)parametersObject
{

}
- (void)fetchFavoritedLevels:(NSObject *)parametersObject
{
    if(isConnected){
        PFUser *user = [PFUser currentUser];
        PFRelation *relation = [user relationForKey:@"favorites"];
        [[relation query] findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
            if (error) {
                // There was an error
            } else {
                for(int x = 0; x < objects.count; x++){
                    [[objects objectAtIndex:x] pinInBackground];
                    PFObject* level = [objects objectAtIndex:x];
                    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
                    NSArray* allKeys = [level allKeys];
                    for(NSString* key in allKeys){
                        [dict setObject:[level objectForKey:key] forKey:key];
                    }
                    dict[@"favorited"] = @"true";
                    [IOSNDKHelper sendMessage:@"fetchFavoritedCallback" withParameters:dict];
                }
                NSDictionary *found = [[NSDictionary alloc] initWithObjectsAndKeys:@"false", @"responce", nil];
                [IOSNDKHelper sendMessage:@"doneFetching" withParameters:found];
            }
        }];
    }
}

@end
