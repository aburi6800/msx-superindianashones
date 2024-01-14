// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx/gfx.h>
#include "define.h"
#include "common.h"
#include "game.h"
#include "screen.h"


// パターンネームテーブル
uint8_t PTN_NAME_TBL[BUFF_SIZE] = {CHR_SPACE};


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
        PTN_NAME_TBL[i] = CHR_SPACE;
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
void buff_wrtbcd(uint8_t x, uint8_t y, unsigned char* value, uint8_t size)
{
    wrtbcdtodec(PTN_NAME_TBL, (y * BUFF_WIDTH) + x, value, size);
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
    buff_wrtbcd(6, 22, game.score, 3);
    // ラウンド
    buff_wrtbcd(19, 22, game.round, 1);
    // 残機
    buff_wrtbcd(29, 22, game.left, 1);
}


/**
 * 仮想画面逆スクロール処理
 *
 * args:
 * - addr               unsigned char*  仮想画面データのポインタ
 *
 * return:
 * - void
 */
void scroll_buff(unsigned char* addr)
{
    #ifndef __INTELLISENSE__
__asm
    // リターンアドレスをスキップ
    LD HL,2
    ADD HL,SP                   // SP+2を引数取得開始アドレスに設定

    // 第1引数(仮想画面データアドレス)を取得
    LD E,(HL)
    INC HL
    LD D,(HL)
    PUSH DE                     // 一旦退避

    // 処理開始仮想画面データアドレス設定
    LD HL,19*BUFF_WIDTH + 27    // スクロール範囲の右下端(27,19)をHLレジスタに設定
    ADD HL,DE

    LD B,18                     // 外部ループカウンタ 18(行数)

    // 外部ループ処理 開始
    // 全行に対しての処理を行う
scroll_buff_L1:
    PUSH BC                     // 外部ループカウンタをスタックに退避
    LD B,24                     // 内部ループカウンタ 24(桁数)

    // 内部ループ処理 開始
    // 1行に対しての処理を行う
    // ここ、LDDRとかに置き換えたい･･･
scroll_buff_L2:
    // 1行上のVRAMアドレスのデータを読む
    PUSH BC
    PUSH HL
    LD BC,BUFF_WIDTH            // HL(現アドレス)から32を減算し1行上のアドレスを設定
    SBC HL,BC
    LD A,(HL)                   // 仮想画面データ読み出し
    POP HL
    POP BC

    // 現在のVRAMアドレスにデータを書き込む
    LD (HL),A                   // 仮想画面データ書き込み

    DEC HL                      // 仮想画面のひとつ左のアドレスに移動
    DJNZ scroll_buff_L2         // B>0の間、scroll_buff_L2ラベルにジャンプ
    // 内部ループ処理 終了

    LD BC,0x0008                // 前回の処理終了アドレスから余白(左4文字＋右4文字=8文字分)を減算し、処理開始アドレスとする
    SBC HL,BC
    POP BC
    DJNZ scroll_buff_L1         // B>0の間、scroll_buff_L1ラベルにジャンプ
    // 外部ループ処理 終了

    // 先頭行クリア
    POP HL                      // 引数(仮想画面先頭アドレス)をスタックから取得
    LD DE,4 + 32
    ADD HL,DE                   // 処理開始アドレスを設定
    LD B,24
scroll_buff_L3:
    LD (HL), 0x20               // 仮想画面に空白を設定
    INC HL
    DJNZ scroll_buff_L3
__endasm
    #endif
}


/**
 * 画面作成処理
 *
 * args:
 * - game_state     uint8_t     ゲーム状態
 *
 * return:
 * - void
 */
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

    if (game.game_state != GAME_STATE_TITLE && game.game_state != GAME_STATE_OVER) {
        buff_wrttext(4, 0, "babfaaabaaaaafbfaafaaaab");
    }

    if (game.game_state != GAME_STATE_TITLE) {
        buff_wrttext(0,15, "hh");
        buff_wrttext(0,16, "hhh");
        buff_wrttext(0,17, "ooh");
        buff_wrttext(0,18, "  h");
        buff_wrttext(0,19, "  h");
    }

    if (game.game_state != GAME_STATE_ALLCLEAR) {
        buff_wrttext(29,15, " hh");
        buff_wrttext(29,16, "hhh");
        buff_wrttext(29,17, "hoo");
        buff_wrttext(29,18, "hof");
        buff_wrttext(29,19, "hfo");
    }

    // 各種数値表示
    display_information();
}
