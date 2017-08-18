//
//  WilddogAuth.m
//  WilddogAuth
//
//  Created by junpengwang on 16/8/5.
//  Copyright © 2016年 Wilddog. All rights reserved.
//

#import <Foundation/Foundation.h>

/** @class WDGAuthErrors
 如果在 WilddogAuth 的回调方法中收到一个非空 NSError 参数，则表示出现了错误。 要想实现正确的错误逻辑处理，请对照常见错误和下面所列的方法特定错误检查错误代码。
 有些错误可通过特定用户操作解决。例如，WDGAuthErrorCodeCredentialTooOldLoginAgain 可通过重新登录该用户解决，WDGAuthErrorCodeInvalidPassword 可通过让用户提供正确密码解决。
 除 WDGAuthErrorCodeSmsFrequencyError 之外，用相同参数重试一个失败的操作决不会成功。切勿对操作在服务器端是否生效进行任何假设。
 
 调试或打印错误时，请查阅 userInfo 字典。WDGAuthErrorNameKey 包含可用于识别错误的跨平台错误名字符串。NSLocalizedDescriptionKey 包含错误说明。此说明只适合开发者使用，不适合用户使用。NSUnderlyingErrorKey 包含引起所述错误的基础错误（如果存在基础错误）。
 除了上面所列的主要字段外，userInfo 字典中可能还提供了一些您在诊断错误时可能觉得有用的其他字段。

 */
@interface WDGAuthErrors : NSObject

/** @var WDGAuthErrorDomain
 @brief Wilddog Auth 错误信息的域名。
 */
extern NSString *const WDGAuthErrorDomain;

/** @var WDGAuthErrorNameKey
 @brief Wilddog Auth 具体错误信息的 key。
 */
extern NSString *const WDGAuthErrorNameKey;

/** @enum WDGAuthErrorCode
 @brief Wilddog Auth 的错误标识。
 */
