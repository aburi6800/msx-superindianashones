// license:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

// PSG DRIVER API
extern void sounddrv_init();
extern void sounddrv_exec();
extern void sounddrv_bgmplay(void *bgm_address);
extern void sounddrv_stop();
extern void sounddrv_sfxplay(void *bgm_address);

// 00:TITLE MUSIC
extern uint16_t MUSIC_00[];


/**
 * 音楽再生開始
 *
 * args:
 * - num            uint8_t     再生する音楽データNo
 *
 * return:
 * - void
 */
void play_music(uint8_t num);


/**
 * 効果音再生開始
 *
 * args:
 * - num            uint8_t     再生する効果音データNo
 *
 * return:
 * - void
 */
void play_sfx(uint8_t num);


#endif
