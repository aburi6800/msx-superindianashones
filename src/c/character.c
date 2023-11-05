// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx/gfx.h>
#include "define.h"
#include "character.h"


// キャラクタ属性
character_t characters[8];

// キャラクタインデックス
uint8_t chr_idx = 0;

// スプライトアトリビュートテーブル
uint8_t SPR_ATTR_TBL[32][4] = {0};


/**
 * キャラクタ移動量設定処理
 * - 予め、x,y,target_x,target_yを設定したcharacter_t型の変数ポインタを与える
 *
 * args:
 * - character_t    対象のキャラクタデータ
 *
 * return:
 * - void
 */
void set_movevalue(character_t character)
{
    // 目標座標と現在の座標の差分をそれぞれ求める
    int16_t ax = character.target_x - character.x;
    int16_t ay = character.target_y - character.y;

    // 計算用座標を現在の座標で初期化する
    character.cx = (float)character.x;
    character.cy = (float)character.y;

    // ax+ay=0なら、x,yの移動量をそれぞれ1として終了する
    if (ax+ay == 0) {
        character.vx = 1;
        character.vy = 1;
        return;
    }

    // 係数を求める
    uint8_t z;
    if (abs(ax) > abs(ay)) {
        z = abs(ax);
    } else {
        z = abs(ay);
    }

    // x,yの移動量を求める
    character.vx = (ax / z);
    character.vy = (ay / z);
}


/**
 * キャラクタ移動処理
 *
 * args:
 * - character_t    対象のキャラクタデータ
 *
 * return:
 * - uint8_t        0=移動中、1=移動終了
 */
uint8_t move_character(character_t character)
{
    // 計算用座標に対して計算
    character.cx += (character.vx * character.speed);
    character.cy += (character.vy * character.speed);

    // 表示用座標を設定
    character.x = (uint8_t)character.cx;
    character.y = (uint8_t)character.cy;

    // 目標座標に到達したか
    if (character.x == character.target_x || character.y == character.target_y) {
        // 目標座標に到達したら移動停止する
        character.f = 0;
        return 1;
    }

    return 0;
}
