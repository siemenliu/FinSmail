//
//  XMAdminViewController.m
//  FinSmail
//
//  Created by 刘晓明 on 2017/8/12.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import "XMAdminViewController.h"
#import <Masonry/Masonry.h>


@implementation XMAdminSmailRecordEntity
- (void)encodeWithCoder:(NSCoder *)aCoder {
    [aCoder encodeObject:self.dateHappen forKey:@"dateHappen"];
    [aCoder encodeObject:self.desc forKey:@"desc"];
    [aCoder encodeObject:@(self.countStar) forKey:@"countStar"];
}
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder {
    if (self = [self init]) {
        self.dateHappen = [aDecoder decodeObjectForKey:@"dateHappen"];
        self.desc = [aDecoder decodeObjectForKey:@"desc"];
        self.countStar = [[aDecoder decodeObjectForKey:@"countStar"] integerValue];
    }
    return self;
}

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

- (void)viewDidLoad {
    [super viewDidLoad];
    
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
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent: @"dataArray.archive"];
    
    NSError *error;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL success = [fileManager removeItemAtPath:filePath error:&error];
    if (success) {
        NSLog(@"remove success");
    } else {
        NSLog(@"Could not delete file -:%@ ",[error localizedDescription]);
    }
}

- (void)handleAdd:(id)sender {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent: @"dataArray.archive"];
    
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    formatter.dateFormat = @"yyyy-MM-dd";
    NSString *dateHappen = [formatter stringFromDate:self.pickerDate.date];
    
    XMAdminSmailRecordEntity *entity = [[XMAdminSmailRecordEntity alloc] init];
    entity.dateHappen = dateHappen;
    entity.desc = self.tfDesc.text;
    entity.countStar = self.countStar?:1;
    
    NSMutableDictionary *data = [NSKeyedUnarchiver unarchiveObjectWithFile:filePath];
    if (!data) {
        data = [NSMutableDictionary dictionary];
    }
    NSMutableDictionary *mapDate = [data objectForKey:@"mapDate"];
    if (!mapDate) {
        mapDate = [NSMutableDictionary dictionary];
        [data setObject:mapDate forKey:@"mapDate"];
    }
    NSMutableArray *list = [mapDate objectForKey:entity.dateHappen];
    if (!list) {
        list = [NSMutableArray array];
        [mapDate setObject:list forKey:entity.dateHappen];
    }
    [list addObject:entity];
    
    if ([NSKeyedArchiver archiveRootObject:data toFile:filePath]) {
        NSLog(@"save success");
    } else {
        NSLog(@"save fail");
    }
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
    return [NSString stringWithFormat:@"%ld", row+1];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    self.countStar = row + 1;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

- (void)dealloc {
    NSLog(@"dd");
}

@end
