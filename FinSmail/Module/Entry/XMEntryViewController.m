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
#import "XMLoginViewController.h"
#import "XMCollectionView.h"
#import <SVProgressHUD/SVProgressHUD.h>

@import WilddogAuth;

@interface XMEntryCollectionCell : UICollectionViewCell
//@property (nonatomic, strong) UIImageView *icon;
@property (nonatomic, strong) UILabel *label;
@end
@implementation XMEntryCollectionCell
- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
//        UIImageView *iconView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"smail"]];
//        self.icon = iconView;
//        [self.contentView addSubview:iconView];
//        [iconView mas_makeConstraints:^(MASConstraintMaker *make) {
//            make.edges.mas_equalTo(UIEdgeInsetsMake(0, 0, 0, 0));
//        }];
        UILabel *label = [[UILabel alloc] init];
        label.text = @"😄";
        label.font = [UIFont systemFontOfSize:24];
        self.label = label;
        [self.contentView addSubview:label];
        [label mas_makeConstraints:^(MASConstraintMaker *make) {
            make.edges.mas_equalTo(UIEdgeInsetsMake(0, 0, 0, 0));
        }];
    }
    return self;
}

- (void)prepareForReuse {
    [super prepareForReuse];
    self.contentView.backgroundColor = [UIColor whiteColor];
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
        self.selectionStyle = UITableViewCellSelectionStyleNone;
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
    collectionView.scrollEnabled = NO;
    self.collectionView = collectionView;
    [container addSubview:collectionView];
    
    
    [container mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.mas_equalTo(UIEdgeInsetsMake(12, 16, 12, 16));
    }];
    
    labelDesc.numberOfLines = 0;
    labelDesc.text = @"no desc";
    [labelDesc setContentHuggingPriority:UILayoutPriorityRequired forAxis:UILayoutConstraintAxisVertical];
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
    [self layoutIfNeeded];
}

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    if (self.entity.countStar == 0) {
        return 1;
    }
    return ABS(self.entity.countStar);
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    XMEntryCollectionCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:NSStringFromClass([XMEntryCollectionCell class]) forIndexPath:indexPath];
    
    if (self.entity.type == XMAdminSmailRecordTypeFine) {
        cell.label.text = @"🌚";
    } else if (self.entity.type == XMAdminSmailRecordTypeBalance) {
        cell.label.text = @"💁🏻‍♂️";
    } else if (self.entity.type == XMAdminSmailRecordTypeBuy) {
        cell.label.text = @"💸";
    } else {
        cell.label.text = @"😄";
    }

    return cell;
}

- (void)prepareForReuse {
    [super prepareForReuse];
    self.entity = nil;
    self.labelDesc.text = nil;
    [self.collectionView reloadData];
    [self setNeedsLayout];
    [self.collectionView reloadData];
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
    if ([XMLoginViewController isLogin]) {
        XMAdminViewController *admin = [[XMAdminViewController alloc] initWithNibName:nil bundle:nil];
        [self.navigationController pushViewController:admin animated:YES];
    } else {
        XMLoginViewController *login = [[XMLoginViewController alloc] initWithNibName:nil bundle:nil];
        [self.navigationController pushViewController:login animated:YES];
    }
    
    
}

- (void)refresh {
    [SVProgressHUD show];
    __weak typeof(self) weakSelf = self;
    [XMAdminViewController wrapDataWithComplete:^(XMAdminSmailWrapEntity *entity, NSError *error) {
        [SVProgressHUD dismissAnimated:YES];
        if (!error) {
            weakSelf.wrapEntity = entity;
            
            // 标题更新
            weakSelf.title = [NSString stringWithFormat:@"%ld", entity.countTotal];
            
            // 数据更新
            [weakSelf.tableView reloadData];
            dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf.tableView reloadData];
            });
        } else {
            
        }
    }];
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

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    return [XMLoginViewController isLogin];
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
//        [dataArray removeObjectAtIndex:indexPath.row];
//        // Delete the row from the data source.
//        [testTableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
//        NSString *dateString = [self.wrapEntity.dateSorted objectAtIndex:indexPath.section];
//        XMAdminSmailRecordEntity *record = [[self.wrapEntity.mapDate objectForKey:dateString] objectAtIndex:indexPath.row];
//        NSMutableArray<XMAdminSmailRecordEntity *> *records = [self.wrapEntity.mapDate objectForKey:dateString];
//        [records removeObjectAtIndex:indexPath.row];
        
        
    }
}
@end
