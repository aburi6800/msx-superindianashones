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
    SIGHT,
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

    // キャラクタ座標は整数部＋小数部1桁の値を10倍して整数として持つ
    // 実際に表示を行う際は、1/10した値（小数部切捨て）を使用する

    // キャラクタ座標
    uint16_t x;
    uint16_t y;

    // キャラクタ移動先座標
    uint16_t target_x;
    uint16_t target_y;

    // キャラクタ座標移動量
    uint16_t vx;
    uint16_t vy;

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


/**
 * キャラクタ移動量設定処理
 * - 予め、x,y,target_x,target_yを設定したcharacter_t型の変数ポインタを与える
 *
 * args:
 * - character      character_t 対象のキャラクタデータ
 *
 * return:
 * - void
 */
void set_movevalue(character_t character);


/**
 * キャラクタ移動処理
 *
 * args:
 * - character      character_t 対象のキャラクタデータ
 *
 * return:
 * - void
 */
void move_character(character_t character);

#endif
