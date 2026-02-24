import pathlib
from typing import List

import pandas as pd
import matplotlib.pyplot as plt

class BenchmarkData:
  def __init__(self, filename: str):
    self.filename = filename
    self.data = pd.read_table(self.filename)
    self.headers = self.data.columns.tolist()


def get_benchmark_files(path: str, format: str = "dat"):
	"""
	This Python function retrieves benchmark files with a specified format from a given path.
	
	:param path: the absolute path to the folder containing the benchmark files
	:param format: format of benchmark data files. default value: "dat"

	:return: A list of files
	"""
	parent_folder = pathlib.Path(path).absolute()
	print("the parent folder is: ", parent_folder)

	benchmark_files = []
	for file in parent_folder.glob("*"+format):
		benchmark_files.append(file) 
	return benchmark_files

def update_plot_setup(num_plots:int = 1,title = "plot", x_axis="X", y_axis="Y",
                      xscale:"ScaleBase | Literal['linear', 'log', 'symlog', 'logit']"='linear',xbase:int=10,
                      yscale:"ScaleBase | Literal['linear', 'log', 'symlog', 'logit']"='linear',ybase:int=10):
  
	plt.rcParams.update(plt.rcParamsDefault)
	plt.figure("Figure: "+title,figsize=[5.4,4.8]) # figure window title, figure size (inches)

	# define base-plot style
	plt.style.use('bmh')
	# plt.style.use('fivethirtyeight')  
	# plt.style.use('seaborn-v0_8-darkgrid')

	plt.rcParams.update({
			"text.usetex": True,
			"font.family": "Computer Modern Serif",
			'font.weight': 'bold',
			'font.size': 13,
	})

	plt.title(title)
	plt.xlabel(x_axis)
	plt.ylabel(y_axis)
 
	if (xscale != 'linear'): 
		plt.xscale(xscale, base=xbase) 

	if (yscale !=  'linear'): 
		plt.yscale(yscale, base=ybase)
  
  # plt.grid(True,'both')
	# plt.axis((0, 6, 0, 20))
 

def show_plot():
	plt.legend()
	plt.show()
 
 
## individual line plot settings  
def plot_line(dataFile, x_axis=0, y_axis=1, lwidth=1.5, co_sy_lt = 'o-' , alpha=1.0, overlap:bool = True):	
  data = dataFile.data
  label = dataFile.filename.stem
  line =  plt.plot(data.iloc[:, x_axis],data.iloc[:,y_axis],co_sy_lt,alpha=alpha,linewidth=lwidth)
  plt.setp(line,label=label)
  return line



if __name__ == "__main__":

	path = str("/home/vassal/codebase/LitmusMath/data/RelDebInfo_Testing/bench5_SP_v_DP")
	format = "DP.dat"
	
	benchFiles = get_benchmark_files(path,format)
	print("Following benchmark data files found:")
	for file in benchFiles:
		print(file.name)
	
  
	mm_strassen = BenchmarkData(benchFiles[0])
	mat_txp = BenchmarkData(benchFiles[1])
	mm_naive = BenchmarkData(benchFiles[2])
	txp_naive = BenchmarkData(benchFiles[3])
 
	# update_plot_setup(title="Matrix Multiplication", x_axis="Matrix Size (elements)", y_axis= "Mean Time(s)",
  #                  xscale='log',xbase=16)

	# log2-log10 plot
	update_plot_setup(title="Matrix Multiplication [FP64] ", x_axis="Matrix Size (elements)", y_axis= "Mean Time(s)",
                   xscale='log',xbase=4,yscale='log',ybase=10)

	plot_line(mm_naive)
	plot_line(mat_txp)
	plot_line(mm_strassen)
	plot_line(txp_naive,alpha=0.35)
 
	# other custom plot options	
	plt.xticks([16, 64, 256, 1024, 4096, 16384,  65536, 262144, 1048576 ])  

	show_plot()
 