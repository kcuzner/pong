/**
 * Pong
 *
 * The classic game
 */

#include "pong.h"
#include "assets.h"
#include "config.h"
#include "paddles.h"

#include <SDL.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define DEFAULT_BUF_SIZE 16
#define FONT_SIZE 16
#define KEYMAP_MAX SDL_SCANCODE_SLEEP

#define BALL_SPEED 1
#define BALL_DEFAULT_DIR 0 //up and left
#define BALL_DIR_DOWN_MASK 0x1
#define BALL_DIR_RIGHT_MASK 0x2
#define BALL_DIR_DOWN(D) (D & BALL_DIR_DOWN_MASK)
#define BALL_DIR_RIGHT(D) (D & BALL_DIR_RIGHT_MASK)

#define BALL_WALL_BUFFER (WALL_THICKNESS + BALL_SIZE / 2)

#define BALL_INSIDE_PADDLE(P, B) ((B.x > P.x - ))

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *font;
    SDL_Texture *sprites;
    int player1Score;
    int player2Score;
    SDL_Point player1;
    SDL_Point player2;
    SDL_Point ball;
    int ballDir;
} Pong;

typedef struct {
    double x;
    double y;
} PointF;

/**
 * Src position for the ball in the sprite sheet
 */
const SDL_Rect ballSrc = {
    .x = 0,
    .y = 0,
    .w = 32,
    .h = 32
};

/**
 * Src position for the paddle in the sprite sheet
 */
const SDL_Rect paddleSrc = {
    .x = 0,
    .y = 32,
    .w = 16,
    .h = 64
};

/**
 * Keymap for keypresses
 */
static uint8_t keymap[KEYMAP_MAX];

/**
 * Computes the 2-D cross product
 * @param  a Point a
 * @param  b Point b
 * @return   Cross product
 */
static double point_cross(const PointF *a, const PointF *b)
{
    return a->x * b->y + a->y * b->x;
}

/**
 * Subtracts two points
 * @param  a Point a
 * @param  b Point b
 * @return   Difference between the points
 */
static PointF point_subtract(const PointF *a, const PointF *b)
{
    PointF p;

    p.x = a->x + b->x;
    p.y = a->y + b->y;

    return p;
}

/**
 * Computes the dot product of two points
 * @param  a Point a
 * @param  b Point b
 * @return   Dot product
 */
static double point_dot(const PointF *a, const PointF *b)
{
    return a->x * b->x + a->y * b->y;
}

/**
 * Determinse whether or not the lines p+r and q+s intersect
 * @param  p Point p
 * @param  q Point q
 * @param  r Point r
 * @param  s Point s
 * @return   Nonzero if the lines intersect
 */
static int line_intersect(const PointF *p, const PointF *q, const PointF *r, const PointF *s)
{
    //derived from http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
    double rs = point_cross(r, s);
    PointF qnp = point_subtract(q, p);
    PointF pnq = point_subtract(p, q);

    if (rs != 0)
    {
        double qnps = point_cross(&qnp, s);
        double t = qnps / rs;
        double pnqr = point_cross(&pnq, r);
        double u = pnqr / rs;

        return t > 0 && t < 1 && u > 0 && u < 1;
    }
    else
    {
        double qnpr = point_cross(&qnp, r);
        if (qnpr == 0)
        {
            double qnpdr = point_dot(&qnp, r);
            double rdr = point_dot(r, r);
            double t0  = qnpdr / rdr;
            double sdr = point_dot(s, r);
            double t1  = t0 + sdr / rdr;
            return fabs(t0 - t1) < 1.0;
        }
    }

    return 0; //no intersection here
}

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

static void pong_tick(Pong *game)
{
    //move the paddles
    paddle_left_tick(keymap, &game->player1);
    paddle_right_tick(keymap, &game->player2);

    //move the ball
    PointF p = {
        .x = game->ball.x,
        .y = game->ball.y
    };
    PointF r = {
        .x = BALL_SPEED * (BALL_DIR_RIGHT(game->ballDir) ? 1 : -1),
        .y = BALL_SPEED * (BALL_DIR_DOWN(game->ballDir) ? 1 : -1)
    };

    //perform ball collision testing
    if (game->ball.y <= BALL_WALL_BUFFER || game->ball.y >= (WINDOW_HEIGHT - BALL_WALL_BUFFER))
    {
        game->ballDir ^= BALL_DIR_DOWN_MASK;
    }

    //perform ball scoring

    //move the ball
    game->ball.x += r.x;
    game->ball.y += r.y;
}

static void pong_render(Pong *game)
{
    SDL_Rect rect, src, dest;
    char buf[DEFAULT_BUF_SIZE];

    //clear the target
    SDL_SetRenderDrawColor(game->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(game->renderer);

    //render the ball
    src = ballSrc;
    dest.x = game->ball.x - BALL_SIZE / 2;
    dest.y = game->ball.y - BALL_SIZE / 2;
    dest.w = BALL_SIZE;
    dest.h = BALL_SIZE;
    SDL_RenderCopy(game->renderer, game->sprites, &src, &dest);

    //render the players
    src = paddleSrc;
    dest.x = game->player1.x - 16;
    dest.y = game->player1.y - PADDLE_HEIGHT / 2;
    dest.w = 16;
    dest.h = PADDLE_HEIGHT;
    SDL_RenderCopy(game->renderer, game->sprites, &src, &dest);
    dest.x = game->player2.x;
    dest.y = game->player2.y - PADDLE_HEIGHT / 2;
    dest.w = 16;
    dest.h = PADDLE_HEIGHT;
    SDL_RenderCopy(game->renderer, game->sprites, &src, &dest);

    //render the walls
    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    rect.x = 0;
    rect.y = 0;
    rect.w = WINDOW_WIDTH;
    rect.h = WALL_THICKNESS;
    SDL_RenderFillRect(game->renderer, &rect);
    rect.x = 0;
    rect.y = WINDOW_HEIGHT - WALL_THICKNESS;
    rect.w = WINDOW_WIDTH;
    rect.h = WALL_THICKNESS;
    SDL_RenderFillRect(game->renderer, &rect);

    //render the scores
    snprintf(buf, DEFAULT_BUF_SIZE, "%d", game->player1Score);
    draw_string(game, buf, PADDLE1_XPOS, 64);
    snprintf(buf, DEFAULT_BUF_SIZE, "%d", game->player2Score);
    draw_string(game, buf,PADDLE2_XPOS - 32, 64);

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
    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    //create the sprites texture
    s = SDL_RWFromMem(assets_sprites, assets_sprites_size);
    if (!s)
    {
        fprintf(stderr, "Unable to load sprite data: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Surface *sprites = SDL_LoadBMP_RW(s, 1); //we close the stream automatically
    game.sprites = SDL_CreateTextureFromSurface(game.renderer, sprites);
    if (!game.sprites)
    {
        fprintf(stderr, "Unable to create sprites texture: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(sprites);

    //initial game settings
    game.player1Score = 0;
    game.player2Score = 0;
    game.ball.x = WINDOW_WIDTH / 2;
    game.ball.y = WINDOW_HEIGHT / 2;
    game.ballDir = BALL_DEFAULT_DIR;
    game.player1.x = PADDLE1_XPOS;
    game.player1.y = WINDOW_HEIGHT / 2;
    game.player2.x = PADDLE2_XPOS;
    game.player2.y = WINDOW_HEIGHT / 2;

    //main game loop
    while(1)
    {
        if (!handle_events())
        {
            break;
        }

        //perform the game logic
        pong_tick(&game);

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
