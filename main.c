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
#define SALVE_ARQUIVO "_salve.bin"



// Tiro
#define MAX_TIROS 100
#define VEL_BALA 5
#define DURACAO_TIRO 20

// Jogador
#define DURACAO_ANIMACAO 15
#define INTERVALO_TIRO 10
#define VEL_MIN 1
#define VEL_MAX 2

// Inimigo
#define TOTAL_INIMIGO 20 //numero máximo de inimigos em cada nivel
#define INIMIGO_MORTO -5
#define CHANCE_DE_TIRO 50 // a chance de o inimigo dar um tiro, de 1 ao definido





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


/*-------------------------------*/
/*--------Meus Includes----------*/
#include "headers/kbhit.h"
#include "headers/auxiliares.h"
#include "headers/estado.h"
#include "headers/controle.h"
#include "headers/inimigo.h"
#include "headers/jogador.h"
#include "headers/mapa.h"
#include "headers/parede.h"
#include "headers/quadro.h"
#include "headers/telas.h"
#include "headers/tiro.h"

#include "inc/kbhit.c"
#include "inc/auxiliares.c"
#include "inc/estado.c"
#include "inc/controle.c"
#include "inc/inimigo.c"
#include "inc/jogador.c"
#include "inc/mapa.c"
#include "inc/parede.c"
#include "inc/quadro.c"
#include "inc/telas.c"
#include "inc/tiro.c"

int main(){

    int i=0;
    char * lista_mapas[2] = {"mapas/mapa_exemplo.txt", "mapas/mapa_turmac.txt"};
    int numero_mapa = 0;
    //inicializa a pontuação zerada
    int pontuacao=0;
    //inicializa o jogador no centro da tela com a velocidade mínima do jogo
    boneco_t jogador = {0, 0, 3, VEL_MIN};

    //estado
    int estado = 0;
    FILE *salve = NULL;

    if((estado=MENU_INICIAL())!=2){
        if(estado==1){
            salve = fopen(SALVE_ARQUIVO,"r");
            if(salve != 0){
                fread(&i, sizeof(int), 1, salve);
            }
        }

        //a partida perdura enquanto o jogador tiver vidas e ainda houver fases disponíveis
        while(jogador.nvidas>0 && i<2){
            partida(i, lista_mapas[i], &jogador, &pontuacao, salve);
            if(salve!=0){ // limpa a sujeira da variavel salve
                salve = 0;
            }
            i++;
        }

        FIM_DE_JOGO(pontuacao + (jogador.nvidas * 100));
    }else FIM_DE_JOGO(-5);

    return 0;
}

