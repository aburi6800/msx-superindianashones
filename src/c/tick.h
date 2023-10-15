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
extern tick_t tick;


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


/**
 * tick値リセット
 * - tick1,2をゼロに初期化する
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void reset_tick();

#endif
