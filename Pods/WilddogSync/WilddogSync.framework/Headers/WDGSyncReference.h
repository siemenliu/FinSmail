//
//  WDGSyncReference.h
//  WilddogSync
//
//  Created by junpengwang on 16/9/8.
//  Copyright © 2016年 wilddog. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WDGSyncQuery.h"
#import "WDGSync.h"
#import "WDGDataSnapshot.h"
#import "WDGMutableData.h"
#import "WDGTransactionResult.h"
#import "WDGServerValue.h"

NS_ASSUME_NONNULL_BEGIN

@class WDGSync;

/**
 `WDGSyncReference` 实例表示要操作的特定数据节点，可以通过 `WDGSyncReference` 实例操作和读取数据。
 `WDGSyncReference` 是 `WDGSyncQuery` 的子类。
 */
@interface WDGSyncReference : WDGSyncQuery

#pragma mark - 获取对子节点的引用

/**
 获得一个在当前节点下指定路径节点处的 `WDGSyncReference` 实例。
 相对路径可以是一个简单的节点路径（例如：`fred`），或者是一个更深的路径（例如：`fred/name/first`）。

 @param pathString 从这个节点到要设定的子节点的相对路径，深层路径多层级间需要使用 `/` 分隔，例如 `a/b` 。如果 path 为空字符串则返回当前引用。如果定位的 path 不存在，依然可以定位，将在后续数据操作时创建不存在的路径节点引用。

 @return 指定节点位置的 `WDGSyncReference` 实例。
 */
- (WDGSyncReference *)child:(NSString *)pathString;


/**
 `childByAppendingPath:` 已废弃, 使用 `child:` 代替。
 */
- (WDGSyncReference *)childByAppendingPath:(NSString *)pathString  __deprecated_msg("use child: instead");


/**
 向当前节点添加子节点。新增子节点的 key 自动生成并保证唯一（例如：`-KdzI7I-AsBST9NlasJM`）。
 新增子节点的 key 基于时间戳和随机算法生成，并按照时间先后进行排序。
 
 详细使用可参考：`childByAutoId 完整指南`

 @return `WDGSyncReference` 新增子节点的实例。
 */
- (WDGSyncReference *)childByAutoId;

#pragma mark - 写数据

/**
 往 `WDGSyncReference` 当前路径写入一个值，这将会覆盖当前路径和子路径的所有数据。
 
 支持的数据类型:

 - NSString -- `\@"Hello World"`
 - NSNumber (包括 BOOL 类型) -- `\@YES`, `\@43`, `\@4.333`
 - NSDictionary -- `\@{\@"key": \@"value", \@"nested": {\@"another": \@"value"}}`
 - NSArray -- `\@[\@"a", \@"b", \@"c"]`

 Wliddog Sync 没有对数组的原生支持，但是支持以数组下标作为 key ，数组元素作为 value 的方式进行存储。
 在数据监听中获取数据时，如果满足条件：当 0 到最大的 key（比如 n ）之间，n+1 个元素中超过一半以上有值，数据将被转换为 NSArray 类型;
 如果不满足条件，Wilddog Sync 处理数据时会将其转换为 NSDictionary 类型。
 
 当 value 为 nil 或者 NSNull 实例时相当于调用 `removeValue:`，这个路径的所有数据和子路径的数据都将被删除。

 `setValue:` 将会删除先前保存的 priority，所以如果要保留先前 priority，必须调用 `setValue:andPriority:`。

 详细使用可参考：`setValue: 完整指南`
 
 @param value 将被写入的数据。
 */
- (void)setValue:(nullable id)value;


/**
 同 `setValue:` 方法类似，增加了一个 block，当写操作完成之后，会回调这个 block。

 @param value 将被写入的数据。
 @param block 当写操作被提交到云端，将触发这个 block。

 @see setValue:
 */
- (void)setValue:(nullable id)value withCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference *ref))block;


