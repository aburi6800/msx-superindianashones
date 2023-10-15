// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx/gfx.h>
#include "control.h"


// 入力バッファ（方向）
uint8_t STICK_BUFF = 0;

// 入力バッファ（トリガ）
uint8_t STRIG_BUFF = 0;

// 移動量テーブル
int8_t vector_x[9] = {  0,  0,  1,  1,  1,  0, -1, -1, -1};
int8_t vector_y[9] = {  0, -1, -1,  0,  1,  1,  1,  0, -1};


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
void get_controls()
{
    // 入力処理
    STICK_BUFF = get_stick(0) + get_stick(1);
    STRIG_BUFF = get_trigger(0) + get_trigger(1);
}
