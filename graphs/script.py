import numpy as np
import matplotlib.pyplot as pl

data = np.genfromtxt('../res/spikes.gdf')

select= np.array([d for d in data if d[1] < 50])
data1= select.transpose()

pl.Figure(figsize = (30,30))

pl.subplot(211)
pl.scatter(0.1*data1[0],data1[1],alpha=0.8, edgecolors='none');

pl.title("Neural network simulation")
pl.xlabel("Time [ms]")
pl.ylabel("Index of the neurons")

axes = pl.gca()
axes.set_xlim([800,1000])
axes.set_ylim([0,50])


pl.subplot(212)
data2 = data.transpose()
n, bins, patches = pl.hist(0.1*data2[0], 12000, normed=0, alpha=0.75)

pl.xlabel("Time [ms]")
pl.ylabel("Number of spikes")

axes = pl.gca()
axes.set_xlim([800,1000])


pl.show();
