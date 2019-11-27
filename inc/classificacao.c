/**************************void listaClassificados**************************/
/* lista os classificados encontrados no classificados.bin                */
/*                                                                       */
/************************************************************************/

void listaClassificados(int linhaInicio){
	int linha = linhaInicio;
	int i;
	FILE *arq;
	arq = fopen(CLASSIFICADOS_ARQUIVO, "r");

	char classificado_texto[50];
	class_t classificado;

	printa(linha, "#  | Jogador       | Pontuacao");
	linha++;
	for(i=0; i<LIMITE_CLASSIFICADOS; i++){
		fread(&classificado, sizeof(class_t), 1, arq);
		sprintf(classificado_texto, "%.2i | %.12s | %d", i+1, classificado.nome, classificado.pontuacao);
		printa(linha, classificado_texto);
		linha++;
	}
	fclose(arq);
	return;
}

/**************************void classifica**************************/
/*  Confere a pontuação do jogador com os ja classificados         */
/*  Classifica o jogador de acordo com sua pontuação               */
/********************************************************************/
int classifica(int pontuacao){
	int buscando = 1;
	int posicao=0;
	char nome[NOME_TAMANHO_MAXIMO];

	class_t classificados[LIMITE_CLASSIFICADOS];

	// faz uma busca dos jogadores ja classificados e insere num vetor
	buscaClassificados(classificados);

	// verifica se o jogador se encaixa em alguma das classificacoes de acordo com sua pontuacao
	do{
		if(classificados[posicao].pontuacao < pontuacao){
			buscando = 0; // caso o usuario se encaixe, a é interrompida
		}else
			posicao++; // caso não se encaixe, pula pra proxima posição
	} while(posicao<LIMITE_CLASSIFICADOS && buscando==1);

	if(buscando==0){ // se a busca foi interrompida, então ele foi classificado naquela posição;
		limpaQuadro();
		gotoxy(0,0);
		if(posicao==0){
			printa(5, "Voce  obteve  a maior pontuação! Parabens\n");
		}else{
			printa(5, "Voce alcancou uma pontuacao consideravel para entrar nas 3 classificacoes, Parabens!\n");
		}
		printa(6, "Digite seu nome para ser inserido na lista: \n");
		printa(7, "");
		fgets(nome, NOME_TAMANHO_MAXIMO, stdin);
		if(nome[strlen(nome)-1] == '\n'){
			nome[strlen(nome)-1]='\0';
		}
		insereClassificado(classificados, posicao, pontuacao, nome);
		return 1;
	}
	return 0;
}
/***********************void insereClassificado***********************/
/*  Insere o novo classificado na sua posição e                      */
/*   reposiciona os classificados abaixo dele(se houver)             */
/********************************************************************/
void insereClassificado(class_t classificado[], int posicao, int pontuacao, char nome[]){
	int i;
	int j;
	FILE *arq;
	arq = fopen(CLASSIFICADOS_ARQUIVO, "w");

	class_t novo_classificado;
	strcpy(novo_classificado.nome, nome);
	novo_classificado.pontuacao = pontuacao;

	for(i=0, j=0; i<LIMITE_CLASSIFICADOS; i++){
		if(posicao==i){
			fwrite(&novo_classificado, sizeof(class_t), 1, arq);
		}else{
			fwrite(&classificado[j], sizeof(class_t), 1, arq);
			j++;
		}
	}
	fclose(arq);
}
/*************************void buscaClassificados***********************/
/*      busca pelos classificados antigos, se não houver, cria         */
/*   um arquivo com classificados ja definidos, e os guarda num vetor */
/********************************************************************/
void buscaClassificados(class_t classificado[]){
	FILE *arq;
	arq = fopen(CLASSIFICADOS_ARQUIVO, "r");
	if(arq == 0){
		geraArquivoClassificados();
		return buscaClassificados(classificado);
	}

	fread(classificado, sizeof(class_t), LIMITE_CLASSIFICADOS, arq);

	return;
}
/**************************void geraArquivoClassificaos**************************/
/*  Função que cria o arquivo de classificados com classificados ja definidos */
/******************************************************************************/
void geraArquivoClassificados(){
	FILE *arq;
	arq = fopen(CLASSIFICADOS_ARQUIVO, "w");
	class_t classificados[LIMITE_CLASSIFICADOS] = {{"DavidBowie", 500}, {"MajorTom", 200}, {"DenRitchie", 30}};
	fwrite(classificados, sizeof(class_t), LIMITE_CLASSIFICADOS, arq);
	fclose(arq);
	return;
}
