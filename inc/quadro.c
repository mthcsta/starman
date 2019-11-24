/***atualiza a Quadro***/
int atualizaQuadro(int mapa[][COLUNAS], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *inimigos_existentes, int *animacao, int *intervalo, int *salvar_estado_mensagem){
    int i;
    int id;
    int x=0, y=0;

    if(*animacao>0) *animacao -= 1;
    if(*intervalo>0) *intervalo -= 1;
    if(*salvar_estado_mensagem>0) *salvar_estado_mensagem -= 1;
    
    if(ehParede(mapa, jogador->x+posicao, jogador->y) || //se a pos x do jogador é igual a da parede de trás OU
       ehParede(mapa, jogador->x+posicao+3, jogador->y) || //se a pos x do jogador é igual a da parede da frente OU
       ehParede(mapa, jogador->x+posicao, jogador->y-1) || //se a pos y do jogador é igual ao chão OU
       ehParede(mapa, jogador->x+posicao, jogador->y)){ //se a pos y do jogador é igual ao teto
        jogador->nvidas--; //o jogador perde uma vida
        *animacao = DURACAO_ANIMACAO; //a perda da vida é sinalizada por animação
        jogador->y = buscaParede(mapa, jogador->x+posicao+4, 0, 1, 0) + 2;
        jogador->velocidade = VEL_MIN;
    }
    for(i=0; i<*inimigos_existentes; i++){
        if(jogador->x+posicao == inimigo[i].x && //verifica se o jogador e inimigo estão na mesma pos x
            (fabs(jogador->y - inimigo[i].y) <= 1)){ //verifica o modulo da distancia vertical entre inimigo e jogador
            jogador->nvidas--; // se o jogador estiver dentro da parte visual do inimigo, jogador perde uma vida.
            *animacao = DURACAO_ANIMACAO; //perda da vida sinalizada por animação
        }

        atualizaInimigo(mapa, &inimigo[i], jogador);
        if(MinMax(0,CHANCE_DE_TIRO)==CHANCE_DE_TIRO){
            geraTiro(tiro, 2, inimigo[i].x, inimigo[i].y);
        }
    }
    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop!=0){
            if(tiro[i].prop==1){ // tiro do jogador
                tiro[i].x += VEL_BALA;
                if(tiro[i].x>=COLUNAS) tiro[i].x = 0;
            }else{ // tiro do inimigo
                tiro[i].x -= VEL_BALA;
                if(tiro[i].x<0) tiro[i].x=COLUNAS-1;
            }
            // remove tiro após o fim da duração
            if(tiro[i].duracao==0){
                tiro[i].prop = 0;
            }else tiro[i].duracao--;
        }
    }
    return x;
}

void geraQuadro(int mapa[][COLUNAS], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *pontuacao, int *inimigos_existentes, int *animacao, int *salvar_estado_mensagem, int nivel){
    int i, id, linha=0, coluna=0, p, reposiciona_escrita=0, posicao_inimigo;

    printf("|\u2605 Fase: %d | \u2665  Vidas: %d | \u263A  Pontos: %d |", nivel, jogador->nvidas, *pontuacao);
    if(*salvar_estado_mensagem>0)  
        printa(2, -1, "Estado Salvo");
    printf("\n");

    /*** Gerando Jogador ***/
    if(*animacao%3==0){

        gotoxy(jogador->x, jogador->y-1);
        printf("\u25E3");
        gotoxy(jogador->x, jogador->y);
        printf("\u2B15\u25A3\u25A4\u2AFA");

    }
    /*** Gerando Inimigos ***/
    for(i=0; i<*inimigos_existentes; i++){
        posicao_inimigo = geraPosicao(inimigo[i].x, posicao);
        if(posicao_inimigo>0){
            gotoxy(posicao_inimigo, inimigo[i].y-1);
            printf("\u29E9\u29E8");
            gotoxy(posicao_inimigo, inimigo[i].y);
            printf("\u25ED\u25EE");
        }
    }
    /*** Gerando Tiros ***/
    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop!=0){
            posicao_inimigo = geraPosicao(tiro[i].x, posicao);
            if(posicao_inimigo>0){
                gotoxy(posicao_inimigo, tiro[i].y);
                if(tiro[i].prop==1) printf("--"); else printf(".");
            }
        }
    }
    /*** Gerando Paredes ***/
    while(linha<LINHAS){
        coluna=0;
        reposiciona_escrita=0; // quando zerado, reposiciona equivale a falso
        p = posicao; //coluna auxiliar
        gotoxy(coluna, linha);
        while(coluna<LARGURA){
            if(p>=COLUNAS) p %=  COLUNAS; //zera periodicamente para repetir o início da tela

            if(ehParede(mapa, p, linha)){
                if(reposiciona_escrita==1){ //
                    reposiciona_escrita=0;
                    gotoxy(coluna, linha);
                }

                printf("\u2588");
                coluna++;
                p++;
            }else if(mapa[linha][p]>0){
                coluna+=mapa[linha][p];
                p+=mapa[linha][p];
                gotoxy(coluna, linha);
            }else{
                coluna++;
                p++;
                reposiciona_escrita=1;
            }
        }
        printf("\n");
        linha++;
    }
}
