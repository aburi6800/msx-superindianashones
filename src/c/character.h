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

#endif
