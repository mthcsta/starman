/*****************************************/
/****** * * * MENU INICIAL * * * ********/
/***************************************/

int MENU_INICIAL(som mp3){
    int continua = 1, i;
    int selecionado_indice = 0;
    char *menu[]={"Novo Jogo", "Carregar Jogo", "Sair"};
    char opcao_menu[20];

    while(continua){ //enquanto a opção do índice nao foi selecionada
        if(kbhit()){//verifica qual tecla eh pressionada
            switch(tolower(getchar())){//o char eh convertido em minúsculo pra evitar erro de Capslock
                case 's': //se a entrada for um S (pra baixo)
                    if(selecionado_indice<2) //verifica se está no ultimo item
                        selecionado_indice++;//senão, leva o cursor pro prox item
                    else selecionado_indice=0;//se sim, leva o cursor pro primeiro item
                    printf("\a");//emite sinal sonoro
                    break;
                case 'w'://se a entrada for um W (pra cima)
                    if(selecionado_indice>0)//verifica se esta no primeiro item
                        selecionado_indice--;//senão, leva o cursor pro item de cima
                    else selecionado_indice=2;//se sim, leva o cursor para baixo
                    printf("\a");//emite sinal sonoro
                    break;
                case ' '://se a entrada for o espaço
                case 10://ou enter
                    printf("\a");
                    return selecionado_indice;//termina a função
                    break;
                default: break;
            }
        }
        limpaQuadro();//limpa a tela para escrever um novo quadro
	i=0;

        while(i<5){
            gotoxy(MinMax(1, 105), MinMax(1, 35));
            printf("\u2726");//gera estrelinhas na tela
            i++;
        }
        logo();//escreve o logo

        for(i=0; i<3; i++){
            if(selecionado_indice==i){//verifica onde esta a opção do cursor
                sprintf(opcao_menu, "\u00BB  %.13s", menu[i]);//pra printar a setinha ao lado da opção
            }else{//para as opcoes nao selecionadas pelo cursor
                sprintf(opcao_menu, "  %.13s", menu[i]);//escreve apenas o texto da opcao
             }
            printa(10+i, opcao_menu);//posiciona o menu
        }

	if(mpg123_read(mp3.mh, mp3.buffer, mp3.buffer_size/4, &mp3.done) == MPG123_OK)
	    ao_play(mp3.dev, mp3.buffer, mp3.done);

        gotoxy(0, 30);
        usleep(4000);
    }

	return 0;
}


/*****************************************/
/****** * * * * * PARTIDA * * * * *******/
/***************************************/
void partida(int nivel, char nome_mapa[], boneco_t * jogador, int * pontuacao, FILE * salve, som mp3){
    int continua=1;//inicia o laço da parte

    boneco_t inimigo[TOTAL_INIMIGO];
    tiro_t tiro[MAX_TIROS]={0};
    int mapa[LINHAS_MAPA][COLUNAS_MAPA];
    int inimigos_existentes;
    int posicao = 0;
    int animacao = 0;
    int intervalo = 0;
    int salvar_estado = 0;
    int salvar_estado_mensagem = 0;

    FILE *arquivo;

    //abre o arquivo do mapa
    arquivo = fopen(nome_mapa, "r");

    inimigos_existentes = geraMapa(arquivo, mapa, jogador, inimigo);

    fclose(arquivo);

    if(salve!=0){
        carregarEstado(salve, &posicao, jogador, pontuacao, tiro, &inimigos_existentes, inimigo, &animacao, &intervalo);
    }

    while(continua){//enquanto a partida nao eh encerrada

        limpaQuadro();//limpa a tela pra escrever novo quadro

        posicao += jogador->velocidade;//altera a pos do jogador de acordo com sua velocidade

        if(posicao<0){
            posicao = COLUNAS_MAPA;
        }
        else if(posicao>=COLUNAS_MAPA){
            posicao=0;
        }

        if(kbhit())
            controle(getchar(), jogador, tiro, mapa, posicao, &intervalo, &salvar_estado);

        if(salvar_estado==1){
            salvar_estado = 0;
            salvar_estado_mensagem = 50;
            salvarEstado(nivel, posicao, *jogador, *pontuacao, tiro, inimigos_existentes, inimigo, animacao, intervalo);
        }

        buscaTiro(jogador, inimigo, tiro, posicao, pontuacao, &inimigos_existentes, &animacao);
        atualizaQuadro(mapa, posicao, jogador, inimigo, tiro, &inimigos_existentes, &animacao, &intervalo, &salvar_estado_mensagem);

        // finaliza partida: quando o jogador perde todas as vidas ou quando nao ha mais inimigos
        if(jogador->nvidas<=0 || inimigos_existentes<=0){
            continua = 0;//encerra o laço da partida
        }

        geraQuadro(mapa, posicao, jogador, inimigo, tiro, pontuacao, &inimigos_existentes, &animacao, &salvar_estado_mensagem, nivel);

	if(mpg123_read(mp3.mh, mp3.buffer, mp3.buffer_size/6, &mp3.done) == MPG123_OK)
	    ao_play(mp3.dev, mp3.buffer, mp3.done);

        // 17000
        usleep(2000);
    }

    return;
}

/*****************************************/
/******* * * * FIM DE JOGO * * * ********/
/***************************************/

void FIM_DE_JOGO(int score){
    char str[40];

    limpaQuadro();
    sprintf(str, "\u225B Pontuacao: %d \u225B", score);//imprime a pontuação final
    printa(8, "FIM DE JOGO");

    // Condição para esconder a pontuação
    // quando FIM_DE_JOGO for chamado para fechar o jogo.
    if(score>0) printa(14, str);
    //imprime créditos e estrelinhas
    printa(20, "CREDITOS  \u26FE");
    printa(21, "/.\u2734.\u2735.\u2736.\u2734.\u2734.\u2735.\u2736.\u2734.\u2735.\u2736.\u2735.\u2736.\u2734.\u2735.\u2736.\u2734.\u2734.\u2735.\u2736.\u2734.\u2735.\u2736.\u2735.\u2736./"); //tem que arrumar
    printa(22, "Matheus Costa        Terumi Tamai");
    printa(29, "\n");
}
