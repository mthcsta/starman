#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "kbhit.c"



#define TOTAL_INIMIGO 20 //numero máximo de inimigos em cada nivel
#define INIMIGO_CHECK 8 // inimigos que checaremos
#define CHECAGEM_VAZIA -5


#define PAREDE 500

#define MAX_TIROS 20
#define VEL_BALA 10

#define LINHAS 35
#define COLUNAS 415

#define LARGURA 105
#define ALTURA 36


#define MIN_LIMIT 5
#define MAX_LIMIT 29

/**

[-5, 2, -5, 9,...]

for(i=0; i<INIMIGO_CHECK; i){
    if(vetor_checagem[i]!=CHECAGEM_VAZIA){

    }

}

**/

/*-------------------------------*/
/*------------Estruturas---------*/

//estrutura do tiro
typedef struct{
    int x, y,
    prop; // 0: Inexistente, 1: Player, 2: Inimigo
}tiro_t;

typedef struct{
    int x, y,
    nvidas,
    velocidade,
    visto; // visto sera utilizado para checagem
} boneco_t;



void buscaTiro(boneco_t *jogador, boneco_t inimigo[], int checagem[], tiro_t tiro[], int * pontuacao){
    int i, j, // iteradores
        id; //variável auxiliar para guardar o id do inimigo

    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop>0){ //se o tiro existe, verifica sua posição em relação ao boneco

            /*** Matando os Inimigos *****/
            if(tiro[i].prop==1){    //se o tiro é do jogador...
                for(j=0; j<INIMIGO_CHECK; j++){ //percorre a lista de checagem dos inimigos na tela
                    if(checagem[j]!=CHECAGEM_VAZIA){    //se o índice da checagem for diferente de vazio
                        id = checagem[j];               //pega o índice do inimigo guardado na checagem
                        if(tiro[i].x<=inimigo[id].x && tiro[i].x+VEL_BALA>=inimigo[id].x &&
                            (tiro[i].y==inimigo[id].y || tiro[i].y==inimigo[id].y-1) ){   //se as coordenadas x e y do tiro == as do inimigo
                            inimigo[id].nvidas--;            //o inimigo perde a vida
                            checagem[j] = CHECAGEM_VAZIA;   //e remove o índice do inimigo da checagem
                            tiro[i].prop=0;                 //e o tiro deixa de existir
//                            printf("O INIMIGO MORREU!!\n");
                            *pontuacao+=10;
                        }
                    }
                }
            }
            /*** Matando o Jogador *****/
            //Se as coordenadas x e y do tiro coincidem com as do jogador, o jogador perde uma vida
            else if(tiro[i].prop==2 && tiro[i].x>=jogador->x && tiro[i].x-VEL_BALA<=jogador->x &&
                    (tiro[i].y==jogador->y || tiro[i].y==jogador->y-1)){
                jogador->nvidas--;
                tiro[i].prop=0; //o tiro deixa de existir
                printf("morreu um pouco\n");
                if(jogador->nvidas == 0){ //se o jogador ficar com 0 vidas, game-over pra ele
                    printf("ja era pra ti\n");
                }
            }

        }
    }
    return;
}

int MinMax(int min, int max){
//    srand(time(0));
    return (min + (rand() % (max-min+1)));
}
//sorteia os movimentos do inimigo

