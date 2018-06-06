/*
 * YK.h
 *
 *  Created on: 2017年5月11日
 *      Author: pyt64
 */

#ifndef YK_H_
#define YK_H_

#include "include.h"
#include "includecpp.h"
#include "complier.h"
#include "const.h"
#include "debug.h"
#include "struct.h"
#include "global.h"

//extern "C"{
#include "commonTool.h"
//}
//根据4个字节，计算长度
int getLengthOfData(__uint8_t b1, __uint8_t b2, __uint8_t b3, __uint8_t b4);
//遥控线程主循环
//void* main_loop_of_YK(void *arg);
int resolveZl(void);

#endif /* YK_H_ */
