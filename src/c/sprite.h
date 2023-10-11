// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include "character.h"


/**
 * スプライトアトリビュート更新
 * - スプライトアトリビュートを更新する
 *
 * args:
 * - uint8_t        chr_idx     対象のキャラクタデータインデックス
 * - uint8_t        attr_idx    対象のアトリビュートデータインデックス
 * - bool           isPlayer    プレイヤーの場合はtrue
 *
 * return:
 * - void
 */
//void update_sprite_attr(uint8_t attr_idx, uint8_t chr_idx, bool isPlayer);
void update_sprite_attr(character_t character, uint8_t attr_idx, bool isPlayer);


/*
 * スプライトアトリビュートテーブルをVRAMへ転送
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void transfer_sprite_attr_tbl();


/*
 * スプライトパターンテーブルをVRAMへ転送
 *
 * args:
 * - chr_cnt        uint8_t     キャラクター数
 *
 * return:
 * - void
 */
void transfer_sprite_ptn_tbl(uint8_t chr_cnt);

#endif
