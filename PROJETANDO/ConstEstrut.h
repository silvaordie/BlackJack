#ifndef CONSTESTRUT_H_
#define CONSTESTRUT_H_

//Definição das constantes a utilizar
#define STRING_SIZE 100       // max size for some strings
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window heigh
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 500       // initial position of the window: x
#define WINDOW_POSY 250       // initial position of the window: y
#define EXTRASPACE 150
#define MARGIN 5
#define MAX_PLAYERS 4         // number of maximum players
#define MAX_DECK_SIZE 52
#define MAX_DECKS 8           //Máximo de baralhos em jogo
#define MIN_DECKS 4           //Minimo de baralhos em jogo
#define MAX_MONEY 500         //Máximo de dinheiro que pode ser lido do ficheiro de configuação
#define ROWS_AI 18            //Número de linhas da matriz das EAS
#define COLUMS_AI 10          //Nmero de colunas da matriz das EA
#define READ_L 256            //Número de caracteres a ler numa linha
#define N_BARALHO 52          //Número de cartas existente num baralho
#define NOME_MAX 8            //Máximo de caracteres no nome de um jogador
#define AI_DELAY 700          //O jogador EA espera 1 s(300ms + 700ms) ate realizar a proxima iogada
/**CARTA -> Estrutura que representa um carta
    - valor -> ID da carta
    - naipe -> Naipe da carta
**/
typedef struct carta{
    int valor;
    int naipe;
}CARTA;

/**NODE_CARTAS -> Estrutura que representa um lista de cartas simplesmente ligada (BARALHO)
    - carta -> Carta presente no nó
    - next -> Ponteiro para o nó seguinte
**/
typedef struct node_carta{
    CARTA carta;
    struct node_carta *next;
}NODE_CARTAS;

/**STACK_NODE -> Estrutura que representa um apilha de cartas duplamente ligada
    - carta -> Carta presente no nó
    - next -> Ponteiro para o nó seguinte
    - prev -> Ponteiro para o nó anterior
**/
typedef struct stack_node{
    CARTA carta;
    struct stack_node *next;
    struct stack_node *prev;
}STACK_NODE;

/**STATS -> Estrutura que representa as estatisticas de um jogador
    - wins -> Número de jogos ganhos pelo jogador
    - losses -> Número de jogos perdidos pelo jogador
    - ties -> Número de jogos empatados pelo jogador
**/
typedef struct stats{
    int wins;
    int losses;
    int ties;
}STATS;
/**JOGADOR -> Estrutura que representa um jogador
    - nome -> Nome do jogador
    - player_pos -> Posição do jogador na mesa
    - tipo -> Tipo do jogador (0- HU , 1- EA)
    - banco -> Valor do dinheiro do jogador
    - aposta -> Valor da aposta do jogador
    - estado -> Estado do jogador (0- BJ normal, 1- Double, 2- Surrender)
    - pontos -> Pontos do jogador
    - ases -> Número de ases efetivos na mão do jogador
    - estatistica -> Estatistica do jogador
    - mao_top -> Ponteiro para a ultima carta da mao do jogador
    - mao_back -> Ponteiro para a primeira carta na mao do jogador
**/
typedef struct jogador{
    char nome[NOME_MAX];
    int player_pos;
    int tipo;
    float banco;
    int aposta;
    int estado;
    int pontos;
    int ases;
    STATS estatistica;
    STACK_NODE *mao_top;
    STACK_NODE *mao_back;
}JOGADOR;

/**NODE_PLAYERS -> Estrutura que representa uma lista de jogadores simplesmente ligada
    - jogador -> Jogador presente no nó
    - next -> Ponteiro para o próximo nó
**/
typedef struct node_players{
    JOGADOR jogador;
    struct node_players *next;
}NODE_PLAYERS;

/**CASA -> Estrutura que representa o jogador da casa
    - mao_top -> Ponteiro para a ultima carta da mao da casa
    - mao_back -> Ponteiro para a primeira carta da ma da casa
    - money_h -> Valor do balanço da casa
    - pontos -> Valor dos pontos da casa
    - ases -> Número de ases efetivos presentes na mão da casa
    - estado -> Estado do jogador da casa (0- A espera dos jogadores, 1- A jogar)
**/
typedef struct casa{
    STACK_NODE *mao_top;
    STACK_NODE *mao_back;
    float money_h;
    int pontos;
    int ases;
    int estado;
}CASA;

#endif
