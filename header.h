#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

//Size of the window
#define SCREEN_W 800
#define SCREEN_H 480

//Parameters of the bar
#define BAR_W 10
#define BAR_H 100
#define BAR_SPEED 8

//Line of separation
#define LINE_W 5

//Ball parameters
#define BALL_SIZE 20
#define BALL_SPEED 2

enum COLLISION{
    upCol, downCol, leftCol, rightCol, barPlayer1Col, barPlayer2Col,
};

typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;


}game;

typedef struct gameState{

    int g_bRunning;


}gameState;

//Prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);
void resetBall(/*SDL_Rect *position,*/ int *x, int *y);
void displayBar(game *myGame, SDL_Rect barPlayer1, SDL_Rect barPlayer2, SDL_Rect ball);
void EventBarMovement(gameState *state, SDL_Rect *barPlayer1, SDL_Rect *barPlayer2);
enum COLLISION collisionBall(SDL_Rect barPlayer1, SDL_Rect barPlayer2, SDL_Rect ball);
void ballDirection(SDL_Rect *barPlayer1, SDL_Rect *barPlayer2, SDL_Rect *ball, int *speedBallX, int *speedBallY, int *pntJ1, int *pntJ2);
void score(int pntJ1, int pntJ2, SDL_Rect *barPlayer1, SDL_Rect *barPlayer2, SDL_Rect *ball);
void resetGame(SDL_Rect *barPlayer1, SDL_Rect *barPlayer2, SDL_Rect *ball);
void rulesDisplay();
void destroy(game *myGame);
void delay(unsigned int frameLimit);
#endif // HEADER_H_INCLUDED



