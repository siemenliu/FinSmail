//
//  WDGSyncErrors.h
//  WilddogSync
//
//  Created by Zheng Li on 1/6/17.
//  Copyright © 2017 wilddog. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

extern NSString *const WDGSyncErrorDomain;
extern NSString *const WDGSyncErrorCodeKey;

/**
 WilddogSync 使用的错误码。
 */
typedef NS_ENUM(NSInteger, WDGSyncErrorCode){
    WDGSyncErrorOperationFailed = 26001,
    WDGSyncErrorServerBusy = 26002,
    WDGSyncErrorServiceUnavailable = 26003,

    WDGSyncErrorPermissionDenied = 26101,
    WDGSyncErrorQuotaLimitExceeded = 26102,
    WDGSyncErrorQPSSpeedLimitExceeded = 26103,
    WDGSyncErrorDataRequestedLimitExceeded = 26104,
    WDGSyncErrorLeafNodeSizeLimitExceeded = 26105,
    WDGSyncErrorInvalidParameters = 26106,

    WDGSyncErrorTransactionDataStale = 26201,
    WDGSyncErrorUserCodeException = 26202,
    WDGSyncErrorTransactionMaxRetries = 26203,
    WDGSyncErrorTransactionOverridden = 26204,
    
    WDGSyncErrorNonExistentNode = 26301,

    WDGSyncErrorUnknown = 29999
};


@interface NSError (WilddogSync)

- (WDGSyncErrorCode)WDGSync_errorCode;

@end

NS_ASSUME_NONNULL_END
