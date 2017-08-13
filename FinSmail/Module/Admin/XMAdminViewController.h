//
//  XMAdminViewController.h
//  FinSmail
//
//  Created by 刘晓明 on 2017/8/12.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface XMAdminSmailRecordEntity : NSObject <NSCoding>
@property (nonatomic, copy) NSString *dateHappen;
@property (nonatomic, copy) NSString *desc;
@property (nonatomic, assign) NSInteger countStar;
- (NSDate *)dateFromDateHappen;
@end

@interface XMAdminViewController : UIViewController

@end
