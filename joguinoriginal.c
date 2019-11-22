#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "kbhit.c"



#define TOTAL_INIMIGO 20 //numero máximo de inimigos em cada nivel
#define INIMIGO_CHECK 8 // inimigos que checaremos
#define INIMIGO_MORTO -5

#define CHANCE_DE_TIRO 200 // a chance de o inimigo dar um tiro, de 1 ao definido

#define PAREDE 500

#define MAX_TIROS 200
#define VEL_BALA 5

#define VEL_MIN 1
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
    prop; // 0: Inexistente, 1: Jogador, 2: Inimigo
}tiro_t;

//estrutura da personagem que pode ser: Jogador ou Inimigo
typedef struct{
    int x, y,
    nvidas,
    velocidade,
    visto; // visto sera utilizado para checagem
} boneco_t;


//Recebe uma estrutura jogador, uma estrutura inimigo, uma lista de inimigos vivos, uma lista dos tiros
//e um total de pontuação do jogador até o momento
void buscaTiro(boneco_t *jogador, boneco_t inimigo[], int inimigo_vivo[], tiro_t tiro[], int * pontuacao){
    int i, j, // iteradores
        id; //variável auxiliar para guardar o id do inimigo

    for(i=0; i<MAX_TIROS; i++){//itera sobre a lista dos tiros
        if(tiro[i].prop>0){ //se o tiro existe, verifica sua posição em relação ao boneco

            /*** Matando os Inimigos *****/
            if(tiro[i].prop==1){    //se o tiro é do jogador...
                for(j=0; j<INIMIGO_CHECK; j++){ //percorre a lista de inimigos vivos dos inimigos na tela
                    if(inimigo_vivo[j]!=INIMIGO_MORTO){    //se o índice da checagem for diferente de vazio
                        id = inimigo_vivo[j];               //pega o índice do inimigo guardado na checagem
                        if(tiro[i].x<=inimigo[id].x && tiro[i].x+VEL_BALA>=inimigo[id].x &&
                            (tiro[i].y==inimigo[id].y || tiro[i].y==inimigo[id].y-1) ){   //se as coordenadas x e y do tiro == as do inimigo
                            inimigo[id].nvidas--;            //o inimigo perde a vida
                            inimigo_vivo[j] = INIMIGO_MORTO;   //e remove o índice do inimigo da checagem
                            tiro[i].prop=0;                 //e o tiro deixa de existir
//                            printf("O INIMIGO MORREU!!\n");
                            *pontuacao+=10;//soma 10 à pontuação do jogador a cada inimigo morto
                        }
                    }
                }
            }
            /*** Matando o Jogador *****/
            //Se as coordenadas x e y do tiro do inimigo coincidem com as do jogador, o jogador perde uma vida
            else if(tiro[i].prop==2 && tiro[i].x>=jogador->x && tiro[i].x-VEL_BALA<=jogador->x &&
                    (tiro[i].y==jogador->y || tiro[i].y==jogador->y-1)){
                jogador->nvidas--;
                tiro[i].prop=0; //o tiro deixa de existir
               // printf("morreu um pouco\n");
                if(jogador->nvidas == 0){ //se o jogador ficar com 0 vidas, game-over pra ele
                //printf("ja era pra ti\n");
                }
            }

        }
    }
    return;
}

int MinMax(int min, int max){
    return (min + (rand() % (max-min+1)));
}

//sorteia os movimentos do inimigo
void atualizaInimigo(int mapa[][COLUNAS], boneco_t *inimigo, boneco_t *jogador){
    int minimo = 1, maximo = 6, andou_x=0;
    if(mapa[inimigo->y-1][inimigo->x] == PAREDE){ //verifica se a próxima posição para baixo é uma parede
        minimo = 1;
        maximo = 3;
    }else if(mapa[inimigo->y][inimigo->x] == PAREDE){
        minimo = 4;
        maximo = 6;
    }else if(MinMax(1, 2)==2){
        if(jogador->y > inimigo->y){
            minimo = 2;
            maximo = 3;
        }else{
            minimo = 4;
            maximo = 5;
        }
    }

    switch(MinMax(minimo, maximo)){
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
        if(andou_x==1 && inimigo->x>=415){
            inimigo->x = 0;
        }else if(inimigo->x<0){
            inimigo->x = 415;
        }
    }
    return;
}

