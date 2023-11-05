// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "define.h"
#include "screen.h"
#include "character_player.h"
#include "character_knife.h"
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

    // キャラクター初期化
    for (uint8_t i=0; i++; i<8) {
        characters[i].f = 0;
        characters[i].update = NULL;
    }

    // プレイヤーキャラクター初期化
    init_character_player();

    // ナイフ初期化
    init_character_knife();

    // ゲーム状態変更
    change_game_state(GAME_STATE_MAIN);
}
