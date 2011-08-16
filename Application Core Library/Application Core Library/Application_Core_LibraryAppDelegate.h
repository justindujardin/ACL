//
//  Application_Core_LibraryAppDelegate.h
//  Application Core Library
//
//  Created by Justin DuJardin on 8/15/11.
//  Copyright 2011 DuJardin Consulting LLC. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface Application_Core_LibraryAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
