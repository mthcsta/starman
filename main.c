#include "main.h"

int main(){

    int nivel=0;

    char local_mapa[NOME_MAPA_TAMANHO];

    //inicializa a pontuação zerada
    int pontuacao=0;
    //inicializa o jogador no centro da tela com a velocidade mínima do jogo
    boneco_t jogador = {0, 0, JOGADOR_NVIDAS, VEL_MIN};

    //estado
    int selecionado = NOVO_JOGO;
    FILE *salve = NULL;
    som mp3;
	//fim das variáveis
	
	if(MUSICA_TEMA == 1){
		mp3 = musica();
	}
    // chama o MENU e recebe uma opção selecionada pelo usuario
    selecionado=MENU_INICIAL(mp3);


    if(selecionado != SAIR){//se a op. do menu selecionada nao for a de sair
        if(selecionado==CONTINUAR_JOGO){//verifica se a op. selecionada eh de cont. o jogo
            salve = carregarNivelEstado(&nivel);//carrega o nivel salvo
        }

        //a partida perdura enquanto o jogador tiver vidas e ainda houver fases disponíveis
        while(jogador.nvidas>0 && existeMapa(nivel, local_mapa) == 1){
            partida(nivel, local_mapa, &jogador, &pontuacao, salve, mp3); // abre a partida com o mapa encontrado;
            if(salve!=0){ // limpa a sujeira da variavel salve
                salve = 0;
            }
            nivel++;
        }

		pontuacao += (jogador.nvidas * 100);
		
		if(classifica(pontuacao) == 0){
	        FIM_DE_JOGO(pontuacao);		
		}else{
	        FIM_DE_JOGO(1);
		}
    }else FIM_DE_JOGO(-5);

    return 0;
}

