#include <stdio.h>

/**

VETOR
   20   24   28   32    36
   0    1    2    3     4
   7    7

   20
*pontero = 7
pontero=20;
pontero++;
pontero=pontero+4;
pontero=24;
*pontero=7

20
24
pontero = 20
i = 2
pontero++
pontero++
pontero+=1
pontero + 2 = 28
**/

int muda(int *pontero, int tamanho){
    for(int i=0; i<tamanho; i++){
        *(pontero+i) = 7;
    }
}

int main(){
    int i, vetor[5];

    muda(&vetor, 5);

    for(i=0; i<5; i++){
        printf("%d\n", vetor[i]);
    }

    return 0;
}
