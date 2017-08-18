//
//  XMAdminViewController.m
//  FinSmail
//
//  Created by 刘晓明 on 2017/8/12.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import "XMAdminViewController.h"
#import <Masonry/Masonry.h>
#import <YYModel/YYModel.h>

@import Wilddog;

@implementation XMAdminSmailWrapEntity
- (BOOL)modelCustomTransformFromDictionary:(NSDictionary *)dic {
    
    // ===== 实例化mapDate
    NSDictionary *mapDate = self.mapDate;
    NSArray<NSString *> *dateList = [mapDate allKeys];
    NSMutableDictionary *dicResult = [NSMutableDictionary dictionary];
    for (NSString *date in dateList) {
        NSArray<NSDictionary *> *recordList = [mapDate objectForKey:date];
        NSMutableArray<XMAdminSmailRecordEntity *> *recordListReplaced = [NSMutableArray arrayWithCapacity:recordList.count];
        for (NSDictionary *itemDic in recordList) {
            XMAdminSmailRecordEntity *record = [XMAdminSmailRecordEntity yy_modelWithJSON:itemDic];
            [recordListReplaced addObject:record];
        }
        
        [dicResult setObject:recordListReplaced forKey:date];
    }
    
    _mapDate = dicResult;
    
    // ===== dateSorted
    
    
    
    return YES;
}
@end

@implementation XMAdminSmailRecordEntity
- (NSDate *)dateFromDateHappen {
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    formatter.dateFormat = @"yyyy-MM-dd";
    NSDate *date = [formatter dateFromString:self.dateHappen];
    return date;
}
@end

@interface XMAdminViewController () <UIPickerViewDataSource, UIPickerViewDelegate, UITextFieldDelegate>
@property (nonatomic, strong) UIDatePicker *pickerDate;
@property (nonatomic, strong) UITextField *tfDesc;
@property (nonatomic, assign) NSInteger countStar;
@end

@implementation XMAdminViewController

- (void)authCheck {
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"输入密码" message:nil preferredStyle:UIAlertControllerStyleAlert];
    [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"密码";
        textField.secureTextEntry = YES;
    }];
    UIAlertAction *actionOk = [UIAlertAction actionWithTitle:@"Ok" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        if (alert.textFields.firstObject) {
            UITextField *tf = alert.textFields.firstObject;
            if (![tf.text isEqualToString:@"helloxiaoming"]) {
                
                [self.navigationController popViewControllerAnimated:YES];
            }
            [alert dismissViewControllerAnimated:YES completion:nil];
        }
    }];
    [alert addAction:actionOk];
    [self.navigationController presentViewController:alert animated:YES completion:nil];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self authCheck];
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    UIBarButtonItem *btnCleanAll = [[UIBarButtonItem alloc] initWithTitle:@"clean" style:UIBarButtonItemStylePlain target:self action:@selector(cleanAllArchive)];
    self.navigationItem.rightBarButtonItem = btnCleanAll;
    
    UIScrollView *scrollView = [[UIScrollView alloc] init];
    [self.view addSubview:scrollView];
    
    UIView *container = [[UIView alloc] init];
    [scrollView addSubview:container];
    
    [scrollView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.top.right.bottom.mas_equalTo(0);
    }];
    
    [container mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.mas_equalTo(UIEdgeInsetsMake(0, 0, 0, 0));
        make.width.equalTo(scrollView);
    }];
    
    // 内容
    
    /// 日期
    UIDatePicker *picker = [[UIDatePicker alloc] init];
    self.pickerDate = picker;
    [container addSubview:picker];
    
    /// 描述
    UITextField *tfDesc = [[UITextField alloc] init];
    self.tfDesc = tfDesc;
    [container addSubview:tfDesc];
    
    /// 星星变量
    UIPickerView *pickerView = [[UIPickerView alloc] init];
    [container addSubview:pickerView];
    
    /// 添加
    UIButton *btnAdd = [UIButton buttonWithType:UIButtonTypeContactAdd];
    [container addSubview:btnAdd];
    
    
    [picker setDatePickerMode:UIDatePickerModeDate];
    [picker mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.right.left.mas_equalTo(0);
    }];
    
    tfDesc.placeholder = @"desc";
    tfDesc.layer.borderColor = [UIColor grayColor].CGColor;
    tfDesc.layer.borderWidth = 1.f / [UIScreen mainScreen].scale;
    tfDesc.delegate = self;
    [tfDesc mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(picker);
        make.top.mas_equalTo(picker.mas_bottom).offset(12);
    }];
    
    pickerView.dataSource = self;
    pickerView.delegate = self;
    [pickerView selectRow:0 inComponent:0 animated:NO];
    [pickerView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(tfDesc.mas_bottom).offset(12);
        make.right.left.mas_equalTo(tfDesc);
    }];
    
    btnAdd.layer.borderColor = [UIColor grayColor].CGColor;
    btnAdd.layer.borderWidth = 1.f / [UIScreen mainScreen].scale;
    [btnAdd addTarget:self action:@selector(handleAdd:) forControlEvents:UIControlEventTouchUpInside];
    [btnAdd mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(pickerView);
        make.top.mas_equalTo(pickerView.mas_bottom).offset(12);
        make.bottom.mas_lessThanOrEqualTo(16);
        make.height.mas_equalTo(44);
    }];
}

- (void)cleanAllArchive {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent: @"data.json"];
    
    NSError *error;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL success = [fileManager removeItemAtPath:filePath error:&error];
    if (success) {
        NSLog(@"remove success");
    } else {
        NSLog(@"Could not delete file -:%@ ",[error localizedDescription]);
    }
}

