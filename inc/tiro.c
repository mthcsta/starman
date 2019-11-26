/**
 * Verifica as posicoes relativas entre o jogador e o tiro do inimigo, e entre o inimigo e o tiro do jogador.
 * Computa os pontos ganhos a cada inimigo morto e as vidas perdidas por tiros do inimigo.
**/
void buscaTiro(boneco_t *jogador, boneco_t inimigo[], tiro_t tiro[],int posicao, int * pontuacao, int *inimigos_existentes, int *animacao){
    int i, j, // iteradores
        id; //variável auxiliar para guardar o id do inimigo

    for(i=0; i<MAX_TIROS; i++){
        if(tiro[i].prop>0){ //se o tiro existe, verifica sua posição em relação ao boneco
             /*****************************/
            /**** Matando os Inimigos ****/
           /*****************************/
            if(tiro[i].prop==1){    //se o tiro é do jogador...
                j = 0;//usamos o indice do inimigo
                while(j<*inimigos_existentes){ //enquanto o indice do inimigo for menor q o n de inimigos existentes
                        if(tiro[i].x<=inimigo[j].x && tiro[i].x+VEL_BALA>=inimigo[j].x &&//confere as coordenandas do inimigo com as do tiro
                            (tiro[i].y==inimigo[j].y || tiro[i].y==inimigo[j].y-1) ){   //se as coordenadas x e y do tiro == as do inimigo
                            inimigo[j].nvidas--;            //o inimigo perde a vida
                            if (inimigo[j].nvidas == 0){    //se o inimigo estiver morto
                                deletaInimigo(inimigo, j, inimigos_existentes);  //chamamos a funcao pra deleta-lo do vetor
                            }
                            tiro[i].prop=0;  //e o tiro deixa de existir
                            *pontuacao+=10; //o jogador ganha 10 pontos
                            j = TOTAL_INIMIGO; //finaliza o laco desse tiro pois n precisa percorrer todos os outros se ja achou o inimigo
                            printf("\a"); //emite sinal sonoro
                        }
                        j++;
                }
            }



	         /***********************************/
            /*** Matando o Jogador por tiros ***/
	       /***********************************/

            //Se o prop do tiro eh o inimigo e as coordenadas x e y do tiro coincidem com as do jogador, o jogador perde uma vida
            else if(*animacao==0 && tiro[i].prop==2 && tiro[i].x>=jogador->x+posicao && tiro[i].x-VEL_BALA<=jogador->x+posicao &&
                    (tiro[i].y==jogador->y || tiro[i].y==jogador->y-1)){
                *animacao = DURACAO_ANIMACAO;//o jogador pisca na tela
                jogador->nvidas--;//o jogador perde uma vida
                tiro[i].prop=0; //o tiro deixa de existir
                printf("\a"); //emite sinal sonoro
            }

        }
    }
    return;
}
/**************************************************************/
/**função padrao que gera tiros para jogador e inimigo.     **/
/**Recebe uma lista de tiros, o proprietário e as pos x e y**/
/**Jogador: chamada na controle.c com entrada do teclado  **/
/**Inimigo: chamada na quadro.c com uma função de sorteio**/
/*********************************************************/
void geraTiro(tiro_t tiro[], int prop, int x, int y){
    int i=0; //iterador
    int continua=1;//marca se o tiro sem prop ainda nao foi achado


    do{
        if(tiro[i].prop == 0){ //se o tiro da lista nao tem proprietatio
            tiro[i].prop = prop; //atribui sua propriedade pra chamada em questao
            tiro[i].duracao = DURACAO_TIRO;//atribui a duracao definida para o proprietario
            tiro[i].x = x;//atribui sua coordenada x
            tiro[i].y = y;//atribui sua coordenada y
            continua=0; //tiro sem prop foi achado
        }
        i++;
    } while(continua);//se o tiro sem prop foi achado e foi possivel atribuir sua prop, encerra laço
    return;
}
