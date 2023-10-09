// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx/gfx.h>
#include "common.h"

#define BUFF_WIDTH          32
#define BUFF_HEIGHT         24
#define BUFF_SIZE           BUFF_WIDTH * BUFF_HEIGHT


// パターンネームテーブル
uint8_t PTN_NAME_TBL[BUFF_SIZE] = {0};


/**
 * 仮想画面バッファクリア
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void buff_clear()
{
    for (uint16_t i = 0; i < BUFF_SIZE; i++) {
        PTN_NAME_TBL[i] = 32;
    }
}


/**
 * テキスト表示
 * - 構造体に指定されたテキストを仮想画面のx,y座標に書き込む。
 * - 画面端のはみ出し処理はしていないので注意。
 *
 * args:
 * - x              uint8_t     更新先のX座標
 * - y              uint8_t     更新先のY座標
 * - text           char*       表示テキストのアドレス
 *
 * return:
 * - void
 */
void buff_wrttext(uint8_t x, uint8_t y, char* text)
{
    for (uint8_t i = 0; i < strlen(text); i++) {
        PTN_NAME_TBL[y*BUFF_WIDTH+x] = text[i];
        x++;
    }
}


/**
 * BCD値表示
 * - 指定されたx,y座標に指定されたデータ(BCD値)を10進数の数値で書き込む。
 * - 画面端のはみ出し処理はしていないので注意。
 *
 * args:
 * - x              uint8_t     出力先のX座標
 * - y              uint8_t     出力先のY座標
 * - value          char*       表示データのアドレス
 *
 * return:
 * - void
 */
void buff_wrtbcd(uint8_t x, uint8_t y, char* value)
{
    wrtbcdtodec(PTN_NAME_TBL, y * BUFF_WIDTH + x, value, sizeof(value));
}


/*
 * VRAM転送
 * - 仮想画面バッファをVRAMのパターンネームテーブルへ転送する。
 *
 * args:
 * - none
 *
 * return:
 * - none
 */
void write_vram()
{
    // パターンネームテーブル更新
    vwrite(PTN_NAME_TBL, 0x1800, 768);
}
