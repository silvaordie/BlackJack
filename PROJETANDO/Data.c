#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ConstEstrut.h"

/** load_decks -> Função que carrega novos baralhos
 * _param _head -> Ponteiro para a primeira carta do baralho
 * _param _nbaralhos -> Número de baralhos lidos do ficheiro de configuração
**/
void load_decks(NODE_CARTAS **_head, int _nbaralhos )
{
    int i=0;
    NODE_CARTAS *new_node=NULL;
    NODE_CARTAS *aux=NULL;
    //Carraga o número de cartas correspondente a 52* nº de baralhos
    for(i=0; i<=(_nbaralhos)*N_BARALHO;i++)
    {   //Cria uma nova carta
        new_node=( NODE_CARTAS *)malloc(sizeof(NODE_CARTAS));
        if(new_node==NULL)
        {
            puts("Erro ao alocar memória");
        }
        //Indica o valor da carta e o seu naipe
        new_node->carta.valor=i%13;
        new_node->carta.naipe=(i%MAX_DECK_SIZE)/13;
        new_node->next=NULL;
        //Caso seja a primeira carta a ser acrescentada ao baralho
        if(*_head==NULL)
        {   //É colocada no inicio da lista
            *_head=new_node;
            (*_head)->next=NULL;
            continue;
        }
        else
        {   //é introduzida no fim do baralho
            aux=*_head;
            while( (aux->next)!= NULL )
            {
                aux=aux->next;
            }
            aux->next=new_node;
        }
    }
}
/** get_player_info -> Função que lê do ficheiro de configuração, as informações do jogador a ser criado
                -Nome
                -Dinheiro inicial
                -Aposta
                -Tipo
 * _param _node -> Jogador a ser criado
 * _param _fp -> Ficheiro que contém as informações dos jogadores
**/
void get_player_info(NODE_PLAYERS *_node,FILE *_fp )
{
    char *str_aux=NULL;
    char linha[READ_L];
    int erro=0;
    //Caso não exista jogador a ler
    if(fgets(linha,READ_L, _fp) == NULL)
        erro=1;
    //Lê até '-'
    str_aux = strtok (linha,"-");
    //Caso o tipo seja 1-EA , 0-HU
    if( !strcmp (str_aux,"HU") )
        _node->jogador.tipo=0;
    else if( !strcmp (str_aux,"EA") )
        _node->jogador.tipo=1;
        else//Tipo desconhecido
            erro=1; //Sinaliza um erro

    //Guarda o nome do jogador
    str_aux = strtok (NULL,"-");
    strcpy(_node->jogador.nome, str_aux);
    if(strlen(_node->jogador.nome)> NOME_MAX)//Nome demasiado comprido
        erro=1; //Sinaliza um erro

    //Guarda o dinheiro inicial do jogador
    str_aux = strtok (NULL,"-");
    sscanf(str_aux,"%f",&(_node->jogador.banco));
    if(_node->jogador.banco > MAX_MONEY || _node->jogador.banco <=0)//Dinheiro inicial inválido
        erro=1;//Sinaliza um erro

    //Guarda o valor da aposta do jogador
    str_aux = strtok (NULL,"-");
    sscanf(str_aux,"%i",&(_node->jogador.aposta));
    if(_node->jogador.aposta>0.25*_node->jogador.banco || _node->jogador.aposta<2)//Aposta inválida
        erro=1; //Sinaliza um erro

    //Caso tenha ocorrido um erro na leitura do ficheiro
    if(erro)
    {   //Dá indicação da ocorrência de um erro ao utilizador e sai do programa
        puts("Erro no ficheiro de configuração");
        exit(EXIT_FAILURE);
    }
}

