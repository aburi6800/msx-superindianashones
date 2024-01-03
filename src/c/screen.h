// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include "define.h"
#include "game.h"


// パターンネームテーブル
extern uint8_t PTN_NAME_TBL[BUFF_SIZE];


/**
 * 仮想画面バッファクリア
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void buff_clear();


/**
 * テキスト表示
 * - 構造体に指定されたテキストを仮想画面バッファのx,y座標に書き込む。
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
void buff_wrttext(uint8_t x, uint8_t y, char* text);


/**
 * BCD値表示
 * - 指定されたx,y座標に指定されたデータ(BCD値)を10進数の数値で仮想画面バッファに書き込む。
 * - 画面端のはみ出し処理はしていないので注意。
 *
 * args:
 * - x              uint8_t     出力先のX座標
 * - y              uint8_t     出力先のY座標
 * - value          unsigned char* 表示データのアドレス
 * - size           uint8_t     表示データのサイズ(byte)
 *
 * return:
 * - void
 */
void buff_wrtbcd(uint8_t x, uint8_t y, unsigned char* value, uint8_t size);


/**
 * 各種情報表示
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void display_information();


/**
 * 仮想画面逆スクロール処理
 *
 * args:
 * - void
 *
 * return:
 * - void
 */
void scroll_buff();


/**
 * 画面作成処理
 *
 * args:
 * - game_state          uint8_t     ゲーム状態
 *
 * return:
 * - void
 */
//void make_screen(uint8_t game_state);
void make_screen();

#endif
