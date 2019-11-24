#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


// Jogo
#define PAREDE 500
#define LINHAS 35
#define COLUNAS 415
#define LARGURA 105


// Tiro
#define MAX_TIROS 100
#define VEL_BALA 5
#define DURACAO_TIRO 20

// Jogador
#define DURACAO_ANIMACAO 15
#define INTERVALO_TIRO 10
#define VEL_MIN 0
#define VEL_MAX 2

// Inimigo
#define TOTAL_INIMIGO 20 //numero máximo de inimigos em cada nivel
#define INIMIGO_MORTO -5
#define CHANCE_DE_TIRO 50 // a chance de o inimigo dar um tiro, de 1 ao definido




/*-------------------------------*/
/*--------Meus Includes----------*/
#include "kbhit.c"
#include "auxiliares.c"


/*-------------------------------*/
/*------------Estruturas---------*/

//estrutura do tiro
typedef struct{
    int x, y,
        prop, // 0: Inexistente, 1: Jogador, 2: Inimigo
        duracao;
} tiro_t;
//estrutura do boneco, que pode ser um jogador ou inimigo
typedef struct{
    int x, y,
        nvidas,
        velocidade;
} boneco_t;


void deletaInimigo(boneco_t inimigo[], int morto, int *inimigos_existentes){
    inimigo[morto] = inimigo[*inimigos_existentes-1]; //puxa o último inimigo vivo pra posicao do eliminado
    *inimigos_existentes -= 1; //elimina o inimigo da ultima pos (pq ele foi passado pra frente)
    return;
}

int ehParede(int mapa[][COLUNAS], int x,int y){
    return mapa[y][x] == PAREDE;
}

/**Verifica as posicoes relativas entre o jogador e o tiro do inimigo, e entre o inimigo e o tiro do jogador.
   Computa os pontos ganhos a cada inimigo morto e as vidas perdidas por tiros do inimigo.
**/

void buscaTiro(boneco_t *jogador, boneco_t inimigo[], tiro_t tiro[],int posicao, int * pontuacao, int *inimigos_existentes, int *animacao){
    int i, j, // iteradores
        id; //variável auxiliar para guardar o id do inimigo

    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop>0){ //se o tiro existe, verifica sua posição em relação ao boneco

            /*** Matando os Inimigos *****/
            if(tiro[i].prop==1){    //se o tiro é do jogador...
                j = 0;//indice do inimigo
                while(j<*inimigos_existentes){ //se o inimigo está vivo
                        if(tiro[i].x<=inimigo[j].x && tiro[i].x+VEL_BALA>=inimigo[j].x &&//confere as coordenandas do inimigo com as do tiro
                            (tiro[i].y==inimigo[j].y || tiro[i].y==inimigo[j].y-1) ){   //se as coordenadas x e y do tiro == as do inimigo
                            inimigo[j].nvidas--;            //o inimigo perde a vida
                            if (inimigo[j].nvidas == 0){    //se o inimigo estiver morto
                                deletaInimigo(inimigo, j, inimigos_existentes);  //chamamos a funcao pra deleta-lo do vetor
                            }
                            tiro[i].prop=0;                 //e o tiro deixa de existir
                            *pontuacao+=10; //o jogador ganha 10 pontos
                            j = TOTAL_INIMIGO; //finaliza o laco desse tiro
                            printf("\a");
                        }
                        j++;
                }
            }
            /*** Matando o Jogador por tiros *****/
            //Se as coordenadas x e y do tiro coincidem com as do jogador, o jogador perde uma vida
            else if(*animacao==0 && tiro[i].prop==2 && tiro[i].x>=jogador->x+posicao && tiro[i].x-VEL_BALA<=jogador->x+posicao &&
                    (tiro[i].y==jogador->y || tiro[i].y==jogador->y-1)){
                *animacao = DURACAO_ANIMACAO;
                jogador->nvidas--;
                tiro[i].prop=0; //o tiro deixa de existir
                printf("\a");
            }

        }
    }
    return;
}


