#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(NODE_PLAYERS *_head,NODE_PLAYERS * crr_player, TTF_Font *, SDL_Surface **, SDL_Renderer *, int _delay );
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(CASA *_house, SDL_Surface **, SDL_Renderer *,TTF_Font *_font );
void RenderPlayerCards(NODE_PLAYERS *_head, SDL_Surface **_cards, SDL_Renderer* _renderer);
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);
// definition of some strings: they cannot be changed when the program is executed !

