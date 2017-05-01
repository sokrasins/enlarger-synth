CC=g++
CFLAGS=-c -Wall

all: synth 

synth: synth.o
	$(CC) synth.o -lrtaudio -o synth

synth.o: synth.cpp
	$(CC) $(CFLAGS) synth.cpp -lrtaudio

clean: 
	rm *o synth

