/*****************************************/
/****** * * * MENU INICIAL * * * ********/
/***************************************/

int MENU_INICIAL(som mp3){
    int continua = 1;
	int i; // iterador
    int selecionado_indice = 0;
    char *menu[]={"Novo Jogo", "Carregar Jogo", "Sair"};
    char opcao_menu[24];

    while(continua){ //enquanto a opção do índice nao foi selecionada
        if(kbhit()){//verifica se um botão foi pressionado
			// se sim, passa pro controleMenu e espera uma resposta...
			if(controleMenu(getchar(), &selecionado_indice)!=-1){ // se o retorno de controleMenu() for diferente de -1, o botão press. foi ENTER/ESPAÇO
				return selecionado_indice; // retorna o indice do botão selecionado na hora que apertou ENTER/ESPAÇO
			}
        }
		

        limpaQuadro();//limpa a tela para escrever um novo quadro
		
		// codigo para as estrelinhas:
		for(i=0; i<5; i++){
            gotoxy(MinMax(1, 105), MinMax(1, 35));
            printf("\u2726");//gera estrelinhas na tela
		}

		// codigo para a logo do game:
        logo();//escreve o logo


		// codigo para a lista de opções do menu:
        for(i=0; i<3; i++){
            if(selecionado_indice==i){//verifica onde esta a opção do cursor
                sprintf(opcao_menu, "\u00BB %.14s \u00AB", menu[i]);//pra printar as setinhas ao lado da opção
            }else{//para as opcoes nao selecionadas pelo cursor
                sprintf(opcao_menu, " %.14s   ", menu[i]);//escreve apenas o texto da opcao
             }
            printa(10+i, opcao_menu);//posiciona o menu
        }


        gotoxy(0, 30); // posiciona o cursor para longe do menu

		
		// A versão com a música pode causar lentidão em alguns computadores na hora de imprimir os dados, 
		// causando um efeito chato para o jogo. Então foi feito de modo opcional na compilação
		// sendo possivel definir na main.h se o jogo terá música tema ou não. 1 é lido como SIM
		// versão com musica:
		if(MUSICA_TEMA == 1){	
			reproduzir(mp3);
        	usleep(10000);
		}
		// versão sem musica:		
		else{
			usleep(50000);
		}

    }

	return 0;
}


