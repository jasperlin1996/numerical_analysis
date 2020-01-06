import matplotlib.pyplot as plt
import numpy as np

def get_points(filename):
    x_pos = []
    y_pos = []
    with open(filename, 'r') as f_:
        for line in f_:
            line_ = line.split()
            x, y = line_[-2], line_[-1]
            x_pos.append(float(x))
            y_pos.append(float(y))
    return np.array(x_pos), np.array(y_pos)

filenames = ['ng.txt', 'nqr.txt', 'oqr.txt']

norm_2 = []
norm_inf = []
x = [4,5,6,7,8,9,10,11,12,13,14,15]
for filename in filenames:
    two, inf = get_points(filename)
    norm_2.append(two)
    norm_inf.append(inf)

for name, two, inf in zip(filenames, norm_2, norm_inf):
    plt.plot(x, two, linewidth=2, label = name[:-4] + ' 2 norm')
    plt.plot(x, inf, linewidth=2, label = name[:-4] + ' Inf norm', linestyle = "--")

plt.legend(loc='upper left')

plt.yscale('log')
plt.show()


