#include <stdio.h>
/*************************************************************************************/
/* * * * * * * * * * * * * * * SALVA O ESTADO DO JOGO * * * * * * * * * * * * * * *  */
/* * *   Escreve no arquivo SALVE_ARQUIVO o estado atual do jogo:                * * */
/*	Nivel, posição do jogador, pontuação, inimigos restantes e suas posições,        */
/*      o estado de animação e o intervalo                                           */
/*************************************************************************************/
void salvarEstado(int nivel, int posicao, boneco_t jogador, int pontuacao, tiro_t tiro[], int inimigos_existentes, boneco_t inimigo[], int animacao, int intervalo){
    FILE *arquivo;
    arquivo = fopen(SALVE_ARQUIVO, "w"); //abre o arquivo em  modo de escrita
    fwrite(&nivel, sizeof(int), 1, arquivo);
    fwrite(&posicao, sizeof(int), 1, arquivo);
    fwrite(&jogador, sizeof(boneco_t), 1, arquivo);
    fwrite(&pontuacao, sizeof(int), 1, arquivo);
    fwrite(tiro, sizeof(MAX_TIROS), 1, arquivo);
    fwrite(&inimigos_existentes, sizeof(int), 1, arquivo);
    fwrite(inimigo, sizeof(boneco_t), inimigos_existentes, arquivo);
    fwrite(&animacao, sizeof(int), 1, arquivo);
    fwrite(&intervalo, sizeof(int), 1, arquivo);
    fclose(arquivo);
    return;
}

/*************************************************************************************/
/* * * * * * * * * * * * * * LE O ÚLTIMO ESTADO DO JOGO * * * * * * * * * * * * * *  */
/* * *   Le as informações sobre o ultimo jogo salvas no arquivo:                * * */
/*	Nivel, posição do jogador, pontuação, inimigos restantes e suas posições,        */
/*      o estado de animação e o intervalo                                           */
/*************************************************************************************/
FILE * carregarNivelEstado(int *nivel){
	FILE *salve;
	salve = fopen(SALVE_ARQUIVO, "r");
	
	fread(nivel, sizeof(int), 1, salve);	
	
	return salve;	
}
void carregarEstado(FILE * salve, int *posicao, boneco_t *jogador, int *pontuacao, tiro_t tiro[], int *inimigos_existentes, boneco_t inimigo[], int *animacao, int *intervalo){
    fread(posicao, sizeof(int), 1, salve);
    fread(jogador, sizeof(boneco_t), 1, salve);
    fread(pontuacao, sizeof(int), 1, salve);
    fread(tiro, sizeof(MAX_TIROS), 1, salve);
    fread(inimigos_existentes, sizeof(int), 1, salve);
    fread(inimigo, sizeof(boneco_t), *inimigos_existentes, salve);
    fread(animacao, sizeof(int), 1, salve);
    fread(intervalo, sizeof(int), 1, salve);
    fclose(salve);
    return;
}
