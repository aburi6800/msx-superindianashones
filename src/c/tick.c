// License:MIT License
// copyright-holders:Hitoshi Iwai

#include "tick.h"


// ゲーム経過時間
tick_t tick;


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
void count_tick()
{
    // tick1加算
    tick.tick1++;

    if (tick.tick1 % 60 == 0) {
        // tick2加算
        tick.tick2++;
    }
}


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
void reset_tick()
{
    tick.tick1 = 0;
    tick.tick2 = 0;
}
