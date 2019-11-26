# joguinho

## Compilando
Para compilar o jogo você precisará ter em sua maquina linux as libs **libao** e **libmpg123**. Para instala-las, digite os comandos:

``sudo apt install libao-dev``

``sudo apt install libmpg123-dev``

para compilar o jogo, use o seguinte comando:

``gcc main.c -o main -lmpg123 -lao``

