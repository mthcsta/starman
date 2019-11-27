/********************int ehParede********************/
/*     Recebe uma posição vertical e horizontal     */
/*    e verifica se corresponde a uma parede        */
/****************************************************/
int ehParede(int mapa[][COLUNAS_MAPA], int x, int y){
    return mapa[y][x] == PAREDE;
}

/********************int buscaParede********************/
/*Função que auxilia na redefinição da pos. do jogador*/
/*verificando as posições onde se encontram as paredes*/
/******************************************************/
int buscaParede(int mapa[][COLUNAS_MAPA], int x, int y, int muda, int limite){
    if(ehParede(mapa, x, y)){
        if(muda==1)
            return y;
        else
            return x;
    }else{
        if(muda==1){
            y++;
        }else{
            x++;
        }
        return buscaParede(mapa, x, y, muda, limite);
    }
}
