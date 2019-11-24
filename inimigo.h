#ifndef INIMIGO_H
#define INIMIGO_H

boneco_t carregaInimigo(int x, int y);

void atualizaInimigo(int mapa[][COLUNAS], boneco_t *inimigo, boneco_t *jogador);

void deletaInimigo(boneco_t inimigo[], int morto, int *inimigos_existentes);

#endif