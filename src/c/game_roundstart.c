// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "define.h"
#include "screen.h"
#include "character_player.h"
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
    character_player_init();

    // ゲーム状態変更
    change_game_state(GAME_STATE_MAIN);
}
