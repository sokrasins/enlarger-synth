#include "../inc/RtAudio.h"              // All audio stuff
#include <cstdlib>
#include <unistd.h>                   // usleep

// Change these if the sample datatype needs to be different
#define FORMAT      RTAUDIO_SINT16    // Data format for the audio buffer
#define SAMP_TYPE   signed short      // stdlib sample datatype
#define SCALE       32767.0           // Scaling factor for above format

#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )

// Define the datatype used for the buffers. Makes it easier to change.
typedef SAMP_TYPE sample;

class Synth {

  public:

    // Class methods
    Synth();
    ~Synth();
    void start();
    void setAudio( double freq, unsigned int waveForm );
    static sample sineWave( double loc, unsigned int period );
    static sample sawWave( double loc, unsigned int period );
    static sample squareWave( double loc, unsigned int period );
    static int sampFreq();
    // int callback( void *outputBuffer, void * /*inputBuffer*/, unsigned int nBufferFrames,
    //               double /*streamTime*/, RtAudioStreamStatus status, void *data );

  private:

    // Class fields
    // static const unsigned int channels = 2;            // Number of audio channels
    unsigned int channels;
    static const int fs = 44100;                           // Audio buffer samples per sec 
    RtAudio dac;                      // Audio interface
    double *data;                     // Audio parameters go here

    // Class methods
    // void errorCallback( RtAudioError::Type type, const std::string & errorText );
    int cleanup();
};