void atualizaInimigo(int mapa[][COLUNAS], boneco_t *inimigo, boneco_t *jogador){
    int minimo = 1, maximo = 5, andou_x=0;

    if(MinMax(1, 10)==5){
        if(inimigo->y > jogador->y){
            minimo = maximo = 2;
        }else{
            minimo = maximo = 1;
        }
    }

    switch(MinMax(minimo, maximo)){
        case 1:
            if(!ehParede(mapa, inimigo->x, inimigo->y+1))
                inimigo->y += 1;
            break;
        case 2:
            if(!ehParede(mapa, inimigo->x, inimigo->y-2) && !ehParede(mapa, inimigo->x, inimigo->y-1))
                inimigo->y -= 1;
            break;

        case 3:
            if(!ehParede(mapa, inimigo->x-1, inimigo->y+1) && !ehParede(mapa, inimigo->x-1, inimigo->y) && !ehParede(mapa, inimigo->x-1, inimigo->y-2)){
                inimigo->x -= 1;
                inimigo->y += 1;
                andou_x=1;
            }
            break;        
        case 4:
            if(!ehParede(mapa, inimigo->x-1, inimigo->y-2) && !ehParede(mapa, inimigo->x-1, inimigo->y) && !ehParede(mapa, inimigo->x-1, inimigo->y-1)){
                inimigo->x -= 1;
                inimigo->y -= 1;
                andou_x=1;
            }
            break;
        case 5:
            if(!ehParede(mapa, inimigo->x-1, inimigo->y)){
                inimigo->x -= 1;
                andou_x=1;
            }
            break;
        default: break;
    }
    if(inimigo->y<=0 || inimigo->y>35) inimigo->y = LINHAS / 2 + 1;
    if(andou_x==1 && inimigo->x<0){
        inimigo->x = COLUNAS - 1;
    }
    return;
}

void atira(tiro_t tiro[], int prop, int x, int y){
    int i=0, continua=1; // iterador
    do{
        if(tiro[i].prop == 0){
            tiro[i].prop = prop;
            tiro[i].duracao = DURACAO_TIRO;
            tiro[i].x = x;
            tiro[i].y = y;
            continua=0;
        }
        i++;
    } while(continua);
    return;
}

int buscaParede(int mapa[][COLUNAS], int x, int y, int muda, int limite){
    if(ehParede(mapa, x, y)){
        if(muda==1)
            return y;
        else
            return x;
    }else{
        if(muda==1){
            y++;
        }else{
            x++;
        }
        return buscaParede(mapa, x, y, muda, limite);
    }
}

/***atualiza a tela***/
int atualizaTela(int mapa[][COLUNAS], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *inimigos_existentes, int *animacao, int *intervalo, int *salvar_estado_mensagem){
    int i;
    int id;
    int x=0, y=0;

    if(*animacao>0) *animacao -= 1;
    if(*intervalo>0) *intervalo -= 1;
    if(*salvar_estado_mensagem>0) *salvar_estado_mensagem -= 1;

    if(ehParede(mapa, jogador->x+posicao, jogador->y) || //se a pos x do jogador é igual a da parede de trás OU
       ehParede(mapa, jogador->x+posicao+3, jogador->y) || //se a pos x do jogador é igual a da parede da frente OU
       ehParede(mapa, jogador->x+posicao, jogador->y-1) || //se a pos y do jogador é igual ao chão OU
       ehParede(mapa, jogador->x+posicao, jogador->y)){ //se a pos y do jogador é igual ao teto
        jogador->nvidas--; //o jogador perde uma vida
        *animacao = DURACAO_ANIMACAO; //a perda da vida é sinalizada por animação
        jogador->y = buscaParede(mapa, jogador->x+posicao+4, 0, 1, 0) + 2;
        jogador->velocidade = VEL_MIN;
    }

    for(i=0; i<*inimigos_existentes; i++){

        if(jogador->x+posicao == inimigo[i].x && //verifica se o jogador e inimigo estão na mesma pos x
            (fabs(jogador->y - inimigo[i].y) <= 1)){ //verifica o modulo da distancia vertical entre inimigo e jogador
            jogador->nvidas--; // se o jogador estiver dentro da parte visual do inimigo, jogador perde uma vida.
            *animacao = DURACAO_ANIMACAO; //perda da vida sinalizada por animação
        }

        atualizaInimigo(mapa, &inimigo[i], jogador);
        if(MinMax(0,CHANCE_DE_TIRO)==CHANCE_DE_TIRO){
            atira(tiro, 2, inimigo[i].x, inimigo[i].y);
        }
    }

    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop!=0){
            if(tiro[i].prop==1){ // tiro do jogador
                tiro[i].x += VEL_BALA;
                if(tiro[i].x>=COLUNAS) tiro[i].x = 0;
            }else{ // tiro do inimigo
                tiro[i].x -= VEL_BALA;
                if(tiro[i].x<0) tiro[i].x=COLUNAS-1;
            }
            // remove tiro após o fim da duração
            if(tiro[i].duracao==0){
                tiro[i].prop = 0;
            }else tiro[i].duracao--;
        }
    }
    return x;
}

