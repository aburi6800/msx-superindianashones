// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>

// 入力方向定数
#define STICK_UP            1
#define STICK_UPRIGHT       2
#define STICK_RIGHT         3
#define STICK_DOWNRIGHT     4
#define STICK_DOWN          5
#define STICK_DOWNLEFT      6
#define STICK_LEFT          7
#define STICK_UPLEFT        8


// 入力バッファ（方向）
extern uint8_t STICK_BUFF;

// 入力バッファ（トリガ）
extern uint8_t STRIG_BUFF;

// 移動量テーブル
extern int8_t vector_x[9];
extern int8_t vector_y[9];


/**
 * スティック/トリガ入力をバッファに設定
 * - H.TIMI割り込みから呼ばれる
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void get_controls();

#endif
