// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef CHARACTER_PLAYER_H
#define CHARACTER_PLAYER_H

// プレイヤー状態
#define PLAYER_MOVE         1
#define PLAYER_JUMP         2
#define PLAYER_SITEMOVE     3


/**
 * プレイヤーキャラクターロジック
 * - プレイヤー本体はキャラクタ属性0
 * - サイトはキャラクタ属性1
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void character_player();

#endif