/**
 同 `setValue:` 方法类似，写入数值的同时为当前节点设置优先级，优先级被用来排序。
 优先级只能是 NSNumber 或 NSString 类型，且 NSNumber 中不能存储 BOOL 类型的数据。优先级默认为 nil。

 详细使用可参考：`setValue:andPriority: 完整指南`

 @param value 将被写入的数据。
 @param priority 要设置的优先级。

 @see setValue:
 */
- (void)setValue:(nullable id)value andPriority:(nullable id)priority;


/**
 同 `setValue:` 方法类似，写入数值的同时为当前节点设置优先级，优先级被用来排序。
 优先级只能是 NSNumber 或 NSString 类型，且 NSNumber 中不能存储 BOOL 类型的数据。优先级默认为 nil。
 同时增加了一个 block，当写操作完成之后，会回调这个 block。

 @param value 将被写入的数据。
 @param priority 要设置的优先级。
 @param block 当写操作被提交到云端，将触发这个 block。

 @see setValue:
 */
- (void)setValue:(nullable id)value andPriority:(nullable id)priority withCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference* ref))block;


/**
 删除当前节点，效果等同于 `setValue:nil`。

 如果父级节点只有当前节点一个子节点，会递归删除父级节点。
 详细使用可参考：`removeValue 完整指南`
 */
- (void)removeValue;


/**
 同 `remove` 方法类似，增加了一个 block，当删除操作完成之后，会回调这个 block。

 @param block 当删除操作被提交到云端，将触发这个 block。
 */
- (void)removeValueWithCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference* ref))block;


/**
 设置当前节点的优先级，支持为每个节点设置优先级 (priority)，用于实现节点按优先级排序。优先级是节点的隐藏属性，默认为 nil。
 不能为不存在的节点设置优先级。因此，新增数据需要设置优先级时，请使用 `setValue:andPriority:`；为已存在的数据设置优先级的时，使用 `setPriority:`。

 节点按照如下优先级规则升序排列：nil < NSNumber < NSString。

 - priority 为 null 的排最先；
 - priority 为数值的次之，按照数值从小到大排序；
 - priority 为字符串的排最后，按照字典序排列。
 - 当两个子节点有相同的 priority（包括没有 priority），它们按照 key 进行排列，数字优先（按数值从小到大排序），其余以字典序排序。
 
 详细使用可参考：`setPriority: 完整指南`

 @warning 数值优先级被作为 IEEE 754 双精度浮点型数字进行解析和排序。Key 以 String 类型进行存储，只有当它能被解析成 32 位整型数字时被当作数字来处理。

 @param priority 指定节点的优先级。
 */
- (void)setPriority:(nullable id)priority;


/**
 同 `setPriority:` 方法类似，增加了一个 block，当设置优先级操作被提交到云端，将触发这个 block。

 @param priority 指定节点的优先级。
 @param block 当设置优先级操作被提交到云端，将触发这个 block。
 */
- (void)setPriority:(nullable id)priority withCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference* ref))block;


/**
 对当前节点进行数据合并操作，更新当前节点下的数据。 
 与 `setValue:` 方法覆盖当前节点下所有数据的方式不同，使用 `updateChildValues:` 方法，不存在的子节点将会被新增，存在的子节点将会被更新。
 使用此方法可以对同一节点的子节点同时进行更新和删除操作。

 详细使用可参考：`updateChildValues: 完整指南`

 @param values 包含要合并的子节点的字典。
 */
- (void)updateChildValues:(NSDictionary *)values;


/**
 同 `updateChildValues:` 方法类似，增加了一个 block，当更新操作完成之后，会回调这个 block。

 @param values 包含要合并的子节点的字典。
 @param block 当数据合并操作提交到 Wilddog Sync 云端，将触发这个 block。
 */
- (void)updateChildValues:(NSDictionary *)values withCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference *ref))block;

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

#pragma mark - 离线操作

/**
 在客户端离线时写入或清除数据，不论客户端是否是主动断开连接，已经设置的离线事件都必定会被执行。

 当客户端断开连接后，向当前的数据节点设置一个指定的值。

 详细使用可参考：`onDisconnectSetValue: 完整指南`

 @param value 在连接中断时需要写入当前位置的值。
 */
