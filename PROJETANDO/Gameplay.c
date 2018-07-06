#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ConstEstrut.h"
#include "Data.h"
#include "AI.h"
#include "Graphics.h"

/** calc_points -> Função que calcula os pontos do jogador ou da casa, dependendo do jogador fornecido
 * \param _card -> Carta que acabou de ser distribuida
 * \param _player -> Jogador que recebeu a carta
 * \param _house -> Jogador da casa
**/
void calc_points(CARTA _card, NODE_PLAYERS *_player, CASA *_house)
{
    //Caso tenha sido fornecido um jogador, calculam-se os pontos do mesmo
    if(_player!=NULL)
    {
        //Caso a carta distribuida tenha sido um ás, incrementa-se a contagem de ases do jogador
        if(_card.valor == 12)
            _player->jogador.ases++;

        //Soma-se o valor real da carta aos pontos do jogador
        _player->jogador.pontos+=convert(_card.valor);

        //Caso a mão do jogador seja "soft" e tenha-se excedido os pontos, reduz-se a contagem dos pontos
        while(_player->jogador.pontos > 21 && _player->jogador.ases>0)
        {
            _player->jogador.pontos-=10;
            _player->jogador.ases--;
        }
        //Caso tenha feito BUST, a mão do jogador perde o valor
        if(_player->jogador.pontos > 21)
        {
            _player->jogador.pontos=0;
        }
    }
    //Caso tenha sido fornecido o jogador da casa, calculam-se os seus pontos
    //(Procedimento igual ao do jogador, apenas altera-se o acesso aos pontos e ases)
    else if(_house != NULL)
    {

        if(_card.valor == 12)
            _house->ases++;

        _house->pontos+=convert(_card.valor);

        while(_house->pontos>21 && _house->ases>0)
        {
            _house->pontos-=10;
            _house->ases--;
        }

        if(_house->pontos>21)
        {
            _house->pontos=0;
        }
    }
}

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
void ng_setup(NODE_PLAYERS **_players_head, NODE_CARTAS **_deck_head,CASA *_house, int nbaralhos, NODE_PLAYERS **_removidos, int *_decks_left, int *_contagem)
{
    NODE_PLAYERS *aux=NULL;
    NODE_PLAYERS *tmp=NULL;
    CARTA card;
    card.valor=-1;
    int i=0;

    aux=*_players_head;
    while(aux!=NULL)
    {   //Limpa a mão de cada jogador e faz reset dos ases, pontos e estado do jogador
        while(aux->jogador.mao_top!=NULL)
        {
            remove_card(&(aux->jogador.mao_top), &(aux->jogador.mao_back) ) ;
        }
        aux->jogador.pontos=0;
        aux->jogador.estado=0;
        aux->jogador.ases=0;
        // Caso o jogador não consiga apostar mais, sai do jogo e é inserida na lista de jogadores removidos
        if(aux->jogador.aposta>aux->jogador.banco)
        {
            tmp=aux;
            aux=aux->next;
            insert_player(_removidos, remove_player(_players_head, tmp) );
        }
        else
            aux=aux->next;
    }
    //Limpa a mao da casa e faz reset dos pontos, ases, e do estado
    while(_house->mao_top!=NULL)
    {
        remove_card(&(_house->mao_top), &(_house->mao_back) );
    }
    _house->pontos=0;
    _house->ases=0;
    _house->estado=0;
    //Caso ainda existam jogadores a jogar, distribui 2 cartas a cada um e à casa
    if(_players_head!=NULL)
    {
        for(i=0;i<2;i++)
        {
            card=deck_card(_deck_head,nbaralhos, _contagem, _decks_left);
            _house->mao_top=give_card( _house->mao_top, &(_house->mao_back), card );
            calc_points(card,NULL,_house);
            aux=*_players_head;
            while(aux!=NULL)
            {
                card=deck_card(_deck_head,nbaralhos,_contagem, _decks_left );
                aux->jogador.mao_top=give_card( aux->jogador.mao_top, &(aux->jogador.mao_back), card );
                calc_points(card,aux,NULL);
                aux=aux->next;
            }
        }
    }

}

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
void end_game(CASA *_house, NODE_PLAYERS **_players_head,  NODE_CARTAS **_deck_head, int _nbaralhos, int *_contagem, int *_decks_left )
{
    NODE_PLAYERS *aux=NULL;
    CARTA card;

    //Caso ainda existam jogadores e a casa não tenha atingido
    while(_house->pontos<=16 && _house->pontos>0 && _players_head!=NULL)
    {
        if(_house->pontos==0)
            break;

        card=deck_card(_deck_head,_nbaralhos, _contagem, _decks_left);
        _house->mao_top=give_card( _house->mao_top, &(_house->mao_back),card);
        calc_points(card,NULL,_house);
    }
    _house->estado=1;

    aux=*_players_head;
    while(aux!=NULL)
    {
        switch(aux->jogador.estado)
        {
        //Caso o Jogador tenha feito double
        case(1):
        {
            if(aux->jogador.pontos > _house->pontos)
            {
                aux->jogador.banco+=2*aux->jogador.aposta;
                _house->money_h-=2*aux->jogador.aposta;
                aux->jogador.estatistica.wins++;
            }
            else
            {
                if( aux->jogador.pontos==0 || (aux->jogador.pontos < _house->pontos  ) )
                {
                    aux->jogador.banco-=2*aux->jogador.aposta;
                    _house->money_h+=2*aux->jogador.aposta;
                    aux->jogador.estatistica.losses++;
                }
                else
                    aux->jogador.estatistica.ties++;

            }

            break;
        }
        //Caso o jogador tenha desistido da jogada
        case(2):
        {
            aux->jogador.banco-=0.5*aux->jogador.aposta;
            aux->jogador.estatistica.losses++;
            _house->money_h+=0.5*aux->jogador.aposta;

            break;
        }
        //Caso o jogador se encontre sobre condições de blackjack "Normais"
        default:
        {   //Caso tenha feito BlackJack
             if(aux->jogador.pontos == 21 && aux->jogador.mao_top->next == aux->jogador.mao_back )
            {   // O jogador ganha ou empata mas recebe sempre 1.5 da aposta
                aux->jogador.banco+=1.5*aux->jogador.aposta;
                _house->money_h-=1.5*aux->jogador.aposta;
                if(_house->pontos!=21)
                    aux->jogador.estatistica.wins++;
                else
                    aux->jogador.estatistica.ties++;
            }
            else
            {   //Caso contrário
                if( aux->jogador.pontos == 0 || aux->jogador.pontos<_house->pontos)
                {   //Caso tenha feito bust, ou tenha menos pontos que a casa, o jogador perde o jogo e o dinheiro da aposta
                    aux->jogador.banco-=aux->jogador.aposta;
                    _house->money_h+=aux->jogador.aposta;
                    aux->jogador.estatistica.losses++;
                }
                else
                {
                    if(aux->jogador.pontos > _house->pontos )
                    {   //Caso tenha mais pontos que a casa, o jogador ganha e recebe o dinheiro da aposta
                        aux->jogador.banco+=aux->jogador.aposta;
                        _house->money_h-=aux->jogador.aposta;
                        aux->jogador.estatistica.wins++;
                    }
                    if(aux->jogador.pontos==_house->pontos)
                    {   //Caso tenha os mesmos pontos que a casa, o jogador empata
                        aux->jogador.estatistica.ties++;
                    }
                }
            }
            break;
            }
        }
        //Atualiza as apostas de cada jogador do tipo EA
        AI_bet_change(*_decks_left, *_contagem, aux);

        aux=aux->next;
    }
}
/** new_wage -> Função que altera a aposta de um determinado jogador
 * \param _head -> Jogador da casa
 * \param _crr_player -> Jogador atual
 * \param _wait_4_ng -> "Jogador" que simboliza o final do jogo
**/
void new_wage(NODE_PLAYERS *_head, NODE_PLAYERS *_crr_player, NODE_PLAYERS *_wait_4_ng)
{   //Caso o jogo tenha terminado
    if(_crr_player== _wait_4_ng)
    {
        NODE_PLAYERS *aux=NULL;
        char str_aux[READ_L];
        int aposta=0;
        int str_equ=1;
        //Lẽ o jogador a atualizar a aposta
        printf("\nInsira o nome do jogador: ");
        fgets(str_aux,READ_L,stdin);
        str_aux[strlen(str_aux)-1]=0;
        aux=_head;

        //Percorre a lista de jogadores à procura do jogador introduzido
        while(aux!=NULL)
        {
            str_equ=strcmp(str_aux,aux->jogador.nome);
            if( str_equ == 0)
                break;
            aux=aux->next;
        }
        //Caso não tenha encontrado o jogador introduzido
        if(aux==NULL)
        {
            printf("\nNão existe o jogador %s !",str_aux);
            puts("\n Lista de Jogadores Atuais: \n");
            aux=_head;
            //Imprime a lista de jogadores em jogo
            while(aux!=NULL)
            {
                printf("\n %s",aux->jogador.nome);
                aux=aux->next;
            }
        }
        else
        {   //Leitura da nova aposta
            printf("\nInsira a nova aposta do jogador '%s': ",aux->jogador.nome);
            fgets(str_aux,NOME_MAX,stdin);
            sscanf(str_aux,"%i",&aposta);
            //Caso a aposta seja positiva, esta é atribuida ao jogador
            if(aposta>0)
                aux->jogador.aposta=aposta;
            else
                puts("Aposta inválida !");
        }
    }
}
/** stand -> Função que passa a jogada para o jogador seguinte
 * \param -> _crr_player -> Jogador que se encontra a jogar
 * \param -> "Jogador" que indica o final do jogo
**/
NODE_PLAYERS *stand(NODE_PLAYERS * _crr_player, NODE_PLAYERS *_wait_4_ng)
{   //Caso o jogador atual seja um jogador válido e o jogo se encontre a decorrer
    if( _crr_player!= NULL && _crr_player!=_wait_4_ng )
        _crr_player=_crr_player->next;
    //Passa a jogada
    return _crr_player;
}
/** hit -> Função que atribui uma carta ao jogador atual
 * \param _crr_player -> Jogador atual
 * \param _wait_4_ng -> "Jogador" que indica o final do jogo
 * \param _deck_head -> Apontador para a primeira carta do baralho
 * \param _nbaralhos -> Número de baralhos lido no icheiro de configuração
 * \param _mao_back -> Ponteiro para a primeira carta da mao do jogador
 * \param _contagem -> Contagem das cartas distribuidas(estratégia Hi-Lo)
 * \param _decks_left -> Número de baralhos por distribuir
**/
void hit(NODE_PLAYERS *_crr_player, NODE_PLAYERS *_wait_4_ng, NODE_CARTAS **_deck_head, int _nbaralhos, STACK_NODE **_mao_back, int *_contagem, int *_decks_left)
{
    CARTA card;
    // Caso possa receber mais uma carta
    if( _crr_player->jogador.pontos<21 &&_crr_player->jogador.pontos!=0 )
    {   //Retira uma carta do baralho
        card=deck_card( _deck_head , _nbaralhos, _contagem, _decks_left);
        //Insere a carta retirada no topo da pilha de cartas do jogador
        _crr_player->jogador.mao_top = give_card( _crr_player->jogador.mao_top , _mao_back ,card );
        //Atualiza os pontos do jogador
        calc_points(card,_crr_player,NULL);
    }
}
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
NODE_PLAYERS *double_play(NODE_PLAYERS *_crr_player, NODE_PLAYERS * _wait_4_ng, NODE_CARTAS **_deck_head, int _nbaralhos, STACK_NODE **_mao_back, int *_contagem, int *_decks_left)
{   //Caso seja um jogador válido
//Caso possua duas cartas na mão e possa duplicar a sua aposta
    if( _crr_player->jogador.mao_top->next == _crr_player->jogador.mao_back && _crr_player->jogador.banco>=2*_crr_player->jogador.aposta )
    {   //Indica que fez um double
        _crr_player->jogador.estado=1;
        //Recebe uma carta
        hit(_crr_player , _wait_4_ng, _deck_head , _nbaralhos , _mao_back, _contagem,_decks_left);
        //Caso tenha feito BUST, a sua mão perde o valor
        if(_crr_player->jogador.pontos >21)
            _crr_player->jogador.pontos=0;
        //Passa a jogada
        _crr_player=_crr_player->next;
    }

    return _crr_player;
}
/** surrender -> Função que permite que o jogador desista do jogo atual
 * \param _crr_player -> Jogador atual
 * \param _wait_4_ng -> "Jogador que simboliza o fim do jogo"
**/
NODE_PLAYERS * surrender(NODE_PLAYERS *_crr_player, NODE_PLAYERS *_wait_4_ng)
{   //Caso o jogador tenha 2 cartas na mao
    if(_crr_player->jogador.mao_top->next == _crr_player->jogador.mao_back )
    {   //Sinaliza que fez surrender
        _crr_player->jogador.estado=2;
        //Passa a jogada
        _crr_player=_crr_player->next;
    }
    return _crr_player;
}

/** add_player -> Função que adiciona um jogador à mesa através do rato
 * \param _players_head -> Ponteiro para o primeiro jogador da lista
 **/
int add_player(NODE_PLAYERS ** _players_head)
{
    int test=1;
    int x,y;
    SDL_Event event;
    //Equanto não tiver pressionado 'a' ou o rato
    while(test)
    {
        while( SDL_PollEvent( &event ) )
        {
            switch(event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
            {
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                    {
                        test=0;
                        SDL_GetMouseState(&x,&y);
                        if(read_player_info(x,y,_players_head) )
                            return 1;
                        else
                            return 0;
                        test=0;
                    }
                    default: break;
                }
                break;
            }
            case SDLK_a:
            {
                test=0;
                break;
            }
            default:break;
            }
        }

    }
    return 0;
}
