/* --------------------------------------------------------------------------
 *
 *      File            Config.h
 *      Created By      Project B team
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2013 XMsoft. All rights reserved.
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __Config_h__
#define __Config_h__

#define BOARD_WIDTH			350	// 가로
#define BOARD_HEIGHT		655	// 세로
#define BOARD_COLUMN_CNT	10	// 블럭이 들어갈 수 있는 행 갯수
#define BOARD_ROW_CNT		18	// 블럭이 들어갈 수 있는 열 갯수

#define BLOCK_MAX_CNT		5	// 최대 블럭 수
#define BLOCK_MATRIX_CNT	4	// 블럭 행 또는 열의 최대 갯수
#define BLOCK_SIZE			BOARD_WIDTH / BOARD_COLUMN_CNT // 블럭 크기

// 블럭타입
typedef enum
{
	BLOCK_TYPE_I,
	BLOCK_TYPE_O,
	BLOCK_TYPE_S,
	BLOCK_TYPE_Z,
	BLOCK_TYPE_L,
	BLOCK_TYPE_J,
	BLOCK_TYPE_T,
	BLOCK_TYPE_P,
	BLOCK_TYPE_Q,
	BLOCK_TYPE_U,
	BLOCK_TYPE_COUNT
} BlockType;

// 블럭구조
#define BLOCK_ARRAY_I	\
{						\
	{ 1 },				\
	{ 1 },				\
	{ 1 },				\
	{ 1 },				\
};

#define BLOCK_ARRAY_O	\
{						\
	{ 1, 1 },			\
	{ 1, 1 },			\
};

#define BLOCK_ARRAY_S	\
{						\
	{ 0, 1, 1 },		\
	{ 1, 1, 0 },		\
};

#define BLOCK_ARRAY_Z	\
{						\
	{ 1, 1, 0 },		\
	{ 0, 1, 1 },		\
};

#define BLOCK_ARRAY_L	\
{						\
	{ 1, 0 },			\
	{ 1, 0 },			\
	{ 1, 1 },			\
};

#define BLOCK_ARRAY_J	\
{						\
	{ 0, 1 },			\
	{ 0, 1 },			\
	{ 1, 1 },			\
};

#define BLOCK_ARRAY_T	\
{						\
	{ 1, 1, 1 },		\
	{ 0, 1, 0 },		\
};

#define BLOCK_ARRAY_P	\
{						\
	{ 1, 1 },			\
	{ 1, 1 },			\
	{ 1, 0 },			\
};

#define BLOCK_ARRAY_Q	\
{						\
	{ 1, 1 },			\
	{ 1, 1 },			\
	{ 0, 1 },			\
};

#define BLOCK_ARRAY_U	\
{						\
	{ 1, 0, 1 },		\
	{ 1, 1, 1 },		\
};

// 터치대상
typedef enum
{
	TOUCH_NONE,
	TOUCH_LEFT,
	TOUCH_RIGHT,
	TOUCH_SHOOT,
	TOUCH_ROTATE
} TouchTarget;

#endif  // __Config_h__