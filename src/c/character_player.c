// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "control.h"
#include "character.h"
#include "character_player.h"


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
    if (characters[0].f == PLAYER_MOVE) {
        characters[0].x = characters[0].x + vector_x[STICK_BUFF];
        if (STICK_BUFF = STICK_UP || STICK_BUFF == STICK_UPLEFT || STICK_BUFF == STICK_UPRIGHT) {
            // ジャンプ初期化
        }
        if (STRIG_BUFF) {
            characters[0].f = PLAYER_SITEMOVE;
        }
        return;
    }

    // 状態:プレイヤージャンプ
    if (characters[0].f == PLAYER_JUMP) {
        // ジャンプ中処理
    }

    // 状態:サイト移動中
    if (characters[0].f == PLAYER_SITEMOVE) {
        characters[1].x = characters[1].x + vector_x[STICK_BUFF];
        characters[1].y = characters[1].y + vector_y[STICK_BUFF];
        if (STRIG_BUFF == 0) {
            characters[0].f = PLAYER_MOVE;
            // ナイフオブジェクトの生成
        }
    }
}
