import sounddevice as sd
import numpy as np
import math
import time
import random

duration = 10
centerF = 500

sd.default.samplerate = 44100 

def getSound(freq, dur):
    sampRate = 44100
    samps = int(round(dur*sampRate))
    inputArray = np.empty(samps)
    for i in range(0, samps):
        inputArray[i] = 2*(math.floor(2*i*freq/sampRate) % 2) - 1
        #inputArray[i] = math.sin(2*3.14*2*i*freq/sampRate)
    return inputArray

#sd.play(getSound(1000, .1), loop=True)

while True:
    print "start" 
    sd.stop()
    print "stopped dev"
    delta = random.randint(20,100)
    sd.play(getSound(centerF+delta, .05), loop=True)
    sd.sleep(50)
    print sd.get_status()
