#ifndef DATA_H_
#define DATA_H_
//**Data.h / Data.c -> Ficheiros que contêm funções relativas ao tratamento de dados**//

/** load_decks -> Função que carrega novos baralhos
 * \param _head -> Ponteiro para a primeira carta do baralho
 * \param _nbaralhos -> Número de baralhos lidos do ficheiro de configuração
**/
void load_decks(NODE_CARTAS **_head, int _nbaralhos );

/** get_player_info -> Função que lê do ficheiro de configuração, as informações do jogador a ser criado
                -Nome
                -Dinheiro inicial
                -Aposta
                -Tipo
 * \param _node -> Jogador a ser criado
 * \param _fp -> Ficheiro que contém as informações dos jogadores
**/
void get_player_info(NODE_PLAYERS *_node,FILE *_fp );

/** load_players -> Carregamento de n jogadores, lidos do ficheiro de configuração
 * \param _head -> Ponteiro para o primeiro jogador da lista
 * \param _house -> Jogador da casa
 * \param _njogadores -> Número de jogadores inicias
 * \param _fp -> Ficheiro de configuração
**/
void load_players(NODE_PLAYERS **_head, CASA **_house,int _njogadores,FILE *_fp);

/** param_in -> Função que lê os parametros do jogo
                -Número de baralhos
                -Número de jogadores
                - INformação de cada jogador
 * \param _head -> Ponteiro para o primeiro jogador da lista de jogadores
 * \param _house -> Jogador da casa
 * \param _nbaralhos -> Número de baralhos inicial
 * \param _njogadores -> Número de jogadores inicial
 * \param file_name -> Nome do ficheiro de configuração do jogo
**/
int param_in(NODE_PLAYERS **_head,CASA **_house, int *_nbaralhos, int *_njogadores, char *file_name);

/** convert -> Função que converte o ID da carta no seu peso correspondente
 * \param _valor -> Valor da carta a ser convertido
**/
int convert(int _valor);

/** deck_card -> Função que retira um carta ao calhas do baralho e retorna o seu valor e naipe
 * \param _head ->Ponteiro para a primeira carta do baralho
 * \param _nbaralhos -> Número de baralhos lidos do ficheiro de configuração
 * \param _contagem -> Contagem das cartas distribuidas (Estratégia Hi-Lo)
 * \param _decks_left -> Número de baralhos por distribuir
**/
CARTA deck_card(NODE_CARTAS **_head, int _nbaralhos, int *_contagem, int *_decks_left);

/** give_card -> Função que insere no topo da pilha das cartas do jogador, a carta retirada do baralho
 * \param _mao_top -> Ponteiro para a ultima carta da mão do jogador
 * \param _mao_back -> Ponteiro para a primeira carta da mão do jogador
 * \param _carta -> Carta qu foi retirada do baralho
**/
STACK_NODE * give_card(STACK_NODE *_mao_top, STACK_NODE **_mao_back , CARTA _carta);

/** remove_card -> Função que retira uma carta do topo da pilha do jogador
 * \param top -> Ponteiro para a ultima carta da mão do jogador
 * \param _back -> Ponteiro para a primeira carta do jogador
**/
void remove_card(STACK_NODE **top, STACK_NODE **_back);

/** remove_player -> Função que remove de uma lista, um determinado jogador
 * \param _head -> Ponteiro para o primeiro jogador da lista
 * \param _player -> Ponteiro para o jogador a ser removido da lista
**/
JOGADOR remove_player(NODE_PLAYERS **_head, NODE_PLAYERS *_player);

/** insert_player -> Função que adiciona ao final de uma lista de jogadores, um jogador novo
 * \param _head -> Ponteiro para o primeiro jogador da lista
 * \param _player -> Informações do jogador a ser adicionado à lista
**/
void insert_player(NODE_PLAYERS **_head, JOGADOR _player );

/** read_player_info -> Função que lê do stdin, as informações do jogador que se pretende ser adicionado
            -Posição
            -Nome
            -Aposta
            -Tipo
            -Dinheiro inicial
 * \param _x -> Posição do rato nas coordenadas x quando foi sinalizada a intenção de adicionar um jogador
 * \param _y -> Posição do rato nas coordenadas y quando foi sinalizada a intenção de adicionar um jogador
 * \param _head -> Ponteiro para o inicio da lista de jogadores
**/
int read_player_info(int _x, int _y, NODE_PLAYERS **_head);

/** Free_All -> Função que liberta toda a memória alocada (excluindo a matriz das acções das EA)
 * \param _players_head -> Ponteiro para o primeiro jogador da lista de jogadores
 * \param _deck_head -> Ponteiro para a primeira carta do baralho
 * \param _removed_players -> Ponteiro para o primeiro jogador que foi removido
 * \param _house -> Jogador da casa
**/
void Free_All(NODE_PLAYERS **_players_head, NODE_CARTAS **_deck_head, NODE_PLAYERS **_removed_players, CASA **_house );

/** file_write -> Função que escreve no ficheiro de estatisticas, todas as estatisticas dos jogadores que participaram no jogo, e o balanço final da casa
 * \param players_head -> Ponteiro para o primeiro jogador da lista de jogadores
 * \param removed_players -> Ponteiro para o primeiro jogador da lista de jogadores removidos
 * \param _money_h -> Balanço do casa
**/
void file_write(NODE_PLAYERS *_players_head, NODE_PLAYERS *_removed_players, float _money_h);
#endif