void atira(tiro_t tiro[], int prop, int x, int y){
    int i=0, continua=1; // iterador
    do{
        if(tiro[i].prop == 0){
            tiro[i].prop = prop;
            tiro[i].x = x;
            tiro[i].y = y;
            continua=0;
        }
        i++;
    } while(continua);
    return;
}


/***atualiza a tela***/
int atualizaTela(int mapa[][COLUNAS], int coluna, boneco_t * jogador, boneco_t inimigo[], int inimigo_vivo[], tiro_t tiro[]){
    int i;
    int id;
    int x=0, y=0;
//    jogador->x += jogador->velocidade;
    for(i=0; i<INIMIGO_CHECK; i++){
        if(inimigo_vivo[i] != INIMIGO_MORTO){
            id = inimigo_vivo[i];
            x = inimigo[id].x;
            y = inimigo[id].y;
            //para testar limite superior:ŝ
          //  if(
         //   if(y < MIN_LIMIT || y > MAX_LIMIT){
                atualizaInimigo(mapa, &inimigo[id], jogador);
                if(MinMax(0,CHANCE_DE_TIRO)==CHANCE_DE_TIRO){
                    atira(tiro, 2, x, y);
                }
           // }
        }
    }
    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop!=0){
            if(tiro[i].prop==1){ // tiro do jogador
                tiro[i].x += VEL_BALA;
                if(tiro[i].x >= coluna + 100 - jogador->x){
                    tiro[i].prop = 0;
                }
            }else{ // tiro do inimigo
                tiro[i].x -= VEL_BALA;
                if(tiro[i].x <= coluna - 100){
                    tiro[i].prop = 0;
                }
            }
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
    inimigo.visto = 0;
    // Dados preenchidos pelos parametros
    inimigo.x = x;
    inimigo.y = y;

    return inimigo;
}

/**Checagem das posições dos inimigos**/
void carregaChecagem(boneco_t inimigo[], int inimigo_vivo[], int pos){

    int i, j=0, // iteradores
        buscando; //auxiliar para verificar se a busca pelo índice vazio continua

    // percorre todos os inimigos ainda não visto e que estão na tela
    // e os coloca na lista de checagem
    for(i=0; i<TOTAL_INIMIGO; i++){
        if(inimigo[i].visto==0 && pos+LARGURA > inimigo[i].x){
            buscando = 1;
            do{
                if(inimigo_vivo[j]==INIMIGO_MORTO){    //busca um indice vazio na lista de checagem
                    inimigo_vivo[j] = i;                //para inserir o inimigo nesse índice
                    inimigo[i].visto=1;             //inimigo já foi visto na tela
                    buscando=0;                     //para de busca um índice vazio
                }
                j++;
            } while(j<INIMIGO_CHECK && buscando);
        }
    }

    return;
}


void le_mapa(FILE *arq, int mapa[][COLUNAS], boneco_t *jogador, boneco_t inimigo[]){
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

    return;
}

void gotoxy(int x,int y){ printf("%c[%d;%df",0x1B,y,x); }

