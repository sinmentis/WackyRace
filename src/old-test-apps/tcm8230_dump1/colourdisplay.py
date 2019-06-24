from optparse import OptionParser
import sys
import os
import numpy as np
from matplotlib.pyplot import figure, show, savefig

# Camera data bus mapping, lsb first
bits = (0, 1, 2, 3, 4, 5, 6, 7)
#bits = (7, 6, 5, 1, 4, 3, 0, 2)

# Create lookup table
pixmap = np.zeros(256, dtype=int)

for m in range(256):
    
    new = 0
    for bit, n in enumerate(bits):
        new = new + (((m & (1 << bit)) != 0) << n)

    pixmap[m] = new


def image_display(filename):

    imfile = open(filename)
    lines = imfile.readlines()

    height = len(lines)
    cimage = None

    for r, line in enumerate(lines):
        (row, rowdata) = line.strip().split(':')

        # Remap data if bits are scrambled.
        raw = [pixmap[int(pix)] for pix in rowdata.split(',')[0: -1]]

        width = len(raw) / 2

        if cimage is None:
            cimage = np.zeros((width, height, 3), dtype=int)
            
        for p in range(width):
            x, y = raw[p * 2], raw[p * 2 + 1]
            red = (y >> 3) * 255 / 31
            green = (((y & 7) << 3) | (x >> 5)) * 255 / 63
            blue = (x & 31) * 255 / 31

            cimage[p, r, :] = (255 - red, 255 - green, 255 - blue)

    fig = figure()
    ax = fig.add_subplot(111)
    ax.imshow(cimage)

    show()


def main (argv=None):

    if argv is None:
        argv = sys.argv
        
    if len(argv) < 2:
        raise ValueError('Missing filename')

    image_display(argv[1])
    return 0


if __name__ == '__main__':
    sys.exit(main())
