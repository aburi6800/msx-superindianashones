// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "control.h"
#include "character.h"
#include "character_player.h"
#include "character_knife.h"
#include "game.h"
#include "tick.h"


// ジャンプカウンタ
uint8_t jump_cnt = 0;

// ジャンプ中の方向
uint8_t jump_stick = 0;

// ジャンプ中のY座標増減値
int8_t jump_vy[12] = {-6, -4, -2, -2, -1, 0, 0, 1, 2, 2, 4, 6};


/**
 * プレイヤーキャラクター初期化
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void init_character_player()
{
    // プレイヤー
    characters[0].type = PLAYER;
    characters[0].color1 = 4;
    characters[0].color2 = 11;
    characters[0].attr_no = 0;
    characters[0].p = 0;
    characters[0].r = 1;
//    characters[0].update = &update_character_player;
    // サイト
    characters[1].type = SIGHT;
    characters[1].color1 = 14;
    characters[1].attr_no = 2;
    characters[1].p = 10;
    characters[1].r = 0;
//    characters[1].update = NULL;

    if (game.game_state == GAME_STATE_TITLE) {
        characters[0].x = 16;
        characters[0].y = 143;
        characters[1].x = 0;
        characters[1].y = 192;
    } else {
        characters[0].f = PLAYER_STATUS_MOVE;
        characters[0].x = 120;
        characters[0].y = 143;
        characters[1].x = 120;
        characters[1].y = 80;
    }

    // スプライトアトリビュート更新
    update_sprite_attr(characters[0]);
    update_sprite_attr(characters[1]);
}


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
void update_character_player()
{
    // 状態:プレイヤー移動中
    if (characters[0].f == PLAYER_STATUS_MOVE) {
        if (tick.tick1 % 8 != 0) {
            return;
        }

        if (STICK_BUFF != 0) {
            characters[0].x += (vector_x[STICK_BUFF] * 4);
            if (STICK_BUFF == 6 || STICK_BUFF == 7 || STICK_BUFF == 8) {
                if (characters[0].p != 4) {
                    characters[0].p = 4;
                    characters[0].r = 0;
                }
            }
            if (STICK_BUFF == 2 || STICK_BUFF == 3 || STICK_BUFF == 4) {
                if (characters[0].p != 0) {
                    characters[0].p = 0;
                    characters[0].r = 0;
                }
            }
            characters[0].r = characters[0].r ^ 1;
            if (STICK_BUFF == STICK_UP || STICK_BUFF == STICK_UPLEFT || STICK_BUFF == STICK_UPRIGHT) {
                // ジャンプ初期化
                jump_cnt = 0;
                jump_stick = STICK_BUFF;
                characters[0].f = PLAYER_STATUS_JUMP;
            }
        }
        if (STRIG_BUFF) {
            characters[0].f = PLAYER_STATUS_SITEMOVE;
        }
    }

    // 状態:プレイヤージャンプ
    if (characters[0].f == PLAYER_STATUS_JUMP) {
        if (tick.tick1 % 8 != 0) {
            return;
        }

        // ジャンプ中処理
        characters[0].x += (vector_x[jump_stick] * 4);
        characters[0].y = characters[0].y + jump_vy[jump_cnt];
        if (jump_cnt++ > 11) {
            characters[0].f = PLAYER_STATUS_MOVE;
            characters[0].y = 143;
        }
    }

    // 状態:サイト移動中
    if (characters[0].f == PLAYER_STATUS_SITEMOVE) {
        characters[1].x += (vector_x[STICK_BUFF] * 2);
        characters[1].y += (vector_y[STICK_BUFF] * 2);
        if (STRIG_BUFF == 0) {
            if (characters[2].f == 0) {
                // ナイフの生成
                generate_knife();
            }
            characters[0].f = PLAYER_STATUS_MOVE;
        }
    }

    // スプライトアトリビュート更新
    update_sprite_attr(characters[0]);
    update_sprite_attr(characters[1]);
}
