#https://github.com/libharu/libharu/wiki/Examples
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ganesh/Programs/PDFc/libharu/src/.libs

CC = gcc

CFLAGS  = -g -Wall
INCLUDES = -I/home/ganesh/Programs/PDFc/libharu/include  -I.
LFLAGS = -L/home/ganesh/Programs/PDFc/libharu/src/.libs
LIBS = -lhpdf

default: demo

demo:  demo.o
	$(CC) $(CFLAGS) -o demo demo.o $(LFLAGS) $(LIBS)

demo.o:  demo.c
	$(CC) $(CFLAGS) $(INCLUDES) -c demo.c

clean: 
	$(RM) demo *.o *~
