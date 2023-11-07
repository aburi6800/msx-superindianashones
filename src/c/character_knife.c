// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "character.h"
#include "character_knife.h"
#include "game.h"
#include "tick.h"


/**
 * ナイフ初期化
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void init_character_knife()
{
    // ナイフ
    characters[2].type = KNIFE;
    characters[2].color1 = 15;
    characters[2].attr_no = 3;
    characters[2].p = 11;
    characters[2].f = 0;
    characters[2].r = 0;
    characters[2].x = 0;
    characters[2].y = 192;
    characters[2].speed = 2;
//    characters[2].update = &update_character_knife;

    // スプライトアトリビュート更新
    update_sprite_attr(characters[2]);
}


/**
 * ナイフ生成
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void generate_knife()
{
    // ナイフオブジェクトの生成
    characters[2].x = characters[0].x;
    characters[2].y = characters[0].y - 16;
    characters[2].target_x = characters[1].x;
    characters[2].target_y = characters[1].y;
    characters[2].f = 1;
    set_movevalue(&characters[2]);
}

/**
 * ナイフロジック
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void update_character_knife()
{
    // ナイフが投げられていなければ終わる
    if (characters[2].f == 0) {
        return;
    }

    // ナイフ移動
    if (move_character(&characters[2]) == 1) {
        characters[2].f = 0;
        characters[2].y = 192;
        // 移動終了時は当たり判定を行う

    } else {
        // ナイフ移動量カウント
    }

    // スプライトアトリビュート更新
    update_sprite_attr(characters[2]);
}
