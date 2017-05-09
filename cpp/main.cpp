#include "synth.h"

int main( int argc, char *argv[] )
{
  unsigned int freq = 20; 
  unsigned int wave = 0;
 

  Synth synth; 
  synth.start();

  while (true) {
   
    // Play with random frequency variation
    /* data[2] = 200 + rand() % 20; */
    /* data[3] = data[2]; */

    // Play a chirp over all waveforms
    synth.setAudio( (double) freq, wave ); 
    freq = (freq+1) % 1000;
    if (freq < 20) {
      freq=20;
      wave = (wave+1) % 3; 
    }
    // std::cout << "Playing freq: " << freq << " Hz\n";
    SLEEP( 1 );
  }

  return 0;
}
