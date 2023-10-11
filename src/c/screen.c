// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx/gfx.h>
#include "common.h"
#include "game.h"

#define BUFF_WIDTH          32
#define BUFF_HEIGHT         24
#define BUFF_SIZE           BUFF_WIDTH * BUFF_HEIGHT


// パターンネームテーブル
uint8_t PTN_NAME_TBL[BUFF_SIZE] = {0};

extern game_t game;


/**
 * 仮想画面バッファクリア
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void buff_clear()
{
    for (uint16_t i = 0; i < BUFF_SIZE; i++) {
        PTN_NAME_TBL[i] = 32;
    }
}


/**
 * テキスト表示
 * - 構造体に指定されたテキストを仮想画面のx,y座標に書き込む。
 * - 画面端のはみ出し処理はしていないので注意。
 *
 * args:
 * - x              uint8_t     更新先のX座標
 * - y              uint8_t     更新先のY座標
 * - text           char*       表示テキストのアドレス
 *
 * return:
 * - void
 */
void buff_wrttext(uint8_t x, uint8_t y, char* text)
{
    for (uint8_t i = 0; i < strlen(text); i++) {
        PTN_NAME_TBL[y * BUFF_WIDTH + x] = text[i];
        x++;
    }
}


/**
 * BCD値表示
 * - 指定されたx,y座標に指定されたデータ(BCD値)を10進数の数値で書き込む。
 * - 画面端のはみ出し処理はしていないので注意。
 *
 * args:
 * - x              uint8_t     出力先のX座標
 * - y              uint8_t     出力先のY座標
 * - value          unsigned char* 表示データのアドレス
 * - size           uint8_t     表示データのサイズ(byte)
 *
 * return:
 * - void
 */
void wrte_bcd(uint8_t x, uint8_t y, unsigned char* value, uint8_t size)
{
    wrtbcdtodec(PTN_NAME_TBL, (y * BUFF_WIDTH) + x, value, size);
}


/*
 * VRAM(パターンネームテーブル)更新
 * - 仮想画面バッファをVRAMのパターンネームテーブルへ転送する。
 *
 * args:
 * - none
 *
 * return:
 * - none
 */
void vwrite_ptn_name_tbl()
{
    // パターンネームテーブル更新
    vwrite(PTN_NAME_TBL, 0x1800, 768);
}


/**
 * 各種情報表示
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void display_information()
{
    // スコア
    wrte_bcd(6, 22, game.score, 3);
    // ラウンド
    wrte_bcd(19, 22, game.round, 1);
    // 残機
    wrte_bcd(29, 22, game.left, 1);
}


/**
 * 画面作成処理
 *
 * args:
 * - state          uint8_t     ゲーム状態
 *
 * return:
 * - void
 */
//void make_screen(uint8_t state)
void make_screen()
{
    // 仮想画面クリア
    buff_clear();

    buff_wrttext(0, 0, "faaf                        aafa");

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

    if (game.state != TITLE && game.state != GAME_OVER) {
        buff_wrttext(4, 0, "babfaaabaaaaafbfaafaaaab");
    }

    if (game.state != TITLE) {
        buff_wrttext(0,15, "hh");
        buff_wrttext(0,16, "hhh");
        buff_wrttext(0,17, "ooh");
        buff_wrttext(0,18, "  h");
        buff_wrttext(0,19, "  h");
    }

    if (game.state != ALL_CLEAR) {
        buff_wrttext(29,15, " hh");
        buff_wrttext(29,16, "hhh");
        buff_wrttext(29,17, "hoo");
        buff_wrttext(29,18, "hof");
        buff_wrttext(29,19, "hfo");
    }

    // 各種数値表示
    display_information();
}
