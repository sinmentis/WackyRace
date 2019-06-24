bits = (0, 7, 6, 4, 1, 4, 3)

for m in range(256):
    
    new = 0
    for bit, n in enumerate(bits):
        new = new + (m & (1 << bit)) << n

    map[new] = m
