// License:MIT License
// copyright-holders:Hitoshi Iwai

#include "game_init.h"
#include "game.h"


/**
 * メイン
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void main()
{
    // ゲーム初期設定
    game_init();

    while(1) {
        // ゲームループ
        game_loop();
    }
}
