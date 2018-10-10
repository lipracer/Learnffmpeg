//
//  ViewController.m
//  Player
//
//  Created by multimedia on 2018/10/10.
//  Copyright © 2018年 ___multiMedia___. All rights reserved.
//

#import "ViewController.h"
#import "PlayerView.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.view = [[PlayerView alloc] initWithFrame: CGRectMake(0, 0, ([UIScreen mainScreen].bounds.size.width),
                                                              ([UIScreen mainScreen].bounds.size.height))];
    
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
