/**************************void atualizaQuadro**************************/
/*contem todos os dados que sofrem alterações no decorrer do jogo.    */
/*Inimigo, jogador, tiro, posição da tela, ...atualiza os dados      */
/********************************************************************/
void atualizaQuadro(int mapa[][COLUNAS_MAPA], int posicao, int *pontuacao, boneco_t * jogador, boneco_t inimigo[], tiro_t tiro[], int *inimigos_existentes, int *animacao, int *intervalo, int *salvar_estado_mensagem){
    int i; // iterador

	// busca todos os tiros existentes no jogo e faz uma checagem
	// se está na mesma posição que um jogador ou inimigo
	// caso esteja, tira a vida dele.
    buscaTiro(jogador, inimigo, tiro, posicao, pontuacao, inimigos_existentes, animacao);


	// Contagens regressivas:
    if(*animacao>0) *animacao -= 1; // enquanto animação for maior que 0, o jogador fica piscando
    if(*intervalo>0) *intervalo -= 1; // enquanto intervalo for maior que zero, o jogador nao pode atirar
    if(*salvar_estado_mensagem>0) *salvar_estado_mensagem -= 1; // enquanto salvar_estado_mensagem for maior que zero, uma mensagem "Estado Salvo" é exibida

	// Checagem se o jogador está encostando em uma parede:
	//  0 indica o primeiro caractere da nave(parte traseira),
	//  3 o ultimo(parte frontal)
    if( ehParede(mapa, jogador->x+posicao+0, jogador->y) || //se a pos x do jogador é igual a da parede de trás OU
        ehParede(mapa, jogador->x+posicao+1, jogador->y) || //se as partes internas
		ehParede(mapa, jogador->x+posicao+2, jogador->y) || // da nave estão numa parede.
		ehParede(mapa, jogador->x+posicao+3, jogador->y)
	  ){ //se a pos x do jogador é igual a da parede da frente

		if(*animacao==0){ // e se o jogador deixou de piscar
			jogador->nvidas--; //o jogador perde uma vida
	        *animacao = DURACAO_ANIMACAO; //a perda da vida é sinalizada por animação
		}
        jogador->y = buscaParede(mapa, jogador->x+posicao+4, 0, 1, 0) + 2; // a posição do jogador é redefinida para um lugar aberto sem parede.
        jogador->velocidade = VEL_MIN; // o jogador volta para a velocidade minima
    }

	// Checando os inimigos existentes:
    for(i=0; i<*inimigos_existentes; i++){
		// Checando se o inimigo está encostando no jogador:
		//
        if( *animacao==0 &&
	    jogador->x+posicao+0 <= inimigo[i].x+1 && jogador->x+posicao+3 >= inimigo[i].x && //verifica se o jogador e inimigo estão na mesma pos x
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
/****************************void geraQuadro****************************/
/*Função para desenhar a tela                                         */
/*              Exibe os dados que a atualizaQuadro atualiza         */
/********************************************************************/
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
	gotoxy(0,0); // posiciona para o inicio das paredes

	if(COLUNAS_MAPA - posicao >= COLUNAS_TELA){ // checa se o mapa precisa ser lido o seu inicio
		for(linha=0; linha<LINHAS_MAPA; linha++){ // percorre as linhas
			reposiciona_escrita = 1; // variavel para auxiliar na reposição da escrita dos blocos
			coluna_tela = 0; // coluna da tela atual(37x105) eh zero
			for(coluna_mapa=posicao; coluna_tela<COLUNAS_TELA; coluna_mapa++, coluna_tela++){ // percorre a coluna
				if(mapa[linha][coluna_mapa] == PAREDE){ // se encontrar uma parede
					if(reposiciona_escrita == 1){ // aciona reposicionamento para escrita
						reposiciona_escrita = 0;
						gotoxy(coluna_tela, linha);
					}
					printf("\u2588");
				}else{
					reposiciona_escrita = 1;
				}
			}
			printf("\n");
		}
	}else{ // caso precise escrever o inicio da matriz do mapa junto do seu fim...
		for(linha=0; linha<LINHAS_MAPA; linha++){ // percorre as linhas
			reposiciona_escrita = 1; // variavel para auxiliar na reposição da escrita dos blocos
			coluna_tela = 0; // coluna da tela atual(37x105) eh zero
			for(coluna_mapa=posicao; coluna_mapa<COLUNAS_MAPA; coluna_mapa++, coluna_tela++){ // percorre a coluna
				if(mapa[linha][coluna_mapa] == PAREDE){ // se encontrar uma parede
					if(reposiciona_escrita == 1){ // aciona reposicionamento para escrita
						reposiciona_escrita = 0;
						gotoxy(coluna_tela, linha);
					}
					printf("\u2588");
				}else{
					reposiciona_escrita = 1;
				}
			}
			for(coluna_mapa=0; coluna_tela<COLUNAS_TELA; coluna_mapa++, coluna_tela++){ // percorre o resto da coluna
				if(mapa[linha][coluna_mapa] == PAREDE){ // se encontrar uma parede
					if(reposiciona_escrita == 1){ // aciona reposicionamento para escrita
						reposiciona_escrita = 0;
						gotoxy(coluna_tela, linha);
					}
					printf("\u2588");
				}else{
					reposiciona_escrita = 1;
				}
			}
			printf("\n");
		}
	}

}
