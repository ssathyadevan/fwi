import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('Agg')
from scipy.ndimage.measurements import label
#import seaborn as sn

results_csv = './all_results.csv'

outputPath="./performance_out/"

results_df = pd.read_csv(results_csv)
print(results_df)


cpu_plot = plt.scatter(results_df.CASE, results_df.CPU)
plt.xlabel("case")
plt.ylabel("cpu_time")
plt.savefig((outputPath + "CPUImage.png"), dpi=400, bbox_inches='tight')
#plt.show()
plt.clf()

wall_plot = plt.scatter(results_df.CASE, results_df.WALL)
plt.xlabel("case")
plt.ylabel("wall_time")
plt.savefig((outputPath + "WALLImage.png"), dpi=400, bbox_inches='tight')
#plt.show()
plt.clf()


time_plot = plt.scatter(results_df.CASE, results_df.TIME)
plt.xlabel("case")
plt.ylabel("time")
plt.savefig((outputPath + "TIMEImage.png"), dpi=400, bbox_inches='tight')
#plt.show()
plt.clf()


e_avg_plot = plt.scatter(results_df.CASE, results_df.E_AVG)
plt.xlabel("case")
plt.ylabel("e_avg")
plt.savefig((outputPath + "AVGImage.png"), dpi=400, bbox_inches='tight')
#plt.show()
plt.clf()


e_mean_plot = plt.scatter(results_df.CASE, results_df.E_MEAN)
plt.xlabel("case")
plt.ylabel("e_mean")
plt.savefig((outputPath + "MEANImage.png"), dpi=400, bbox_inches='tight')
#plt.show()
plt.clf()

core_plot = plt.scatter(results_df.CASE, results_df.CORES)
plt.xlabel("case")
plt.ylabel("cores")
plt.savefig((outputPath + "COREImage.png"), dpi=400, bbox_inches='tight')
#plt.show()
plt.clf()


