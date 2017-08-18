//
//  XMLoginViewController.m
//  FinSmail
//
//  Created by 刘晓明 on 2017/8/18.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import "XMLoginViewController.h"
#import <Masonry/Masonry.h>
#import "XMAdminViewController.h"
#import <SVProgressHUD/SVProgressHUD.h>

@import Wilddog;
@import WilddogAuth;

@interface XMLoginViewController ()
@property (nonatomic, strong) UITextField *tfUserName;
@property (nonatomic, strong) UITextField *tfPassword;
@end

@implementation XMLoginViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    self.title = @"登录";
    
    [self setupView];
}

- (void)setupView {
    UITextField *tfUserName = [[UITextField alloc] init];
    self.tfUserName = tfUserName;
    [self.view addSubview:tfUserName];
    
    UITextField *tfPassword = [[UITextField alloc] init];
    self.tfPassword = tfPassword;
    [self.view addSubview:tfPassword];
    
    UIButton *btnLogin = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [self.view addSubview:btnLogin];
    
    tfUserName.placeholder = @"用户名";
    tfUserName.keyboardType = UIKeyboardTypeEmailAddress;
    [tfUserName setAutocorrectionType:UITextAutocorrectionTypeNo];
    [tfUserName setAutocapitalizationType:UITextAutocapitalizationTypeNone];
    tfUserName.layer.cornerRadius = 3;
    tfUserName.layer.borderColor = [UIColor grayColor].CGColor;
    tfUserName.layer.borderWidth = 1.f / [UIScreen mainScreen].scale;
    [tfUserName mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(16);
        make.right.mas_equalTo(-16);
        make.top.mas_equalTo(150);
    }];
    
    tfPassword.placeholder = @"密码";
    tfPassword.secureTextEntry = YES;
    tfPassword.layer.cornerRadius = 3;
    tfPassword.layer.borderColor = [UIColor grayColor].CGColor;
    tfPassword.layer.borderWidth = 1.f / [UIScreen mainScreen].scale;
    [tfPassword mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(tfUserName);
        make.top.mas_equalTo(tfUserName.mas_bottom).offset(8);
    }];
    
    btnLogin.layer.cornerRadius = 3;
    btnLogin.layer.borderColor = [UIColor grayColor].CGColor;
    btnLogin.layer.borderWidth = 1.f / [UIScreen mainScreen].scale;
    [btnLogin setTitle:@"登录" forState:UIControlStateNormal];
    [btnLogin addTarget:self action:@selector(handleLogin:) forControlEvents:UIControlEventTouchUpInside];
    [btnLogin mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(tfUserName);
        make.top.mas_equalTo(tfPassword.mas_bottom).offset(16);
    }];
}

+ (BOOL)isLogin {
    WDGOptions *options = [[WDGOptions alloc] initWithSyncURL:@"https://wd9641493984khzlrv.wilddogio.com"];
    [WDGApp configureWithOptions:options];
    WDGAuth *auth = [WDGAuth auth];
    return !![auth currentUser];
}

+ (void)logout {
    WDGOptions *options = [[WDGOptions alloc] initWithSyncURL:@"https://wd9641493984khzlrv.wilddogio.com"];
    [WDGApp configureWithOptions:options];
    WDGAuth *auth = [WDGAuth auth];
    NSError *error;
    [auth signOut:&error];
    if (error) {
        [SVProgressHUD showErrorWithStatus:error.localizedDescription];
    } else {
        [SVProgressHUD showSuccessWithStatus:@"登出成功"];
    }
}

- (void)handleLogin:(id)sender {
    NSString *userName = self.tfUserName.text;
    NSString *password = self.tfPassword.text;
    
    WDGOptions *options = [[WDGOptions alloc] initWithSyncURL:@"https://wd9641493984khzlrv.wilddogio.com"];
    [WDGApp configureWithOptions:options];
    WDGAuth *auth = [WDGAuth auth];
    
    [auth signInWithEmail:userName
                 password:password
               completion:^(WDGUser *user, NSError *error) {
                   if (!error) {                       
                       XMAdminViewController *admin = [[XMAdminViewController alloc] initWithNibName:nil bundle:nil];
                       [self.navigationController pushViewController:admin animated:YES];
                   } else {
                       [SVProgressHUD showErrorWithStatus:error.localizedDescription];
                   }
               }];
}

@end
