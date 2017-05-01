/******************************************/
/*
  playsaw.cpp
  by Gary P. Scavone, 2006

  This program will output sawtooth waveforms
  of different frequencies on each channel.
*/
/******************************************/

#include "inc/RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16
#define SCALE  32767.0

// Define unix-specific sleep routine
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )

#define BASE_RATE 0.005
#define TIME      1.0
#define FS        44100

void usage( void ) {
  // Error function in case of incorrect command-line
  // argument specifications
  std::cout << "\nuseage: playsaw N fs <device> <channelOffset> <time>\n";
  std::cout << "    where N = number of channels,\n";
  std::cout << "    fs = the sample rate,\n";
  std::cout << "    device = optional device to use (default = 0),\n";
  std::cout << "    channelOffset = an optional channel offset on the device (default = 0),\n";
  std::cout << "    and time = an optional time duration in seconds (default = no limit).\n\n";
  exit( 0 );
}

void errorCallback( RtAudioError::Type type, const std::string &errorText )
{
  // This example error handling function does exactly the same thing
  // as the embedded RtAudio::error() function.
  std::cout << "in errorCallback" << std::endl;
  if ( type == RtAudioError::WARNING )
    std::cerr << '\n' << errorText << "\n\n";
  else if ( type != RtAudioError::WARNING )
    throw( RtAudioError( errorText, type ) );
}

unsigned int channels;
RtAudio::StreamOptions options;
unsigned int frameCounter = 0;
bool checkCount = false;
unsigned int nFrames = 0;
const unsigned int callbackReturnValue = 1;

// Returns the scaled amplitude of a sine wave at a specified point in the wave's period 
MY_TYPE sineWave( double loc, unsigned int period ) {
  return (MY_TYPE) ( sin( loc / (double) period * 2 * 3.141 ) * SCALE );
}

// Returns the scaled amplitude of a square wave at a specified point in the wave's period
MY_TYPE squareWave( double loc, unsigned int period ) {
  if ( loc >= period/2.0 ) return (MY_TYPE) (1.0 * SCALE);
  else return (MY_TYPE) (-1.0 * SCALE);
}

// Returns the scaled amplitude of a sawtooth wave at a specified point in the wave's period
MY_TYPE sawWave( double loc, unsigned int period ) {
  return (MY_TYPE) ( ( ( 2.0 * loc / (double) period ) - 1.0 ) * SCALE );
}

// Callback, generates a sine wave on each channel
int callback( void *outputBuffer, void * /*inputBuffer*/, unsigned int nBufferFrames, 
          double /*streamTime*/, RtAudioStreamStatus status, void *data ) {

  unsigned int i, j, period;
  extern unsigned int channels;
  MY_TYPE *buffer = (MY_TYPE *) outputBuffer;
  double *waveParams = (double *) data;

  if ( status ) {
    std::cout << "Stream underflow detected!" << std::endl;
  }

  // For all audio channels
  for ( j=0; j<channels; j++ ) {
    period = (unsigned int) ( 1.0/waveParams[channels+j] * FS );
    // For every expected frame
    for ( i=0; i<nBufferFrames; i++ ) {
      if (waveParams[4] == 0) {
        *buffer++ = sineWave(waveParams[j], period);
      } else if (waveParams[4] == 1) {
        *buffer++ = sawWave(waveParams[j], period);
      } else {
        *buffer++ = squareWave(waveParams[j], period);
      }

      waveParams[j] = (double) ( ( (unsigned int) waveParams[j] + 1) % period );
    }
  } 

  frameCounter += nBufferFrames;
  if ( checkCount && ( frameCounter >= nFrames ) ) return callbackReturnValue;
  return 0;

}

int main( int argc, char *argv[] )
{
  unsigned int bufferFrames, fs, device = 0, offset = 0, freq = 20; 

  RtAudio dac;
  if ( dac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 1 );
  }

  // Hard-code app-specific params
  channels = 2;
  fs =  FS;

  // Make array containing the parametric synthesis parameters. Set intial freqs to 20 and 40 Hz
  double *data = (double *) calloc( channels*2+1, sizeof( double ) );
  data[2] = 20; 
  data[3] = 40; 

  // Let RtAudio print messages to stderr.
  dac.showWarnings( true );

  // Set our stream parameters for output only.
  bufferFrames = 512;
  RtAudio::StreamParameters oParams;
  oParams.deviceId = device;
  oParams.nChannels = channels;
  oParams.firstChannel = offset;

  // Always just get the default device
  oParams.deviceId = dac.getDefaultOutputDevice();

  // Set performance flags
  options.flags = RTAUDIO_HOG_DEVICE;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;
  options.flags |= RTAUDIO_NONINTERLEAVED;

  try {
    dac.openStream( &oParams, NULL, FORMAT, fs, &bufferFrames, &callback, (void *)data, &options, &errorCallback );
    dac.startStream();
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
    goto cleanup;
  }

  if ( checkCount ) {
    while ( dac.isStreamRunning() == true ) {
      SLEEP( 100 );
    }
  } else {
    data[4] = 2;
    // Main loop: do forever
    while (true) {
      // Play with random frequency variation
      /* data[2] = 200 + rand() % 20; */
      /* data[3] = data[2]; */

      // Play a chirp over all waveforms
      data[2] = freq; 
      data[3] = freq*2;
      freq = (freq+1) % 500;
      if (freq < 20) {
        freq=20;
        if (++data[4] > 2) data[4] = 0;
      }
      // std::cout << "Playing freq: " << freq << " Hz\n";
      SLEEP( 1 );
    }
  }

 cleanup:
  if ( dac.isStreamOpen() ) dac.closeStream();
  free( data );

  return 0;
}
