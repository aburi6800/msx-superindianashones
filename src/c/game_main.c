// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "define.h"
#include "game.h"
#include "tick.h"
#include "screen.h"
#include "character_player.h"
#include "character_knife.h"


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
//    for (int i = 0; i++; i<8) {
//        if (characters[i].update != NULL) {
//            characters[i].update();
//        }
//    }
    if (tick.tick1 % 2 == 0) {
        update_character_player();
        update_character_knife();
    } else {
        // 仮想画面スクロール
        scroll_buff();
    }

    return;
}
