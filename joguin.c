#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define TOTAL_INIMIGO 20 //numero máximo de inimigos em cada nivel
#define INIMIGO_CHECK TOTAL_INIMIGO/2 // inimigos que checaremos
#define CHECAGEM_VAZIA -5

#define MAX_TIROS 20


#define WIDTH 105
#define HEIGHT 36

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



void buscaTiro(boneco_t player, boneco_t inimigo[], int checagem[], tiro_t tiro[]){
    int i, j, // iteradores
        id; //variável auxiliar para guardar o id do inimigo

    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop>0){ //se o tiro existe, verifica sua posição em relação ao boneco

            /*** Matando os Inimigos *****/
            if(tiro[i].prop==1){    //se o tiro é do player...
                for(j=0; j<INIMIGO_CHECK; j++){ //percorre a lista de checagem dos inimigos na tela
                    if(checagem[j]!=CHECAGEM_VAZIA){    //se o índice da checagem for diferente de vazio
                        id = checagem[j];               //pega o índice do inimigo guardado na checagem
                        if(tiro[i].x==inimigo[id].x && tiro[i].y==inimigo[id].y){   //se as coordenadas x e y do tiro coincidir com as do inimigo
                            inimigo[id].nvidas--;            //o inimigo perde a vida
                            checagem[j] = CHECAGEM_VAZIA;   //e remove o índice do inimigo da checagem
                            tiro[i].prop=0;                 //e o tiro deixa de existir
                            printf("O INIMIGO MORREU!!\n");
                        }
                    }
                }
            }
            /*** Matando o Jogador *****/
            //Se as coordenadas x e y do tiro coincidem com as do jogador, o jogador perde uma vida
            else if(tiro[i].prop==2 && tiro[i].x==player.x && tiro[i].y==player.y){
                player.nvidas--;
                tiro[i].prop=0; //o tiro deixa de existir
                printf("morreu um pouco\n");
                if(player.nvidas == 0){ //se o jogador ficar com 0 vidas, game-over pra ele
                    printf("ja era pra ti\n");
                }
            }

        }
    }
    return;
}

int atualizaTela(boneco_t player, boneco_t inimigo[], int checagem[], tiro_t tiro[]){
    player.x += player.velocidade;
}

/***gera os inimigos na tela***/
void carregaInimigos(boneco_t inimigo[]){
    int i;
    int min=10, max=400; //valores para gerar as posições aleatórias

    srand(time(0));

    //define os atributos da estrutura de um inimigo
    for(i=0; i<TOTAL_INIMIGO; i++){
        inimigo[i].velocidade = 1;
        inimigo[i].nvidas = 1;
        inimigo[i].visto = 0;
        inimigo[i].x = min + (rand() % (max-min+1));
        inimigo[i].y = min + (rand() % (max-min+1));
    }

    return;
}

/**Checagem das posições dos inimigos**/
void carregaChecagem(boneco_t inimigo[], int checagem[], int pos){

    int i, j=0; // iteradores
        buscando; //auxiliar para verificar se a busca pelo índice vazio continua

    // percorre todos os inimigos ainda não visto e que estão na tela
    // e os coloca na lista de checagem
    for(i=0; i<TOTAL_INIMIGO; i++){
        if(inimigo[i].visto==0 && pos+WIDTH > inimigo[i].x){
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

int main(){
                    /** X,  Y, vidas, velocidade, visto  */
    boneco_t player = {20, 20, 3, 1, 1};
    boneco_t inimigo[TOTAL_INIMIGO];
    tiro_t tiro[MAX_TIROS];

    int i, checagem[INIMIGO_CHECK];

    for(i=0; i<INIMIGO_CHECK; i++){
        checagem[i] = CHECAGEM_VAZIA;
    }

    carregaInimigos(inimigo);

    carregaChecagem(inimigo, checagem, player.x);

    for(int k=0; k<INIMIGO_CHECK; k++){
        if(checagem[k]!=CHECAGEM_VAZIA)
            printf("\nfoi visto o inimigo #%d\n", checagem[k]);
    }


    tiro[0].prop = 1;
    tiro[0].x = 40;
    tiro[0].y = 20;

    tiro[1].prop = 2;
    tiro[1].x = 20;
    tiro[1].y = 20;

/*
    while(1){
        atualizaTela(player, inimigo, checagem, tiro);
        sleep(0.5);
    }
*/
    buscaTiro(player, inimigo, checagem, tiro);




    return 0;
}
