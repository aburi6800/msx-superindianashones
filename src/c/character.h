// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

// キャラクタ属性
typedef struct {
    // キャラクタX座標
    uint16_t x;

    // キャラクタY座標
    uint16_t y;

    // キャラクタパターン
    uint8_t p;

    // キャラクタアニメーションカウンタ
    uint8_t r;

    // キャラクタカラー
    uint8_t c[2];

    // キャラクタ状態
    uint8_t f;

    // キャラクタ番号
    uint8_t chr_num;
} character_t;


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
void vwrite_sprite_attr_tbl();

#endif
