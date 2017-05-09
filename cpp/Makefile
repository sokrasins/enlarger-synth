CC=g++
CFLAGS=-c -Wall

all: main 

main: ./synth/synth.o main.o 
	$(CC) synth/synth.o main.o -lrtaudio -o photo 

main.o: main.cpp 
	$(CC) $(CFLAGS) main.cpp

synth.o: ./synth/synth.cpp
	$(CC) $(CFLAGS) ./synth/synth.cpp -lrtaudio 

# cam.o: ./cam/cam.cpp
# 	$(CC) $(CFLAGS) ./cam/cam.cpp

clean: 
	rm -f *o synth/*o cam/*o photo 

