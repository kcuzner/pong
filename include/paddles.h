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
 * Initializes the paddles
 * @param left  Left paddle
 * @param right Right paddle
 */
void paddle_init(SDL_Rect *left, SDL_Rect *right);

/**
 * Called to move the left paddle if needed
 * @param paddle Paddle to move
 * @param keymap Keymap with current keys
 * @param ball   Ball position
 */
void paddle_left_tick(SDL_Rect *paddle, const uint8_t keymap[], const SDL_Point *ball);

/**
 * Called to move the right paddle if needed
 * @param paddle Paddle to move
 * @param keymap Keymap with current keys
 * @param ball   Ball position
 */
void paddle_right_tick(SDL_Rect *paddle, const uint8_t keymap[], const SDL_Point *ball);

#endif //_PADDLES_H_
