#ifndef TELAS_H
#define _TELAS_H

int MENU_INICIAL(void);

void partida(int nivel, char nome_mapa[], boneco_t * jogador, int * pontuacao, FILE * salve);

void FIM_DE_JOGO(int score);

#endif