//
//  XMSubAccountViewController.m
//  FinSmail
//
//  Created by 刘晓明 on 2017/9/13.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import "XMSubAccountViewController.h"

@interface XMSubAccountViewController ()

@end

@implementation XMSubAccountViewController

- (instancetype)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        
        self.title = @"子账号";
        self.tabBarItem = [[UITabBarItem alloc] initWithTitle:self.title image:[UIImage imageNamed:@"subaccount"] tag:1];
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    self.view.backgroundColor = [UIColor purpleColor];
    
    
}

@end
