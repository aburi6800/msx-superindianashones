// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx.h>
#include <msx/gfx.h>
#include "character.h"
#include "control.h"
#include "tick.h"
#include "screen.h"
#include "game.h"
#include "game_title.h"


// 定数（ドア制御用）
#define DOOR_STATE_INIT     0
#define DOOR_STATE_WAIT     1
#define DOOR_STATE_MOVE     2
#define DOOR_STATE_END      3
#define DOOR_MODE_OPEN      0
#define DOOR_MODE_CLODE     1
#define DOOR_LEFT           0
#define DOOR_RIGHT          30
#define DOOR_WAIT_VALUE     30


// ゲーム情報
extern game_t game;

// ゲーム経過時間
extern tick_t tick;

// キャラクタ属性
extern character_t characters[8];


// ドアの座標
uint8_t door_x = 0;

// ドアの段階
uint8_t door_cnt = 0;

// ドアの状態
uint8_t door_state = 0;

// ドアのモード
uint8_t door_mode = 0;

// ドアのウェイトカウンタ
uint8_t door_wait_count = 0;


// キャラクターインデックス
extern uint8_t chr_idx;


/**
 * タイトル画面
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_title()
{
    if (game.substate == 0) {
        // ゲーム画面初期化
        make_screen();

        // 各種データ初期化
        game.score[0] = 0x12;
        game.score[1] = 0x34;
        game.score[2] = 0x56;
        game.round[0] = 0x78;
        game.left[0] = 0x90;

        // タイトルロゴ描画
        char title1[] = {161, 162, 163, 164, 165, 166, 167, 0};
        char title2[] = {193, 194, 195, 196, 197, 198, 199, 0};
        char title3[] = {168, 169, 170, 171, 172, 173, 174, 0};
        char title4[] = {200, 201, 202, 203, 204, 205, 206, 0};
        char title5[] = {175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 0};
        buff_wrttext(11, 2, title1);
        buff_wrttext(11, 3, title2);
        buff_wrttext(15, 4, title3);
        buff_wrttext(15, 5, title4);
        buff_wrttext( 8, 7, title5);
        buff_wrttext( 8,16, "[ABURI GAMES 2023");
        buff_wrttext( 7,18, "ALL RIGHT RESERVED");

        // キャラクターの初期座標設定
        uint8_t title_init_x[6] = { 16,   5,   8,  21,  26,  27};
        uint8_t title_init_y[6] = {143,   3,   1,   2,   1,   3};
        uint8_t i = 0;
        for (i = 0; i < 6; i++) {
            characters[i].x = title_init_x[i];
            characters[i].y = title_init_y[i];
        }
        characters[0].p = 0;
        characters[0].r = 1;
        characters[0].c[0] = 4;
        characters[0].c[1] = 11;

        // スプライトアトリビュートセット
        update_sprite_attr(characters[0], 0, true);

        // 目の初期設定
        for (uint8_t i = 1; i < 5; i++) {
            characters[i].f = get_rnd() % 2;
            if (characters[i].f == 1) {
                buff_wrttext(characters[i].x, characters[i].y, "m");
            }
        }
        characters[i].chr_num = 1;

        // サブステータスを変更
        game.substate = 1;
        chr_idx = 0;
        return;
    }

    if (game.substate == 1) {
        // 目の状態変更
        if (tick.tick1 % 30 == 0) {
            if (get_rnd() % 2 == 0) {
                if (characters[chr_idx].f == 1) {
                    characters[chr_idx].f = 0;
                    buff_wrttext(characters[chr_idx].x, characters[chr_idx].y, "m");
                } else {
                    characters[chr_idx].f = 1;
                    buff_wrttext(characters[chr_idx].x, characters[chr_idx].y, " ");
                }
            }
            chr_idx++;
            if (chr_idx > 5) {
                chr_idx = 1;
            }
        }

        // PUSH START 表示
        if (tick.tick2 % 2 == 0) {
            buff_wrttext(11, 12, "PUSH START");
        } else {
            buff_wrttext(11, 12, "          ");
        }

        // 入力判定
        if (get_strig_buff()) {
            game.substate = 2;
        }

        return;
    }

    if (game.substate == 2) {
        for (uint8_t i = 12; i < 19; i++) {
            buff_wrttext(7, i, "                   ");
        }
        game.substate = 3;
        return;
    }

    if (game.substate == 3) {
        if (characters[0].x >= 208) {
            // 次のstateの準備
            door_mode = DOOR_MODE_OPEN;
            door_state = DOOR_STATE_INIT;
            door_x = DOOR_RIGHT;
            game.substate = 4;
        } else {
            if (tick.tick1 % 8 == 0) {
                characters[0].x = characters[0].x + 4;
                characters[0].r = characters[0].r ^ 1;
                update_sprite_attr(characters[0], 0, true);
            }
        }
        return;
    }

    if (game.substate == 4) {
        door_anim();
        if (door_state == DOOR_STATE_END) {
            game.substate = 5;
        }
        return;
    }

    if (game.substate == 5) {
        if (tick.tick1 % 8 == 0) {
            if (characters[0].x >= 240) {
                characters[0].y = 211;
                game.substate = 6;
            } else {
                characters[0].x = characters[0].x + 4;
                characters[0].r = characters[0].r ^ 1;
            }
            update_sprite_attr(characters[0], 0, true);
        }
        return;
    }
}


/**
 * ドアアニメーション処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void door_anim()
{

    if (door_state == DOOR_STATE_INIT) {
        door_wait_count = DOOR_WAIT_VALUE;
        if (door_mode == DOOR_MODE_OPEN) {
            door_cnt = 0;
        } else {
            door_cnt = 5;
        }
        door_state = DOOR_STATE_MOVE;
        return;
    }

    if (door_state == DOOR_STATE_MOVE) {
        if (door_mode == DOOR_MODE_OPEN) {
            door_cnt++;
        } else {
            door_cnt--;
        }

        if (door_cnt == 0 || door_cnt == 5) {
            door_state = DOOR_STATE_END;
        } else {
            door_wait_count = DOOR_WAIT_VALUE;
            door_state = DOOR_STATE_WAIT;
        }

        switch (door_cnt) {
            case 1:
                buff_wrttext(door_x, 17, "oo");
                buff_wrttext(door_x, 18, "of");
                buff_wrttext(door_x, 19, "fo");
                break;
            case 2:
                buff_wrttext(door_x, 17, "of");
                buff_wrttext(door_x, 18, "fo");
                buff_wrttext(door_x, 19, "  ");
                break;
            case 3:
                buff_wrttext(door_x, 17, "fo");
                buff_wrttext(door_x, 18, "  ");
                break;
            case 4:
                buff_wrttext(door_x, 17, "  ");
                break;
            default:
                break;
        }
    }

    if (door_state == DOOR_STATE_WAIT) {
        door_wait_count--;
        if (door_wait_count == 0) {
            door_state = DOOR_STATE_MOVE;
        }
        return;
    }
}
