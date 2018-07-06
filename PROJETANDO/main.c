/**                                 PROJETO DE PROGRAMAÇÃO -> ENTREGA FINAL                               **/
/**                             Autores: José Silva 84109, Pedro Almeida 84164                            **/
/**                                        DATA LIMITE: 21/5                                              **/
/** Jogo de Black_Jack com múltiplos jogadores(maximo de 4 na mesa)com funcionalidades acrescentadas e EA **/
/**           O programa foi testado várias vezes, e parece estar tudo de acordo com o enunciado          **/
/**                       A ORDEM DO JOGO É DETERMINADA PELA ORDEM DE CHEGADA À MESA                      **/

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "ConstEstrut.h"
#include "Data.h"
#include "AI.h"
#include "Gameplay.h"
#include "Graphics.h"

int main( int argc, char* args[] )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    NODE_PLAYERS *wait_4_ng=(NODE_PLAYERS *)malloc(sizeof(NODE_PLAYERS)); //"Jogador" que indica o final do jogo
    NODE_PLAYERS *players_head=NULL;                                      //Ponteiro para o primeiro jogador da lista
    NODE_PLAYERS *removed_players=NULL;                                   //Ponteiro para o primeiro jogador da lista dos removidos
    CASA *house_player=NULL;                                              //Jogador da casa
    NODE_PLAYERS *crr_player=NULL;                                        //Jogador que se encontra a jogar
    NODE_CARTAS *deck_head=NULL;                                          //Ponteiro para a primeira carta do baralho
    int nbaralhos=0, njogadores=0, decks_left, contagem=0;                //numero de baralhos inicial / numero de jogadores inicial/ número de baralhos por distribuir / contagemdas cartas distribuidas
    int **action=NULL;
    //srand(456); -> Devido a um problema de origem desconhecida, a semente aleatoria é gerada na 1ª chamada da função deck_card
    int AI_delay= AI_DELAY;

    //Leitura dos parâmetros do jogo
    param_in(&players_head,&house_player, &nbaralhos, &njogadores, args[1]);
    //Carregamento inicial dos baralhos
    load_decks(&deck_head,nbaralhos);
	// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);
    //Prepara um jogo novo
    ng_setup(&players_head , &deck_head , house_player , nbaralhos,&removed_players, &decks_left, &contagem);
    //Carrega a matriz das acções das EA
    action=Load_AI(args[2]);
    //Dá a vez ao 1º Jogador
    crr_player=players_head;

 	while( quit == 0 )
    {   //Caso o jogo esteja a decorrer e a casa tenha um BJ, acaba o jogo
        if( house_player->pontos==21 && crr_player!=NULL && crr_player!=wait_4_ng )
        {
            crr_player=NULL;
        }
        //Caso todos os jogadores tenham feito a sua jogada / Casa tenha feito BJ
        if( crr_player==NULL)
        {   //Termina o jogo
            end_game( house_player, &players_head, &deck_head, nbaralhos , &contagem, &decks_left);
            //Sinaliza que o jogo terminou
            crr_player = wait_4_ng;
        }
        else
        {   //Caso seja uma EA
            if(crr_player->jogador.tipo == 1)
            {   //Espera mais 700 ms(valor inicial)
                SDL_Delay( AI_delay);
                //Realiza a decisão correspondente as cartas distribuidas
                crr_player=decision_AI(crr_player,wait_4_ng,&deck_head, nbaralhos, &(crr_player->jogador.mao_back), &(crr_player->jogador.pontos), &(crr_player->jogador.ases), action, house_player , &contagem , &decks_left);
            }
            else
            {   //Caso um jogador humano se encontre a jogar e este tenha excedido ou igualado 21 pontos
                if( ( crr_player->jogador.pontos==0  || crr_player->jogador.pontos>=21 ) && crr_player!= wait_4_ng && crr_player!=NULL )
                {   //Caso tenha feito bust, perde o valor da jogada
                    if( crr_player->jogador.pontos!=21 )
                        crr_player->jogador.pontos=0;
                    //Passa a jogada
                    crr_player=crr_player->next;
                }
            }

        }
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
			if( event.type == SDL_QUIT )
            {
                quit=1;
            }
			else if ( event.type == SDL_KEYDOWN )
			{
				switch ( event.key.keysym.sym )
				{
                    case SDLK_q:
                    {
                        quit=1;
                        break;
                    }
					case SDLK_s:
                    {   //Stand
                        if(crr_player != NULL && crr_player != wait_4_ng)
                        {
                            if(crr_player->jogador.tipo == 0)
                                crr_player=stand(crr_player,wait_4_ng);
                        }
                        break;
                    }
					case SDLK_h:
					{  //Hit
					    if(crr_player != NULL && crr_player != wait_4_ng)
                        {
                            if(crr_player->jogador.tipo == 0)
                                hit(crr_player,wait_4_ng,&deck_head,nbaralhos, &(crr_player->jogador.mao_back) , &contagem, &decks_left);
                        }
                        break;
					}
                    case SDLK_d:
                    {   //Double
					    if(crr_player != NULL && crr_player != wait_4_ng)
                        {
                            if(crr_player->jogador.tipo == 0)
                                crr_player=double_play(crr_player,wait_4_ng,&deck_head,nbaralhos, &(crr_player->jogador.mao_back), &(crr_player->jogador.pontos), &(crr_player->jogador.ases) , &contagem, &decks_left);
                        }
                        break;
                    }
                    case SDLK_r:
                    {   //Surrender
                        if(crr_player != NULL && crr_player != wait_4_ng)
                        {
                            if(crr_player->jogador.tipo == 0)
                                crr_player=surrender(crr_player,wait_4_ng);
                        }
                        break;
                    }
                    case SDLK_b:
                    {   //Bet
                        if(crr_player == wait_4_ng)
                            new_wage(players_head, crr_player,wait_4_ng);
                        break;
                    }
                    case SDLK_n:
                    {   //New Game
                        if(crr_player==wait_4_ng && players_head!=NULL)
                        {   //Caso o jogo tenha terminado, prepara-se um jogo novo
                            ng_setup (&players_head , &deck_head , house_player , nbaralhos, &removed_players , &decks_left, &contagem);
                            crr_player=players_head;
                        }
                        break;
                    }
                    case SDLK_a:
                    {   //Add PLayer
                        if(crr_player==wait_4_ng)
                        {   //Caso o jogo tenha terminado
                            puts("Selecione a posição do novo jogador com o rato");
                            if (add_player(&players_head) )
                            {
                                SDL_ShowSimpleMessageBox(0, "Erro", "Local Invalido !", window );
                            }
                        }
                        break;
                    }
                    case SDLK_UP:
                    {   //Inc Delay
                        if(AI_delay<2700)
                            AI_delay+=100;
                        break;
                    }
                    case SDLK_DOWN:
                    {   //Dec Delay
                        if(AI_delay>0)
                            AI_delay-=100;
                        break;
                    }
					default:
						break;
                    }
				}
        }
        // render game table
        RenderTable(players_head,crr_player, serif, imgs, renderer, AI_delay);
        // render house cards
        RenderHouseCards(house_player, cards, renderer, serif);
        // render player cards
        RenderPlayerCards(players_head, cards, renderer);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
    	// add a delay
		SDL_Delay( delay );
    }

    // free memory allocated for images and textures and close everything including fonts
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
    //Escreve o icheiro de estatisticas
	file_write(players_head, removed_players, house_player->money_h);
    //Liberta a memoria alocada
    Free_All(&players_head,&deck_head,&removed_players,&house_player);
    //Liberta a matriz das acções das EA
    int i;
    for(i=0; i<19; i++)
    {
        free(action[i]);
    }
    free(action);
	return EXIT_SUCCESS;
}

