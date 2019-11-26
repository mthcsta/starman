#include "main.h"

int main(){

    int nivel=0;

    char local_mapa[50];

    //inicializa a pontuação zerada
    int pontuacao=0;
    //inicializa o jogador no centro da tela com a velocidade mínima do jogo
    boneco_t jogador = {0, 0, 3, VEL_MIN};

    //estado
    int selecionado = NOVO_JOGO;
    FILE *salve = NULL;
	//fim das variáveis

    // chama o MENU e recebe uma opção selecionada pelo usuario
    selecionado=MENU_INICIAL();

    if(selecionado != SAIR){//se a op. do menu selecionada nao for a de sair
        if(selecionado==CONTINUAR_JOGO){//verifica se a op. selecionada eh de cont. o jogo
            salve = carregarNivelEstado(&nivel);//carrega o nivel salvo
        }

        //a partida perdura enquanto o jogador tiver vidas e ainda houver fases disponíveis
        while(jogador.nvidas>0 && existeMapa(nivel) != NIVEL_INEXISTENTE){ 
            
            sprintf(local_mapa, MAPA_CAMINHO, nivel);

            partida(nivel, local_mapa, &jogador, &pontuacao, salve);
            if(salve!=0){ // limpa a sujeira da variavel salve
                salve = 0;
            }
            nivel++;
        }

        FIM_DE_JOGO(pontuacao + (jogador.nvidas * 100));
    }else FIM_DE_JOGO(-5);

    return 0;
}

