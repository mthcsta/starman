#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

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

void logo(void){
  printa(2, 3, ".dP\"Y8 888888    db    88\"\"Yb 8b    d8    db    88b 88 ");
  printa(2, 4, "\\`Ybo.\"   88     dPYb   88__dP 88b  d88   dPYb   88Yb88 ");
  printa(2, 5, "o.\\`Y8b   88    dP__Yb  88\"Yb  88YbdP88  dP__Yb  88 Y88 ");
  printa(2, 6, "8bodP'   88   dP\"\"\"\"Yb 88  Yb 88 YY 88 dP\"\"\"\"Yb 88  Y8 ");
}
