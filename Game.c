#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include "header.h"

int init(char *title,int xpos,int ypos,int width,int height,int flags,game *myGame){


    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;


    //initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);


            }


    }else{


        return 0;
    }



    return 1;



}

//BUT : Initialisation du terrain.
//SORTIE : Les bars qui bougent.
void displayBar(game *myGame, SDL_Rect barPlayer1, SDL_Rect barPlayer2, SDL_Rect ball)
{
    //Draw the separation line
    SDL_Rect separationLine;
    separationLine.x = SCREEN_W / 2;
    separationLine.y = 0;
    separationLine.w = LINE_W;
    separationLine.h = SCREEN_H;

    //Draw in texture
    SDL_SetRenderDrawColor(myGame->g_pRenderer,255,255,255,255);
    SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture);

    SDL_RenderFillRect(myGame->g_pRenderer, &barPlayer1);
    SDL_RenderFillRect(myGame->g_pRenderer, &barPlayer2);
    SDL_RenderFillRect(myGame->g_pRenderer, &separationLine);
    SDL_RenderFillRect(myGame->g_pRenderer, &ball);

    SDL_SetRenderTarget(myGame->g_pRenderer, NULL);

    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);

    SDL_RenderPresent(myGame->g_pRenderer);
    SDL_SetRenderDrawColor(myGame->g_pRenderer,0,0,0,255);

    SDL_DestroyTexture(myGame->g_ptexture);
    SDL_RenderClear(myGame->g_pRenderer);

}

//BUT : Faire bouger les bars de haut en bas pour deux joueurs.
//ENTREE : Flèche du haut/bas ou Z/S.
//SORTIE : Incrémentation ou décrémentation du y du rectangle.
void EventBarMovement(gameState *state, SDL_Rect *barPlayer1, SDL_Rect *barPlayer2)
{
    SDL_Event event;

     if (SDL_PollEvent(&event)){
        switch(event.type){

            case SDL_QUIT:  state->g_bRunning = 0;
                            break;

            case SDL_KEYDOWN:   switch(event.key.keysym.sym){
                                    case SDLK_z:    barPlayer1->y -= BAR_SPEED;
                                                    break;

                                    case SDLK_s:    barPlayer1->y += BAR_SPEED;
                                                    break;

                                    case SDLK_UP:   barPlayer2->y -= BAR_SPEED;
                                                    break;

                                    case SDLK_DOWN: barPlayer2->y += BAR_SPEED;
                                }
             default :  break;
        }
    }
}

//BUT : Vérification des collisions avec la balle.
enum COLLISION collisionBall(SDL_Rect barPlayer1, SDL_Rect barPlayer2, SDL_Rect ball)
{
    //Collisions du mur haut/bas.
    if (ball.y <= 0)
        return upCol;
    else if (ball.y >= SCREEN_H - ball.h)
        return downCol;
    //Collisions du mur gauche/droite.
    else if (ball.x <= 0)
        return leftCol;
    else if (ball.x + ball.w >= SCREEN_W)
        return rightCol;
    //Collision sur les raquettes.
    else if (ball.x <= barPlayer1.x + barPlayer1.w && ball.y + ball.h >= barPlayer1.y && ball.y <= barPlayer1.y + barPlayer1.h)
        return barPlayer1Col;
    else if (ball.x + ball.w >= barPlayer2.x && ball.y + ball.h >= barPlayer2.y && ball.y <= barPlayer2.y + barPlayer2.h)
        return barPlayer2Col;
    else return -1;
}

void ballDirection(SDL_Rect *barPlayer1, SDL_Rect *barPlayer2, SDL_Rect *ball, int *speedBallX, int *speedBallY, int *pntJ1, int *pntJ2)
{
    ball->x += *speedBallX;
    ball->y += *speedBallY;

    switch (collisionBall(*barPlayer1, *barPlayer2, *ball)){

        case upCol:
        case downCol:
            *speedBallY = -*speedBallY;
            break;

        case barPlayer1Col:
        case barPlayer2Col:
            *speedBallX = -*speedBallX;
            break;

        case rightCol:
            //Point pour le joueur 1.
            *pntJ1 = *pntJ1 + 1;
            score(*pntJ1, *pntJ2, barPlayer1, barPlayer2, ball);
            break;

        case leftCol:
            //Point pour le joueur 2.
            *pntJ2 = *pntJ2 + 1;
            score (*pntJ1, *pntJ2, barPlayer1, barPlayer2, ball);
            break;
    }
}

void score(int pntJ1, int pntJ2, SDL_Rect *barPlayer1, SDL_Rect *barPlayer2, SDL_Rect *ball)
{
    printf("Player 1  %d - %d  Player 2\n", pntJ1, pntJ2);
    resetGame(barPlayer1, barPlayer2, ball);
}

void resetGame(SDL_Rect *barPlayer1, SDL_Rect *barPlayer2, SDL_Rect *ball)
{
    barPlayer1->x = 0;
    barPlayer1->y = SCREEN_H / 2 - barPlayer1->h / 2;

    barPlayer2->x = SCREEN_W - barPlayer2->w;
    barPlayer2->y = SCREEN_H / 2 - barPlayer2->h / 2;

    ball->x = SCREEN_W / 2 - ball->w / 2;
    ball->y = SCREEN_H / 2 - ball->h / 2;
}

void rulesDisplay()
{
    printf("Player 1 : Move your bar with Z/S.\n");
    printf("Player 2 : Move your bar with the up/down arrows.\n");
    printf("You have to send the ball to your opponent. If you fail, the point goes to the other player.\n");
}

void destroy(game *myGame){

      //Destroy render
     if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

      //Destroy texture
     if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);

    //Destroy surface
     if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);

    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}

void delay(unsigned int frameLimit)
{
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}
