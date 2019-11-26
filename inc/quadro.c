/***atualiza a Quadro***/
void atualizaQuadro(int mapa[][COLUNAS_MAPA], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *inimigos_existentes, int *animacao, int *intervalo, int *salvar_estado_mensagem){
    int i; // iterador
	
	// Contagens regressivas:
    if(*animacao>0) *animacao -= 1;
    if(*intervalo>0) *intervalo -= 1;
    if(*salvar_estado_mensagem>0) *salvar_estado_mensagem -= 1;


    if(ehParede(mapa, jogador->x+posicao, jogador->y) || //se a pos x do jogador é igual a da parede de trás OU
       ehParede(mapa, jogador->x+posicao+3, jogador->y)){ //se a pos x do jogador é igual a da parede da frente
        jogador->nvidas--; //o jogador perde uma vida
        *animacao = DURACAO_ANIMACAO; //a perda da vida é sinalizada por animação
        jogador->y = buscaParede(mapa, jogador->x+posicao+4, 0, 1, 0) + 2;
        jogador->velocidade = VEL_MIN;
    }
    for(i=0; i<*inimigos_existentes; i++){
        if( *animacao==0 && (jogador->x+posicao == inimigo[i].x || jogador->x+posicao+3 == inimigo[i].x) && //verifica se o jogador e inimigo estão na mesma pos x
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
                if(tiro[i].x>=COLUNAS_MAPA) tiro[i].x = 0;
            }else{ // tiro do inimigo
                tiro[i].x -= VEL_BALA;
                if(tiro[i].x<0) tiro[i].x=COLUNAS_MAPA-1;
            }
            // remove tiro após o fim da duração
            if(tiro[i].duracao==0){
                tiro[i].prop = 0;
            }else tiro[i].duracao--;
        }
    }
    return;
}

void geraQuadro(int mapa[][COLUNAS_MAPA], int posicao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *pontuacao, int *inimigos_existentes, int *animacao, int *salvar_estado_mensagem, int nivel){
    int i, linha=0, coluna_tela=0, coluna_mapa=0, reposiciona_escrita=0, posicao_inimigo;

    //Print na primeira linha na tela dos dados do jogo (Fase, Vidas e Pontos)
    printf("|\u2605 Fase: %d | \u2665  Vidas: %d | \u263A  Pontos: %d |", nivel, jogador->nvidas, *pontuacao);
    if(*salvar_estado_mensagem>0)      //quando o estado do jogo eh salvo
        printa(-1, "Estado Salvo"); //uma mensagem aparece na tela para confirmar
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
                if(tiro[i].prop==1) printf("\u22C6\u066D"); else printf("\u1A15");
            }
        }
    }
    /*** Gerando Paredes ***/
	linha = 0;//contador de linhas_LINHAS_MAPA
    while(linha<LINHAS_MAPA){ //enquanto nao chegar na ultima linha do mapa
        coluna_tela = 0;//coluna da tela atual (36x105) eh zero
        reposiciona_escrita=0; //n ha reposicionamento da escrita
        coluna_mapa = posicao; //coluna do vetor do mapa (35x415) recebe o valor da posição da tela
        gotoxy(coluna_tela, linha);//vai pra essa posição e imprime
        while(coluna_tela<COLUNAS_TELA){//percorre a coluna
            if(coluna_mapa>=COLUNAS_MAPA) 
		coluna_mapa %= COLUNAS_MAPA; //zera periodicamente para repetir o início da tela

            if(ehParede(mapa, coluna_mapa, linha)){//se encontrar um caracter
                if(reposiciona_escrita==1){ //aciona reposicionamento para escrita
                    reposiciona_escrita=0;
                    gotoxy(coluna_tela, linha);
                }

                printf("\u2588");
                coluna_tela++;
                coluna_mapa++;
            }else{
                coluna_tela++;
                coluna_mapa++;
                reposiciona_escrita=1;
            }
        }
        printf("\n");
        linha++;
    }
    
    
}
