// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "define.h"
#include "game.h"
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
    update_character_player();
    update_character_knife();
    return;
}
