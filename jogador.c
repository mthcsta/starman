int geraPosicao(int x, int posicao){
    int reposicao = x - posicao;
    if(posicao <= (COLUNAS - LARGURA) && reposicao>0 && x<LARGURA+posicao-1){
        return reposicao;
    }else if(posicao>(COLUNAS - LARGURA)){
        reposicao = x - posicao;
        if(x<(COLUNAS - LARGURA)) reposicao += COLUNAS;
        if(reposicao>0 && reposicao<(LARGURA-1)){
            return reposicao;
        }
    }
    return 0;
}