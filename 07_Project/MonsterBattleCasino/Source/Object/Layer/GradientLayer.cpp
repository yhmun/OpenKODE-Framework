//
//  GradientLayer.m
//  SapusTongue
//
//  Created by Ricardo Quesada on 09/10/08.
//  Copyright 2008 Sapus Media. All rights reserved.
//

#import "GradientLayer.h"

// Simple Layer that lets you draw a gradient layer.
// The sky is drawn using this Layer.
// TIP:
//   . OpenGL Quads are very efficient.
//   . In this case, the whole sky is drawn with only one cheap OpenGL call without using Texture Memory
//   . Instead of having a big image of a solid or gradient color, you can use a Quad

@implementation GradientLayer

-(void) setBottomColor: (unsigned int) colorb topColor:(unsigned int) colort {

	GLubyte r1, g1, b1, a1;
	GLubyte rb, gb, bb, ab;
	GLubyte rt, gt, bt, at;

	rb = (colorb>>24) & 0xff;
	gb = (colorb>>16) & 0xff;
	bb = (colorb>>8) & 0xff;
	ab = (colorb) & 0xff;

	rt = (colort>>24) & 0xff;
	gt = (colort>>16) & 0xff;
	bt = (colort>>8) & 0xff;
	at = (colort) & 0xff;
	
	for(unsigned int i = 0; i < sizeof(squareColors) / sizeof( squareColors[0] ); i++)
	{
		if(i < 8 ) {
			r1 = rb;
			g1 = gb;
			b1 = bb;
			a1 = ab;
		} else {
			r1 = rt;
			g1 = gt;
			b1 = bt;
			a1 = at;
		}

		if( i % 4 == 0 )
			squareColors[i] = r1;
		else if( i % 4 == 1)
			squareColors[i] = g1;
		else if( i % 4 ==2  )
			squareColors[i] = b1;
		else
			squareColors[i] = a1;
	}	
}

@end
