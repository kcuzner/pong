/**
 * Pong
 *
 * The classic game
 *
 * Paddle game logic
 */

#include "paddles.h"
#include "config.h"

void paddle_init(SDL_Rect *left, SDL_Rect *right)
{
    left->x = PADDLE1_XPOS;
    left->y = WINDOW_HEIGHT / 2;
    left->w = 2;
    left->h = PADDLE_HEIGHT;

    right->x = PADDLE2_XPOS;
    right->y = WINDOW_HEIGHT / 2;
    right->w = 2;
    right->h = PADDLE_HEIGHT;
}

/**
 * Called to move the left paddle if needed
 * @param keymap Keymap with current keys
 * @param paddle Paddle to move
 */
void paddle_left_tick(const uint8_t keymap[], SDL_Rect *paddle)
{
    if (paddle->y > WALL_THICKNESS && keymap[SDL_SCANCODE_A])
    {
        paddle->y -= 2;
    }
    else if ((paddle->y + paddle->h) < (WINDOW_HEIGHT - WALL_THICKNESS) && keymap[SDL_SCANCODE_Z])
    {
        paddle->y += 2;
    }
}

/**
 * Called to move the right paddle if needed
 * @param keymap Keymap with current keys
 * @param paddle Paddle to move
 */
void paddle_right_tick(const uint8_t keymap[], SDL_Rect *paddle)
{
    if (paddle->y > WALL_THICKNESS && keymap[SDL_SCANCODE_UP])
    {
        paddle->y -= 2;
    }
    else if ((paddle->y + paddle->h) < (WINDOW_HEIGHT - WALL_THICKNESS) && keymap[SDL_SCANCODE_DOWN])
    {
        paddle->y += 2;
    }
}
