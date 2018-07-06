#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
//**Gameplay.h / Gameplay.c -> Ficheiros que contêm funções relativas ao funcionamento do jogo em geral**//

/** calc_points -> Função que calcula os pontos do jogador ou da casa, dependendo do jogador fornecido
 * \param _card -> Carta que acabou de ser distribuida
 * \param _player -> Jogador que recebeu a carta
 * \param _house -> Jogador da casa
**/
void calc_points(CARTA _card, NODE_PLAYERS *_player, CASA *_house);

/** ng_setup -> Função que prepara a mesa para um jogo novo
        - Limpa mas mãos dos jogadores e da casa
        - Faz reset dos ases de cada jogador e da casa, assim como os pontos de todos e o estado dos jogadores
        - Distribui 2 cartas a cada jogador e à casa
        - Remove da mesa os jogadores que já não têm capacidade de permanecer no jogo
        - Atualiza a lista de jogadores removidos da mesa
 * \param _players_head -> Ponteiro para o primeiro jogador da lista dos jogadores
 * \param _deck_head -> Ponteiro para o primeira carta do baralho
 * \param _house -> Ponteiro para o jogador da casa
 * \param nbaralhos -> Número de baralhos presentes no ficheiro de configuração inicial
 * \param _removidas -> Ponteiro para o inicio da lista dos jogadores removidos da mesa
 * \param _dekcs_left -> Número de baralhos por distribuir
 * \param _contagem -> Contagem das cartas distribuidas (Estratégia Hi-Lo)
**/
void ng_setup(NODE_PLAYERS **_players_head, NODE_CARTAS **_deck_head,CASA *_house, int nbaralhos, NODE_PLAYERS **_removidos, int *_decks_left, int *_contagem);

/** end_game -> Função que finaliza o jogo
            -Realiza a jogada da casa
            -Determina quem ganhou, perdeu, empatou e realizou blackjack
            -Atualiza o valor do dinheiro do jogador consoante o seu estado
            -Atualiza o valor da aposta das EA
 * \param _house -> Ponteiro para o jogador da casa
 * \param _players_head -> Ponteiro para o primeiro jogador da lista dos jogadores
 * \param _deck_head -> Ponteiro para a primeira carta do baralho
 * \param _nbaralhos -> Número de baralhos lidos na configuração inicial
 * \param _contagem -> Contagem das cartas distribuidas
 * \param _decks_left -> Número de baralhos por distribuir
**/
 void end_game(CASA *_house, NODE_PLAYERS **_players_head,  NODE_CARTAS **_deck_head, int _nbaralhos, int *_contagem, int *_decks_left );

 /** new_wage -> Função que altera a aposta de um determinado jogador
 * \param _head -> Jogador da casa
 * \param _crr_player -> Jogador atual
 * \param _wait_4_ng -> "Jogador" que simboliza o final do jogo
**/
void new_wage(NODE_PLAYERS *_head, NODE_PLAYERS *_crr_player, NODE_PLAYERS *_wait_4_ng);

/** stand -> Função que passa a jogada para o jogador seguinte
 * \param -> _crr_player -> Jogador que se encontra a jogar
 * \param -> "Jogador" que indica o final do jogo
**/
NODE_PLAYERS *stand(NODE_PLAYERS * _crr_player, NODE_PLAYERS *_wait_4_ng);

/** hit -> Função que atribui uma carta ao jogador atual
 * \param _crr_player -> Jogador atual
 * \param _wait_4_ng -> "Jogador" que indica o final do jogo
 * \param _deck_head -> Apontador para a primeira carta do baralho
 * \param _nbaralhos -> Número de baralhos lido no icheiro de configuração
 * \param _mao_back -> Ponteiro para a primeira carta da mao do jogador
 * \param _contagem -> Contagem das cartas distribuidas(estratégia Hi-Lo)
 * \param _decks_left -> Número de baralhos por distribuir
**/
void hit(NODE_PLAYERS *_crr_player, NODE_PLAYERS *_wait_4_ng, NODE_CARTAS **_deck_head, int _nbaralhos, STACK_NODE **_mao_back, int *_contagem, int *_decks_left);

/** double_play -> Função que realiza a acção double
            - Dar uma carta e passar a jogada
 * \param _crr_player -> Jogador atual
 * \param _wait_4_ng -> "Jogador" que simboliza o final do jogo
 * \param _deck_head -> Ponteiro para a primeira carta do baralho
 * \param _nbaralhos -> Número de baralhos lidos no ficheiro de configuração
 * \param _mao_back -> Ponteiro para a primeira carta da mao do jogador
 * \param _pontos -> Pontos do jogador atual
 * \param _ases -> Número de ases efetivos na mao do jogador
 * \param _contagem -> Contagem das cartas distribuidas (Estrategia Hi-Lo)
 * \param _decks_left -> Número de baralhos a distribuir
**/
NODE_PLAYERS *double_play(NODE_PLAYERS *_crr_player, NODE_PLAYERS * _wait_4_ng, NODE_CARTAS **_deck_head, int _nbaralhos, STACK_NODE **_mao_back, int *_pontos, int *_ases, int *_contagem, int *_decks_left);

/** surrender -> Função que permite que o jogador desista do jogo atual
 * \param _crr_player -> Jogador atual
 * \param _wait_4_ng -> "Jogador que simboliza o fim do jogo"
**/
NODE_PLAYERS * surrender(NODE_PLAYERS *_crr_player, NODE_PLAYERS *_wait_4_ng);

/** add_player -> Função que adiciona um jogador à mesa através do rato
 * \param _players_head -> Ponteiro para o primeiro jogador da lista
 **/
int add_player(NODE_PLAYERS ** _players_head);

#endif
