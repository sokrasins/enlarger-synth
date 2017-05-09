#include "synth/synth.h"

int main( int argc, char *argv[] )
{
  unsigned int freq = 20; 
  unsigned int wave = 0;
 

  Synth synth; 
  synth.start();

  while (true) {

    // Modify sample from Synth class object
    synth.setAudio( (double) freq, wave ); 
   
    // Play with random frequency variation
    freq = 200 + rand() % 20;

    // Play a chirp over all waveforms
    /* freq = (freq+1) % 1000; */
    /* if (freq < 20) { */
    /*   freq=20; */
    /*   wave = (wave+1) % 3; */ 
    /* } */
    // std::cout << "Playing freq: " << freq << " Hz\n";
    SLEEP( 10 );
  }

  return 0;
}
