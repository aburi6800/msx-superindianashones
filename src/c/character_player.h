// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef CHARACTER_PLAYER_H
#define CHARACTER_PLAYER_H

// プレイヤー状態ENUM
typedef enum {
    PLAYER_STATUS_INIT,
    PLAYER_STATUS_MOVE,
    PLAYER_STATUS_JUMP,
    PLAYER_STATUS_SITEMOVE,
    PLAYER_STATUS_MISS
} player_status_t;


/**
 * プレイヤーキャラクター初期化
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void character_player_init();


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