/***gera os inimigos na tela***/
boneco_t carregaInimigo(int x, int y){
    boneco_t inimigo;

    // dados preenchidos definidos pela funçao
    inimigo.velocidade = 1;
    inimigo.nvidas = 1;
    // Dados preenchidos pelos parametros
    inimigo.x = x;
    inimigo.y = y;

    return inimigo;
}

int le_mapa(FILE *arq, int mapa[][COLUNAS], boneco_t *jogador, boneco_t inimigo[]){
    int y=0, x=0, // iteradores
        id=0, pulador=0; // id do inimigo que sera inserido
    char c;

    do{
        c = fgetc(arq);
        if(c != EOF){
            if(c!='\n'){
                if(c=='C'){
                    mapa[y][x] = PAREDE;
                    if(pulador>0){
                        mapa[y][x-pulador] = pulador;
                        pulador=0;
                    }
                }else if(c==' '){
                    mapa[y][x] = 0;
                    pulador++;
                }else if(c=='X'){
                    mapa[y][x] = 0;
                    inimigo[id] = carregaInimigo(x, y);
                    id++;
                }else if(c=='@'){
                    mapa[y][x] = 0;
                    jogador->y = y;
                    jogador->x = x;
                }
                x++;
            }else{
                y++;
                x=0;
                pulador=0;
            }
        }
    }while(!feof(arq));

    return id;
}


int geraPosicao(int x, int posicao){
    int reposicao = x - posicao;
    if(posicao <= (COLUNAS - LARGURA) && reposicao>0 && x<LARGURA+posicao-1){
        return reposicao;
    }else if(posicao>(COLUNAS - LARGURA)){
        reposicao = x - posicao;
        if(x<(COLUNAS - LARGURA)) reposicao += COLUNAS;
        if(reposicao>0 && reposicao<(LARGURA-1)){
            return reposicao;
        }
    }
    return 0;
}

void geraQuadro(int mapa[][COLUNAS], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *pontuacao, int *inimigos_existentes, int *animacao, int *salvar_estado_mensagem){
    int i, id, linha=0, coluna=0, p, reposiciona_escrita=0, posicao_inimigo;

    printf("Vidas: %d Pontos: %d", jogador->nvidas, *pontuacao);
    if(*salvar_estado_mensagem>0)  
        printa(2, -1, "Estado Salvo");
    printf("\n");

    /*** Gerando Jogador ***/

    if(*animacao%3==0){

        gotoxy(jogador->x, jogador->y-1);
        printf("@");
        gotoxy(jogador->x, jogador->y);
        printf("@@@@");

    }

    /*** Gerando Inimigos ***/

    for(i=0; i<*inimigos_existentes; i++){
        posicao_inimigo = geraPosicao(inimigo[i].x, posicao);
        if(posicao_inimigo>0){
            gotoxy(posicao_inimigo, inimigo[i].y-1);
            printf("XX");
            gotoxy(posicao_inimigo, inimigo[i].y);
            printf("XX");
        }
    }

    /*** Gerando Tiros ***/
    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop!=0){
            posicao_inimigo = geraPosicao(tiro[i].x, posicao);
            if(posicao_inimigo>0){
                gotoxy(posicao_inimigo, tiro[i].y);
                if(tiro[i].prop==1) printf("--"); else printf(".");
            }
        }
    }

    /*** Gerando Paredes ***/
    while(linha<LINHAS){
        coluna=0;
        reposiciona_escrita=0; // quando zerado, reposiciona equivale a falso
        p = posicao; //coluna auxiliar
        gotoxy(coluna, linha);
        while(coluna<LARGURA){
            if(p>=COLUNAS) p %=  COLUNAS; //zera periodicamente para repetir o início da tela

            if(ehParede(mapa, p, linha)){
                if(reposiciona_escrita==1){ //
                    reposiciona_escrita=0;
                    gotoxy(coluna, linha);
                }

                printf("\u2588");
                coluna++;
                p++;
            }else if(mapa[linha][p]>0){
                coluna+=mapa[linha][p];
                p+=mapa[linha][p];
                gotoxy(coluna, linha);
            }else{
                coluna++;
                p++;
                reposiciona_escrita=1;
            }
        }
        printf("\n");
        linha++;
    }


}

