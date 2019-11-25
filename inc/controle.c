/***Movimenta a nave e atira de acordo com a entrada do teclado***/
void controle(int c, boneco_t * jogador, tiro_t tiro[], int mapa[][COLUNAS], int posicao, int *intervalo, int *salvar_estado){

    int reposiciona;

    c = tolower(c); //evita que o controle nao funcione caso o capslock esteja ativado
	
  
    switch(c){
        case 's': //se a entrada eh a tecla s
            if(!ehParede(mapa, jogador->x+posicao, jogador->y+1)){//caso a prox pos pra baixo do jogador nao seja PAREDE
                jogador->y+=1;                                   //desloca a nave para baixo
            }
            break;
        case 'w': //se a entrada eh a tecla w
            if(!ehParede(mapa, jogador->x+posicao, jogador->y-2)){//caso a prox pos pra cima do jogador nao seja PAREDE
                jogador->y-=1;                                   //desloca a nave para cima
            }
            break;
        case 'd'://se a entrada eh a tecla s
            if(jogador->velocidade < VEL_MAX){ //confere se a velocidade eh menor que a max
                jogador->velocidade++;         //antes de acelerar mais
            }
            break;
        case 'a'://se a entrada eh a tecla a
            if(jogador->velocidade > VEL_MIN){ //confere se a velocidade ainda eh maior que a min
                jogador->velocidade--;         //antes de desacelerar
            }
            break;
        case ' '://se a entrada eh um espaço
            if(*intervalo==0){ //(cooldown) se o tiro nao foi dado recentemente
		//gera o tiro a partir da posição do jogador, dentro dos limites da tela
		//ex.:tela na pos 400, jogador na pos 19: pos do jogador=419
		//sendo o limite da tela 414, %COLUNAS garante que o tiro iniciará na pos verdadeira
		//do jogador(x=4)
                geraTiro(tiro, 1, (jogador->x + posicao) % COLUNAS, jogador->y);
                *intervalo = INTERVALO_TIRO;//e atribui um valor de espera para novo tiro
            }
            break;

        case 'g'://se a entrada eh um g
            *salvar_estado = 1; //o estado do jogo é salvo(como?)
            break;

        default:
         break;
    }
    return;
}
