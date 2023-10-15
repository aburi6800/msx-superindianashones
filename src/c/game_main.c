// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "define.h"
#include "game.h"
#include "character_player.h"


/*
 * ゲームメイン処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_main()
{
    if (game.substate == 0) {
        // なんらかの初期処理

        game.substate = 1;
        return;
    }

    if (game.substate == 1) {
        // プレイヤー処理
        character_player();

        // 敵処理

        return;
    }
}
