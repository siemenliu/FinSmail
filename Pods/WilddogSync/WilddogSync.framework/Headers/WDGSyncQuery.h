//
//  WDGSyncQuery.h
//  Wilddog
//
//  Created by Garin on 15/7/7.
//  Copyright (c) 2015年 Wilddog. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WDGDataEventType.h"
#import "WDGDataSnapshot.h"

NS_ASSUME_NONNULL_BEGIN

typedef int64_t WDGSyncHandle;

/**
 查询指定路径和指定条件下的数据。
 */
@interface WDGSyncQuery : NSObject

#pragma mark - 监听数据变化，读取数据

/**
 监听指定节点的数据。
 这是从 Wilddog Sync 云端监听数据的主要方式，当监听到当前节点的初始数据或当前节点的数据改变时，将会触发指定事件对应的回调 block。
 可使用 `removeObserverWithHandle:` 方法移除监听。

 详细使用可参考：`observeEventType:withBlock: 完整指南`

 @param eventType `WDGDataEventType` 类型，表示监听的事件类型。
 @param block 当监听到当前节点的初始数据或当前节点的数据改变时，将会触发指定事件对应的回调 block。
 @return `WDGSyncHandle` 值，用于调用方法 `removeObserverWithHandle:` 移除这个监听。
 */
- (WDGSyncHandle)observeEventType:(WDGDataEventType)eventType withBlock:(void (^)(WDGDataSnapshot *snapshot))block;


/**
 监听指定节点的数据。
 这是从 Wilddog Sync 云端监听数据的主要方式，当监听到当前节点的初始数据或当前节点的数据改变时，将会触发指定事件对应的回调 block。
 此外，对于 `WDGDataEventTypeChildAdded`, `WDGDataEventTypeChildMoved` 和 `WDGDataEventTypeChildChanged` 事件，回调 block 将带有当前排序下前一节点的 key 值。
 可使用 `removeObserverWithHandle:` 方法移除监听。

 @param eventType `WDGDataEventType` 类型，表示监听的事件类型。
 @param block 当监听到当前节点的初始数据或当前节点的数据改变时，将会触发指定事件对应的回调 block。block 将传输一个 `WDGDataSnapshot` 类型的数据和前一个节点的 key 值。
 @return `WDGSyncHandle` 值，用于调用方法 `removeObserverWithHandle:` 移除这个监听。
 */
- (WDGSyncHandle)observeEventType:(WDGDataEventType)eventType andPreviousSiblingKeyWithBlock:(void (^)(WDGDataSnapshot *snapshot, NSString *__nullable prevKey))block;


/**
 监听指定节点的数据。
 这是从 Wilddog Sync 云端监听数据的主要方式，当监听到当前节点的初始数据或当前节点的数据改变时，将会触发指定事件对应的回调 block。
 当客户端失去对该节点的读取权限时会调用 `cancelBlock`。导致失去读取权限的原因包括：规则表达式限制，数据限制，套餐限制超出等。
 可使用 `removeObserverWithHandle:` 方法移除监听。

 @param eventType `WDGDataEventType` 类型，表示监听的事件类型。
 @param block 当监听到当前节点的初始数据或当前节点的数据改变时，将会触发指定事件对应的回调 block。
 @param cancelBlock 当客户端失去对该节点的读取权限时会调用 `cancelBlock`。
 @return `WDGSyncHandle` 值，用于调用方法 `removeObserverWithHandle:` 移除这个监听。
 */
- (WDGSyncHandle)observeEventType:(WDGDataEventType)eventType withBlock:(void (^)(WDGDataSnapshot *snapshot))block withCancelBlock:(nullable void (^)(NSError *error))cancelBlock;


