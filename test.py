# imports
import numpy as np
import matplotlib.pyplot as plt
import scipy.io.wavfile as wav
import random
import cv2
import sounddevice as sd

# Start capturing video from webcam
cap = cv2.VideoCapture(1)

# WHat size is the image?
width = int(cap.get(3))
height = int(cap.get(4))

# Impose our own brightness and contrast, and check them
cap.set(10, .1)
cap.set(11, .8)

brightness = cap.get(10)
contrast = cap.get(11)

# Debugging: see cap values
print "width: " + str(width)
print "height: " + str(height)
print "brightness: " + str(brightness)
print "contrast: " + str(contrast)

# Until q is pressed, show webcam image
while True:
    ret, frame = cap.read()

    b, g, r = cv2.split(frame)
    # bigImg = cv2.resize(r, (20000, int(20000/(width*height))))

    freqs = [None]*width 
    for i in range(0, width):
        sum = 0
        for j in range(0, height):
            sum = sum + r[j, i]
        freqs[i] = sum/height

    cv2.imshow('frame', r)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    xOld = np.linspace(0, 20000, len(freqs))
    xNew = np.linspace(0, 20000, 20000)

    longFreqs = np.interp(xNew, xOld, freqs)
    longFreqs *= 1000
    longFreqs -= np.amin(longFreqs)

    # Generate time-domain signal
    audio = np.fft.irfft(longFreqs)

    # Create a wav file
    # wav.write('test.wav', 40000, audio)

    sd.play(audio[0:10000], 20000)

# Close video stream
cap.release()
cv2.destroyAllWindows()

# Show frequency spectrum
# plt.plot(longFreqs)
# plt.show()

# Now, interpolate last image to frequency range

