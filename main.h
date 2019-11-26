/*-------------------------------*/
/*--------Meus Defines----------*/

//Menu
#define NOVO_JOGO 0
#define CONTINUAR_JOGO 1
#define SAIR 2

// Jogo
#define PAREDE 500
#define LINHAS_MAPA 35
#define COLUNAS_MAPA 415
#define COLUNAS_TELA 105
#define SALVE_ARQUIVO "_salve.bin"
#define NIVEL_INEXISTENTE -5
#define MAPA_CAMINHO "mapas/nivel%d.txt"

// Tiro
#define MAX_TIROS 100
#define VEL_BALA 5
#define DURACAO_TIRO 20

// Jogador
#define DURACAO_ANIMACAO 15
#define INTERVALO_TIRO 6
#define VEL_MIN 1
#define VEL_MAX 2

// Inimigo
#define TOTAL_INIMIGO 20 //numero máximo de inimigos em cada nivel
#define INIMIGO_MORTO -5
#define CHANCE_DE_TIRO 60 // a chance de o inimigo dar um tiro, de 1 ao definido


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
/*------------Includes-----------*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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
