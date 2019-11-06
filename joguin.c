#include <stdio.h>
#define TINIMIGO 21 //numero máximo de inimigos em cada nivel
#define MAX_TIROS 10
//estrutura do tiro
typedef struct{
    int x, y, existe;
}tiro_t;


//estrutura de um jogador
typedef struct {
    int x, y;
    int nvidas;
    tiro_t tiros[MAX_TIROS];
}jogador_t;

//estrutura de um inimigo
typedef struct{
    int x, y;
    tiro_t tiros;
}inimigo_t;


int main(){

    //
    /**
        vetor_para_checagem = [inimigo[0], inimigo[1]];

        for(i=0; i<tamanho_tiros_jogador; i++){
            for(j=0; j<tamanho_vetor_para_checagem; j++){
                if(jogador.tiros[i].x == vetor_para_checagem[j].x && jogador.tiros[i].y == vetor_para_checagem[j].y)
                    return MATOU;
            }
        }

    **/

    jogador_t jogador = {20, 20, {0}};

    inimigo_t inimigo = {40, 20, {1,1}};

    jogador.tiros[0].x = 30;
    jogador.tiros[0].y = 20;







    return 0;
}
