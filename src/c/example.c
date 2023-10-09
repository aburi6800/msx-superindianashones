// License:MIT License
// copyright-holders:Hitoshi Iwai

//#include <stdio.h>
//#include <string.h>
#include <stdint.h>
#include <msx.h>
#include <msx/gfx.h>
#include "common.h"
#include "screen.h"
#include "example.h"


// 定数（インライン展開される）
#define MSX_H_TIMI          0xfd9f
#define VRAM_START          0x1800
#define VRAM_WIDTH          32
#define VRAM_HEIGHT         24
#define CHR_SPACE           0x20

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


// マクロ（インライン展開される）
#define VPOS(x, y)  (VRAM_START + VRAM_WIDTH * y + x)
#define POS(x, y)   (VRAM_WIDTH * y + x)


// define_graphic_chars.asm:CHR_PTN_TBL ラベルの参照(パターンジェネレータテーブル)
extern uint8_t CHR_PTN_TBL[];

// define_graphic_chars.asm:CHR_COL_TBL ラベルの参照(カラーテーブル)
extern uint8_t CHR_COL_TBL[];

// define_sprite_chars.asm:SPR_PTN_TBL ラベルの参照
extern uint8_t SPR_PTN_TBL[];


// スプライトアトリビュートテーブル
uint8_t SPR_ATTR_TBL[32][4] = {0};

// 入力バッファ（方向）
uint8_t STICK_BUFF = 0;

// 入力バッファ（トリガ）
uint8_t STRIG_BUFF = 0;


// スコアデータ
unsigned char score[3] = {0x00, 0x00, 0x00};

// 画面更新完了フラグ
bool isUpdated = false;

// 経過時間
uint16_t tick1 = 0;
uint16_t tick2 = 0;

// ゲーム状態
uint8_t state;
uint8_t substate;


// キャラクタX座標
uint16_t x[8];

// キャラクタY座標
uint16_t y[8];

// キャラクタパターン
uint8_t p[8];

// キャラクタアニメーションカウンタ
uint8_t r[8];

// キャラクタカラー
uint8_t c[8];

// キャラクタ状態
uint8_t f[8];

// キャラクタ番号
uint8_t chr_num = 0;


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


/**
 * 画面更新
 * - H.TIMI割り込みから呼ばれる
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void screen_update()
{
    // 画面更新済（ロジック処理未終了）なら抜ける
    if (isUpdated) {
        return;
    }

    // 入力処理
    STICK_BUFF = get_stick(0) + get_stick(1);
    STRIG_BUFF = get_trigger(0) + get_trigger(1);

    // 経過時間加算
    count_tick();

    #ifndef __INTELLISENSE__
    __asm
    DI
    __endasm;
    #endif

    // パターンネームテーブル更新
    write_vram();

    // スプライトアトリビュートテーブル更新
    vwrite(SPR_ATTR_TBL, 0x1B00, 128);

    #ifndef __INTELLISENSE__
    __asm
    EI
    __endasm;
    #endif

    // 画面更新済（ロジック処理可）に設定する
    isUpdated = true;
    return;
}


/**
 * tick値加算
 * - tick1は毎フレーム加算する
 * - tick2は60フレームごとに加算する（＝秒カウント）
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void count_tick()
{
    // tick1加算
    tick1++;

    if (tick1 % 60 == 0) {
        // tick2加算
        tick2++;
    }
}


/**
 * ゲーム状態変更
 * - state_tで定義したゲーム状態を渡すことで状態を変更し、サブステータスをリセットする
 * - 状態変更後、経過時間をゼロにリセットする
 *
 * args:
 * - distState      state_t     変更後のゲーム状態
 *
 * return:
 * - void
*/
void change_state(state_t distState)
{
    // ゲーム状態を変更
    state = distState;
    substate = 0;

    // 経過時間リセット
    tick1 = 0;
    tick2 = 0;
}


/**
 * スプライトアトリビュート更新
 * - スプライトアトリビュートを更新する
 *
 * args:
 * - uint8_t        chr_idx     対象のキャラクタデータインデックス
 * - uint8_t        attr_idx    対象のアトリビュートデータインデックス
 * - bool           isPlayer    プレイヤーの場合はtrue
 *
 * return:
 * - void
 */
void update_sprite_attr(uint8_t attr_idx, uint8_t chr_idx, bool isPlayer)
{
    uint8_t ptn = (p[chr_idx] * 4) + (r[chr_idx] * 8);

    SPR_ATTR_TBL[attr_idx][0] = y[chr_idx];
    SPR_ATTR_TBL[attr_idx][1] = x[chr_idx];
    SPR_ATTR_TBL[attr_idx][2] = ptn;
    SPR_ATTR_TBL[attr_idx][3] = c[chr_idx];
    if (isPlayer) {
        attr_idx++;
        SPR_ATTR_TBL[attr_idx][0] = y[chr_idx];
        SPR_ATTR_TBL[attr_idx][1] = x[chr_idx];
        SPR_ATTR_TBL[attr_idx][2] = ptn + 4;
        SPR_ATTR_TBL[attr_idx][3] = c[chr_idx + 1];
    }
}


