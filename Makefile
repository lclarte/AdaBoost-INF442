# --- macros
CC=/usr/local/openmpi-3.0.1/bin/mpic++
CFLAGS= -O3 -I /usr/local/opencv-3.4.1/include
LIBS = -L /usr/local/opencv-3.4.1/lib64 -lopencv_core -lopencv_imgcodecs -lopencv_highgui

# --- targets
all: main

main: utilitaires.o classifieur.o main.o
	$(CC) -o main utilitaires.o classifieur.o main.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<
        
# --- remove binary and executable files
clean:
	rm -f main *.o
