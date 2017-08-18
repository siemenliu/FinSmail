//
//  WDGWilddogAuthProvider.h
//  WilddogAuth
//
//  Created by Garin on 16/9/28.
//  Copyright © 2016年 wilddog. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WDGAuthCredential;

NS_ASSUME_NONNULL_BEGIN

/** @var WDGWilddogAuthProviderID
 email & password 或者 phone & password 登录方式的 providerID。具体值为 “password”。
 */
extern NSString *const WDGWilddogAuthProviderID;

/** @class WDGWilddogAuthProvider
 email & password 或者 phone & password 登录方式的工厂类，用于生成 WDGAuthCredential 凭证。
 */
@interface WDGWilddogAuthProvider : NSObject

/** @fn credentialWithEmail:password:
 
 @brief 创建一个 email & password 登录方式的 WDGAuthCredential 凭证。
 @param email 用户的 email 地址。
 @param password 用户的登录密码。
 @return WDGAuthCredential 对象，里面包含 email & password 登录方式凭证。
 
 */
+ (WDGAuthCredential *)credentialWithEmail:(NSString *)email password:(NSString *)password;

/** @fn credentialWithPhone:password:
 
 @brief 创建一个 phone & password 登录方式的 WDGAuthCredential 凭证。
 @param phone 用户的手机号码。
 @param password 用户的登录密码。
 @return WDGAuthCredential 对象，里面包含 phone & password 登录方式凭证。
 
 */
+ (WDGAuthCredential *)credentialWithPhone:(NSString *)phone password:(NSString *)password;

/** @fn init
 @brief 这个类不需要初始化。
 */
- (nonnull instancetype)init NS_UNAVAILABLE;
@end

NS_ASSUME_NONNULL_END

