#ifndef CONTROLE_H
#define CONTROLE_H

void controlePartida(int c, boneco_t * jogador, tiro_t tiro[], int mapa[][COLUNAS_MAPA], int posicao, int *intervalo, int *salvar_estado);

int controleMenu(int c, int *selecionado_indice);

#endif
