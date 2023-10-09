// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>


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
 * - value          char*       表示データのアドレス
 *
 * return:
 * - void
 */
void buff_wrtbcd(uint8_t x, uint8_t y, char* value);


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
void write_vram();