/**
 监听指定节点的数据。
 这是从 Wilddog Sync 云端监听数据的主要方式，当监听到当前节点的初始数据或当前节点的数据改变时，将会触发指定事件对应的回调 block。
 此外，对于 `WDGDataEventTypeChildAdded`, `WDGDataEventTypeChildMoved` 和 `WDGDataEventTypeChildChanged` 事件，回调 block 将带有当前排序下前一节点的 key 值。
 当客户端失去对该节点的读取权限时会调用 `cancelBlock`。导致失去读取权限的原因包括：规则表达式限制，数据限制，套餐限制超出等。
 可使用 `removeObserverWithHandle:` 方法移除监听。

 @param eventType `WDGDataEventType` 类型，表示监听的事件类型。
 @param block 当监听到当前节点的初始数据或当前节点的数据改变时，将会触发指定事件对应的回调 block。block 将传输一个 `WDGDataSnapshot` 类型的数据和前一个子节点的 key 值。
 @param cancelBlock 当客户端失去对该节点的读取权限时会调用 `cancelBlock`。
 @return `WDGSyncHandle` 值，用于调用方法 `removeObserverWithHandle:` 移除这个监听。
 */
- (WDGSyncHandle)observeEventType:(WDGDataEventType)eventType andPreviousSiblingKeyWithBlock:(void (^)(WDGDataSnapshot *snapshot, NSString *__nullable prevKey))block withCancelBlock:(nullable void (^)(NSError *error))cancelBlock;


/**
 同 `observeEventType:withBlock:` 类似，不同之处在于 `observeSingleEventOfType:withBlock:` 中的回调方法只被触发一次，之后会自动取消监听。

 @param eventType `WDGDataEventType` 类型，表示监听的事件类型。
 @param block 当从云端获取到结果时，将回调这个 block。
 */
- (void)observeSingleEventOfType:(WDGDataEventType)eventType withBlock:(void (^)(WDGDataSnapshot *snapshot))block;


/**
 同 `observeEventType:withBlock:` 类似，不同之处在于 `observeSingleEventOfType:withBlock:` 中的回调函数只被执行一次。
 此外，对于 `WDGDataEventTypeChildAdded`, `WDGDataEventTypeChildMoved` 和 `WDGDataEventTypeChildChanged` 事件，回调 block 将带有 priority 排序下前一节点的 key 值。

 @param eventType `WDGDataEventType` 类型，表示监听的事件类型。
 @param block 当从云端获取到结果时，将回调这个 block。block 将传输一个 `WDGDataSnapshot` 类型的数据和前一个子节点的 key 值。
 */
- (void)observeSingleEventOfType:(WDGDataEventType)eventType andPreviousSiblingKeyWithBlock:(void (^)(WDGDataSnapshot *snapshot, NSString *__nullable prevKey))block;


/**
 同 `observeEventType:withBlock:` 类似，不同之处在于 `observeSingleEventOfType:withBlock:` 中的回调函数只被执行一次。
 当客户端没有对该节点的访问权限时 `cancelBlock` 会被调用。

 @param eventType `WDGDataEventType` 类型，表示监听的事件类型。
 @param block 当从云端获取到结果时，将回调这个 block。
 @param cancelBlock 当客户端没有对该节点的访问权限时 `cancelBlock` 会被调用。
 */
- (void)observeSingleEventOfType:(WDGDataEventType)eventType withBlock:(void (^)(WDGDataSnapshot *snapshot))block withCancelBlock:(nullable void (^)(NSError *error))cancelBlock;


/**
 同 `observeEventType:withBlock:` 类似，不同之处在于 `observeSingleEventOfType:withBlock:` 中的回调函数只被执行一次。
 此外，对于 `WDGDataEventTypeChildAdded`, `WDGDataEventTypeChildMoved` 和 `WDGDataEventTypeChildChanged` 事件，回调 block 将带有 priority 排序下前一节点的 key 值。
 当客户端没有对该节点的访问权限时 `cancelBlock` 会被调用。

 @param eventType `WDGDataEventType` 类型，表示监听的事件类型。
 @param block 当从云端获取到结果时，将回调这个 block。block 将传输一个 `WDGDataSnapshot` 类型的数据和前一个子节点的 key 值。
 @param cancelBlock 当客户端没有对该节点的访问权限时 `cancelBlock` 会被调用。
 */