/** load_players -> Carregamento de n jogadores, lidos do ficheiro de configuração
 * _param _head -> Ponteiro para o primeiro jogador da lista
 * _param _house -> Jogador da casa
 * _param _njogadores -> Número de jogadores inicias
 * _param _fp -> Ficheiro de configuração
**/
void load_players(NODE_PLAYERS **_head, CASA **_house,int _njogadores,FILE *_fp)
{
    int i=0;
    NODE_PLAYERS *new_node=NULL;
    NODE_PLAYERS *aux=NULL;
    //Carrega o número de jogadores indicado no ficheiro de configuração
    for(i=0;i<_njogadores;i++)
    {
        //Cria um jogador novo e inicializa todos os seus parametros a 0(excepto a posição)
        new_node=(NODE_PLAYERS *)malloc(sizeof(NODE_PLAYERS));
        //Lê do ficheiro de configuração, toda a informação do jogador
        get_player_info(new_node,_fp);
        new_node->jogador.mao_top=NULL;
        new_node->jogador.mao_back=NULL;
        new_node->jogador.estado=0;
        new_node->jogador.estatistica.losses=0;
        new_node->jogador.estatistica.wins=0;
        new_node->jogador.estatistica.ties=0;
        new_node->jogador.ases=0;
        new_node->next=NULL;
        //A posição do jogador irá corresponder á sua posição na ordem da lista
        new_node->jogador.player_pos=i;
        //Caso seja o primeiro jogador lido, é inserido no inicio da lista
        if(*_head==NULL)
        {
            *_head=new_node;
           (*_head)->next=NULL;
        }
        else
        {   //É inserido no fim da lista
            aux=*_head;
            while( (aux->next)!= NULL )
            {
                aux=aux->next;
            }
            aux->next=new_node;
        }
    }

    //Cria o jogador da casa, e inicializa todos os seus parametros a 0;
    (*_house)=(CASA *)malloc(sizeof (CASA) );
    (*_house)->money_h=0;
    (*_house)->mao_top=NULL;
    (*_house)->mao_back=NULL;
    (*_house)->ases=0;
    (*_house)->pontos=0;
}
/** param_in -> Função que lê os parametros do jogo
                -Número de baralhos
                -Número de jogadores
                - INformação de cada jogador
 * _param _head -> Ponteiro para o primeiro jogador da lista de jogadores
 * _param _house -> Jogador da casa
 * _param _nbaralhos -> Número de baralhos inicial
 * _param _njogadores -> Número de jogadores inicial
 * _param file_name -> Nome do ficheiro de configuração do jogo
**/
int param_in(NODE_PLAYERS **_head,CASA **_house, int *_nbaralhos, int *_njogadores, char *file_name)
{
    FILE *config=NULL;
    char linha[READ_L];
    char *info=NULL;
    int erro=0;
    //Abertura do ficheiro de configuração
    config=fopen(file_name,"r");
    if(config==NULL)
    {   //Caso não se tenha conseguido abrir o ficheiro de configuração
        puts("Erro ao abrir ficheiro de configuração !");
        exit(EXIT_FAILURE);
    }
    //Lê o número de baralhos e o número de jogadores
    fgets(linha, READ_L, config);
    info = strtok (linha,"-");
    sscanf(info,"%i",_nbaralhos);
    if(*_nbaralhos < MIN_DECKS || *_nbaralhos > MAX_DECKS)//Número de baralhos inválido
        erro=1; //Sinaliza um erro

    info = strtok (NULL, "-");
    sscanf(info,"%i",_njogadores);
    if(*_njogadores > MAX_PLAYERS || *_njogadores <=0)//Número de jogadores inválido
        erro=1;//SInaliza um erro
    //Caso tenha sido detetado um erro
    if(erro)
    {   //Indica-o ao utilizador e sai do programa
        puts("Erro no ficheiro de configuração");
        exit(EXIT_FAILURE);
    }
    //Carrega o número de jogadores lidos do ficheiro e fecha-o
    load_players(_head,_house,*_njogadores,config);
    fclose(config);
    return 0;
}
/** convert -> Função que converte o ID da carta no seu peso correspondente
 * _param _valor -> Valor da carta a ser convertido
**/
int convert(int _valor)
{
    switch(_valor)
    {
        case(0):return  2 ;break; case(1):return 3  ;break;
        case(2):return  4 ;break; case(3):return 5  ;break;
        case(4):return  6 ;break; case(5):return 7  ;break;
        case(6):return  8 ;break; case(7):return 9  ;break;
        case(8):return  10;break; case(9):return 10 ;break;
        case(10):return 10;break; case(11):return 10;break;
        case(12):return 11;break; default:return 0;break;
    }
}
/** deck_card -> Função que retira um carta ao calhas do baralho e retorna o seu valor e naipe
 * _param _head ->Ponteiro para a primeira carta do baralho
 * _param _nbaralhos -> Número de baralhos lidos do ficheiro de configuração
 * _param _contagem -> Contagem das cartas distribuidas (Estratégia Hi-Lo)
 * _param _decks_left -> Número de baralhos por distribuir
**/
CARTA deck_card(NODE_CARTAS **_head, int _nbaralhos, int *_contagem, int *_decks_left)
{
    static int i1st = 0;
    int card=0,i=0,count=1;
    NODE_CARTAS *c_removida=NULL;
    NODE_CARTAS *aux=NULL;
    CARTA retorno;
    //Caso o baralho esteja vazio
    if(*_head==NULL)
    {   //Carrega novamente o número de baralhos lidos inicialmente e reinicia a contagem das cartas
        load_decks(_head,_nbaralhos);
        *_contagem=0;
    }

    aux=*_head;
    if(aux!=NULL)
    {   //Determina o número de cartas restantes
        while(aux->next!=NULL)
        {
            count++;
            aux=aux->next;
        }
    }
    //Determina o número de baralhos por distribuir
    *_decks_left=(count/N_BARALHO);
    //Caso seja a primeira vez que a função é chamada, inicializa-se a semente aleatória
    if( i1st == 0)
    {
        srand(456);
        i1st = 1;
    }
    //Escolhe-se a posição da carta a ser retirada
    card=rand()%(count);
    c_removida=*_head;
    while(i<card)
    {//Precorre-se o baralho à procura da posição da carta
        aux=c_removida;
        c_removida=c_removida->next;
        i++;
    }
    //Caso a carta removida esteja na primeira posição do baralho
    if(c_removida == *_head)
    {
        retorno=c_removida->carta;
        *_head=(*_head)->next;
        free(c_removida);
    }
    else //Caso esteja no meio / Fim
    {
        retorno=c_removida->carta;
        aux->next=c_removida->next;
        free(c_removida);
    }
    //Atualiza a contagem das cartas
    if( convert(retorno.valor) >= 10 )
        *_contagem-=1;
    else if(convert(retorno.valor) <= 6 )
        *_contagem+=1;
    //Retorna os valores da carta removida
    return retorno;
}
/** give_card -> Função que insere no topo da pilha das cartas do jogador, a carta retirada do baralho
 * \param _mao_top -> Ponteiro para a ultima carta da mão do jogador
 * \param _mao_back -> Ponteiro para a primeira carta da mão do jogador
 * \param _carta -> Carta qu foi retirada do baralho
**/
STACK_NODE * give_card(STACK_NODE *_mao_top, STACK_NODE **_mao_back , CARTA _carta)
{
    //Cria uma carta com os valores da carta retirada
    STACK_NODE *new_node= (STACK_NODE *)malloc( sizeof(STACK_NODE) );
    if(new_node == NULL)//Caso não tenha conseguido criar a carta
    {   //Sinaliza-o ao utilizador e sai do programa
        puts("Erro ao allocar memória");
        exit(EXIT_FAILURE);
    }
    //Atribui os valores à nova carta
    new_node->carta=_carta;
    new_node->prev= NULL;
    //Caso seja a primeira carta a entrar na pilha
    if(_mao_top == NULL)
      *_mao_back=new_node;
    else //Caso contrário
        _mao_top->prev=new_node;

    new_node->next=_mao_top;
    //Retorna o novo topo da pilha
    return new_node;
}
/** remove_card -> Função que retira uma carta do topo da pilha do jogador
 * \param top -> Ponteiro para a ultima carta da mão do jogador
 * \param _back -> Ponteiro para a primeira carta do jogador
**/
void remove_card(STACK_NODE **top, STACK_NODE **_back)
{
    STACK_NODE *aux=NULL;
    //Liberta o antigo top e indica o novo
    aux=*top;
    *top= (*top)->next;
    free(aux);

    //Caso a pilha continue com cartas
    if(*top != NULL)
        (*top)->prev = NULL;
    else//Caso tenha sido retirada a última carta da pilha
    {
        *_back= NULL;
    }
}
/** remove_player -> Função que remove de uma lista, um determinado jogador
 * \param _head -> Ponteiro para o primeiro jogador da lista
 * \param _player -> Ponteiro para o jogador a ser removido da lista
**/
JOGADOR remove_player(NODE_PLAYERS **_head, NODE_PLAYERS *_player)
{
    NODE_PLAYERS *aux=*_head;
    NODE_PLAYERS *tmp=*_head;
    JOGADOR removed_player;
    //Caso o jogador a remover seja o primeiro
    if(_player == *_head)
    {
        removed_player=(*_head)->jogador;
        aux=*_head;
        *_head=(*_head)->next;
        free(aux);
    }
    else
    {
        aux=*_head;
        while(aux!=_player)
        {
            tmp=aux;
            aux=aux->next;
        }

        tmp->next=aux->next;
        removed_player=aux->jogador;
        free(aux);
    }
    //Retorna as informaçẽos do jogador removido
    return removed_player;
}
/** insert_player -> Função que adiciona ao final de uma lista de jogadores, um jogador novo
 * \param _head -> Ponteiro para o primeiro jogador da lista
 * \param _player -> Informações do jogador a ser adicionado à lista
**/
void insert_player(NODE_PLAYERS **_head, JOGADOR _player )
{
    NODE_PLAYERS *new_node=NULL;
    NODE_PLAYERS *aux=NULL;
    //Cria um jogador novo com as informações do jogador fornecido
    new_node=(NODE_PLAYERS *)malloc(sizeof(NODE_PLAYERS));
    new_node->jogador=_player;
    new_node->next=NULL;
    //Caso seja o primeiro jogador a ser inserido na lista
    if(*_head==NULL)
    {
        *_head=new_node;
    }
    else
    {
        aux=*_head;
        while(aux->next != NULL)
        {
            aux=aux->next;
        }
        aux->next=new_node;

    }
}
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
int read_player_info(int _x, int _y, NODE_PLAYERS **_head)
{
    char str_aux[READ_L];
    JOGADOR new_player;
    NODE_PLAYERS *aux=*_head;
    int pos=0,erro=0;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW);
    //Converte a coordenada x, na posição do rectângulo destinado ao jogador
    while(pos<MAX_PLAYERS)
    {
        if( _x>pos*(separatorPos/4-5)+10 && _x<(pos+1)*(separatorPos/4-5)+10 )
            break;
        else
            pos++;
    }
    //Verifica se já existe um jogador nessa posição
    while(aux!=NULL)
    {   //Caso já existe, a função dá a indicação de tal
        if(aux->jogador.player_pos == pos)
            return 1;
        else
            aux=aux->next;
    }
    //Caso não exista um jogador no quadrado selecionado, e tenha sido selecionado um quadrado
    if(_y > 0.55f*HEIGHT_WINDOW)
    {   //Lê o nome do novo jogador
        puts("Insira o nome do novo jogador: ");
        fgets(str_aux,READ_L,stdin);
        if(strlen(str_aux)>NOME_MAX)
        {//Caso exceda o tamanho limite, sinaliza um erro
            erro=1;
        }
        else//Retira o '\n' da string
            str_aux[strlen(str_aux)-1]=0;
        strcpy(new_player.nome, str_aux);
        while(aux!=NULL)//Verfica se já esxiste um jogador com esse nome
        {
            if(!strcmp(aux->jogador.nome, new_player.nome))
                erro=1;
        }
        //Lê o valor do dinheiro inicial do jogador
        puts("Insira o valor do dinheiro inicial do jogador: ");
        fgets(str_aux,READ_L, stdin);
        sscanf(str_aux,"%f",&(new_player.banco));
        if(new_player.banco <0 )
        {
            erro=1;
        }
        //Lê o valor da aposta do jogador
        puts("Insira o valor da aposta do jogador: ");
        fgets(str_aux,READ_L,stdin);
        sscanf(str_aux,"%i",&(new_player.aposta));
        if(new_player.aposta <0)
        {
            erro=1;
        }        //Lê o tipo do jogador
        puts("Insira o tipo do jogador(HU- 0 , EA- 1): ");
        fgets(str_aux,READ_L,stdin);
        sscanf(str_aux,"%i",&(new_player.tipo));
        if(new_player.tipo!= 1 && new_player.tipo!= 0)
        {   printf("%i", new_player.tipo);
            erro=1;
        }
        new_player.ases=0;
        new_player.estado=0;
        new_player.estatistica.wins=0;
        new_player.estatistica.losses=0;
        new_player.estatistica.ties=0;
        new_player.mao_back=NULL;
        new_player.mao_top=NULL;
        new_player.player_pos=pos;
        new_player.pontos=0;
        if(erro)
        {
            puts("Impossível de criar jogador");
            puts("Requesitos: \n Dinheiro inicial positivo;\n Aposta positiva;\n Nome entre 8 caracteres;\n Tipo: 0 ou 1")  ;
        }
        else
            insert_player(_head, new_player);
        return 0;
    }
    else
    //Caso a posição selecionada para o jogador não seja válida, a função retorna 1
        return 1;

}

