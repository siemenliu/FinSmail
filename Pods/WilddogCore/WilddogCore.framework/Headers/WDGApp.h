//
//  WDGApp.h
//  WilddogSync
//
//  Created by junpengwang on 16/9/8.
//  Copyright © 2016年 wilddog. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WDGOptions;

NS_ASSUME_NONNULL_BEGIN

typedef void (^WDGAppVoidBoolCallback)(BOOL success);

/**
 `WDGApp` 是野狗各个功能模块的入口。用于初始化时的必要参数配置。
 一个 `WDGApp` 实例对应一个野狗应用，在 SDK 内以应用名字区分。
 */
@interface WDGApp : NSObject

/**
 初始化默认的 Wilddog app。默认的 app 名字是 `__WDGAPP_DEFAULT`。如果配置失败，会抛出异常。
 这个方法是线程安全的。

 @param options 配置 Wilddog 应用所需的 `WDGOptions` 实例。
 */
+ (void)configureWithOptions:(WDGOptions *)options;


/**
 用 options 和 name 配置一个 Wilddog app。如果配置失败，会抛出异常。
 这个方法是线程安全的。

 @param name 开发者自己起名的应用名字。这个名字只能包含字母、数字和下划线。
 @param options 配置 Wilddog 应用所需的 `WDGOptions` 实例。
 */
+ (void)configureWithName:(NSString *)name options:(WDGOptions *)options;


/**
 返回默认的 `WDGApp` 实例，即通过 `configureWithOptions:` 配置的实例。如果默认 app 不存在，则返回 nil。
 这个方法是线程安全的。
 
 @return `WDGApp` 实例。
 */
+ (nullable WDGApp *)defaultApp NS_SWIFT_NAME(defaultApp());


/**
 返回一个之前通过 `configureWithName:options:` 配置的 WDGApp 实例。如果这个 app 不存在, 则返回 nil。
 这个方法是线程安全的。
 
 @return `WDGApp` 实例。
 */
+ (nullable WDGApp *)appNamed:(NSString *)name;


/**
 返回所有现存的 `WDGApp` 实例。如果没有 `WDGApp` 实例，则返回 nil。
 这个方法是线程安全的。

 @return 包含所有 `WDGApp` 实例的字典，key 为 `WDGApp` 实例对应的名字。
 */
+ (nullable NSDictionary *)allApps;


/**
 清除当前的 `WDGApp`, 释放相关的数据，并回收它的名字以便将来使用。
 这个方法是线程安全的。
 
 @param completion 删除成功后，会调用这个回调函数。
 */
- (void)deleteApp:(WDGAppVoidBoolCallback)completion;


/**
 `WDGApp` 实例不能直接初始化。
 请用 `configure`, 或者 `configureWithOptions:`, 或者 `configureWithNames:options:` 初始化。
 随后通过 `defaultApp` 或者 `appNamed:` 获得 `WDGApp` 实例。
 */
- (nonnull instancetype)init NS_UNAVAILABLE;


/**
 获取这个 `WDGApp` 实例的名字。
 */
@property(nonatomic, copy, readonly) NSString *name;


/**
 初始化 SDK 的配置参数。
 */
@property(nonatomic, readonly) WDGOptions *options;

@end

NS_ASSUME_NONNULL_END
