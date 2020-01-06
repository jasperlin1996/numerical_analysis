import numpy as np
import seaborn as sns; sns.set()
import matplotlib.pyplot as plt

data = np.genfromtxt('determine.csv', delimiter=',', dtype=np.float)
data = data[1250:1750, 1250:1750]
print(data[1250:1750, 1250:1750].shape)
ax = sns.heatmap(data=data, cmap="RdYlGn")
plt.show()
