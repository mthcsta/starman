
int MENU_INICIAL(void){
    int continua = 1, i;
    int selecionado_indice = 0;
    char *menu[]={"Novo Jogo", "Carregar Jogo", "Sair"};
    char selecionado_texto[20];

    while(continua){
        if(kbhit()){
            switch(tolower(getchar())){
                case 's':
                    if(selecionado_indice<2)
                        selecionado_indice++;
                    else selecionado_indice=0;
                    printf("\a");                    
                    break;
                case 'w':
                    if(selecionado_indice>0)
                        selecionado_indice--;
                    else selecionado_indice=2;
                    printf("\a");
                    break;
                case ' ':
                case 10:
                    printf("\a");
                    return selecionado_indice;
                    break;
                default: break; 
            }
        }
        limpaQuadro();
        logo();
        for(i=0; i<3; i++){

            if(selecionado_indice==i){
                sprintf(selecionado_texto, "\u00BB  %.13s", menu[i]);
            }else 
                sprintf(selecionado_texto, "  %.13s", menu[i]);
            printa(2, 10+i, selecionado_texto);
        }
    }
}



void partida(int nivel, char nome_mapa[], boneco_t * jogador, int * pontuacao, FILE * salve){

    int continua=1;

    boneco_t inimigo[TOTAL_INIMIGO];
    tiro_t tiro[MAX_TIROS]={0};
    int mapa[LINHAS][COLUNAS];
    int i, j;
    int inimigos_existentes;
    int posicao = 0;
    int animacao = 0;
    int intervalo = 0;
    int salvar_estado = 0;
    int salvar_estado_mensagem = 0;

    FILE *arquivo;


    arquivo = fopen(nome_mapa, "r");

    inimigos_existentes = geraMapa(arquivo, mapa, jogador, inimigo);

    fclose(arquivo);

    if(salve!=0){
        carregarEstado(salve, &posicao, jogador, pontuacao, tiro, &inimigos_existentes, inimigo, &animacao, &intervalo);
    }

    while(continua){

        limpaQuadro();

        posicao += jogador->velocidade;

        if(posicao<0) posicao = COLUNAS;
        else if(posicao>=COLUNAS) posicao=0;

        if(kbhit())
            controle(getchar(), jogador, tiro, mapa, posicao, &intervalo, &salvar_estado);

        if(salvar_estado==1){
            salvar_estado = 0;
            salvar_estado_mensagem = 50;
            salvarEstado(nivel, posicao, *jogador, *pontuacao, tiro, inimigos_existentes, inimigo, animacao, intervalo);
        }

        buscaTiro(jogador, inimigo, tiro, posicao, pontuacao, &inimigos_existentes, &animacao);
        atualizaQuadro(mapa, posicao, jogador, inimigo, tiro, &inimigos_existentes, &animacao, &intervalo, &salvar_estado_mensagem);

        // finaliza partida:
        if(jogador->nvidas==0 || inimigos_existentes<=0){
            continua = 0;
        }
        printf("%d|",nivel);

        geraQuadro(mapa, posicao, jogador, inimigo, tiro, pontuacao, &inimigos_existentes, &animacao, &salvar_estado_mensagem);

        // 17000
        usleep(40000);
    }

    return;
}


void FIM_DE_JOGO(int score){
    char str[11 + 6];

    limpaQuadro();
    sprintf(str, "Pontuacao: %d", score);
    printa(2, 8, "FIM DE JOGO");
    
    // Condição para esconder a pontuação 
    // quando FIM_DE_JOGO for chamado para fechar o jogo.
    if(score>0) printa(2, 14, str);

    printa(2, 20, "CREDITOS");
    printa(2, 21, "/**-----------------------------------**/");
    printa(2, 22, "Matheus Costa        Terumi Tamai");
    printa(1, 29, "\n");
}