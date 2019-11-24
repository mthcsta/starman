/***gera os inimigos na tela***/
boneco_t carregaInimigo(int x, int y){
    boneco_t inimigo;

    // dados preenchidos definidos pela funçao
    inimigo.velocidade = 1;
    inimigo.nvidas = 1;
    // Dados preenchidos pelos parametros
    inimigo.x = x;
    inimigo.y = y;

    return inimigo;
}

void atualizaInimigo(int mapa[][COLUNAS], boneco_t *inimigo, boneco_t *jogador){
    int minimo = 1, maximo = 5, andou_x=0;

    if(MinMax(1, 10)==5){
        if(inimigo->y > jogador->y){
            minimo = maximo = 2;
        }else{
            minimo = maximo = 1;
        }
    }

    switch(MinMax(minimo, maximo)){
        case 1:
            if(!ehParede(mapa, inimigo->x, inimigo->y+1))
                inimigo->y += 1;
            break;
        case 2:
            if(!ehParede(mapa, inimigo->x, inimigo->y-2) && !ehParede(mapa, inimigo->x, inimigo->y-1))
                inimigo->y -= 1;
            break;

        case 3:
            if(!ehParede(mapa, inimigo->x-1, inimigo->y+1) && !ehParede(mapa, inimigo->x-1, inimigo->y) && !ehParede(mapa, inimigo->x-1, inimigo->y-2)){
                inimigo->x -= 1;
                inimigo->y += 1;
                andou_x=1;
            }
            break;        
        case 4:
            if(!ehParede(mapa, inimigo->x-1, inimigo->y-2) && !ehParede(mapa, inimigo->x-1, inimigo->y) && !ehParede(mapa, inimigo->x-1, inimigo->y-1)){
                inimigo->x -= 1;
                inimigo->y -= 1;
                andou_x=1;
            }
            break;
        case 5:
            if(!ehParede(mapa, inimigo->x-1, inimigo->y)){
                inimigo->x -= 1;
                andou_x=1;
            }
            break;
        default: break;
    }
    if(inimigo->y<=0 || inimigo->y>35) inimigo->y = LINHAS / 2 + 1;
    if(andou_x==1 && inimigo->x<0){
        inimigo->x = COLUNAS - 1;
    }
    return;
}

void deletaInimigo(boneco_t inimigo[], int morto, int *inimigos_existentes){
    inimigo[morto] = inimigo[*inimigos_existentes-1]; //puxa o último inimigo vivo pra posicao do eliminado
    *inimigos_existentes -= 1; //elimina o inimigo da ultima pos (pq ele foi passado pra frente)
    return;
}
