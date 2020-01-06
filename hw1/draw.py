import numpy as np
import matplotlib.pyplot as plt

def f(x, direction):
    return float(np.sqrt(-16*(((x - 3)**2)/25 - 1))* direction + 4)

def g(x, direction):
    return float(x**2 + x*2 + 1) * direction

def get_points(filename):
    x_pos = []
    y_pos = []
    with open(filename, 'r') as f_:
        for line in f_:
            x, y = line.split()
            x_pos.append(float(x))
            y_pos.append(float(y))
    return np.array(x_pos), np.array(y_pos)

x_pos_red, y_pos_red = get_points('converge_red.txt')
x_pos_green, y_pos_green = get_points('converge_green.txt')
x_pos_nope, y_pos_nope = get_points('converge_nope.txt')

fx_points     = np.arange(-15.0, 15.2, 0.0001, dtype=float)
fy_points_top = np.array(list(map(f, fx_points, np.repeat(1, len(fx_points)))))
fy_points_bot = np.array(list(map(f, fx_points, np.repeat(-1, len(fx_points)))))
gx_points     = np.arange(-15.0, 15.2, 0.0001, dtype=float)
gy_points_top = np.array(list(map(g, gx_points, np.repeat(1, len(gx_points)))))


plt.xlim(-150, 150)
plt.ylim(-150, 150)

plt.scatter(x='x', y='y', data={'x': x_pos_green, 'y': y_pos_green}, s = 0.001, c = '#3f78ef')
plt.scatter(x='x', y='y', data={'x': x_pos_red, 'y': y_pos_red}, s = 0.001, c = 'red')
plt.scatter(x='x', y='y', data={'x': x_pos_nope, 'y': y_pos_nope}, s = 0.001, c = 'yellow')

plt.scatter(x='x', y='y', data={'x': fx_points, 'y': fy_points_top}, s = 1, c = "black")
plt.scatter(x='x', y='y', data={'x': fx_points, 'y': fy_points_bot}, s = 1, c = "black")
plt.scatter(x='x', y='y', data={'x': gx_points, 'y': gy_points_top}, s = 1, c = "black")
plt.plot([-150, 150], [0, 0], c = '#000000')
plt.plot([0, 0], [-150, 150], c = '#000000')
# plt.annotate(s="Result Point", xy=[
#             x_pos[-1], y_pos[-1]], xytext=[x_pos[-1], y_pos[-1]+5], ha = 'center', arrowprops={'shrink': 0.05})

plt.show()
