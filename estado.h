#ifndef ESTADO_H
#define ESTADO_H

void salvarEstado(int nivel, int posicao, boneco_t jogador, int pontuacao, tiro_t tiro[], int inimigos_existentes, boneco_t inimigo[], int animacao, int intervalo);

void carregarEstado(FILE * salve, int *posicao, boneco_t *jogador, int *pontuacao, tiro_t tiro[], int *inimigos_existentes, boneco_t inimigo[], int *animacao, int *intervalo);

#endif