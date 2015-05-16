/**
 * Pong
 *
 * The classic game
 *
 * Paddle game logic
 */

#ifndef _PADDLES_H_
#define _PADDLES_H_

#include <stdint.h>
#include <SDL.h>

/**
 * Called to move the left paddle if needed
 * @param keymap Keymap with current keys
 * @param paddle Paddle to move
 */
void paddle_left_tick(const uint8_t keymap[], SDL_Point *paddle);

/**
 * Called to move the right paddle if needed
 * @param keymap Keymap with current keys
 * @param paddle Paddle to move
 */
void paddle_right_tick(const uint8_t keymap[], SDL_Point *paddle);

#endif //_PADDLES_H_
