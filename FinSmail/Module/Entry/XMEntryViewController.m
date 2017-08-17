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
#import "XMCollectionView.h"

@interface XMEntryCollectionCell : UICollectionViewCell

@end
@implementation XMEntryCollectionCell
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

@interface XMEntryCell : UITableViewCell <UICollectionViewDelegate, UICollectionViewDataSource>
@property (nonatomic, strong) XMAdminSmailRecordEntity *entity;

@property (nonatomic, strong) UILabel *labelDesc;
@property (nonatomic, strong) XMCollectionView *collectionView;
@end

@implementation XMEntryCell

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        [self setupView];
    }
    return self;
}

- (void)setupView {
    UIView *container = [[UIView alloc] init];
    [self.contentView addSubview:container];
    
    UILabel *labelDesc = [[UILabel alloc] init];
    self.labelDesc = labelDesc;
    [container addSubview:labelDesc];
    
    UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
    layout.minimumLineSpacing = 4;
    layout.minimumInteritemSpacing = 4;
    layout.itemSize = [self itemSize];
    XMCollectionView *collectionView = [[XMCollectionView alloc] initWithFrame:self.contentView.bounds collectionViewLayout:layout];
    self.collectionView = collectionView;
    [container addSubview:collectionView];
    [container mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.mas_equalTo(UIEdgeInsetsMake(12, 16, 12, 16));
    }];
    
    labelDesc.numberOfLines = 0;
    labelDesc.text = @"no desc";
    [labelDesc mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.right.mas_equalTo(0);
    }];
    
    [collectionView registerClass:[XMEntryCollectionCell class] forCellWithReuseIdentifier:NSStringFromClass([XMEntryCollectionCell class])];
    collectionView.delegate = self;
    collectionView.dataSource = self;
    collectionView.backgroundColor = [UIColor whiteColor];
    [collectionView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(labelDesc.mas_bottom).offset(12);
        make.left.right.mas_equalTo(0);
        make.bottom.mas_equalTo(0);
        make.height.mas_greaterThanOrEqualTo(44);
    }];
}

- (CGSize)itemSize {
    NSInteger count = 10;
    CGFloat width = self.contentView.frame.size.width;
    CGFloat widthRemoveSpace = width - (4 * (count + 1));
    CGFloat widthFinal = widthRemoveSpace / count;
    return CGSizeMake(widthFinal, widthFinal);
}

- (void)setEntity:(XMAdminSmailRecordEntity *)entity {
    _entity = entity;
    
    self.labelDesc.text = entity.desc;
    [self.collectionView reloadData];
}

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return self.entity.countStar;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    XMEntryCollectionCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:NSStringFromClass([XMEntryCollectionCell class]) forIndexPath:indexPath];

    return cell;
}
@end

@interface XMEntryViewController()
@property (nonatomic, strong) XMAdminSmailWrapEntity *wrapEntity;
@end

@implementation XMEntryViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 视图基础
    self.view.backgroundColor = [UIColor whiteColor];
    
    // 右上角action按钮
    UIBarButtonItem *itemAdmin = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAction target:self action:@selector(handleAction:)];
    
    UIBarButtonItem *itemRefresh = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRedo target:self action:@selector(refresh)];
    
    NSMutableArray *arrItem = [self.navigationItem.rightBarButtonItems mutableCopy];
    if (!arrItem) {
        arrItem = [NSMutableArray array];
    }
    [arrItem addObject:itemAdmin];
    [arrItem addObject:itemRefresh];
    [self.navigationItem setRightBarButtonItems:arrItem animated:YES];
    
    // tableview配置
    [self.tableView registerClass:[XMEntryCell class] forCellReuseIdentifier:NSStringFromClass([XMEntryCell class])];
    
    [self refresh];
}

- (void)handleAction:(id)sender {
    XMAdminViewController *admin = [[XMAdminViewController alloc] initWithNibName:nil bundle:nil];
    [self.navigationController pushViewController:admin animated:YES];
}

- (void)refresh {
    XMAdminSmailWrapEntity *wrapEntity = [XMAdminViewController wrapData];
    self.wrapEntity = wrapEntity;
    
    // 标题更新
    self.title = [NSString stringWithFormat:@"%ld", wrapEntity.countTotal];
    
    // 数据更新
    [self.tableView reloadData];
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadData];
    });
}

#pragma mark - delegate and datasource

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return self.wrapEntity.dateSorted.count;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    NSString *dateString = [self.wrapEntity.dateSorted objectAtIndex:section];
    NSArray <XMAdminSmailRecordEntity *> *entityList = [self.wrapEntity.mapDate objectForKey:dateString];
    return entityList.count;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    return [self.wrapEntity.dateSorted objectAtIndex:section];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    XMEntryCell *cell = [tableView dequeueReusableCellWithIdentifier:NSStringFromClass([XMEntryCell class]) forIndexPath:indexPath];
    NSString *dateString = [self.wrapEntity.dateSorted objectAtIndex:indexPath.section];
    NSArray <XMAdminSmailRecordEntity *> *entityList = [self.wrapEntity.mapDate objectForKey:dateString];
    XMAdminSmailRecordEntity *entity = [entityList objectAtIndex:indexPath.row];
    cell.entity = entity;
    return cell;
}
@end
