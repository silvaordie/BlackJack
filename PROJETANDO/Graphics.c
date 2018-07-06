#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "ConstEstrut.h"
#include "Data.h"
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
const char myName[] = "Jose Silva e Pedro Almeida";
const char myNumber[] = "         84109 84164";
/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param _money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(NODE_PLAYERS *_head,NODE_PLAYERS *_crr_player, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, int _delay)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color red = { 255, 0, 0 }; // red
    SDL_Color green ={ 0,150,0,0};
    SDL_Color yellow={255,255,0,0};
    SDL_Color white={255,255,255,0};

    char name_info_str[STRING_SIZE];
    char name_money_str[STRING_SIZE];
    char delay_info[STRING_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    SDL_Rect controls;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height;
    NODE_PLAYERS *aux=NULL;
    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);
    //Mostra o valor atual do Delay
    sprintf(delay_info,"Delay: %0.1fs",(float)(300+_delay)/1000 );
    RenderText(10, 10, delay_info, _font, &white, _renderer);
    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(separatorPos+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    height += RenderText(separatorPos+3*MARGIN, height, myNumber, _font, &black, _renderer);
    //height +=10;
    aux=_head;
    while(aux!=NULL)
    {
        if(aux->jogador.tipo ==0)
            sprintf(name_money_str,"HU %s: %0.1f euros",aux->jogador.nome,aux->jogador.banco);
        else
            sprintf(name_money_str,"EA %s: %0.1f euros",aux->jogador.nome,aux->jogador.banco);

        if(aux==_crr_player)
            height+= RenderText(separatorPos+3*MARGIN,height,name_money_str,_font,&red,_renderer);
        else
            height+= RenderText(separatorPos+3*MARGIN,height,name_money_str,_font,&black,_renderer);

        aux=aux->next;
    }
    //Mostra o retangulo onde estarão os controlos
    controls.x=separatorPos+2;
    controls.y=height;
    controls.w=189;
    controls.h=HEIGHT_WINDOW-height-2;
    SDL_SetRenderDrawColor(_renderer, 5, 5, 5, 5 );
    SDL_RenderDrawRect(_renderer, &controls);
    //Mostra os controlos
    height += RenderText(separatorPos+3*MARGIN, height, "Controlos: ", _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, "[H] - Hit", _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, "[S] - Stand", _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, "[D] - Double", _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, "[R] - Surrender", _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, "[B] - Bet", _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, "[N] - New Game", _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, "[A] - New Player", _font, &black, _renderer);

    // renders the squares + name for each player
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
    aux=_head;
    // renders the areas for each player: names and money too !
    int i=0;
    for (aux = _head; i<4 ; i++)
    {
        playerRect.x = i*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);

        aux=_head;
        while(aux!=NULL)
        {
            if(aux->jogador.player_pos==i)
            {
                sprintf(name_info_str,"%s:%i euros Pontos:%i", aux->jogador.nome, aux->jogador.aposta,aux->jogador.pontos);
                if(aux==_crr_player)
                {
                    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
                    RenderText(playerRect.x+20, playerRect.y-30, name_info_str, _font, &red, _renderer);
                }
                else
                {
                    SDL_SetRenderDrawColor(_renderer, 0, 150, 0, 0 );
                    if(aux->jogador.pontos==0)
                    {
                        sprintf(name_info_str,"%s:%i euros", aux->jogador.nome, aux->jogador.aposta);
                        RenderText(playerRect.x+20, playerRect.y-30, name_info_str, _font, &green, _renderer);
                        RenderText(playerRect.x+170, playerRect.y-30, "BU", _font, &red, _renderer);
                    }
                    else
                        if(aux->jogador.pontos==21 && aux->jogador.mao_top->next == aux->jogador.mao_back)
                        {
                            sprintf(name_info_str,"%s:%i euros", aux->jogador.nome, aux->jogador.aposta);
                            RenderText(playerRect.x+20, playerRect.y-30, name_info_str, _font, &green, _renderer);
                            RenderText(playerRect.x+170, playerRect.y-30, "BJ", _font, &yellow, _renderer);
                        }
                        else
                            RenderText(playerRect.x+20, playerRect.y-30, name_info_str, _font, &green, _renderer);
                    }
                SDL_RenderDrawRect(_renderer, &playerRect);
            }
            else
            {
                SDL_SetRenderDrawColor(_renderer, 0, 150, 0, 0 );
                SDL_RenderDrawRect(_renderer, &playerRect);
            }
            aux=aux->next;
        }
    }


    // destroy everything
    SDL_DestroyTexture(table_texture);
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer);
void RenderHouseCards(CASA *_house, SDL_Surface **_cards, SDL_Renderer* _renderer,TTF_Font *_font)
{
    SDL_Color red = { 255, 0, 0 }; // red
    SDL_Color yellow={255,255,0,0};
    SDL_Color white = { 255, 255, 255 }; // white
    int card, x, y,draw_card;
    int div = WIDTH_WINDOW/CARD_WIDTH;
    STACK_NODE *_mao=NULL;
    int _pos_house_hand=0;
    char house_pts[STRING_SIZE];

    sprintf(house_pts,"Pontos %i",_house->pontos);

    // drawing all house cards
    _mao=_house->mao_top;
    while ( _mao!=NULL )
    {
        _pos_house_hand++;
        _mao=_mao->next;
    }
    card=0;

        for ( _mao=_house->mao_back; _mao!=NULL; _mao=_mao->prev)
        {
                card++;
            // calculate its position
            x = (div/2-_pos_house_hand/2+card)*CARD_WIDTH + 15;
            y = (int) (0.26f*HEIGHT_WINDOW);
            draw_card=_mao->carta.valor+(_mao->carta.naipe*13);
            // render it !
            RenderCard(x, y, draw_card, _cards, _renderer);
        }
        if ( _house->estado == 0)
        {
            x = (div/2-_pos_house_hand/2+1)*CARD_WIDTH + 15;
            y = (int) (0.26f*HEIGHT_WINDOW);
            RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
        }
        else
            if(_house->pontos==0)
            {
                RenderText(x-CARD_WIDTH*_pos_house_hand,y+60,"BUST", _font,&red,_renderer);
            }
            else
                if(_house->pontos==21 && _house->mao_top->next== _house->mao_back)
                    RenderText(x-CARD_WIDTH*_pos_house_hand-60,y+60,"BLACKJACK", _font,&yellow,_renderer);
                else
                    RenderText(x-CARD_WIDTH*_pos_house_hand,y+60,house_pts, _font,&white,_renderer);

    // just one card ?: draw a card face down

}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(NODE_PLAYERS *_head, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int pos, x, y, card, draw_card;
    NODE_PLAYERS *aux;
    STACK_NODE *tmp;
    // for every card of every player
    for ( aux=_head; aux!=NULL; aux=aux->next)
    {
        card=0;
        for ( tmp = aux->jogador.mao_back; tmp!=NULL ; tmp=tmp->prev)
        {
            // draw all cards of the player: calculate its position: only 4 positions are available !
            pos = card % 4;
            x = (int) aux->jogador.player_pos*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15;
            y = (int) (0.55f*HEIGHT_WINDOW)+10;
            if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
            if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
            draw_card=(tmp->carta.valor)+(13*(tmp->carta.naipe));
            // render it !
            RenderCard(x, y, draw_card, _cards, _renderer);
            card++;
        }
    }
}

/**
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param _x X coordinate of the card position in the window
 * \param _y Y coordinate of the card position in the window
 * \param _num_card card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;
    // render it !

        card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
        SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);
         // destroy everything
        SDL_DestroyTexture(card_text);

}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
    int i;
    char filename[STRING_SIZE];

     // loads all cards to an array
    for (i = 0 ; i < MAX_DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * UnLoadCards: unloads all card images of the memory
 * \param _cards vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    int i;
    for (i = 0 ; i < MAX_DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
	SDL_Texture *text_IST;
	SDL_Rect boardPos;

    // space occupied by the logo
	boardPos.x = x;
	boardPos.y = y;
	boardPos.w = _logoIST->w;
	boardPos.h = _logoIST->h;

    // render it
	text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
	SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
	SDL_DestroyTexture(text_IST);
	return _logoIST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;
	solidRect.x = x;
	solidRect.y = y;
    // create a surface from the string text with a predefined font
	text_surface = TTF_RenderText_Blended(_font,text,*_color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
	}
    // create texture
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
	// Init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
	window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
	}

	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

	return renderer;
}