/*****************************************/
/****** * * * * * PARTIDA * * * * *******/
/***************************************/
void partida(int nivel, char nome_mapa[], boneco_t * jogador, int * pontuacao, FILE * salve, som mp3){
    int continua=1;//inicia o laço da parte

	// Inicializando as variaveis usadas nesta função:
    FILE *arquivo; // arquivo do mapa
    boneco_t inimigo[TOTAL_INIMIGO]; // vetor de inimigos
    tiro_t tiro[MAX_TIROS]={0}; // vetor de tiros
    int mapa[LINHAS_MAPA][COLUNAS_MAPA]; // matriz com as linhas e colunas do mapa que será carregado pelo arquivo *.txt
    int inimigos_existentes; // variavel auxiliar, usada para guardar o numero de inimigos(vivos) no mapa
    int posicao = 0; // posição inicial para exibição dos quadros.
    int animacao = 0; // variavel auxiliar para fazer a animação do jogador piscando
    int intervalo = 0; // variavel auxiliar para fazer o intervalo de tempo de espera do jogador para atirar
    int salvar_estado = 0; // variavel auxiliar para pegar checagem da controle() e salvar estado na partida()
    int salvar_estado_mensagem = 0; // variavel auxiliar para exibição da mensagem "Estado Salvo"
	// fim das variaveis


	// Inicio para a função:

    arquivo = fopen(nome_mapa, "r"); //abre o arquivo do mapa

	// carrega o arquivo do mapa na matriz mapa, ja definindo a posição do jogador e do vetor de inimigos
	// retorna um numero que corresponde ao numero de inimigos encontrados no arquivo
    inimigos_existentes = geraMapa(arquivo, mapa, jogador, inimigo);

	// fecha o arquivo do mapa
    fclose(arquivo);

	// verifica se a partida foi aberta no modo "Carregar Jogo"
    if(salve!=0){// se sim, pega os dados e define para a partida.
        carregarEstado(salve, &posicao, jogador, pontuacao, tiro, &inimigos_existentes, inimigo, &animacao, &intervalo);
    }

    while(continua){//enquanto a partida nao eh encerrada

        limpaQuadro();//limpa a tela pra escrever novo quadro

        posicao += jogador->velocidade;//altera a pos do jogador de acordo com sua velocidade

       	if(posicao>=COLUNAS_MAPA){ // caso o jogador tenha chegado na ultima coluna da matriz
            posicao=0;			  // volta para a primeira(de mesma linha).
        }

        if(kbhit()){ // captura qualquer botão digitado pelo usuario
			// chama o controle passando o botão pressionado
            controlePartida(getchar(), jogador, tiro, mapa, posicao, &intervalo, &salvar_estado);
		}

		// caso o botão pressionado seja 'G', salvar_estado é definida para 1(verdadeiro)
        if(salvar_estado==1){ // e é feito o salvamento dos dados de todo o jogo.
            salvar_estado = 0; // salvar_estado volta para seu valor de inicio 0(falso)
            salvar_estado_mensagem = 50; // uma mensagem indicando que o estado foi salvo é exibida com duração de 50 quadros.
			// função que salva o estado em um arquivo:
            salvarEstado(nivel, posicao, *jogador, *pontuacao, tiro, inimigos_existentes, inimigo, animacao, intervalo);
        }

		/*---------------------------------------------------------*/		
		/*---------------Altera todos os dados---------------------*/
		// atualiza todos os dados existentes no jogo: posições dos tiros, 
		// posições dos inimigos, posição do jogador, etc...
        atualizaQuadro(mapa, posicao, pontuacao, jogador, inimigo, tiro, &inimigos_existentes, &animacao, &intervalo, &salvar_estado_mensagem);


        // finaliza partida: quando o jogador perde todas as vidas ou quando nao ha mais inimigos
        if(jogador->nvidas<=0 || inimigos_existentes<=0){
            continua = 0;//encerra o laço da partida
        }
		
		/*----------------------------------------------------------*/
		/*---------------Exibe todos os dados-----------------------*/
		// após atualizar todos os dados do jogo, 
		// os exibe na tela com uma chamada de geraQuadro():
		// exibindo desde a pontuação, vidas, etc.
        geraQuadro(mapa, posicao, jogador, inimigo, tiro, pontuacao, &inimigos_existentes, &animacao, &salvar_estado_mensagem, nivel);

		
		// A versão com a música pode causar lentidão em alguns computadores na hora de imprimir os dados, 
		// causando um efeito chato para o jogo. Então foi feito de modo opcional na compilação
		// sendo possivel definir na main.h se o jogo terá música tema ou não. 1 é lido como SIM
		// versão com musica:
		if(MUSICA_TEMA == 1){
			reproduzir(mp3);
        	usleep(10000);
		}
		// versão sem musica:
		else{ 
			usleep(50000);
		}

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
	if(score==1){
		listaClassificados(12);
    }else if(score>0) printa(12, str);
    //imprime créditos e estrelinhas
    printa(16, "CREDITOS  \u26FE");
    gotoxy(27,17);
    printf("/.\u2734.\u2735.\u2736.\u2734.\u2734.\u2735.\u2736.\u2734.\u2735.\u2736.\u2735.\u2736.\u2734.\u2735.\u2736.\u2734.\u2734.\u2735.\u2736.\u2734.\u2735.\u2736.\u2735.\u2736./");
	printa(19, "Programação && Design");
    printa(20, "Matheus Costa        Terumi Tamai");
	
	if(MUSICA_TEMA == 1){ // se a musica tema estiver presente(ativa)
		// exibe mensagem de creditos ao autor
		printa(22, "Musica");
		printa(23, "David Bowie - Starman (8bits)");
	}

    printa(29, "\n");
}
