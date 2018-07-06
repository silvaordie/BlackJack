#include <stdlib.h>
#include <stdio.h>
#include "ConstEstrut.h"
#include "Data.h"
#include "Gameplay.h"
/** AI_bet_change -> Altera a aposta de um jogador com base na contagem das cartas distribuidas
 * \param _decks_left -> Número de baralhos por distribuir
 * \param _contagem -> Contagem das cartas distribuidas
 * \param _crr_player -> Jogador atual
**/
void AI_bet_change(int _decks_left, int _contagem, NODE_PLAYERS *_crr_player)
{
    int new_wage=0;
    //Caso existam baralhos por distribuir, a contagem é dividade pelo número destes
    if(_decks_left>0)
        new_wage=( _contagem / _decks_left );
    //Caso contrario a contagem passa diretamente
    else
        new_wage= _contagem;

    //Caso seja uma EA a jogar
    if(_crr_player->jogador.tipo==1 )
    {   //Caso não tenha dinheiro suficiente, o jogador joga com a aposta máxima (2/5 do dinheiro)
        if( ( _crr_player->jogador.aposta*2*new_wage >= ( 2*_crr_player->jogador.banco)/5 ) || _crr_player->jogador.aposta>= _crr_player->jogador.banco )
        {   //Caso o jogador poss baixar a aposta
             if((2*_crr_player->jogador.banco)/5 > 2)
                  _crr_player->jogador.aposta= (3*_crr_player->jogador.banco)/5 ;
        }
        else
        {   //Caso contrário, o jogador duplica a aposta o número de vezes da contagem
            if(  (_crr_player->jogador.aposta*new_wage <= ( 3*_crr_player->jogador.banco)/5 ) && new_wage>0 )
            {
                _crr_player->jogador.aposta=_crr_player->jogador.aposta*2*new_wage;
            }
        }
    }
}
/**
 *Load_AI -> Função que inicializa e carrega a matriz das decisões das EA
 *\param  ai_comfig_file -> Nome do ficheiro de configuração das EA
**/
int ** Load_AI(char *ai_config_file)
{
    FILE *config_file=NULL;
    char c_lido;
    int i=0;
    int **action=NULL;

    //Criar uma matriz 18x10 que contem as decisões dos jogadores EA
    action=(int **)malloc(ROWS_AI*sizeof(int *));
    if(action==NULL)
    {
        puts("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    for(i=0;i<=ROWS_AI;i++)
    {
        action[i]=(int *)malloc(COLUMS_AI*sizeof(int));
        if(action[i]==NULL)
        {
            puts("Erro ao alocar memória");
            exit(EXIT_FAILURE);
        }
    }
    //Abertura do ficheiro de configuração das EA para leitura
    config_file=fopen(ai_config_file,"r");
    i=0;
    while( !feof(config_file) )
    {
        //Ler o próximo caracter do ficheiro e guardá-lo na matriz das acções
        c_lido=fgetc(config_file);
        //Apenas lê os caracteres que "interessam"
        if(c_lido>= '0' && c_lido<= '3' && c_lido!= EOF)
        {//Converter o caracter em inteiro
            action[i/10][i%10]=c_lido-'0';
            i++;
        }
    }
    //Fechar o ficheiro de configuração das EA
    fclose(config_file);

    return action;
}

/** get_decision -> Função que acede à matriz das acções, e retorna a decisão correspondente (0- Stand 1-Hit 2- Surr 3-Double)
 * \param action -> Matriz que contém as decisões das EA
 * \param _player -> Jogador que a decisão vai afetar
 * \param _house -> Jogador da casa
**/
int get_decision(int **action, NODE_PLAYERS *_player, CASA *_house)
{
    int coluna, linha;
    //Escolha da coluna da matriz das decisões (Consoante os pontos da casa)
    if(_house->mao_top->carta.valor == 12)
        coluna = 9;
    else
        coluna=convert(_house->mao_top->carta.valor)-2;
    //Escolha da linha da matriz das decisões (Consoante os pontos do jogador)
    //Hard
    if(_player->jogador.ases>0)
    {
        if(_player->jogador.pontos <19)
        {
            linha=_player->jogador.pontos - 2;
        }
        else
            linha=17;
    }
    //Soft
    else
    {
        if(_player->jogador.pontos <8)
        {
            linha=0;
        }
        else
        {
            if(_player->jogador.pontos>17)
            {
                linha=9;
            }
            else
            {
                linha=_player->jogador.pontos - 8;
            }
        }
    }
    //Retorna a decisão a efetuar
    return action[linha][coluna];
}
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
NODE_PLAYERS *decision_AI(NODE_PLAYERS *_crr_player, NODE_PLAYERS * _wait_4_ng, NODE_CARTAS **_deck_head, int _nbaralhos, STACK_NODE **_mao_back, int *_pontos, int *_ases, int **_action, CASA *_house, int *_contagem, int *_decks_left)
{
    //Caso tenha feito bust o jogador passa a jogada forsosamente
    if(_crr_player->jogador.pontos== 0)
    {
        _crr_player=_crr_player->next;
    }
    //Caso contrário efetua a jogada
    else
    {
        switch( get_decision(_action,_crr_player,_house) )
        {
        //Stand
        case(0):
        {
            _crr_player=stand(_crr_player,_wait_4_ng);
            break;
        }
        //Hit
        case(1):
        {
            hit(_crr_player,_wait_4_ng,_deck_head,_nbaralhos,_mao_back,_contagem,_decks_left);
            break;
        }
        //Surrender
        case(2):
        {
            if( _crr_player->jogador.mao_top->next == _crr_player->jogador.mao_back )
                _crr_player=surrender(_crr_player,_wait_4_ng);
            //Caso não possa fazer surrender, o jogador faz hit
            else
                hit(_crr_player,_wait_4_ng,_deck_head,_nbaralhos,_mao_back, _contagem, _decks_left);

            break;
        }
        //Double
        case(3):
        {// O jogador so pode fazer double se tiver 2 cartas, e se puder duplicar a sua aposta
            if( _crr_player->jogador.mao_top->next == _crr_player->jogador.mao_back && _crr_player->jogador.banco>=2*_crr_player->jogador.aposta )
                _crr_player=double_play(_crr_player,_wait_4_ng,_deck_head,_nbaralhos,_mao_back,_pontos,_ases, _contagem, _decks_left);
            else
                //Caso não possa fazer double:
               if(_crr_player->jogador.ases>0 && _crr_player->jogador.pontos == 18)
                {//O jogador faz stand
                    _crr_player=stand(_crr_player,_wait_4_ng);
                }
                else
                {//O jogador faz hit
                    hit(_crr_player,_wait_4_ng,_deck_head,_nbaralhos,_mao_back, _contagem, _decks_left);
                }
            break;
        }
        default:break;
        }
    }
    //Caso tenha passado a jogada, retorna o novo jogador atual
    return _crr_player;
}
