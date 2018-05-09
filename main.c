#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "header.h"

int main(int argc, char *argv[])
{
    game myGame;
    gameState state;
    SDL_Rect barPlayer1, barPlayer2, ball;

    char choice;
    int speedBallX = BALL_SPEED, speedBallY = BALL_SPEED, pntJ1 = 0, pntJ2 = 0;
    unsigned int frameLimit = SDL_GetTicks() + 16;

    barPlayer1.w = BAR_W;
    barPlayer1.h = BAR_H;
    barPlayer2.w = BAR_W;
    barPlayer2.h = BAR_H;
    ball.w = BALL_SIZE;
    ball.h = BALL_SIZE;

    state.g_bRunning = 1;

    resetGame(&barPlayer1, &barPlayer2, &ball);

    while(state.g_bRunning){
        system("@cls||clear");
        printf("Press E to start\n");
        printf("Press R for the rules.\n");

        choice = getc(stdin);
        choice = toupper(choice);
        switch(choice)
        {
            case 'E':   init("Pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_W,SCREEN_H,SDL_WINDOW_SHOWN,&myGame);
                        while(state.g_bRunning && pntJ1 < 3 && pntJ2 < 3){
                            EventBarMovement(&state, &barPlayer1, &barPlayer2);
                            ballDirection(&barPlayer1, &barPlayer2, &ball, &speedBallX, &speedBallY, &pntJ1, &pntJ2);
                            displayBar(&myGame, barPlayer1, barPlayer2, ball);
                        }

                        if (pntJ1 > pntJ2)
                            printf("Player 1 won !!");
                        else
                            printf("Player 2 won !!");

                        pntJ1 = 0;
                        pntJ2 = 0;

                        destroy(&myGame);
                        SDL_Quit();

                        system("pause");
                        break;

            case 'R':   system("@cls||clear");
                        rulesDisplay();
                        system("pause");
                        break;

            default:    break;
        }

        delay(frameLimit);
        frameLimit = SDL_GetTicks() + 16;
    }

    destroy(&myGame);

    SDL_Quit();

    return 0;
}
