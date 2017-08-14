//
//  XMCollectionView.m
//  FinSmail
//
//  Created by 刘晓明 on 2017/8/14.
//  Copyright © 2017年 xiaoming. All rights reserved.
//

#import "XMCollectionView.h"
#import <Masonry/Masonry.h>

@implementation XMCollectionView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (void)layoutSubviews {
    [super layoutSubviews];
    
    [self mas_updateConstraints:^(MASConstraintMaker *make) {
        make.height.mas_equalTo(self.contentSize.height);
    }];
}

@end
