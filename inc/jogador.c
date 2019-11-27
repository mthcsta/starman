/********************int geraPosicao********************/
/*posiciona as estruturas do jogador e inimigo de      */
/*  acordo com a matriz mapa                           */
/*******************************************************/
int geraPosicao(int x, int posicao){
    int reposicao = x - posicao;
    if(posicao <= (COLUNAS_MAPA - COLUNAS_TELA) && reposicao>0 && x<COLUNAS_TELA+posicao-1){
        return reposicao;
    }else if(posicao>(COLUNAS_MAPA - COLUNAS_TELA)){
        reposicao = x - posicao;
        if(x<(COLUNAS_MAPA - COLUNAS_TELA)) reposicao += COLUNAS_MAPA;
        if(reposicao>0 && reposicao<(COLUNAS_TELA-1)){
            return reposicao;
        }
    }
    return 0;
}
