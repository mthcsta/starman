#ifndef MUSICA_H
#define MUSICA_H

typedef struct {
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
	ao_device *dev;
} som;

som musica();

#endif
