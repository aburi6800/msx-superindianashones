// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "control.h"
#include "character.h"
#include "character_player.h"
#include "game.h"
#include "tick.h"

// ジャンプカウンタ
uint8_t jump_cnt = 0;

// ジャンプ中のY座標増減値
int8_t jump_vy[12] = {-4, -4, -2, -2, -1, 0, 0, 1, 2, 2, 4, 4};


/**
 * プレイヤーキャラクターロジック
 * - プレイヤー本体はキャラクタ属性0
 * - サイトはキャラクタ属性1
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void character_player()
{
    // 状態:プレイヤー移動中
    if (characters[0].f == PLAYER_STATUS_MOVE) {
        if (tick.tick1 % 8 == 0) {
            characters[0].x = characters[0].x + (vector_x[STICK_BUFF] * 4);
            if (STICK_BUFF = STICK_UP || STICK_BUFF == STICK_UPLEFT || STICK_BUFF == STICK_UPRIGHT) {
                // ジャンプ初期化
                jump_cnt = 0;
                characters[0].f = PLAYER_STATUS_JUMP;
            }
            if (STRIG_BUFF) {
                characters[0].f = PLAYER_STATUS_SITEMOVE;
            }
        }
    }

    // 状態:プレイヤージャンプ
    if (characters[0].f == PLAYER_STATUS_JUMP) {
        // ジャンプ中処理
        characters[0].y = characters[0].y + jump_vy[jump_cnt];
        if (jump_cnt++ > 11) {
            characters[0].f = PLAYER_STATUS_MOVE;
        }
    }

    // 状態:サイト移動中
    if (characters[0].f == PLAYER_STATUS_SITEMOVE) {
        characters[1].x = characters[1].x + vector_x[STICK_BUFF];
        characters[1].y = characters[1].y + vector_y[STICK_BUFF];
        if (STRIG_BUFF == 0) {
            characters[0].f = PLAYER_STATUS_MOVE;
            // ナイフオブジェクトの生成
        }
    }

    update_sprite_attr(characters[0]);
}
