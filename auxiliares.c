#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#ifndef LARGURA
#define LARGURA 105
#endif

//sorteia entre um numero minimo e máximo
int MinMax(int min, int max){
    return (min + (rand() % (max-min+1)));
}

// foi adicionado +2 ao Y pois os dados começam a ser trabalhados a partir da segunda linha.
// foi adicionado +1 ao X pois o visual trabalha a partir do 1, os dados a partir do 0.
void gotoxy(int x,int y){ printf("%c[%d;%df",0x1B,2+y,1+x); }

void printa(int pos, int linha, char *string){
    gotoxy(LARGURA/pos-strlen(string)/2,linha);
    if(strlen(string)>0) printf("%s", string);
}

void limpaQuadro(void){
    printf("\033[H\033[J"); 
    return;
}
