// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef GAME_TITLE_H
#define GAME_TITLE_H


// ドア制御用定数
#define DOOR_STATE_INIT     0
#define DOOR_STATE_WAIT     1
#define DOOR_STATE_MOVE     2
#define DOOR_STATE_END      3
#define DOOR_MODE_OPEN      0
#define DOOR_MODE_CLODE     1
#define DOOR_LEFT           0
#define DOOR_RIGHT          30
#define DOOR_WAIT_VALUE     30


/**
 * タイトル画面
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_title();


/**
 * ドアアニメーション処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void door_anim();

#endif
