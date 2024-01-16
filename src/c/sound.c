// license:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include "sound.h"


/**
 * 音楽再生
 *
 * args:
 * - num            uint8_t     再生する音楽データNo
 *
 * return:
 * - void
 */
void play_music(uint8_t num)
{
    switch (num) {
        case 0:
            sounddrv_bgmplay(MUSIC_00);
        default:
            break;
    }
    return;
}


/**
 * 効果音再生
 *
 * args:
 * - num            uint8_t     再生する効果音データNo
 *
 * return:
 * - void
 */
void play_sfx(uint8_t num)
{
    switch (num) {
        case 0:
            sounddrv_sfxplay(MUSIC_00);
        default:
            break;
    }
    return;
}
