// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>


// キャラクターロジックの関数ポインタ宣言
//typedef void (*character_module)(character_t character);


// キャラクタ種類ENUM
typedef enum {
    NONE,
    PLAYER,
    KNIFE,
    BAT,
    SKELTON
} character_type_t;


// キャラクタ属性
typedef struct {
    // キャラクタ種類
    character_type_t type;

    // キャラクタ状態
    uint8_t f;

    // スプライトアトリビュート番号
    uint8_t attr_no;

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

    // 処理モジュールのポインタ
//    character_module module;
} character_t;
extern character_t characters[8];


// スプライトアトリビュートテーブル
extern uint8_t SPR_ATTR_TBL[32][4];

#endif
