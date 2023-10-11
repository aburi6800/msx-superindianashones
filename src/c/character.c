// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx/gfx.h>
#include "character.h"


// キャラクタ属性
character_t characters[8];

// スプライトアトリビュートテーブル
uint8_t SPR_ATTR_TBL[32][4] = {0};

// キャラクタインデックス
uint8_t chr_idx = 0;


/**
 * スプライトアトリビュート更新
 * - スプライトアトリビュートを更新する
 *
 * args:
 * - uint16_t       character_p 対象のキャラクタデータインデックス
 * - uint8_t        attr_idx    対象のアトリビュートデータインデックス
 * - bool           isPlayer    プレイヤーの場合はtrue
 *
 * return:
 * - void
 */
void update_sprite_attr(character_t character, uint8_t attr_idx, bool isPlayer)
{
    uint8_t ptn = (character.p * 4) + (character.r * 8);

    SPR_ATTR_TBL[attr_idx][0] = character.y;
    SPR_ATTR_TBL[attr_idx][1] = character.x;
    SPR_ATTR_TBL[attr_idx][2] = ptn;
    SPR_ATTR_TBL[attr_idx][3] = character.c[0];
    if (isPlayer) {
        attr_idx++;
        SPR_ATTR_TBL[attr_idx][0] = character.y;
        SPR_ATTR_TBL[attr_idx][1] = character.x;
        SPR_ATTR_TBL[attr_idx][2] = ptn + 4;
        SPR_ATTR_TBL[attr_idx][3] = character.c[1];
    }
}


/*
 * スプライトアトリビュートテーブルをVRAMへ転送
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void vwrite_sprite_attr_tbl()
{
    vwrite(SPR_ATTR_TBL, 0x1B00, 128);
}
