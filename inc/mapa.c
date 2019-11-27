/********************int existeMapa********************/
/*   verifica se existe um mapa para ser aberto      */
/****************************************************/
int existeMapa(int nivel, char local_mapa[]){
	FILE * arq;

	sprintf(local_mapa, MAPA_CAMINHO, nivel); // monta uma string do caminho do arquivo mapa + o numero do nivel
	arq = fopen(local_mapa, "r"); // tenta abrir o arquivo de mapa com o nivel passado como parametro.
	if(arq==0){ // caso o mapa não exista, retorna 0(falso)
        return 0;
    }else{ // caso exista, retorna 1(verdadeiro)
		return 1;
	}
}
/********************int geraMapa********************/
/* gera uma matriz com os dados do arquivo de texto */
/* do mapa                                          */
/****************************************************/
int geraMapa(FILE *arq, int mapa[][COLUNAS_MAPA], boneco_t *jogador, boneco_t inimigo[]){
    int y=0, x=0; // iteradores
    int id=0; // id do inimigo que sera inserido
    char c;

    do{
        c = fgetc(arq);//pega cada char
        if(c != EOF){//enquanto n chegar no fim do arquivo
            if(c!='\n'){//e enquanto n houver quebra de linha
                if(c=='C'){//verifica se o char corresponde ao C
                    mapa[y][x] = PAREDE;//se sim, aloca ele numa matriz de parede na pos encontrada
                }else if(c==' '){//se o char eh espaço (espaço vazio)
                    mapa[y][x] = 0;//atribui 0 àquela pos da matriz
                }else if(c=='X'){//se o char é X (inimigo)
                    mapa[y][x] = 0;
                    inimigo[id] = carregaInimigo(x, y);
                    id++;
                }else if(c=='@'){//se o char eh @ (a nave)
                    mapa[y][x] = 0;
                    jogador->y = y;
                    jogador->x = x;
                }
                x++;
            }else{
                y++;
                x=0;
            }
        }
    }while(!feof(arq));

    return id;
}