- (void)onDisconnectSetValue:(nullable id)value;


/**
 在客户端离线时写入或清除数据，不论客户端是否是主动断开连接，已经设置的离线事件都必定会被执行。

 当客户端断开连接后，向当前的数据节点设置一个指定的值。

 @param value 在连接中断时需要写入当前位置的值。
 @param block 当设置离线事件的操作被提交到云端，将触发这个 block。
 */
- (void)onDisconnectSetValue:(nullable id)value withCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference *ref))block;


/**
 在客户端离线时写入或清除数据，不论客户端是否是主动断开连接，已经设置的离线事件都必定会被执行。

 当客户端断开连接后，指定的数据和优先级会被写入当前位置。

 @param value 在连接中断时需要写入当前位置的值。
 @param priority 在连接中断时需要写入当前位置的优先级。
 */
- (void)onDisconnectSetValue:(nullable id)value andPriority:(id)priority;


/**
 在客户端离线时写入或清除数据，不论客户端是否是主动断开连接，已经设置的离线事件都必定会被执行。

 当客户端断开连接后，指定的数据和优先级会被写入当前位置。

 @param value 在连接中断时需要写入当前位置的值。
 @param priority 在连接中断时需要写入当前位置的优先级。
 @param block 当设置离线事件的操作被提交到云端，将触发这个 block。
 */
- (void)onDisconnectSetValue:(nullable id)value andPriority:(nullable id)priority withCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference *ref))block;


/**
 在客户端离线时写入或清除数据，不论客户端是否是主动断开连接，已经设置的离线事件都必定会被执行。

 当客户端断开连接后，删除当前位置上的数据。
 */
- (void)onDisconnectRemoveValue;


/**
 在客户端离线时写入或清除数据，不论客户端是否是主动断开连接，已经设置的离线事件都必定会被执行。

 当客户端断开连接后，删除当前位置上的数据。

 @param block 当设置离线事件的操作被提交到云端，将触发这个 block。
 */
- (void)onDisconnectRemoveValueWithCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference *ref))block;


/**
 在客户端离线时写入或清除数据，不论客户端是否是主动断开连接，已经设置的离线事件都必定会被执行。

 当客户端断开连接后，指定的子节点将被写入到当前位置的子节点集合中。

 @param values 在连接断开之后，用来更新当前位置的包含子节点键和值的字典。
 */
- (void)onDisconnectUpdateChildValues:(NSDictionary *)values;


/**
 在客户端离线时写入或清除数据，不论客户端是否是主动断开连接，已经设置的离线事件都必定会被执行。

 当客户端断开连接后，指定的子节点将被写入到当前位置的子节点集合中。

 @param values 在连接断开之后，用来更新当前位置的包含子节点键和值的字典。
 @param block 当设置离线事件的操作被提交到云端，将触发这个 block。
 */
- (void)onDisconnectUpdateChildValues:(NSDictionary *)values withCompletionBlock:(void (^)(NSError *__nullable error, WDGSyncReference *ref))block;


/**
 取消之前在当前节点下注册的所有离线操作。
 */
- (void)cancelDisconnectOperations;


/**
 取消之前在当前节点下注册的所有离线操作。
 
 @param block 当取消离线事件的操作被提交到云端，将触发这个 block。
 */
- (void)cancelDisconnectOperationsWithCompletionBlock:(nullable void (^)(NSError *__nullable error, WDGSyncReference *ref))block;

#pragma mark - 手动连接管理

/**
 手动断开与 Wilddog Sync 云端的连接，关闭自动重连，可以用 `goOnline` 恢复连接。
 
 详细使用可参考：`goOffline 完整指南`
 */
+ (void)goOffline;


/**
 手动恢复与 Wilddog Sync 云端的连接，开启自动重连。
 
 详细使用可参考：`goOnline 完整指南`
 */
+ (void)goOnline;

#pragma mark - 事务