void atualizaInimigo(int mapa[][COLUNAS], boneco_t *inimigo){
    int minimo = 1, maximo = 6;
    if(mapa[inimigo->y-1][inimigo->x] == PAREDE){
        minimo = 1;
        maximo = 3;
    }
    if(mapa[inimigo->y][inimigo->x] == PAREDE){
        minimo = 4;
        maximo = 6;
    }

    switch(MinMax(minimo, maximo)){
        case 1:
            inimigo->x += 1;
            break;
        case 2:
            inimigo->y += 1;
            break;
        case 3:
            inimigo->x += 1;
            inimigo->y += 1;
            break;
        case 4:
            inimigo->x -= 1;
            inimigo->y -= 1;
            break;
        case 5:
            inimigo->y -= 1;
        case 6:
            inimigo->x -= 1;
        default: break;
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
int atualizaTela(int mapa[][COLUNAS], int coluna, boneco_t jogador, boneco_t inimigo[], int checagem[], tiro_t tiro[]){
    int i;
    int id;
    int x=0, y=0;
    jogador.x += jogador.velocidade;
    for(i=0; i<INIMIGO_CHECK; i++){
        if(checagem[i] != CHECAGEM_VAZIA){
            id = checagem[i];
            x = inimigo[id].x;
            y = inimigo[id].y;
            //para testar limite superior:
          //  if(
         //   if(y < MIN_LIMIT || y > MAX_LIMIT){
                atualizaInimigo(mapa, &inimigo[id]);
                if(MinMax(0,5)==5){
                    atira(tiro, 2, x, y);
                }
           // }
        }
    }
    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop!=0){
            if(tiro[i].prop==1){ // tiro do jogador
                tiro[i].x += VEL_BALA;
                if(tiro[i].x >= coluna + 105){
                    tiro[i].prop = 0;
                }
            }else{ // tiro do inimigo
                tiro[i].x -= VEL_BALA;
                if(tiro[i].x <= coluna){
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
void carregaChecagem(boneco_t inimigo[], int checagem[], int pos){

    int i, j=0, // iteradores
        buscando; //auxiliar para verificar se a busca pelo índice vazio continua

    // percorre todos os inimigos ainda não visto e que estão na tela
    // e os coloca na lista de checagem
    for(i=0; i<TOTAL_INIMIGO; i++){
        if(inimigo[i].visto==0 && pos+LARGURA > inimigo[i].x){
            buscando = 1;
            do{
                if(checagem[j]==CHECAGEM_VAZIA){    //busca um indice vazio na lista de checagem
                    checagem[j] = i;                //para inserir o inimigo nesse índice
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

void geraQuadro(int mapa[][COLUNAS], int atual, boneco_t jogador, boneco_t inimigo[], int checagem[], tiro_t tiro[], int pontuacao){
    int i, id, linha=0, coluna=0, p, mudou=0, posicao_inimigo;

    printf("Vidas: %d Pontos: %d \n", jogador.nvidas, pontuacao);

    /*** Gerando Jogador ***/

    gotoxy(jogador.x, jogador.y+1);
    printf("@");
    gotoxy(jogador.x, jogador.y+2);
    printf("@@@@");

    /*** Gerando Inimigos ***/
    p = atual;
    for(i=0; i<INIMIGO_CHECK; i++){
        if(checagem[i]!=CHECAGEM_VAZIA){
            id = checagem[i];
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
        mudou=0;
        p = atual; //coluna auxiliar
        gotoxy(1+coluna, 2+linha);
        while(coluna<105){
            if(p>=415) p %=  415; //zera periodicamente para repetir o início da tela

            if(mapa[linha][p]==PAREDE){
                if(mudou==1){ //
                    mudou=0;
                    gotoxy(1+coluna, 2+linha);
                }

                printf("C");
                coluna++;
                p++;
            }else if(mapa[linha][p]>0){
                coluna+=mapa[linha][p];
                p+=mapa[linha][p];
                gotoxy(1+coluna, 2+linha);
            //printf("L");
            }else{
//            printf("0");
                coluna++;
                p++;
                mudou=1;
            }
        }
        printf("\n");
        linha++;
    }


}

void printa(int pos, int linha, char *string){
    gotoxy(LARGURA/pos-strlen(string)/2,linha);
    if(string!="") printf("%s", string);
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
            jogador->velocidade++;
            break;
        case 'a':
            jogador->velocidade--;
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
    int i, j, checagem[INIMIGO_CHECK];
    int pontuacao=0;


    FILE *arquivo;

    arquivo = fopen("mapa_turmac.txt", "r");

    le_mapa(arquivo, mapa, &jogador, inimigo);


    for(i=0; i<INIMIGO_CHECK; i++){
        checagem[i] = CHECAGEM_VAZIA;
    }


    int seg=0;

    boneco_t inn;
    int foto = 0;

    printf("%d", mapa[1][0]);


    while(1){

        foto += jogador.velocidade;

        if(kbhit())
            controle(getchar(), &jogador, tiro);


        carregaChecagem(inimigo, checagem, foto);
        buscaTiro(&jogador, inimigo, checagem, tiro, &pontuacao);
        atualizaTela(mapa, foto, jogador, inimigo, checagem, tiro);

        printf("\033[H\033[J");

        if(jogador.nvidas==0){
            FIM_DE_JOGO(pontuacao);
            break;
        }

        geraQuadro(mapa, foto, jogador, inimigo, checagem, tiro, pontuacao);

        // 17000
        usleep(80000);
    }

    return 0;
}
