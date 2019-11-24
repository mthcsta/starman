#ifndef QUADRO_H
#define QUADRO_H

int atualizaQuadro(int mapa[][COLUNAS], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *inimigos_existentes, int *animacao, int *intervalo, int *salvar_estado_mensagem);

void geraQuadro(int mapa[][COLUNAS], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *pontuacao, int *inimigos_existentes, int *animacao, int *salvar_estado_mensagem, int nivel);

#endif