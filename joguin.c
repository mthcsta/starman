#include <stdio.h>
#define TINIMIGO 21 //numero máximo de inimigos em cada nivel
#define MAX_TIROS 10
//estrutura do tiro
typedef struct{
    int x, y, existe;
}tiro_t;

typedef struct{
    int x, y, nvidas=1;
    tiro_t tiros[MAX_TIROS];
} boneco_t;

void vaiMatar(boneco_t *boneco, tiro_t *tiro){
    if(tiro->x==boneco->x && tiro->y==boneco->y){

    }

}


int main(){
    int num = 5;
    vaiMatar(&num);
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
    /**

        Y
      20 |    --             xx
              30             40     X
    **/

                    /** X,  Y, tiros  */
    boneco_t jogador = {20, 20, {0}};
    boneco_t inimigo = {40, 20, {1,1}};

    jogador.tiros[0].x = 30;
    jogador.tiros[0].y = 20;







    return 0;
}
