#ifndef AI_H_
#define AI_H_
/** AI.h / AI.c -> Ficheiro que contém funções relativas ao funcionamento das EA**/

/** AI_bet_change -> Altera a aposta de um jogador com base na contagem das cartas distribuidas
 * \param _decks_left -> Número de baralhos por distribuir
 * \param _contagem -> Contagem das cartas distribuidas
 * \param _crr_player -> Jogador atual
**/
void AI_bet_change(int _decks_left, int _contagem, NODE_PLAYERS *_crr_player);

 /**
 *Load_AI -> Função que inicializa e carrega a matriz das decisões das EA
 *\param  ai_comfig_file -> Nome do ficheiro de configuração das EA
**/
int ** Load_AI(char *ai_config_file);

/** get_decision -> Função que acede à matriz das acções, e retorna a decisão correspondente (0- Stand 1-Hit 2- Surr 3-Double)
 * \param action -> Matriz que contém as decisões das EA
 * \param _player -> Jogador que a decisão vai afetar
 * \param _house -> Jogador da casa
**/
int get_decision(int **action, NODE_PLAYERS *_player, CASA *_house);

/** decision_AI -> Função que realiza a acção correta ao devido jogador Artificial, caso este passe a jogada é retornado o ponteiro do próximo jogador
 * \param _crr_player -> jogador que vai ser afetado pela decisão
 * \param _wait_4_ng -> "Jogador" que indica que o jogo encontra-se no fim
 * \param _deck_head -> Apontador para o inicio do baralho
 * \param _nbaralhos -> Número de baralhos inicialmente configurados
 * \param _mao_back -> Apontador para a primeira carta da mão do jogador (_crr_player)
 * \param _pontos -> Pontos do jogador atual (Passados por refrência)
 * \param _ases -> Ases "ativos" na mao do jogador
 * \param _action -> Matriz com as acções das EA
 * \param _house -> Jogador da casa
 * \param _contagem -> Contagem das cartas distribuídas (Estratégia Hi-Lo)
 * \param _decks_left -> Número de baralhos a distribuir
**/
NODE_PLAYERS *decision_AI(NODE_PLAYERS *_crr_player, NODE_PLAYERS * _wait_4_ng, NODE_CARTAS **_deck_head, int _nbaralhos, STACK_NODE **_mao_back, int *_pontos, int *_ases, int **_action, CASA *_house, int *_contagem, int *_decks_left);

#endif
