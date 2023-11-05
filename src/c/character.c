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
 * - character      character_t 対象のキャラクタデータ
 *
 * return:
 * - void
 */
void set_movevalue(character_t character)
{


}


/**
 * キャラクタ移動処理
 *
 * args:
 * - character      character_t 対象のキャラクタデータ
 *
 * return:
 * - void
 */
void move_character(character_t character)
{


}
