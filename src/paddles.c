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
void paddle_left_tick(SDL_Rect *paddle, const uint8_t keymap[], const SDL_Point *ball)
{
    //Variables:
    //keymap[SDL_SCANCODE_UP]           Whether or not the up key is pressed
    //keymap[SDL_SCANCODE_DOWN]         Whether or not the down key is pressed
    //paddle->y                         The "Y" position of the paddle
    //WALL_THICKNESS                    Thickness of the walls on the top and bottom
    //WINDOW_HEIGHT                     Height of the pong window

}

/**
 * Called to move the right paddle if needed
 * @param keymap Keymap with current keys
 * @param paddle Paddle to move
 */
void paddle_right_tick(SDL_Rect *paddle, const uint8_t keymap[], const SDL_Point *ball)
{
    //an ai is playing the game
    if (paddle->y > WALL_THICKNESS && ball->y < (paddle->y + paddle->h / 2))
    {
        paddle->y -= 2;
    }
    else if ((paddle->y + paddle->h) < (WINDOW_HEIGHT - WALL_THICKNESS) && ball->y > (paddle->y + paddle->h / 2))
    {
        paddle->y += 2;
    }
}
