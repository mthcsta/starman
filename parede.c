int ehParede(int mapa[][COLUNAS], int x,int y){
    return mapa[y][x] == PAREDE;
}

int buscaParede(int mapa[][COLUNAS], int x, int y, int muda, int limite){
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