/**
 * 初期設定処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_init()
{
    // H.TIMIフック設定
    #ifndef __INTELLISENSE__
    __asm
    DI
    __endasm;
    #endif
    uint8_t *h_timi = (uint8_t *)MSX_H_TIMI;
    uint16_t hook_addr = (uint16_t)&screen_update;
    h_timi[0] = 0xc3; // JP
    h_timi[1] = (uint8_t)(hook_addr & 0xff);
    h_timi[2] = (uint8_t)((hook_addr & 0xff00) >> 8);
    #ifndef __INTELLISENSE__
    __asm
    EI
    __endasm;
    #endif

    // 画面初期化
    set_color(15, 1, 1);
    set_mangled_mode();
    msx_set_sprite_mode(sprite_large);

    // パターンジェネレータテーブル設定
    vwrite(CHR_PTN_TBL, 0x0000, 0x0800);
    vwrite(CHR_PTN_TBL, 0x0800, 0x0800);
    vwrite(CHR_PTN_TBL, 0x1000, 0x0800);

    // カラーテーブル設定
    vwrite(CHR_COL_TBL, 0x2000, 0x0800);
    vwrite(CHR_COL_TBL, 0x2800, 0x0800);
    vwrite(CHR_COL_TBL, 0x3000, 0x0800);

    // スプライトパターンテーブル設定
    vwrite(SPR_PTN_TBL, 0x3800, 32*19);

    // 仮想画面クリア
    buff_clear();

    // ゲーム状態初期化
    change_state(TITLE);
}


/**
 * 画面作成処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void make_screen()
{
    // 仮想画面クリア
    buff_clear();

    buff_wrttext(0, 0, "faaf                        aafa");

    if (state != TITLE && state != GAME_OVER) {
        buff_wrttext(4, 0, "babfaaabaaaaafbfaafaaaab");
    }
    buff_wrttext(0, 1, "aafd                        ceba");
    buff_wrttext(0, 2, "bff                          aaf");
    buff_wrttext(0, 3, "fac                          daf");
    buff_wrttext(0, 4, "ad                            fa");
    buff_wrttext(0, 5, "a d                           ca");
    buff_wrttext(0, 6, "a                            e a");
    buff_wrttext(0, 7, "d                              e");
    buff_wrttext(0, 8, " d                            e ");
    buff_wrttext(0, 9, "b                               ");
    buff_wrttext(0,10, "f                              b");
    buff_wrttext(0,11, "bd                            bf");
    buff_wrttext(0,12, "d                             eb");
    buff_wrttext(0,13, "                               e");
    buff_wrttext(0,20, "gggggggggggggggggggggggggggggggg");
    buff_wrttext(0,22, " SCORE        ROUND      LEFT");

    if (state != TITLE) {
        buff_wrttext(0,15, "hh");
        buff_wrttext(0,16, "hhh");
        buff_wrttext(0,17, "ooh");
        buff_wrttext(0,18, "  h");
        buff_wrttext(0,19, "  h");
    }

    if (state != ALL_CLEAR) {
        buff_wrttext(29,15, " hh");
        buff_wrttext(29,16, "hhh");
        buff_wrttext(29,17, "hoo");
        buff_wrttext(29,18, "hof");
        buff_wrttext(29,19, "hfo");
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


/**
 * タイトル画面
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void title()
{
    if (substate == 0) {
        // ゲーム画面初期化
        make_screen();

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
            x[i] = title_init_x[i];
            y[i] = title_init_y[i];
        }
        p[0] = 0;
        r[0] = 1;
        c[0] = 4;
        c[1] = 11;

        // スプライトアトリビュートセット
        update_sprite_attr(0, 0, true);

        // 目の初期設定
        for (uint8_t i = 1; i < 5; i++) {
            f[i] = get_rnd() % 2;
            if (f[i] == 1) {
                buff_wrttext(x[i], y[i], "m");
            }
        }
        chr_num = 1;
        add_bcd(0x0010, score, sizeof(score));
        buff_wrtbcd(6 ,22, score);

        // サブステータスを変更
        substate = 1;
        return;
    }

    if (substate == 1) {
        // 目の状態変更
        if (tick1 % 30 == 0) {
            if (get_rnd() % 2 == 0) {
                if (f[chr_num] == 1) {
                    f[chr_num] = 0;
                    buff_wrttext(x[chr_num], y[chr_num], "m");
                } else {
                    f[chr_num] = 1;
                    buff_wrttext(x[chr_num], y[chr_num], " ");
                }
            }
            chr_num++;
            if (chr_num > 5) {
                chr_num = 1;
            }
        }

        // PUSH START 表示
        if (tick2 % 2 == 0) {
            buff_wrttext(11, 12, "PUSH START");
        } else {
            buff_wrttext(11, 12, "          ");
        }

        // 入力判定
        if (STRIG_BUFF) {
            substate = 2;
        }

        return;
    }

    if (substate == 2) {
        for (uint8_t i = 12; i < 19; i++) {
            buff_wrttext(7, i, "                   ");
        }
        substate = 3;
        return;
    }

    if (substate == 3) {
        if (x[0] >= 208) {
            // 次のstateの準備
            door_mode = DOOR_MODE_OPEN;
            door_state = DOOR_STATE_INIT;
            door_x = DOOR_RIGHT;
            substate = 4;
            return;
        } else {
            if (tick1 % 8 == 0) {
                x[0] = x[0] + 4;
                r[0] = r[0] ^ 1;
                update_sprite_attr(0, 0, true);
            }
        }
    }

    if (substate == 4) {
        door_anim();
        if (door_state == DOOR_STATE_END) {
            substate = 5;
        }
    }

    if (substate == 5) {
        if (tick1 % 8 == 0) {
            if (x[0] >= 240) {
                y[0] = 211;
                substate = 6;
            } else {
                x[0] = x[0] + 4;
                r[0] = r[0] ^ 1;
            }
            update_sprite_attr(0, 0, true);
        }
    }
}


/**
 * ゲームループ
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_loop()
{
    while(1) {
        // 画面更新済（ロジック処理可）なら処理する
        if (isUpdated) {
            switch (state) {
                case TITLE:
                    title();
                default:
                    break;
            }

            // 画面更新完了フラグを画面更新未完了に設定
            isUpdated = false;
        }

    }
}