void FIM_DE_JOGO(int score){
    char str[11 + 6];

    limpaQuadro();

    sprintf(str, "Pontuacao: %d", score);

    printa(2, 8, "FIM DE JOGO");

    if(score>0){
        printa(2, 14, str);
    }

    printa(2, 20, "CREDITOS");
    printa(2, 21, "/**-----------------------------------**/");
    printa(2, 22, "Matheus Costa        Terumi Tamai");
    printa(1, 29, "\n");
}

void controle(int c, boneco_t * jogador, tiro_t tiro[], int mapa[][COLUNAS], int posicao, int *intervalo, int *salvar_estado){

    int reposiciona;

    c = tolower(c);

    switch(c){
        case 's':
            if(!ehParede(mapa, jogador->x+posicao, jogador->y+1)){
                jogador->y+=1;
            }
            break;
        case 'w':
            if(!ehParede(mapa, jogador->x+posicao, jogador->y-2)){
                jogador->y-=1;
            }
            break;
        case 'd':
            if(jogador->velocidade < VEL_MAX){ //confere se a velocidade eh menor que a max
                jogador->velocidade++;         //antes de acelerar mais
            }
            break;
        case 'a':
            if(jogador->velocidade > VEL_MIN){ //confere se a velocidade ainda eh maior que a min
                jogador->velocidade--;         //antes de desacelerar
            }
            break;
        case ' ':
            if(*intervalo==0){
                atira(tiro, 1, (jogador->x + posicao) % COLUNAS, jogador->y);
                *intervalo = INTERVALO_TIRO;
            }
            break;

        case 'g':
            *salvar_estado = 1;
            break;

        default:
         break;
    }
    return;
}

void salvarEstado(int nivel, int posicao, boneco_t jogador, int pontuacao, tiro_t tiro[], int inimigos_existentes, boneco_t inimigo[], int animacao, int intervalo){
    FILE *arquivo;
    arquivo = fopen("_salve.bin", "w");
    fwrite(&nivel, sizeof(int), 1, arquivo);
    fwrite(&posicao, sizeof(int), 1, arquivo);
    fwrite(&jogador, sizeof(boneco_t), 1, arquivo);
    fwrite(&pontuacao, sizeof(int), 1, arquivo);
    fwrite(tiro, sizeof(MAX_TIROS), 1, arquivo);
    fwrite(&inimigos_existentes, sizeof(int), 1, arquivo);
    fwrite(inimigo, sizeof(boneco_t), inimigos_existentes, arquivo);
    fwrite(&animacao, sizeof(int), 1, arquivo);
    fwrite(&intervalo, sizeof(int), 1, arquivo);
    fclose(arquivo);
}
void carregarEstado(FILE * salve, int *posicao, boneco_t *jogador, int *pontuacao, tiro_t tiro[], int *inimigos_existentes, boneco_t inimigo[], int *animacao, int *intervalo){
    fread(posicao, sizeof(int), 1, salve);
    fread(jogador, sizeof(boneco_t), 1, salve);
    fread(pontuacao, sizeof(int), 1, salve);
    fread(tiro, sizeof(MAX_TIROS), 1, salve);
    fread(inimigos_existentes, sizeof(int), 1, salve);
    fread(inimigo, sizeof(boneco_t), *inimigos_existentes, salve);
    fread(animacao, sizeof(int), 1, salve);
    fread(intervalo, sizeof(int), 1, salve);
    fclose(salve);
    return;
}

