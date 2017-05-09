CC=g++
CFLAGS=-c -Wall

all: main 

main: ./synth/synth.o main.o
	$(CC) synth/synth.o main.o -lrtaudio -o photo 

main.o: main.cpp 
	$(CC) $(CFLAGS) main.cpp

synth.o: ./synth/synth.cpp
	$(CC) $(CFLAGS) ./synth/synth.cpp -lrtaudio

clean: 
	rm -f *o synth/*o photo 

