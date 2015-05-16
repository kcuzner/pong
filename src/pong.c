/**
 * Pong
 *
 * The classic game
 */

#include "pong.h"
#include "assets.h"

#include <SDL.h>
#include <string.h>

#define DEFAULT_BUF_SIZE 16
#define FONT_SIZE 16
#define KEYMAP_MAX SDL_SCANCODE_SLEEP

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *font;
    int player1Score;
    int player2Score;
    SDL_Point player1;
    SDL_Point player2;
    SDL_Point ball;
} Pong;

/**
 * Keymap for keypresses
 */
static uint8_t keymap[KEYMAP_MAX];

/**
 * Handles events from SDL
 * @return 0 if the event loop should stop
 */
static int handle_events()
{
    SDL_Event e;
    SDL_KeyboardEvent *ke;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return 0;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            ke = (SDL_KeyboardEvent*)(&e);
            if (ke->keysym.scancode > 0xFF)
                continue;

            keymap[ke->keysym.scancode] = 1;
        }
        else if (e.type == SDL_KEYUP)
        {
            ke = (SDL_KeyboardEvent*)(&e);
            if (ke->keysym.scancode > 0xFF)
                continue;

            keymap[ke->keysym.scancode] = 0;
        }
    }

    return 1;
}

/**
 * Renders a string
 * @param game Game to use for rendering
 * @param str  String to render
 * @param x    X position of the string
 * @param y    Y position of the string
 */
static void draw_string(Pong *game, const char* str, int x, int y)
{
    int i;
    int len = strlen(str);
    SDL_Rect src, dest;

    src.w = FONT_SIZE;
    src.h = FONT_SIZE;

    dest.y = y;
    dest.w = src.w; //no scaling
    dest.h = src.h; //no scaling
    for (i = 0; i < len; i++)
    {
        //determine src based on the current char (it is a 16x16 grid of chars)
        src.x = (str[i] & 0x0F) * FONT_SIZE;
        src.y = ((str[i] & 0xF0) >> 4) * FONT_SIZE;

        //generate dest based on i
        dest.x = x + i * FONT_SIZE;

        SDL_RenderCopy(game->renderer, game->font, &src, &dest);
    }
}

static void pong_render(Pong *game)
{
    SDL_Rect rect;
    char buf[DEFAULT_BUF_SIZE];

    //render the ball

    //render the players

    //render the walls
    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    rect.x = 0;
    rect.y = 0;
    rect.w = 480;
    rect.h = 16;
    SDL_RenderFillRect(game->renderer, &rect);
    rect.x = 0;
    rect.y = 480 - 16;
    rect.w = 480;
    rect.h = 16;
    SDL_RenderFillRect(game->renderer, &rect);

    //render the scores
    snprintf(buf, DEFAULT_BUF_SIZE, "%d", game->player1Score);
    draw_string(game, buf, 32, 64);
    snprintf(buf, DEFAULT_BUF_SIZE, "%d", game->player2Score);
    draw_string(game, buf, 480 - 48, 64);

    //finish rendering this frame
    SDL_RenderPresent(game->renderer);
}

void pong_main(void)
{
    Pong game;
    SDL_Window *window = NULL;

    //initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL not initialized: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //create the window
    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 480, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL could not create window: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //create a hardware accelerated renderer for the window
    game.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    //create the font texture
    SDL_RWops *s = SDL_RWFromMem(assets_font, assets_font_size);
    if (!s)
    {
        fprintf(stderr, "Unable to load font data: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Surface *font = SDL_LoadBMP_RW(s, 1); //we close the stream automatically
    game.font = SDL_CreateTextureFromSurface(game.renderer, font);
    if (!game.font)
    {
        fprintf(stderr, "Unable to create font texture: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(font);

    game.player1Score = 0;
    game.player2Score = 0;
    while(1)
    {
        if (!handle_events())
        {
            break;
        }

        //render the game
        pong_render(&game);
    }

    //destroy our window and associated resources
    SDL_DestroyTexture(game.font);
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(window);

    //all done!
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