void partida(int nivel, char nome_mapa[], boneco_t * jogador, int * pontuacao, FILE * salve){
    
    int continua=1;
                    /** X,  Y, vidas, velocidade  */
    boneco_t inimigo[TOTAL_INIMIGO];
    tiro_t tiro[MAX_TIROS]={0};
    int mapa[LINHAS][COLUNAS];
    int i, j;
    int inimigos_existentes;
    int posicao = 0;
    int animacao = 0;
    int intervalo = 0;
    int salvar_estado = 0;
    int salvar_estado_mensagem = 0;

    FILE *arquivo;


    arquivo = fopen(nome_mapa, "r");

    inimigos_existentes = le_mapa(arquivo, mapa, jogador, inimigo);

    fclose(arquivo);

    if(salve!=0){
        carregarEstado(salve, &posicao, jogador, pontuacao, tiro, &inimigos_existentes, inimigo, &animacao, &intervalo);
    }

    while(continua){

        limpaQuadro();

        posicao += jogador->velocidade;

        if(posicao<0) posicao = COLUNAS;
        else if(posicao>=COLUNAS) posicao=0;

        if(kbhit())
            controle(getchar(), jogador, tiro, mapa, posicao, &intervalo, &salvar_estado);

        if(salvar_estado==1){
            salvar_estado = 0;
            salvar_estado_mensagem = 50;
            salvarEstado(nivel, posicao, *jogador, *pontuacao, tiro, inimigos_existentes, inimigo, animacao, intervalo);
        }

        buscaTiro(jogador, inimigo, tiro, posicao, pontuacao, &inimigos_existentes, &animacao);
        atualizaTela(mapa, posicao, jogador, inimigo, tiro, &inimigos_existentes, &animacao, &intervalo, &salvar_estado_mensagem);

        // finaliza partida:
        if(jogador->nvidas==0 || inimigos_existentes<=0){
            continua = 0;
        }
        printf("%d|",nivel);

        geraQuadro(mapa, posicao, jogador, inimigo, tiro, pontuacao, &inimigos_existentes, &animacao, &salvar_estado_mensagem);

        // 17000
        usleep(40000);
    }

    return;
}



int MENU_INICIAL(void){
    int continua = 1, i;
    int selecionado_indice = 0;
    char *menu[]={"Novo Jogo", "Carregar Jogo", "Sair"};
    char selecionado_texto[20];

    while(continua){
        if(kbhit()){
            switch(tolower(getchar())){
                case 's':
                    if(selecionado_indice<2)
                        selecionado_indice++;
                    else selecionado_indice=0;
                    printf("\a");                    
                    break;
                case 'w':
                    if(selecionado_indice>0)
                        selecionado_indice--;
                    else selecionado_indice=2;
                    printf("\a");
                    break;
                case ' ':
                case 10:
                    printf("\a");
                    return selecionado_indice;
                    break;
                default: break; 
            }
        }

        limpaQuadro();
        logo();

        for(i=0; i<3; i++){

            if(selecionado_indice==i){
                sprintf(selecionado_texto, "\u00BB  %.13s", menu[i]);
            }else 
                sprintf(selecionado_texto, "  %.13s", menu[i]);
            printa(2, 10+i, selecionado_texto);
        }
    }
}

int main(){

    int i=0;
    char * lista_mapas[2] = {"mapas/mapa_exemplo.txt", "mapas/mapa_turmac.txt"};
    int numero_mapa = 0;
    //inicializa a pontuação zerada
    int pontuacao=0;
    //inicializa o jogador no centro da tela com a velocidade mínima do jogo
    boneco_t jogador = {0, 0, 20, VEL_MIN};

    //estado 
    int estado = 0;
    FILE *salve = NULL;

    if((estado=MENU_INICIAL())!=2){
        if(estado==1){
            salve = fopen("_salve.bin","r");
            if(salve != 0){
                fread(&i, sizeof(int), 1, salve);
            }
        }

        //a partida perdura enquanto o jogador tiver vidas e ainda houver fases disponíveis        
        do{
            partida(i, lista_mapas[i], &jogador, &pontuacao, salve);
            i++;
        } while(jogador.nvidas>0 && i<2);

        FIM_DE_JOGO(pontuacao + (jogador.nvidas * 100));
    }else FIM_DE_JOGO(-5);

    return 0;
}

