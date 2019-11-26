#define BITS 8

som musica()
{
	som mp3;

    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, "./sound.mp3");
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_LITTLE;
    format.matrix = "M";
    dev = ao_open_live(driver, &format, NULL);

	/** Guarda os dados numa struct para ser usado fora desta função **/
	mp3.mh = mh;
	mp3.buffer = buffer;
	mp3.buffer_size = buffer_size;
	mp3.done = done;
	mp3.dev = dev;
	

    return mp3;
}

