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
