#ifndef MUSICA_H
#define MUSICA_H

/*Includes para libao e libmpg123*/
#include <ao/ao.h>
#include <mpg123.h>

typedef struct {
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
	ao_device *dev;
} som;

som musica();

void reproduzir(som mp3);

#endif