/** Free_All -> Função que liberta toda a memória alocada (excluindo a matriz das acções das EA)
 * \param _players_head -> Ponteiro para o primeiro jogador da lista de jogadores
 * \param _deck_head -> Ponteiro para a primeira carta do baralho
 * \param _removed_players -> Ponteiro para o primeiro jogador que foi removido
 * \param _house -> Jogador da casa
**/
void Free_All(NODE_PLAYERS **_players_head, NODE_CARTAS **_deck_head, NODE_PLAYERS **_removed_players, CASA **_house )
{
    NODE_CARTAS *tmp=NULL;
    NODE_PLAYERS *aux=NULL;

    //Liberta os baralhos carregados
    while(*_deck_head!=NULL)
    {
        tmp=*_deck_head;
        free(tmp);
        *_deck_head=(*_deck_head)->next;
    }

    //Liberta todos os jogadores em jogo
    while(*_players_head!=NULL)
    {
        aux=*_players_head;
        //Liberta a mão do jogador
        while(aux->jogador.mao_top!=NULL)
        {
            remove_card(&(aux->jogador.mao_top), &(aux->jogador.mao_back));
        }
        free(aux);
        *_players_head=(*_players_head)->next;
    }
    //Liberta os jogadores que forma removidos da mesa
    while(*_removed_players!=NULL)
    {
        aux=*_removed_players;
        free(aux);
        *_removed_players=(*_removed_players)->next;
    }
    //Liberta o jogador da casa e a sua mão
    while((*_house)->mao_top!=NULL)
    {
        remove_card( &((*_house)->mao_top ), &((*_house)->mao_back));
    }
    free(*_house);
}
/** file_write -> Função que escreve no ficheiro de estatisticas, todas as estatisticas dos jogadores que participaram no jogo, e o balanço final da casa
 * \param players_head -> Ponteiro para o primeiro jogador da lista de jogadores
 * \param removed_players -> Ponteiro para o primeiro jogador da lista de jogadores removidos
 * \param _money_h -> Balanço do casa
**/
void file_write(NODE_PLAYERS *_players_head, NODE_PLAYERS *_removed_players, float _money_h)
{
    FILE *dados_jogo;
    //Abre o ficheiro de estatisticas para escrita
    dados_jogo=fopen("stats.txt","w");
    if(dados_jogo == NULL)
    {   //Caso não tenha sido possível abrir o ficheiro de estistica, é indicado ao utilizador e sai do programa
        puts("Erro ao abrir o ficheiro de estatística !");
        exit(EXIT_FAILURE);
    }
    //Escreve a estatistica dos jogadores da mesa
    NODE_PLAYERS *aux=_players_head;
    while(aux!=NULL)
    {
        fprintf(dados_jogo,"%s :\n Vitorias: %i\n Derrotas: %i\n Empates: %i\n Balanço Final: %f\n" ,aux->jogador.nome,aux->jogador.estatistica.wins,aux->jogador.estatistica.losses,aux->jogador.estatistica.ties,aux->jogador.banco);
        aux=aux->next;
    }
    //Escreve a estatistica dos jogadores removidos da mesa
    aux=_removed_players;
    while(aux!=NULL)
    {
        fprintf(dados_jogo,"%s :\n Vitorias: %i\n Derrotas: %i\n Empates: %i\n Balanço Final: %f\n" ,aux->jogador.nome,aux->jogador.estatistica.wins,aux->jogador.estatistica.losses,aux->jogador.estatistica.ties,aux->jogador.banco);
        aux=aux->next;
    }
    //Escreve o balanço final da casa
    fprintf(dados_jogo,"\n\n Balanço da casa: %f",_money_h);
    //Fecha o ficheiro de estatisticas
    fclose(dados_jogo);
}
