import pyaudio
import math
import numpy as np
import matplotlib.pyplot as plt
import time

# Global audio parameters
freq = 1000
sampRate = 44100

inputArray = np.empty(sampRate)
for i in range(0, sampRate):
    inputArray[i] = 2*(math.floor(2*i*freq/sampRate) % 2) - 1

data = inputArray.astype(np.float)

# Audio callback, called everytime the audio player runs out of sammples
def callback(in_data, frame_count, time_info, status):
    global data 
    senddata = data[0:frame_count]
    # data = data.astype(np.float32).tostring()
    # print len(data)
    data = np.roll(inputArray, frame_count)
    return (senddata, pyaudio.paContinue)

### MAIN
p = pyaudio.PyAudio()

stream = p.open(format=pyaudio.paFloat32,
                channels=1,
                rate=sampRate,
                output=True,
                stream_callback=callback)

stream.start_stream()

while stream.is_active():
    time.sleep(1)

stream.stop_stream()
stream.close()

p.terminate()
