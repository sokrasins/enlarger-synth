#include <iostream>                   // std::cout
#include <cstdlib>
#include <cmath>                      // sine
#include <unistd.h>                   // usleep
#include "inc/RtAudio.h"              // All audio stuff
#include "synth.h"

// TODO: What do I do with this error callback? Figure out how its used.
void Synth::errorCallback( RtAudioError::Type type, const std::string &errorText ) {
 
  // This example error handling function does exactly the same thing
  // as the embedded RtAudio::error() function.
  std::cout << "in errorCallback" << std::endl;
  if ( type == RtAudioError::WARNING )
    std::cerr << '\n' << errorText << "\n\n";
  else if ( type != RtAudioError::WARNING )
    throw( RtAudioError( errorText, type ) );
}


// Returns the scaled amplitude of a sine wave at a specified point in the wave's period 
sample Synth::sineWave( double loc, unsigned int period ) {
  return (sample) ( sin( loc / (double) period * 2 * 3.141 ) * SCALE );
}

// Returns the scaled amplitude of a square wave at a specified point in the wave's period
sample Synth::squareWave( double loc, unsigned int period ) {
  if ( loc >= period/2.0 ) return (sample) (1.0 * SCALE);
  else return (sample) (-1.0 * SCALE);
}

// Returns the scaled amplitude of a sawtooth wave at a specified point in the wave's period
sample Synth::sawWave( double loc, unsigned int period ) {
  return (sample) ( ( ( 2.0 * loc / (double) period ) - 1.0 ) * SCALE );
}

// Callback, generates a sine wave on each channel
int Synth::callback( void *outputBuffer, void * /*inputBuffer*/, unsigned int nBufferFrames, 
          double /*streamTime*/, RtAudioStreamStatus status, void *data ) {

  unsigned int i, j, period;
  sample *buffer = (sample *) outputBuffer;
  double *waveParams = (double *) data;

  if ( status ) {
    std::cout << "Stream underflow detected!" << std::endl;
  }

  std::cout << "channels: " << this->channels << "\n";

  // For all audio channels
  for ( j=0; j<this->channels; j++ ) {
    period = 220; //(unsigned int) ( 1.0/waveParams[this->channels+j] * this->fs );
    std::cout << "period: " << period << std::endl;
    // For every expected frame
    for ( i=0; i<nBufferFrames; i++ ) {
      if (waveParams[4] == 0) {
        *buffer++ = Synth::sineWave(waveParams[j], period);
      } else if (waveParams[4] == 1) {
        *buffer++ = Synth::sawWave(waveParams[j], period);
      } else {
        *buffer++ = Synth::squareWave(waveParams[j], period);
      }

      waveParams[j] = (double) ( ( (unsigned int) waveParams[j] + 1) % period );
    }
  } 

  return 0;

}

Synth::Synth() {

  // Set all my private params to things that make sense
  data = (double *) calloc( this->channels*2+1, sizeof(double) ); 

  // Set a starting frequency for fun
  this->data[2] = 20;
  this->data[3] = 40;
  
  // Make sure I can cram samples down something's throat, if not, then just quit
  if ( this->dac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 1 );
  }

  // For debug
  this->dac.showWarnings( true );
  
  std::cout << "channels: " << channels;
}

// What should happen whern I die?
Synth::~Synth() {
  free(this->data);
}

// Start this train
void Synth::start() {

  // Shoutout to all my local homies
  unsigned int bufferFrames = 512, device = 0, offset = 0;

  RtAudio::StreamParameters oParams;
  oParams.deviceId = device;
  oParams.nChannels = this->channels;
  oParams.firstChannel = offset;

  // Always just get the default device
  oParams.deviceId = dac.getDefaultOutputDevice();

  // Set performance flags
  RtAudio::StreamOptions options;   // audio stream parameters
  options.flags = RTAUDIO_HOG_DEVICE;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;
  options.flags |= RTAUDIO_NONINTERLEAVED;


  try {
    dac.openStream( &oParams, NULL, FORMAT, this->fs, &bufferFrames, (RtAudioCallback) &Synth::callback, 
                    (void *)this->data, &options, (RtAudioErrorCallback) &Synth::errorCallback );
    dac.startStream();
  } catch ( RtAudioError& e ) {
    e.printMessage();
    cleanup();
  }

  this->data[4] = 2;
}

void Synth::setAudio( double freq, unsigned int waveForm ) {
  this->data[0] = freq;
  this->data[1] = freq;
  this->data[4] = waveForm;
}

int Synth::cleanup() {
  if ( dac.isStreamOpen() ) dac.closeStream();
  free( data );

  return 0;
}
