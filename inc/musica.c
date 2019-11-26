/*
	Parte do codigo abaixo foi copiado do site: https://hzqtc.github.io/2012/05/play-mp3-with-libmpg123-and-libao.html
	contendo implementação das libs ao e mpg123, 
	mpg123 para fazer a leitura do arquivo de musica 
	e a "ao" para fazer a execução do buffer capturado pela leitura de mpg123. 

	Nós editamos para ir para uma struct os dados, 
	para serem usados dentro do nosso programa de jogo,
	nas funções de tela inicial e de partida.
*/

#define BITS 8

som musica()
{
	som mp3;

    int err;
    int driver;
    ao_sample_format format;
    int channels, encoding;
    long rate;



    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mp3.mh = mpg123_new(NULL, &err);
    mp3.buffer_size = mpg123_outblock(mp3.mh);
    mp3.buffer = (unsigned char*) malloc(mp3.buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mp3.mh, "./musica/tema.mp3");
    mpg123_getformat(mp3.mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_LITTLE;
    format.matrix = "M";
    mp3.dev = ao_open_live(driver, &format, NULL);
	
    return mp3;
}

// função que reproduz a musica no joguinho:
void reproduzir(som mp3){
	if(mpg123_read(mp3.mh, mp3.buffer, mp3.buffer_size/BUFFER_TAMANHO, &mp3.done) == MPG123_OK){
	    ao_play(mp3.dev, (char*)mp3.buffer, mp3.done); // foi feito casting para char* pois dava Warning aqui.
	}else{
		mpg123_seek(mp3.mh, 0, SEEK_SET);
		return reproduzir(mp3);
	}
	return;
}