+ (NSString *)filePath {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent: @"data.json"];
    return filePath;
}

+ (void)wrapDataWithComplete:(void (^)(XMAdminSmailWrapEntity *entity, NSError *error))complete {
    WDGOptions *option = [[WDGOptions alloc] initWithSyncURL:@"https://wd9641493984khzlrv.wilddogio.com"];
    [WDGApp configureWithOptions:option];
    //获取一个指向根节点的 WDGSyncReference 实例
    WDGSyncReference *ref = [[WDGSync sync] referenceWithPath:@"/"];
    [ref observeSingleEventOfType:WDGDataEventTypeValue withBlock:^(WDGDataSnapshot *snapshot) {
        NSLog(@"%@", snapshot.value);
        NSDictionary *result = snapshot.value;
        XMAdminSmailWrapEntity *entity = [XMAdminSmailWrapEntity yy_modelWithJSON:result];
        complete(entity, nil);
    } withCancelBlock:^(NSError *error) {
        NSLog(@"%@", error.description);
        complete(nil, error);
    }];
//
//    NSString *filePath = [XMAdminViewController filePath];
//    NSData *data = [NSData dataWithContentsOfFile:filePath];
//
//    if (!data) {
//        return nil;
//    }
//
//    NSError *error;
//    NSDictionary *dataDic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&error];
//    XMAdminSmailWrapEntity *wrapEntity = nil;
//    if (!error) {
//        wrapEntity = [XMAdminSmailWrapEntity yy_modelWithJSON:dataDic];
//    } else {
//        NSLog(@"%@", error.localizedDescription);
//    }
//
//    return wrapEntity;
}

- (void)handleAdd:(id)sender {
    
    
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    formatter.dateFormat = @"yyyy-MM-dd";
    NSString *dateHappen = [formatter stringFromDate:self.pickerDate.date];
    
    NSDictionary *entityDic = @{
                                @"dateHappen": dateHappen,
                                @"desc": self.tfDesc.text?:@"No Description",
                                @"countStar": @(self.countStar?:0)
                                };
//    XMAdminSmailRecordEntity *entity = [[XMAdminSmailRecordEntity alloc] init];
//    entity.dateHappen = dateHappen;
//    entity.desc = self.tfDesc.text;
//    entity.countStar = self.countStar?:0;
    
    [self.class wrapDataWithComplete:^(XMAdminSmailWrapEntity *entity, NSError *error) {
        NSMutableDictionary *data = [[entity yy_modelToJSONObject] mutableCopy];
        if (!data) {
            data = [NSMutableDictionary dictionary];
        }
        NSMutableDictionary *mapDate = [[data objectForKey:@"mapDate"] mutableCopy];
        if (!mapDate) {
            mapDate = [NSMutableDictionary dictionary];
        }
        [data setObject:mapDate forKey:@"mapDate"];
        NSMutableArray *list = [[mapDate objectForKey:dateHappen] mutableCopy];
        if (!list) {
            list = [NSMutableArray array];
        }
        [list addObject:entityDic];
        [mapDate setObject:list forKey:dateHappen];
        
        // 计算总值
        NSInteger countStarTotal = 0;
        NSArray<NSString *> *dateStringList = [mapDate allKeys];
        for (NSString *dateString in dateStringList) {
            NSArray<NSDictionary *> *entityList = [mapDate objectForKey:dateString];
            for (NSDictionary *entity in entityList) {
                countStarTotal = countStarTotal + [entity[@"countStar"] integerValue];
            }
        }
        [data setObject:@(countStarTotal) forKey:@"countTotal"];
        
        // 排序
        NSArray<NSString *> *dataSorted = [[mapDate allKeys] sortedArrayUsingComparator:^NSComparisonResult(NSString * _Nonnull obj1, NSString *  _Nonnull obj2) {
            NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
            formatter.dateFormat = @"yyyy-MM-dd";
            NSDate *date1 = [formatter dateFromString:obj1];
            NSDate *date2 = [formatter dateFromString:obj2];
            return [date1 compare:date2];
        }];
        [data setObject:dataSorted forKey:@"dateSorted"];
        
        if (!error) {
            //初始化 WDGApp
            WDGOptions *option = [[WDGOptions alloc] initWithSyncURL:@"https://wd9641493984khzlrv.wilddogio.com"];
            [WDGApp configureWithOptions:option];
            //获取一个指向根节点的 WDGSyncReference 实例
            WDGSyncReference *ref = [[WDGSync sync] reference];
            [ref setValue:data withCompletionBlock:^(NSError * _Nullable error, WDGSyncReference * _Nonnull ref) {
                if (error) {
                    [self alert:error.localizedDescription];
                } else {
                    [self alert:@"success save to cloud"];
                }
            }];
        }
        
        if (error) {
            [self alert:error.localizedDescription];
        }
    }];
    
    
}

- (void)alert:(NSString *)message {
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:nil message:nil preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *actionOk = [UIAlertAction actionWithTitle:@"Ok" style:UIAlertActionStyleCancel handler:nil];
    [alert addAction:actionOk];
    
    alert.title = message;
    
    [self.navigationController presentViewController:alert animated:YES completion:nil];
}

// returns the number of 'columns' to display.
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

// returns the # of rows in each component..
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    return 100;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    return [NSString stringWithFormat:@"%ld", row-49];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    self.countStar = row-49;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

- (void)dealloc {
    NSLog(@"dd");
}

@end
