//
//  XMAdminViewController.h
//  FinSmail
//
//  Created by 刘晓明 on 2017/8/12.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, XMAdminSmailRecordType) {
    XMAdminSmailRecordTypeFine, // 罚款
    XMAdminSmailRecordTypeBuy, // 消费
    XMAdminSmailRecordTypeReward, // 酬劳
    XMAdminSmailRecordTypeBalance // 平衡
};

@interface XMAdminSmailRecordEntity : NSObject
@property (nonatomic, copy) NSString *dateHappen;
@property (nonatomic, copy) NSString *desc;
@property (nonatomic, assign) NSInteger countStar;
@property (nonatomic, assign) XMAdminSmailRecordType type;
- (NSDate *)dateFromDateHappen;
@end

@interface XMAdminSmailWrapEntity : NSObject
@property (nonatomic, assign) NSInteger countTotal;
@property (nonatomic, copy) NSDictionary<NSString *, NSArray<XMAdminSmailRecordEntity *> *> *mapDate;
@property (nonatomic, copy) NSArray<NSString *> *dateSorted;
@end

@interface XMAdminViewController : UIViewController
+ (NSString *)filePathUser;
+ (void)wrapDataWithComplete:(void (^)(XMAdminSmailWrapEntity *entity, NSError *error))complete;
@end
