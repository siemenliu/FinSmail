//
//  XMTabBarViewController.m
//  FinSmail
//
//  Created by 刘晓明 on 2017/9/13.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import "XMTabBarViewController.h"
#import "XMEntryViewController.h"
#import "XMSubAccountViewController.h"

@interface XMTabBarViewController ()

@end

@implementation XMTabBarViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    /// 初始化首页
    NSMutableArray<UIViewController *> *controllers = [NSMutableArray array];
    
    {
        XMEntryViewController *entryViewController = [[XMEntryViewController alloc] initWithStyle:UITableViewStylePlain];
        
        UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:entryViewController];
        
        [controllers addObject:navigationController];
    }
    
    {
        XMSubAccountViewController *subAccountViewController = [[XMSubAccountViewController alloc] initWithNibName:nil bundle:nil];
        
        UINavigationController *navi = [[UINavigationController alloc] initWithRootViewController:subAccountViewController];
        
        [controllers addObject:navi];
    }
    
    self.viewControllers = controllers;
}

@end
