#include <stdio.h>

void salvarEstado(int nivel, int posicao, boneco_t jogador, int pontuacao, tiro_t tiro[], int inimigos_existentes, boneco_t inimigo[], int animacao, int intervalo){
    FILE *arquivo;
    arquivo = fopen(SALVE_ARQUIVO, "w");
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
