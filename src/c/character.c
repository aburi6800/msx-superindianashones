// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx/gfx.h>
#include "define.h"
#include "character.h"

//#define abs(x) ((x) < 0 ? -(x) : (x))

// キャラクタ属性
character_t characters[8+1];

// スプライトアトリビュートテーブル
uint8_t SPR_ATTR_TBL[32+1][4] = {0};

// キャラクタインデックス
uint8_t chr_idx = 0;


/**
 * キャラクタ移動量設定処理
 * - 予め、x,y,target_x,target_yを設定したcharacter_t型の変数ポインタを与える
 *
 * args:
 * - character_t    対象のキャラクタデータのポインタ
 *
 * return:
 * - void
 */
void set_movevalue(character_t *character)
{
    character->dx = abs(character->target_x - character->x);
    character->dy = abs(character->target_y - character->y);
    if (character->x < character->target_x) {
        character->sx = 1 * character->speed;
    } else {
        character->sx = -1 * character->speed;
    }
    if (character->y < character->target_y) {
        character->sy = 1 * character->speed;
    } else {
        character->sy = -1 * character->speed;
    }
    character->err = character->dx - character->dy;
}


/**
 * キャラクタ移動処理
 *
 * args:
 * - character_t    対象のキャラクタデータのポインタ
 *
 * return:
 * - uint8_t        0=移動中、1=移動終了
 */
uint8_t move_character(character_t *character)
{
    // 目標座標に到達したか
    if (abs(character->x - character->target_x) < 4 && abs(character->y - character->target_y) < 4) {
        // 目標座標に到達したら移動停止する
        character->f = 0;
        character->y = 192;
        return 1;
    }

    int err2 = 2 * character->err;
    if (err2 > -character->dy) {
        character->err -= character->dy;
        character->x += character->sx;
    }
    if (err2 < character->dx) {
        character->err += character->dx;
        character->y += character->sy;
    }

    return 0;
}
