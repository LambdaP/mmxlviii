#include <stdio.h>

#include "game.h"

void draw (const game_t g)
{
        for (int i = 0; i < 4; i++) {
                printf("%4d %4d %4d %4d\n",
                                g.t[4*i],
                                g.t[4*i+1],
                                g.t[4*i+2],
                                g.t[4*i+3]);
        }

        printf("Score: %d\n\n", g.score);
}

int deal_char(game_t *g) {
        int c;
        do {
                c = getchar();
        } while (c != 'z'
            && c != 'q'
            && c != 's'
            && c != 'd');

        switch (c) {
                case 'q': return step(g, LEFT);
                case 'z': return step(g, UP);
                case 'd': return step(g, RIGHT);
                case 's': return step(g, DOWN);
        }

        return CONTINUE_GAME;
}

void interface (void) {
        init();
        game_t g;
        init_game(&g);

        int i;
        for(i = CONTINUE_GAME; i == CONTINUE_GAME; i = deal_char(&g)) {
                draw(g);
        }

        draw(g);

        if (i == LOSE_GAME) {
                printf("YOU LOSE.\n");
        } else {
                printf("YOU WIN.\n");
        }
}

int main (void) {
        interface();

        return 0;
}
