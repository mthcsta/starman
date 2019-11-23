#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "kbhit.c"



#define TOTAL_INIMIGO 20 //numero máximo de inimigos em cada nivel
#define INIMIGO_MORTO -5

#define CHANCE_DE_TIRO 50 // a chance de o inimigo dar um tiro, de 1 ao definido

#define PAREDE 500

#define DURACAO_ANIMACAO 15

#define MAX_TIROS 100
#define VEL_BALA 5
#define DURACAO_TIRO 15

#define INTERVALO_TIRO 10
#define VEL_MIN -2
#define VEL_MAX 2

#define LINHAS 35
#define COLUNAS 415
#define LARGURA 105


#define MIN_LIMIT 5
#define MAX_LIMIT 29

/*-------------------------------*/
/*------------Estruturas---------*/

//estrutura do tiro
typedef struct{
    int x, y,
        prop, // 0: Inexistente, 1: Player, 2: Inimigo
        duracao;
} tiro_t;

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

int MinMax(int min, int max){
    return (min + (rand() % (max-min+1)));
}

void buscaTiro(boneco_t *jogador, boneco_t inimigo[], tiro_t tiro[],int posicao, int * pontuacao, int *inimigos_existentes, int *animacao){
    int i, j, // iteradores
        id; //variável auxiliar para guardar o id do inimigo

    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop>0){ //se o tiro existe, verifica sua posição em relação ao boneco

            /*** Matando os Inimigos *****/
            if(tiro[i].prop==1){    //se o tiro é do jogador...
                j = 0;//indice do inimigo
                while(inimigo[j].nvidas>0 && j < TOTAL_INIMIGO){ //se o inimigo está vivo
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




//sorteia os movimentos do inimigo
void atualizaInimigo(int mapa[][COLUNAS], boneco_t *inimigo, boneco_t *jogador){
    int minimo = 1, maximo = 6, andou_x=0;
    if(mapa[inimigo->y-1][inimigo->x] == PAREDE){
        minimo = 1;
        maximo = 3;
    }else if(mapa[inimigo->y][inimigo->x] == PAREDE){
        minimo = 4;
        maximo = 6;
    }

    switch(/*MinMax(minimo, maximo)*/ 0){
        case 1:
            inimigo->x += 1;
            andou_x=1;
            break;
        case 2:
            inimigo->y += 1;
            break;
        case 3:
            inimigo->x += 1;
            inimigo->y += 1;
            andou_x=1;
            break;
        case 4:
            inimigo->x -= 1;
            inimigo->y -= 1;
            andou_x=2;
            break;
        case 5:
            inimigo->y -= 1;
        case 6:
            inimigo->x -= 1;
            andou_x=2;
        default: break;
    }
    if(andou_x>0){
        if(andou_x==1 && inimigo->x>=COLUNAS){
            inimigo->x = 0;
        }else if(inimigo->x<0){
            inimigo->x = COLUNAS;
        }
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


/***atualiza a tela***/
int atualizaTela(int mapa[][COLUNAS], int coluna, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *inimigos_existentes, int *animacao, int *intervalo){
    int i;
    int id;
    int x=0, y=0;

    if(*animacao>0) *animacao -= 1;
    if(*intervalo>0) *intervalo -= 1;

    if(mapa[jogador->y][jogador->x+coluna]==PAREDE || mapa[jogador->y][jogador->x+2+coluna]==PAREDE || mapa[jogador->y-1][jogador->x+coluna]==PAREDE){
        jogador->nvidas--;
        *animacao = DURACAO_ANIMACAO;
        jogador->y=(LINHAS / 2) + 1;
    }

    for(i=0; i<*inimigos_existentes; i++){
        
        if(jogador->x+coluna == inimigo[i].x && 
            (fabs(jogador->y - inimigo[i].y) <= 1)){
            jogador->nvidas--;
            *animacao = DURACAO_ANIMACAO;
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
                if(tiro[i].x>COLUNAS) tiro[i].x = 0;
            }else{ // tiro do inimigo
                tiro[i].x -= VEL_BALA;
                if(tiro[i].x<0) tiro[i].x=COLUNAS;
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
                    inimigo[id] = carregaInimigo(x, y);
                    id++;
                }else if(c=='@'){
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

void gotoxy(int x,int y){ printf("%c[%d;%df",0x1B,2+y,1+x); }

int geraPosicao(int x, int posicao){
    int reposicao = x - posicao;
    if(posicao <= (COLUNAS - LARGURA) && reposicao>0 && x<LARGURA+posicao){
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

void geraQuadro(int mapa[][COLUNAS], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *pontuacao, int *inimigos_existentes, int *animacao){
    int i, id, linha=0, coluna=0, p, reposiciona_escrita=0, posicao_inimigo;

    int mtx=0, mty=0;

    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop==1){
            mtx = tiro[i].x;
            mty = tiro[i].y;
        }
    }

    printf("Vidas: %d Pontos: %d // %d / %d  || %d:%d\n", jogador->nvidas, *pontuacao, posicao, (posicao+jogador->x) % COLUNAS, mtx, mty);

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

            if(mapa[linha][p]==PAREDE){
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

void printa(int pos, int linha, char *string){
    gotoxy(LARGURA/pos-strlen(string)/2,linha);
    if(strlen(string)>0) printf("%s", string);
}

void FIM_DE_JOGO(int score){
    printa(2, 12, "FIM DE JOGO");

    printa(2, 18, "Pontuacao: ");
    printf("%d", score);

    printa(2, 26, "CREDITOS");
    printa(2, 28, "Matheus Costa        Terumi Tamai");
    printa(2, 32, "");
}

void controle(int c, boneco_t * jogador, tiro_t tiro[], int mapa[][COLUNAS], int foto, int *intervalo){
    
    int reposiciona;

    c = tolower(c);

    switch(c){
        case 's':
            jogador->y+=1;
            break;
        case 'w':
            if(mapa[jogador->y-2][jogador->x+foto]!=PAREDE){
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
                atira(tiro, 1, (jogador->x + foto) % COLUNAS, jogador->y);
                *intervalo = INTERVALO_TIRO;
            }
            break;

        default:
         break;
    }
    return;
}

void partida(char nome_mapa[], boneco_t * jogador, int * pontuacao){
                    /** X,  Y, vidas, velocidade  */
    boneco_t inimigo[TOTAL_INIMIGO];
    tiro_t tiro[MAX_TIROS]={0};
    int mapa[LINHAS][COLUNAS];
    int i, j;
    int inimigos_existentes;
    int foto = 0;
    int animacao = 0;
    int intervalo = 0;

    FILE *arquivo;
    

    arquivo = fopen(nome_mapa, "r");

    inimigos_existentes = le_mapa(arquivo, mapa, jogador, inimigo);

    fclose(arquivo);

    while(1){

        printf("\033[H\033[J");

        foto += jogador->velocidade;

        if(foto<0) foto = COLUNAS;
        else if(foto>=COLUNAS) foto=0;

        if(kbhit())
            controle(getchar(), jogador, tiro, mapa, foto, &intervalo);

        buscaTiro(jogador, inimigo, tiro, foto, pontuacao, &inimigos_existentes, &animacao);
        atualizaTela(mapa, foto, jogador, inimigo, tiro, &inimigos_existentes, &animacao, &intervalo);

        // finaliza partida:
        if(jogador->nvidas==0 || inimigos_existentes<=0){
            break;
        }

        geraQuadro(mapa, foto, jogador, inimigo, tiro, pontuacao, &inimigos_existentes, &animacao);

        // 17000
        usleep(40000);
    }

    return;
}

int main(){

    int i=0;
    char * lista_mapas[3] = {"mapa_exemplo.txt", "mapa_turmac.txt"};
    int numero_mapa = 0;

    int pontuacao=0;

    boneco_t jogador = {0, 0, 20, 0};


    do{
        partida(lista_mapas[i], &jogador, &pontuacao);
        i++;
    } while(jogador.nvidas>0 && i<2);

    FIM_DE_JOGO(pontuacao + (jogador.nvidas * 100));


    return 0;
}
