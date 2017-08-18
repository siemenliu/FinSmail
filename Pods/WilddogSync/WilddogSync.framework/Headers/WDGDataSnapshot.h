//
//  WDGDataSnapshot.h
//  Wilddog
//
//  Created by Garin on 15/7/7.
//  Copyright (c) 2015年 Wilddog. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WDGSyncReference;

NS_ASSUME_NONNULL_BEGIN

/**
 `WDGDataSnapshot` 是当前指定节点下数据的快照，`WDGDataSnapshot` 不会随当前节点数据的变化而发生改变。
 我们无法直接创建这个对象，而应当在 `[WDGSyncReference observeEventType:withBlock:]` 或 `[WDGSyncReference observeSingleEventOfType:withBlock:]` 的回调函数中获取它。
 */
@interface WDGDataSnapshot : NSObject

#pragma mark - 检索快照

/**
 根据相对路径，来获取当前节点下子节点的数据快照。
 相对路径可以是一个字节点的 key 值（例如：`Beijing`），也可以是更深层次的路径（例如：`Beijing/pm25`）。
 如果相对路径下并没有数据，则返回 nil。
 根据指定的相对路径，来获取当前节点下的 `WDGDataSnapshot`。

 @param childPathString 节点数据的相对路径，多层级间需要使用 `/` 分隔。

 @return `WDGDataSnapshot` 实例。
 */
- (WDGDataSnapshot *)childSnapshotForPath:(NSString *)childPathString;


/**
 判断是否存在某个指定的子节点。如果指定节点下的数据不为空，则返回 YES。
 
 @param childPathString 相对路径

 @return 如果指定路径下存在子节点，返回 YES，否则返回 NO。
 */
- (BOOL)hasChild:(NSString *)childPathString;


/**
 如果 `WDGDataSnapshot` 存在子节点返回 YES，否则返回 NO。
 你可以通过使用 `hasChildren` 方法来确定当前的数据快照是否含有子节点，进而决定是否利用 `children` 属性遍历数据。

 @return 如果 `WDGDataSnapshot` 存在子节点返回 YES，否则返回 NO。
 */
- (BOOL)hasChildren;


/**
 判断当前 `WDGDataSnapshot` 实例中是否包含数据。使用 exists 方法进行非空判断比 `snapshot.value != nil` 更高效。

 @return 如果 `WDGDataSnapshot` 包含非空数据，返回 YES。
 */
- (BOOL)exists;

#pragma mark - 数据导出

/**
 将 `WDGDataSnapshot` 中的全部内容导出。
 `valueInExportFormat` 方法和 `value` 方法类似，都可以导出数据。但是当节点的 priority 值不为空时，`valueInExportFormat` 会导出包含 priority 的数据，适合用于备份。
 */
- (id __nullable)valueInExportFormat;

#pragma mark - 属性

/**
 当前数据快照包含的数据。数据类型取决于节点下的数据内容。

 可能返回的数据类型包括:

 - NSDictionary
 - NSArray
 - NSNumber (包含 BOOL 类型)
 - NSString
 */
@property (strong, readonly, nonatomic, nullable) id value;


/**
 `WDGDataSnapshot` 的子节点的总数。
 */
@property (readonly, nonatomic) NSUInteger childrenCount;


/**
 当前数据快照所关联的 `WDGSyncReference` 实例。
 */
@property (nonatomic, readonly, strong) WDGSyncReference *ref;


/**
 当前 `WDGDataSnapshot` 所属节点的 key。
 */
@property (strong, readonly, nonatomic) NSString *key;


/**
 当前 `WDGDataSnapshot` 中，所有子节点的迭代器。
 例如：
 ```objectivec
 for (WDGDataSnapshot *child in snapshot.children) {
     ...
 }
 ```
 */
@property (strong, readonly, nonatomic) NSEnumerator <WDGDataSnapshot *> *children;


/**
 当前节点的 priority 值。优先级不存在时为 nil。
 */
@property (strong, readonly, nonatomic, nullable) id priority;

@end

NS_ASSUME_NONNULL_END
