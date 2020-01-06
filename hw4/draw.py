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

set_x, set_y = get_points(sys.argv[1])
x_pos, y_pos = get_points(sys.argv[2] + "_" + sys.argv[1])

plt.plot([-15, 15], [0, 0], c='#000000')
plt.plot([0, 0], [-15, 15], c='#000000')
# plt.xlim(-5, 5)
# plt.ylim(-5, 5)

plt.plot(set_x, set_y, linewidth=3)
plt.plot(x_pos, y_pos, linewidth=3)
plt.show()