- (void)observeSingleEventOfType:(WDGDataEventType)eventType andPreviousSiblingKeyWithBlock:(void (^)(WDGDataSnapshot *snapshot, NSString *__nullable prevKey))block withCancelBlock:(nullable void (^)(NSError *error))cancelBlock;

#pragma mark - 移除数据监听

/**
 移除监听事件。移除使用 `observeEventType:withBlock:` 方法设置的数据监听。

 详细使用可参考：`removeObserverWithHandle: 完整指南`

 @param handle 由 `observeEventType:withBlock:` 返回的 `WDGSyncHandle`。
 */
- (void)removeObserverWithHandle:(WDGSyncHandle)handle;


/**
 移除当前节点下使用 `observeEventType:withBlock:` 方法注册的所有的监听事件。
 */
- (void)removeAllObservers;


/**
 在某一节点处通过调用 `keepSynced:YES` 方法，即使该节点处没有进行过监听，此节点处的数据也将自动下载存储并与云端保持同步。

 详细使用可参考：`keepSynced: 完整指南`

 @param keepSynced 参数设置为 YES，则在此节点处同步数据；设置为 NO，停止同步。
 */
- (void)keepSynced:(BOOL)keepSynced;

#pragma mark - 查询和限制

/**
 创建一个新的 `WDGSyncQuery` 实例，获取当前排序下从第一个节点开始的最多 (limit) 条数据。
 详细使用可参考：`queryLimitedToFirst: 完整指南`

 @param limit 能够获取的子节点的最大数量。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryLimitedToFirst:(NSUInteger)limit;


/**
 创建一个新的 `WDGSyncQuery` 实例，获取当前排序下，从最后一个节点开始向前的最多 (limit) 条数据。
 详细使用可参考：`queryLimitedToLast: 完整指南`

 @param limit 能够获取的子节点的最大数量。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryLimitedToLast:(NSUInteger)limit;


/**
 创建一个新的 `WDGSyncQuery` 实例，按子节点下指定的 key 对应的 value 对结果进行排序。
 此方法可以与 `queryStartingAtValue:`、`queryEndingAtValue:` 或 `queryEqualToValue:` 方法联合使用。

 详细使用可参考：`queryOrderedByChild: 完整指南`

 @param key 指定用来排序的子节点的 key。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryOrderedByChild:(NSString *)key;


/**
 创建一个新的 `WDGSyncQuery` 实例，按子节点的 key 对结果以字典序进行排序。
 此方法可以与 `queryStartingAtValue:`、`queryEndingAtValue:` 或 `queryEqualToValue:` 方法联合使用。

 详细使用可参考：`queryOrderedByKey 完整指南`

 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryOrderedByKey;


/**
 创建一个新的 `WDGSyncQuery` 实例，按节点的 value 对结果排序。
 此方法可以与 `queryStartingAtValue:`、`queryEndingAtValue:` 或 `queryEqualToValue:` 方法联合使用。

 详细使用可参考：`queryOrderedByValue 完整指南`

 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryOrderedByValue;


/**
 创建一个新的 `WDGSyncQuery` 实例，按节点的 priority 对结果排序。
 节点按照如下优先级规则升序排列：nil < NSNumber < NSString。

 - priority 为 nil 的排最先；
 - priority 为数值的次之，按照数值从小到大排序；
 - priority 为字符串的排最后，按照字典序排列；
 - 当两个子节点有相同的 priority（包括没有 priority），它们按照 key 进行排列，数字优先（按数值从小到大排序），其余以字典序排序。

 此方法可以与 `queryStartingAtValue:`、`queryEndingAtValue:` 或 `queryEqualToValue:` 方法联合使用。

 详细使用可参考：`queryOrderedByPriority 完整指南`

 @warning 数值优先级被作为 IEEE 754 双精度浮点型数字进行解析和排序。Key 以 String 类型进行存储，只有当它能被解析成 32 位整型数字时被当作数字来处理。

 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryOrderedByPriority;


/**
 创建一个新的 `WDGSyncQuery` 实例，可以查询所有大于或等于指定的 key、value 或 priority 的节点，具体取决于所选的排序方法。
 此方法应与 `queryOrderedByPriority`、`queryOrderedByKey`、`queryOrderedByValue` 或 `queryOrderedByChild:` 方法联合使用。

 详细使用可参考：`queryStartingAtValue: 完整指南`

 @param startValue query 查询返回值的下界，所有返回值均大于等于 startValue。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryStartingAtValue:(nullable id)startValue;


/**
 创建一个新的 `WDGSyncQuery` 实例，可以查询所有大于或等于指定的 value 或 priority 的节点，具体取决于所选的排序方法。
 当查询到的 value 与 startValue 相等时，则只保留 key 大于等于 childKey 的节点。
 此方法应与 `queryOrderedByPriority`、`queryOrderedByValue` 或 `queryOrderedByChild:` 方法联合使用。
 该方法可用于分页。

 @param startValue query 查询返回值的下界，所有返回值均大于等于 startValue。
 @param childKey 当 query 查询到的值和 startValue 相等时，返回其中 key 大于等于 childKey 的节点。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryStartingAtValue:(nullable id)startValue childKey:(nullable NSString *)childKey;


/**
 创建一个新的 `WDGSyncQuery` 实例，可以查询所有小于或等于指定的 key、value 或 priority 的节点，具体取决于所选的排序方法。
 此方法应与 `queryOrderedByPriority`、`queryOrderedByKey`、`queryOrderedByValue` 或 `queryOrderedByChild:` 方法联合使用。

 详细使用可参考：`queryEndingAtValue: 完整指南`

 @param endValue query 查询返回值的上界，所有返回值均小于等于 endValue。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryEndingAtValue:(nullable id)endValue;


/**
 创建一个新的 `WDGSyncQuery` 实例，可以查询所有小于或等于指定的 value 或 priority 的节点，具体取决于所选的排序方法。
 当查询到的 value 与 endValue 相等时，则只保留 key 小于等于 childKey 的节点。
 此方法应与 `queryOrderedByPriority`、`queryOrderedByValue` 或 `queryOrderedByChild:` 方法联合使用。
 该方法可用于分页。

 @param endValue query 查询返回值的上界，所有返回值均小于等于 endValue。
 @param childKey 当 query 查询到的值和 endValue 相等时，返回其中 key 小于等于 childKey 的节点。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryEndingAtValue:(nullable id)endValue childKey:(nullable NSString *)childKey;


/**
 创建一个新的 `WDGSyncQuery` 实例，可以查询等于指定的 key、value 或 priority 的节点，具体取决于所选的排序方法。可用于精确查询。
 此方法应与 `queryOrderedByPriority`、`queryOrderedByKey`、`queryOrderedByValue` 或 `queryOrderedByChild:` 方法联合使用。

 详细使用可参考：`queryEqualToValue: 完整指南`

 @param value query 查询到的值都等于 value。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryEqualToValue:(nullable id)value;


/**
 创建一个新的 `WDGSyncQuery` 实例，可以查询等于指定的 value 或 priority 的节点，具体取决于所选的排序方法。可用于精确查询。
 并且 query 查询到的 key 都等于 childKey。由于 key 是唯一的，查询最多返回一个节点。
 此方法应与 `queryOrderedByPriority`、`queryOrderedByValue` 或 `queryOrderedByChild:` 方法联合使用。

 @param value query 查询到的值都等于 value。
 @param childKey query 查询到的 key 都等于 childKey。
 @return `WDGSyncQuery` 实例。
 */
- (WDGSyncQuery *)queryEqualToValue:(nullable id)value childKey:(nullable NSString *)childKey;

#pragma mark - 属性

/**
 这个 `WDGSyncQuery` 所在路径下的 `WDGSyncReference` 实例。
 */
@property (nonatomic, readonly, strong) WDGSyncReference *ref;

@end

NS_ASSUME_NONNULL_END
