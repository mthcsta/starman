#ifndef CLASSIFICACAO_H
#define CLASSIFICACAO_H

typedef struct{
	char nome[NOME_TAMANHO_MAXIMO];
	int pontuacao;
} class_t;

void listaClassificados(int linhaInicio);

int classifica(int pontuacao);

void insereClassificado(class_t classificado[], int posicao, int pontuacao, char nome[]);

void buscaClassificados(class_t classificado[]);

void geraArquivoClassificados();

#endif
