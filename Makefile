#$(pkg-config allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
CFLAGS = -Wall -std=c99
EXECUTAVEL = ballz
ALLEGRO_FLAGS = -lallegro -lallegro_font -lallegro_primitives -lallegro_image -lallegro_acodec -lallegro_audio

all: $(EXECUTAVEL)

#regras de ligação
ballz: ballz.o lista.o desenho.o dinamica.o menu.o
	gcc ballz.o lista.o desenho.o dinamica.o menu.o -o ballz $(ALLEGRO_FLAGS) -lm

#regras de compilação
ballz.o: ballz.c estruturas.h lista.h desenho.h dinamica.h menu.h
	gcc -c ballz.c $(CFLAGS)

lista.o: lista.c lista.h
	gcc -c lista.c $(CFLAGS)

desenho.o: desenho.c desenho.h estruturas.h lista.h
	gcc -c desenho.c $(CFLAGS)

dinamica.o: dinamica.c dinamica.h estruturas.h lista.h
	gcc -c dinamica.c $(CFLAGS)

menu.o: menu.c menu.h estruturas.h lista.h
	gcc -c menu.c $(CFLAGS)

#remoção de arquivos temporários
backup:
	tar -cvf backup.tar .
clean:
	rm -f *~ *.o
purge:
	rm -f ballz backup.tar
