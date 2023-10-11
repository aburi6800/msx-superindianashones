// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>


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
void poling_controls();


/**
 * スティック入力のバッファを取得
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
uint8_t get_stick_buff();


/**
 * トリガ入力のバッファを取得
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
uint8_t get_strig_buff();

#endif