typedef NS_ENUM(NSInteger, WDGAuthErrorCode) {
    
    /** 服务异常，操作失败。
     */
    WDGAuthErrorCodeFailure = 22001,
    
    /** 该 token 已过期。
     */
    WDGAuthErrorCodeExpiredToken = 22002,
    
    /** oauth token 已过期，获取失败，请联系 support@wilddog.com。
     */
    WDGAuthErrorCodeExpiredFetchToken = 22003,
    
    /** 用户提供的 token 无效。
     */
    WDGAuthErrorCodeInvalidToken = 22004,
    
    /** 用户创建失败，请重试。
     */
    WDGAuthErrorCodeMaxRetries = 22005,
    
    /** 未开启身份认证功能，请在控制台开启后重试。
     */
    WDGAuthErrorCodeAuthenticationDisabled = 22006,
    
    /** token 认证需要超级密钥。
     */
    WDGAuthErrorCodeInvalidConfiguration = 22007,
    
    /** 身份认证提供商调用错误，请联系 support@wilddog.com。
     */
    WDGAuthErrorCodeInvalidProvider = 22008,
    
    /** 该邮箱地址无效。
     */
    WDGAuthErrorCodeInvalidEmail = 22009,
    
    /** 该密码不正确。
     */
    WDGAuthErrorCodeInvalidPassword = 22010,
    
    /** 该用户不存在。
     */
    WDGAuthErrorCodeInvalidUser = 22011,
    
    /** 身份认证过程中，发生了安全错误。
     */
    WDGAuthErrorCodeInvalidOrigin = 22012,
    
    /** 该邮箱地址已经使用。
     */
    WDGAuthErrorCodeEmailTaken = 22013,
    
    /** 该身份认证凭证无效。
     */
    WDGAuthErrorCodeInvalidCredentials = 22014,
    
    /** 该身份认证参数无效。
     */
    WDGAuthErrorCodeInvalidArguments = 22015,
    
    /** 第三方身份认证提供商发生错误，查看信息了解详情。
     */
    WDGAuthErrorCodeProviderError = 22016,
    
    /** 超级密钥无效，请在控制面板重新生成。
     */
    WDGAuthErrorCodeInvalidSecret = 22017,
    
    /** 本次重置密码请求无效的。
     */
    WDGAuthErrorCodeInvalidObbCode = 22018,
    
    /** 该野狗用户不存在，请检查 appID。
     */
    WDGAuthErrorCodeAppUserNotExist = 22101,
    
    /** 不是管理者，没有操作权限。
     */
    WDGAuthErrorCodeNotAdminToken = 22103,
    
    /** 每个 oauth 身份认证提供商，用户只能绑定一次。
     */
    WDGAuthErrorCodeProvideAlreadyLinked = 22201,
    
    /** 用户已经升级 API，但 token 仍是旧版。
     */
    WDGAuthErrorCodeTokenVersionError = 22202,
    
    /** 邮箱地址已经被其他账户使用。
     */
    WDGAuthErrorCodeEmailAlreadyInUse = 22203,
    
    /** 该身份已经与其他账户绑定。
     */
    WDGAuthErrorCodeProviderAlreadyLinkedAnotherAccount = 22204,
    
    /** 该账户没有绑定邮箱。
     */
    WDGAuthErrorCodeEmailMissing = 22205,
    
    /** 没有对应用户记录，该用户可能已经被删除。
     */
    WDGAuthErrorCodeUserNotFound = 22206,
    
    /** 该 token 解析失败。
     */
    WDGAuthErrorCodeTokenError = 22207,
    
    /** 每个用户只能绑定一次野狗登录方式。
     */
    WDGAuthErrorCodeProviderAlreadyLinked = 22208,
    
    /** 该用户尝试安全敏感操作，但登录时间过长，需重新登录。
     */
    WDGAuthErrorCodeCredentialTooOldLoginAgain = 22209,
    
    /** 该用户没有 Wilddog 登录方式。
     */
    WDGAuthErrorCodeNoPasswordProvider = 22210,
    
    /** 密码的长度必须在 6 到 32 位。
     */
    WDGAuthErrorCodePasswordLengthError = 22211,
    
    /** 昵称长度必须小于 20 位。
     */
    WDGAuthErrorCodeDisplayNameLengthError = 22212,
    
    /** 照片地址长度必须小于 1024 位。
     */
    WDGAuthErrorCodePhotoUrlLengthError = 22213,
    
    /** 此 token 不属于该应用。
     */
    WDGAuthErrorCodeTokenNotForThisApp = 22214,
    
    /** 该 token 类型不正确。
     */
    WDGAuthErrorCodeTokenTypeError = 22215,
    
    /** Wilddog ID token 自定义字段不正确。
     */
    WDGAuthErrorCodeIDTokenClaimsError = 22216,
    
    /** 该 token 的 Wilddog ID 无效。
     */
    WDGAuthErrorCodeTokenUserIDNotValid = 22217,
    
    /** Wilddog 属性不正确。
     */
    WDGAuthErrorCodeTokenWilddogError = 22218,
    
    /** 该手机号码不正确。
     */
    WDGAuthErrorCodeInvalidPhone = 22219,
    
    /** 该邮箱不存在。
     */
    WDGAuthErrorCodeEmailNotExist = 22220,
    
    /** 该手机号不存在。
     */
    WDGAuthErrorCodePhoneNotExist = 22221,
    
    /** 该手机未发送过验证码，请检查。
     */
    WDGAuthErrorCodeSmsCodeNotExist = 22222,
    
    /** 发送验证码发生错误，请重试。
     */
    WDGAuthErrorCodeSmsSendError = 22223,
    
    /** 该手机号已被其他账户使用。
     */
    WDGAuthErrorCodePhoneAlreadyInUse = 22224,
    
    /** 照片地址或昵称包含非法字符。
     */
    WDGAuthErrorCodePhotoUrlOrDisplayNameError = 22225,
    
    /** 短信验证码错误，请重新发送验证码。
     */
    WDGAuthErrorCodeSmsCodeError = 22226,
    
    /** 短信服务错误，请重试。
     */
    WDGAuthErrorCodeSmsServerError = 22227,

    /** 短信禁止使用，请申请短信服务。
     */
    WDGAuthErrorCodeSmsForbiddenError = 22230,
    
    /** 短信类型为空。
     */
    WDGAuthErrorCodeInvalidSmsTypeError = 22232,
    
    /** 微信小程序登录错误，请查看详情。
     */
    WDGAuthErrorCodeWeappError = 22233,
    
    /** 微信小程序用户信息签名校验失败，请重新检查后重试。
     */
    WDGAuthErrorCodeWeappUserInfoSignCheckFailed = 22234,
    
    /** 短信发送过于频繁。
     */
    WDGAuthErrorCodeSmsFrequencyError = 22235,
    
    /** 帐户余额不足5元，不能开启短信服务。
     */
    WDGAuthErrorCodeSmsPayLimitError = 22236,
    
    /** 账户余额不足。
     */
    WDGAuthErrorCodeSmsPayNoMoneyError = 22237,
    
    /** 账户余额不足，本次发送无法完成，请充值。
     */
    WDGAuthErrorCodeSmsPayNoMoneyToPayError = 22238,
    
    /** 发送接口调用频繁，超过每秒 10 次，请稍后重试。
     */
    WDGAuthErrorCodeSmsAPICheckError = 22239,
    
    /** 发生未知错误。
     */
    WDGAuthErrorCodeUnknownError = 29999
};

@end
