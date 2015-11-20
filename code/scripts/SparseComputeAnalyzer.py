#! c:\python27\python.exe

import sys
import os
import subprocess
import collections
from array import array

g_ModelNames = ["MNIST", "CIFAR", "IMG1K", "IMG22K"]

g_PassNames = ["ForwardProp ", "BackwardProp ", "WeightUpdate "]
ForwardProp, BackwardProp, WeightUpdate = range(3)

class ModelPerf:
    def __init__(self):
		self.name = ""
		self.kernels = []
		self.forwardProp = []
		self.backwardProp = []
		self.weightUpdate = []
		
    def Print(self):
		print self.name
		for i in range(0, len(self.kernels)):
			resultString = "\t" + "{0:<10s}".format(self.kernels[i])
			totalTime = self.forwardProp[i] + self.backwardProp[i] + self.weightUpdate[i]
			resultString += "\t" + "{0:0.2f}".format(self.forwardProp[i])
			resultString += "\t" + "{0:0.2f}".format(self.backwardProp[i]) 
			resultString += "\t" + "{0:0.2f}".format(self.weightUpdate[i])
			resultString += "\t" + "{0:0.2f}".format(totalTime)
			print resultString

					
def UpdateModelPerf(modelPerf, row):

	w = row.split()
	if len(w) < 2:
		return
	if w[0] == "KernelVersion:":
		modelPerf.kernels.append(w[1])
		return
	if w[0] == "ForwardProp:":
		modelPerf.forwardProp.append(float(w[1]))
		return
	if w[0] == "BackwardProp:":
		modelPerf.backwardProp.append(float(w[1]))
		return
	if w[0] == "WeightUpdate:":
		modelPerf.weightUpdate.append(float(w[1]))
		return
	
 		
def GetPerfResults(inputFile):

	flog = open(inputFile, "r")
	modelPerf = ModelPerf()
	for row in flog:
		w = row.split()
		if len(w) < 2:
			continue
		if w[0] == "Model:" and not w[1] == modelPerf.name:
			modelPerf.name = w[1]
		elif not modelPerf.name == "":
			UpdateModelPerf(modelPerf, row)
			if (len(modelPerf.weightUpdate) == 4):
				modelPerf.Print()
				modelPerf = ModelPerf()
		continue
	
def main():
	if not (len(sys.argv) == 2):
		print "Usage: " + sys.argv[0] + "<InputFile.txt>"
		quit()
		
	inputFile = sys.argv[1]
	
	if not os.path.exists(inputFile):
		print "Error: " + inputFile + " not found"
		quit()
	
	GetPerfResults(inputFile)


main()
