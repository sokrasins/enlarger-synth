CC=g++
CFLAGS=-c -Wall

all: main 

main: synth.o main.o
	$(CC) synth.o main.o -lrtaudio -o synth 

main.o: main.cpp 
	$(CC) $(CFLAGS) main.cpp

synth.o: synth.cpp
	$(CC) $(CFLAGS) synth.cpp -lrtaudio

clean: 
	rm -f *o synth

