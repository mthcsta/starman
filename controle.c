void controle(int c, boneco_t * jogador, tiro_t tiro[], int mapa[][COLUNAS], int posicao, int *intervalo, int *salvar_estado){

    int reposiciona;

    c = tolower(c);

    switch(c){
        case 's':
            if(!ehParede(mapa, jogador->x+posicao, jogador->y+1)){
                jogador->y+=1;
            }
            break;
        case 'w':
            if(!ehParede(mapa, jogador->x+posicao, jogador->y-2)){
                jogador->y-=1;
            }
            break;
        case 'd':
            if(jogador->velocidade < VEL_MAX){ //confere se a velocidade eh menor que a max
                jogador->velocidade++;         //antes de acelerar mais
            }
            break;
        case 'a':
            if(jogador->velocidade > VEL_MIN){ //confere se a velocidade ainda eh maior que a min
                jogador->velocidade--;         //antes de desacelerar
            }
            break;
        case ' ':
            if(*intervalo==0){
                geraTiro(tiro, 1, (jogador->x + posicao) % COLUNAS, jogador->y);
                *intervalo = INTERVALO_TIRO;
            }
            break;

        case 'g':
            *salvar_estado = 1;
            break;

        default:
         break;
    }
    return;
}