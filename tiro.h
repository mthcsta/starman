#ifndef TIRO_H
#define TIRO_H

void buscaTiro(boneco_t *jogador, boneco_t inimigo[], tiro_t tiro[],int posicao, int * pontuacao, int *inimigos_existentes, int *animacao);

void geraTiro(tiro_t tiro[], int prop, int x, int y);

#endif