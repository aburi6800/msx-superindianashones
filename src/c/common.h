// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

/**
 * BCD加算
 * - 対象データに対し引数で与えた16ビットのBCD値を加算する。
 *
 * args:
 * - addvalue       uint16_t    加算値
 * - distaddr       char        加算対象データアドレス
 * - distbytes      uint8_t     加算対象データバイト数
 *
 * return:
 * - void
 */
void add_bcd(uint16_t addvalue, unsigned char* distaddr, uint8_t distbytes);


/**
 * BCD→10進変換転送
 *
 * args:
 * - distaddr       char        書き込み先アドレス
 * - distoffset     uint16_t    書き込み先オフセット値
 * - dataaddr       char        表示データアドレス
 * - databytes      uint8_t     表示データバイト数
 *
 * return:
 * - void
 */
void wrtbcdtodec(unsigned char* distaddr, uint16_t distoffset, unsigned char* dataaddr, uint8_t databytes);

#endif