/**
 用于多客户端并发写入操作时保证数据一致性，可以避免并发修改当前节点时的数据冲突。 
 与 `setValue:` 直接覆盖以前的数据不同，在不同客户端并发修改时，`runTransactionBlock:` 不会单纯覆盖节点数据。
 客户端提交事务至云端，如果数据已被其他客户端修改，那么云端会拒绝当前操作，并将新值返回到客户端，客户端使用新值再次运行事务处理。 
 在 `runTransactionBlock:` 的执行过程中客户端可能会重复写入直到成功，也可以在执行过程中返回 `[WDGTransactionResult abort]` 手动中止事务。
 
 详细使用可参考：`runTransactionBlock: 完整指南`

 @param block 接收当前数据，返回一个 `WDGTransactionResult` 实例。
 */
- (void)runTransactionBlock:(WDGTransactionResult* (^) (WDGMutableData *currentData))block;


/**
 用于多客户端并发写入操作时保证数据一致性，可以避免并发修改当前节点时的数据冲突。
 与 `setValue:` 直接覆盖以前的数据不同，在不同客户端并发修改时，`runTransactionBlock:` 不会单纯覆盖节点数据。
 客户端提交事务至云端，如果数据已被其他客户端修改，那么云端会拒绝当前操作，并将新值返回到客户端，客户端使用新值再次运行事务处理。
 在 `runTransactionBlock:` 的执行过程中客户端可能会重复写入直到成功，也可以在执行过程中返回 `[WDGTransactionResult abort]` 手动中止事务。

 @param block 接收当前数据，返回一个 `WDGTransactionResult` 实例。
 @param completionBlock 无论本次事务处理结果如何，当事务完成时这个 block 将被回调。
 */
- (void)runTransactionBlock:(WDGTransactionResult* (^) (WDGMutableData *currentData))block andCompletionBlock:(void (^) (NSError *__nullable error, BOOL committed, WDGDataSnapshot *__nullable snapshot))completionBlock;


/**
 用于多客户端并发写入操作时保证数据一致性，可以避免并发修改当前节点时的数据冲突。
 与 `setValue:` 直接覆盖以前的数据不同，在不同客户端并发修改时，`runTransactionBlock:` 不会单纯覆盖节点数据。
 客户端提交事务至云端，如果数据已被其他客户端修改，那么云端会拒绝当前操作，并将新值返回到客户端，客户端使用新值再次运行事务处理。
 在 `runTransactionBlock:` 的执行过程中客户端可能会重复写入直到成功，也可以在执行过程中返回 `[WDGTransactionResult abort]` 手动中止事务。
 
 @param block 接收当前数据，返回一个 `WDGTransactionResult` 实例。
 @param completionBlock 无论本次事务处理结果如何，当事务完成时这个 block 将被回调。
 @param localEvents 若当前节点已经建立了监听，每次执行 block 都会触发一次监听事件。将其设置为 NO 来阻止本地建立的监听触发中间状态的事件，只有事务操作成功时才触发监听事件。
 */
- (void)runTransactionBlock:(WDGTransactionResult* (^) (WDGMutableData *currentData))block andCompletionBlock:(nullable void (^) (NSError *__nullable error, BOOL committed, WDGDataSnapshot *__nullable snapshot))completionBlock withLocalEvents:(BOOL)localEvents;

#pragma mark - 检索字符串描述

/**
 获取当前 Wilddog Sync 节点的绝对 URL。

 @return 当前 Wilddog Sync 节点的绝对 URL。
 */
- (NSString *)description;

#pragma mark - 属性

/**
 当前节点的父节点引用。

 @warning 根节点的 parent 为 nil。
 */
@property (strong, readonly, nonatomic, nullable) WDGSyncReference *parent;


/**
 根结点的引用。
 */
@property (strong, readonly, nonatomic) WDGSyncReference *root;


/**
 当前节点的 key 值。
 */
@property (strong, readonly, nonatomic) NSString *key;


/**
 这个引用所属的指向 Wilddog 数据库节点的 URL。
 */
@property (strong, readonly, nonatomic) NSString *URL;


/**
 当前 `WDGSyncReference` 实例相关的 `WDGSync` 实例。
 */
@property (strong, readonly, nonatomic) WDGSync *sync;

@end

NS_ASSUME_NONNULL_END
