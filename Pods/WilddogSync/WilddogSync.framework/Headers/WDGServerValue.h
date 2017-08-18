//
//  WDGServerValue.h
//  WilddogSync
//
//  Created by junpengwang on 16/9/8.
//  Copyright © 2016年 wilddog. All rights reserved.
//

NS_ASSUME_NONNULL_BEGIN

/**
 用于写入 Wilddog Sync 时间戳。
 */
@interface WDGServerValue : NSObject

/**
 继承自 `NSObject` 的初始化方法不可用。

 @return 无效的 `WDGServerValue` 实例。
 */
- (instancetype)init NS_UNAVAILABLE;


/**
 可以作为 value 或者 priority 写入 Wilddog Sync 中，写入的字典数据会由 Wilddog Sync 自动转换为自 Unix epoch 开始的的毫秒数。
 详细使用可参考：`timestamp 完整指南`
 
 @return 返回一个载有 `\@{ \@".sv": \@"timestamp" }` 的字典。
 */
+ (NSDictionary *)timestamp;

@end

NS_ASSUME_NONNULL_END
