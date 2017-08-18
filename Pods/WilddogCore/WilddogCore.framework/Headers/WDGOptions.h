//
//  WDGOptions.h
//  WilddogSync
//
//  Created by junpengwang on 16/9/8.
//  Copyright © 2016年 wilddog. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 用于 Sync 和 Auth SDK 初始化时的参数配置。
 */
@interface WDGOptions : NSObject

/**
 Sync 的根路径 URL，例如: `\@"https://your-appid.wilddogio.com"` 。
 */
@property(nonatomic, readonly, copy) NSString *syncURL;


/**
 初始化 `WDGOptions` 实例。

 @param syncURL Sync 的根路径 URL，例如: `\@"https://your-appid.wilddogio.com"` 。

 @return 初始化成功的 `WDGOptions` 实例。
 */
- (instancetype)initWithSyncURL:(NSString *)syncURL;

@end

NS_ASSUME_NONNULL_END
