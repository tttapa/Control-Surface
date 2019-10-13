from scipy.signal import lfilter
import numpy as np

alpha = 0.25
b = np.array((alpha,), dtype=np.float32)
a = np.array((1, alpha-1), dtype=np.float32)

signal = np.array((100, 100, 25, 25, 50, 123, 465, 75, 56, 50, 23, 41), 
                  dtype=np.float32)
output = lfilter(b, a, signal)
print('array<uint16_t, {}> signal = {{'.format(len(signal)))
for i in signal:
    print(int(i), end=', ')
print('};')
print('array<uint16_t, {}> expected = {{'.format(len(signal)))
for o in output:
    print(int(round(o)), end=', ')
print('};')