import sys
import numpy as np
import matplotlib.pyplot as plt


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

set_x, set_y = get_points("offDiagVar.txt")

plt.title("Off Diagnial Variation(log)")
plt.plot(set_x, set_y, linewidth=3)
plt.yscale('log')
plt.show()

set_x, set_y = get_points("result.txt")

plt.title("N M relation")
plt.plot(set_x, set_y, linewidth=3)
# plt.yscale('log')
plt.show()
