//
//  GradientLayer.h
//  SapusTongue
//
//  Created by Ricardo Quesada on 09/10/08.
//  Copyright 2008 Sapus Media. All rights reserved.
//


#import "cocos2d.h"

@interface GradientLayer : CCColorLayer {
}

-(void) setBottomColor: (unsigned int) cb topColor:(unsigned int) ct;

@end
