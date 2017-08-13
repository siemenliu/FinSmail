//
//  XMEntryViewController.m
//  FinSmail
//
//  Created by 刘晓明 on 2017/8/12.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import "XMEntryViewController.h"
#import <Masonry/Masonry.h>
#import "XMAdminViewController.h"

@interface XMEntryCell : UICollectionViewCell

@end
@implementation XMEntryCell
- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        UIImageView *iconView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"smail"]];
        [self.contentView addSubview:iconView];
        [iconView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.edges.mas_equalTo(UIEdgeInsetsMake(0, 0, 0, 0));
        }];
    }
    return self;
}
@end

@interface XMEntryViewController ()
@property (nonatomic, strong) NSDictionary<NSDate *, NSArray<XMAdminSmailRecordEntity *> *> *dataSource;
@property (nonatomic, strong) NSArray<NSDate *> *dateSorted;
@end

@implementation XMEntryViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.collectionView registerClass:[XMEntryCell class] forCellWithReuseIdentifier:NSStringFromClass([XMEntryCell class])];
    [self.collectionView registerClass:[UICollectionReusableView class] forSupplementaryViewOfKind:UICollectionElementKindSectionHeader withReuseIdentifier:NSStringFromClass([UICollectionReusableView class])];
    
    UICollectionViewFlowLayout *layout = (UICollectionViewFlowLayout *)self.collectionView.collectionViewLayout;
    layout.minimumLineSpacing = 4;
    layout.minimumInteritemSpacing = 4;
    layout.itemSize = [self itemSize];
    layout.headerReferenceSize = CGSizeMake(self.view.frame.size.width, 44);
    
    UIBarButtonItem *itemAdmin = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAction target:self action:@selector(handleAction:)];
    
    UIBarButtonItem *itemRefresh = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRedo target:self action:@selector(refresh)];
    
    NSMutableArray *arrItem = [self.navigationItem.rightBarButtonItems mutableCopy];
    if (!arrItem) {
        arrItem = [NSMutableArray array];
    }
    [arrItem addObject:itemAdmin];
    [arrItem addObject:itemRefresh];
    [self.navigationItem setRightBarButtonItems:arrItem animated:YES];
    
    [self refresh];
}

- (void)refresh {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent: @"dataArray.archive"];
    NSDictionary *data = [NSKeyedUnarchiver unarchiveObjectWithFile:filePath];
    self.dataSource = [data objectForKey:@"mapDate"];
    
    self.dateSorted = [[self.dataSource allKeys] sortedArrayUsingComparator:^NSComparisonResult(NSString * _Nonnull obj1, NSString *  _Nonnull obj2) {
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        formatter.dateFormat = @"yyyy-MM-dd";
        NSDate *date1 = [formatter dateFromString:obj1];
        NSDate *date2 = [formatter dateFromString:obj2];
        return [date1 compare:date2];
    }];
    
    [self.collectionView reloadData];
}

- (void)handleAction:(id)sender {
    XMAdminViewController *adminVC = [[XMAdminViewController alloc] initWithNibName:nil bundle:nil];
    [self.navigationController pushViewController:adminVC animated:YES];
}

- (CGSize)itemSize {
    NSInteger count = 5;
    CGFloat width = self.view.frame.size.width;
    CGFloat widthRemoveSpace = width - (4 * (count + 1));
    CGFloat widthFinal = widthRemoveSpace / count;
    return CGSizeMake(widthFinal, widthFinal);
}

#pragma mark - datasource and delegate

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return self.dateSorted.count;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    NSDate *dateKey = [self.dateSorted objectAtIndex:section];
    NSArray<XMAdminSmailRecordEntity *> *listEntity = [self.dataSource objectForKey:dateKey];
    
    return listEntity.count;
}

- (UICollectionReusableView *)collectionView:(UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath {
    
    NSDate *date = [self.dateSorted objectAtIndex:indexPath.section];
    
    UICollectionReusableView *headerView = [collectionView dequeueReusableSupplementaryViewOfKind:UICollectionElementKindSectionHeader withReuseIdentifier:NSStringFromClass([UICollectionReusableView class]) forIndexPath:indexPath];
    
    UILabel *label = [headerView viewWithTag:'t'];
    if (!label) {
        UILabel *label = [[UILabel alloc] init];
        label.tag = 't';
        label.text = [date description];
        [headerView addSubview:label];
        [label mas_makeConstraints:^(MASConstraintMaker *make) {
            make.centerY.mas_equalTo(headerView);
            make.left.mas_equalTo(16);
            make.right.mas_equalTo(-16);
        }];
    }
    label.text = [date description];
    
    return headerView;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    XMEntryCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:NSStringFromClass([XMEntryCell class]) forIndexPath:indexPath];
    
    
    
    return cell;
}
    
@end