void geraQuadro(int mapa[][COLUNAS], int atual, boneco_t * jogador, boneco_t inimigo[], int inimigo_vivo[], tiro_t tiro[], int *pontuacao){
    int i, id, linha=0, coluna=0, p, reposiciona_escrita=0, posicao_inimigo;

    printf("Vidas: %d Pontos: %d \n", jogador->nvidas, *pontuacao);

    /*** Gerando Jogador ***/

    gotoxy(jogador->x, jogador->y+1);
    printf("@");
    gotoxy(jogador->x, jogador->y+2);
    printf("@@@@");

    /*** Gerando Inimigos ***/
    p = atual;
    for(i=0; i<INIMIGO_CHECK; i++){
        if(inimigo_vivo[i]!=INIMIGO_MORTO){
            id = inimigo_vivo[i];
            if(inimigo[id].y>0 && inimigo[id].x>0){
                if(p>=415) p %= 415;
                posicao_inimigo = inimigo[id].x - p;
                if(p <= 310 && posicao_inimigo>0){
                    gotoxy(posicao_inimigo, inimigo[id].y+1);
                    printf("XX");
                    gotoxy(posicao_inimigo, inimigo[id].y+2);
                    printf("XX");
                }else if(p>310){
                    posicao_inimigo = inimigo[id].x + 415 - p;

                    if(posicao_inimigo>0 && posicao_inimigo<104){
                        gotoxy(posicao_inimigo, inimigo[id].y+1);
                        printf("XX");
                        gotoxy(posicao_inimigo, inimigo[id].y+2);
                        printf("XX");
                    }

                }
            }

        }
    }

    /*** Gerando Tiros ***/
    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop!=0){
            gotoxy(tiro[i].x, tiro[i].y);
//            printf("%d", tiro[i].prop);
            if(tiro[i].prop==1) printf("--"); else printf(".");
        }
    }

    /*** Gerando Paredes ***/
    while(linha<LINHAS){
        coluna=0;
        reposiciona_escrita=0; // quando zerado, reposiciona equivale a falso
        p = atual; //coluna auxiliar
        gotoxy(1+coluna, 2+linha);
        while(coluna<LARGURA){
            if(p>=415) p %=  415; //zera periodicamente para repetir o início da tela

            if(mapa[linha][p]==PAREDE){
                if(reposiciona_escrita==1){ //
                    reposiciona_escrita=0;
                    gotoxy(1+coluna, 2+linha);
                }

                printf("C");
                coluna++;
                p++;
            }else if(mapa[linha][p]>0){
                coluna+=mapa[linha][p];
                p+=mapa[linha][p];
                gotoxy(1+coluna, 2+linha);
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

void controle(int c, boneco_t * jogador, tiro_t tiro[]){

    switch(c){
        case 's':
            jogador->y+=1;
            break;
        case 'w':
            jogador->y-=1;
            break;
        case 'd':
            if(jogador->velocidade < VEL_MAX){ //confere se a velocidade eh menor que a max
                jogador->velocidade++;         //antes de acelerar mais
                break;
            }
            break;
        case 'a':
            if(jogador->velocidade > VEL_MIN){ //confere se a velocidade ainda eh maior que a min
                jogador->velocidade--;         //antes de desacelerar
                break;
            }
            break;
        case ' ':
            atira(tiro, 1, jogador->x, jogador->y+2);
            break;

        default:
         break;
    }
    return;
}

int main(){

                    /** X,  Y, vidas, velocidade, visto  */
    boneco_t jogador = {0, 0, 3, 1, 1};
    boneco_t inimigo[TOTAL_INIMIGO];
    tiro_t tiro[MAX_TIROS]={0};
    int mapa[LINHAS][COLUNAS];
    int i, j;
    int inimigo_vivo[INIMIGO_CHECK];

    int pontuacao=0;


    FILE *arquivo;

    arquivo = fopen("mapa_turmac.txt", "r");

    le_mapa(arquivo, mapa, &jogador, inimigo);


    for(i=0; i<INIMIGO_CHECK; i++){
        inimigo_vivo[i] = INIMIGO_MORTO;
    }


    int seg=0;

    boneco_t inn;
    int foto = 0;

    printf("%d", mapa[1][0]);


    while(1){

        foto += jogador.velocidade;

        if(foto<0) foto = 415;
        else if(foto>415) foto=0;

        if(kbhit())
            controle(getchar(), &jogador, tiro);


        carregaChecagem(inimigo, inimigo_vivo, foto);
        buscaTiro(&jogador, inimigo, inimigo_vivo, tiro, &pontuacao);
        atualizaTela(mapa, foto, &jogador, inimigo, inimigo_vivo, tiro);

        printf("\033[H\033[J");

        if(jogador.nvidas==0){
            FIM_DE_JOGO(pontuacao);
            break;
        }

        geraQuadro(mapa, foto, &jogador, inimigo, inimigo_vivo, tiro, &pontuacao);

        // 17000
        usleep(20000);
    }

    return 0;
}
