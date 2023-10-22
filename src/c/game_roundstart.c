// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "define.h"
#include "screen.h"
#include "game.h"
#include "game_roundstart.h"


/*
 * ゲームラウンドスタート処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_roundstart()
{
    // 仮実装

    // 仮想画面バッファクリア
    buff_clear();

    // 画面描画
    make_screen();

    // プレイヤーキャラクター初期化
    characters[0].type = PLAYER;
    characters[0].x = 15*8;
    characters[0].y = 143;
    characters[0].p = 0;
    characters[0].r = 1;
    characters[0].c[0] = 4;
    characters[0].c[1] = 11;
    characters[0].attr_no = 0;
    update_sprite_attr(characters[0]);

    // ゲーム状態変更
    change_game_state(GAME_STATE_MAIN);
}
