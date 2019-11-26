#ifndef TELAS_H
#define _TELAS_H

int MENU_INICIAL(som mp3);

void partida(int nivel, char nome_mapa[], boneco_t * jogador, int * pontuacao, FILE * salve, som mp3);

void FIM_DE_JOGO(int score);

#endif
