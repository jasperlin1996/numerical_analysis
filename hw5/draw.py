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
# x_pos, y_pos = get_points(sys.argv[2] + "_" + sys.argv[1])

# plt.plot([-5, 500], [0, 0], c='#000000')
# plt.plot([0, 0], [-5, 500], c='#000000')
plt.xlim(0.9, 2.0)
# plt.ylim(-5, 5)

plt.plot(set_x, set_y, linewidth=3, marker = 'o')
plt.xticks([1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.85, 1.9], ["1.0", "1.1", "1.2", "1.3", "1.4", "1.5", "1.6", "1.7", "1.8", "1.85", "1.9"])
plt.show()