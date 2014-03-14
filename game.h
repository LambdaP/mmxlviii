#ifndef __GAME_H__
#define __GAME_H__

#define WINNING_VALUE 2048

#define LOSE_GAME (-1)
#define CONTINUE_GAME 0
#define WIN_GAME 1

typedef struct game {
        int t [16];
        unsigned int score;
} game_t;

typedef enum dir {LEFT, UP, RIGHT, DOWN} dir_t;

void init (void);
void draw (const game_t game);
int step(game_t *game, dir_t);
void init_game (game_t *g);

#endif // __GAME_H__
