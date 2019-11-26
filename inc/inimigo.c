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

void atualizaInimigo(int mapa[][COLUNAS_MAPA], boneco_t *inimigo, boneco_t *jogador){
    int minimo = 1, maximo = 5;//variaveis para sorteio de movimento
    int andou_x=0;

    if(MinMax(1, 10)==5){//gera movimentos esporadicos direcionados ao jogador
        if(inimigo->y > jogador->y){//se o inimigo esta abaixo do jogador
            minimo = maximo = 2;//direciona para o case 2(inimigo se move pra cima)
        }else{
            minimo = maximo = 1;//direciona para o case 1(o inimigo se move pra baixo)
        }
    }
    /**Gera os movimentos dos inimigos***/
    switch(MinMax(minimo, maximo)){//faz um sorteio com srand
        case 1://se a prox pos. do inimigo pra baixo nao eh parede
            if(!ehParede(mapa, inimigo->x, inimigo->y+1))
                inimigo->y += 1;//o inimigo se move uma pos. pra baixo
            break;
        case 2://se nem a prox nem a seguinte posição acima do inimigo eh parede
            if(!ehParede(mapa, inimigo->x, inimigo->y-2) && !ehParede(mapa, inimigo->x, inimigo->y-1))
                inimigo->y -= 1;//o inimigo se move para cima
            break;

        case 3://se as proximas posições do inimigo...
            if(!ehParede(mapa, inimigo->x-1, inimigo->y+1) &&//pra esq e pra baixo
	        !ehParede(mapa, inimigo->x-1, inimigo->y) && //pra esq
            !ehParede(mapa, inimigo->x-1, inimigo->y-2)){//pra esq e acima, nao sao parede
                inimigo->x -= 1;//o inimigo se move pra esq e pra baixo
                inimigo->y += 1;
                andou_x=1;//seu avanço na esq eh marcado(?????)
            }
            break;
        case 4://se a proxima posiçao do inimigo...
            if(!ehParede(mapa, inimigo->x-1, inimigo->y-2) &&//pra esq e pra cima
 	        !ehParede(mapa, inimigo->x-1, inimigo->y) &&//pra esq
            !ehParede(mapa, inimigo->x-1, inimigo->y-1)){//pra esq e pra cima, n eh parede
                inimigo->x -= 1;//move o inimigo pra esq
                inimigo->y -= 1;//e pra cima
                andou_x=1;//seu avanço na esq eh marcado
            }
            break;
        case 5://se a proxima posiçao do inimigo...
            if(!ehParede(mapa, inimigo->x-1, inimigo->y)){//pra esq nao eh parede
                inimigo->x -= 1;//o inimigo se move pra esq
                andou_x=1;//seu avanço na esq eh marcado
            }
            break;
        default: break;
    }//caso o inimigo esteja fora da tela, eh reposicionado dentro do mapa
    if(inimigo->y<=0 || inimigo->y>35){
        inimigo->y = LINHAS_MAPA / 2 + 1;
    }
    if(andou_x==1 && inimigo->x<0){
        inimigo->x = COLUNAS_MAPA - 1;
    }
    return;
}

void deletaInimigo(boneco_t inimigo[], int morto, int *inimigos_existentes){
    inimigo[morto] = inimigo[*inimigos_existentes-1]; //puxa o último inimigo vivo pra posicao do eliminado
    *inimigos_existentes -= 1; //elimina o inimigo da ultima pos (pq ele foi passado pra frente)
    return;
}
