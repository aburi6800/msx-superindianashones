// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx/gfx.h>
#include "control.h"


// 入力バッファ（方向）
uint8_t STICK_BUFF = 0;

// 入力バッファ（トリガ）
uint8_t STRIG_BUFF = 0;


/**
 * 入力をバッファに設定
 *
 * args:
 * - distaddr       char        書き込み先アドレス
 * - distoffset     uint16_t     書き込み先オフセット値
 * - dataaddr       char        表示データアドレス
 * - databytes      uint8_t     表示データバイト数
 *
 * return:
 * - void
 */
void poling_controls()
{
    // 入力処理
    STICK_BUFF = get_stick(0) + get_stick(1);
    STRIG_BUFF = get_trigger(0) + get_trigger(1);
}

uint8_t get_stick_buff()
{
    return STICK_BUFF;
}

uint8_t get_strig_buff()
{
    return STRIG_BUFF;
}
