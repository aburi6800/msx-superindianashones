// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef TICK_H
#define TICK_H

#include <stdint.h>


// ゲーム経過時間
typedef struct {
    uint16_t tick1;
    uint16_t tick2;
} tick_t;


/**
 * tick値加算
 * - tick1は毎フレーム加算する
 * - tick2は60フレームごとに加算する（＝秒カウント）
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void count_tick();

#endif