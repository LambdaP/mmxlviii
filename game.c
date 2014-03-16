#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "game.h"

void init (void)
{
        srand((unsigned) time(NULL));
}

/*
 * Given a 4-numbers row, move them all towards the first one,
 * leaving zeroes in the places newly empty.
 */
void push (int *p1, int *p2, int *p3, int *p4)
{
        if (*p3 == 0) {
                *p3 = *p4;
                *p4 = 0;
        }
        if (*p2 == 0) {
                *p2 = *p3;
                *p3 = *p4;
                *p4 = 0;
        }
        if (*p1 == 0) {
                *p1 = *p2;
                *p2 = *p3;
                *p3 = *p4;
                *p4 = 0;
        }
}

static inline void replace_and_score (int *p, int val, unsigned int *score)
{
        *p = val;
        *score += val;
}

/*
 * Given a 4-numbers row, fusion the neighbouring cases bearing the
 * same number, leaving zeroes in the places newly empty.
 */
void fusion (unsigned int *score, int *p1, int *p2, int *p3, int *p4)
{
        if (*p1 == *p2) {
                replace_and_score(p1, 2 * (*p1), score);

                if (*p3 == *p4) {
                        *p2 = *p3 + *p4;
                        *score += *p2;
                        *p3 = 0;
                } else {
                        *p2 = *p3;
                        *p3 = *p4;
                }

                *p4 = 0;
        } else if (*p2 == *p3) {
                replace_and_score(p2, 2 * (*p2), score);
                *p3 = *p4;
                *p4 = 0;
        } else if (*p3 == *p4) {
                replace_and_score(p3, 2 * (*p3), score);
                *p4 = 0;
        }
}

/*
 * Given a gameset and four positions, use those positions as
 * a row and apply push and fusion.
 */
void move4 (game_t *g, int n1, int n2, int n3, int n4)
{
        int *p1, *p2, *p3, *p4;
        p1 = g->t + n1;
        p2 = g->t + n2;
        p3 = g->t + n3;
        p4 = g->t + n4;
        push(p1, p2, p3, p4);
        fusion(&(g->score), p1, p2, p3, p4);
}

/*
 * Given a gameset and a direction, move the corresponding rows,
 * completing a single move.
 */
void move(game_t *g, dir_t d)
{
        switch (d) {
                case LEFT:  move4 (g, 0 , 1 , 2 , 3);
                            move4 (g, 4 , 5 , 6 , 7);
                            move4 (g, 8 , 9 , 10, 11);
                            move4 (g, 12, 13, 14, 15);
                            break;
                case RIGHT: move4 (g, 3 , 2 , 1 , 0);
                            move4 (g, 7 , 6 , 5 , 4);
                            move4 (g, 11, 10, 9 , 8);
                            move4 (g, 15, 14, 13, 12);
                            break;
                case UP:    move4 (g, 0,  4,  8,  12);
                            move4 (g, 1,  5,  9,  13);
                            move4 (g, 2,  6,  10, 14);
                            move4 (g, 3,  7,  11, 15);
                            break;
                case DOWN:  move4 (g, 12, 8 , 4,  0);
                            move4 (g, 13, 9 , 5,  1);
                            move4 (g, 14, 10, 6,  2);
                            move4 (g, 15, 11, 7,  3);
                            break;
                default:    exit(1);
        }
}

/*
 * Try to make a move, then compare it to the previous gameset to
 * ensure the board has changed.
 *
 * Return 1 if the game has changed, 0 otherwise.
 */
int simulate(game_t *g, dir_t d)
{
        static game_t virtual;
        memcpy(&virtual, g, sizeof(game_t));

        move(g, d);

        return memcmp(&virtual, g, sizeof(game_t));
}

/*
 * Count the empty tiles on the board.
 */
int nempty (const int t [16])
{
        int ret = 0;
        for (int i = 0; i < 16; i++) {
                if (t[i] == 0) {
                        ret++;
                }
        }
        return ret;
}

/*
 * Give a random integer between a and b.
 */
int randab (int a, int b)
{
        double ratio = ((double) rand() / (double) RAND_MAX);
        return a + (int) ((b - a + 1) * ratio);
}

/*
 * Make a new tile appear on the board.
 */
void pop_new (int t [16])
{
        int tmp = randab(0,2);
        int new = (tmp == 0) ? 4 : 2;
        int n = nempty(t);
        int new_case = randab(1, n);

        for(int i = 0; i < 16; i++) {
                if (t[i] == 0) {
                        new_case--;
                        if(new_case == 0) {
                                t[i] = new;
                                return;
                        }
                }
        }
}

/*
 * Check if there is a valid possible move on the board.
 *
 * Return 1 if the board has a valid move, 0 otherwise.
 */
int exist_move (const int t [16])
{
        // A non-full board has a valid move.
        if (nempty(t)) {
                return 1;
        }

        static game_t virtual;
        memcpy(virtual.t, t, sizeof(game_t));
        virtual.score = 0;

        return (simulate(&virtual, LEFT)
             || simulate(&virtual, UP)
             || simulate(&virtual, RIGHT)
             || simulate(&virtual, DOWN));
}

/*
 * Check if the game should end.
 *
 * Return CONTINUE_GAME if the game should continue,
 *        WIN_GAME if the game was won,
 *        LOSE_GAME if there are no more possible moves.
 */
int finish (int t [16])
{
        for (int i = 0; i < 16; i++) {
                if (t[i] == WINNING_VALUE) {
                        return WIN_GAME;
                }
        }

        if (nempty(t)) {
                return CONTINUE_GAME;
        } else if (!exist_move(t)) {
                return LOSE_GAME;
        }

        return CONTINUE_GAME;
}

/*
 * Given a gameset and a direction, make a move if possible, then
 * return the status (continue, win, lose) of the game.
 */
int step (game_t *g, dir_t d)
{
        if (simulate(g, d)) {
                pop_new(g->t);
                return finish(g->t);
        }

        return CONTINUE_GAME;
}

void init_game (game_t *g)
{
        memset(g, 0, sizeof(game_t));
        pop_new(g->t);
        pop_new(g->t);
}
