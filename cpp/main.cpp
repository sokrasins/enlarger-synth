#include "synth.h"

int main( int argc, char *argv[] )
{
  Synth synth; 
  synth.start();

  while (true) {
    // Play with random frequency variation
    /* data[2] = 200 + rand() % 20; */
    /* data[3] = data[2]; */

    // Play a chirp over all waveforms
    /* data[2] = freq; */ 
    /* data[3] = freq*2; */
    /* freq = (freq+1) % 500; */
    /* if (freq < 20) { */
    /*   freq=20; */
    /*   if (++data[4] > 2) data[4] = 0; */
    /* } */
    /* // std::cout << "Playing freq: " << freq << " Hz\n"; */
    /* SLEEP( 1 ); */
  }

  return 0;
}
