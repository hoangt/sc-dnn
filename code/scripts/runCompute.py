#! c:\python27\python.exe

import sys
import os
import subprocess


def ComputeModel(sparsityFile, modelName):
	
	flog = open(sparsityFile, "r")
	for row in flog:
		w = row.split()
		if len(w) > 4 and not w[0].startswith(";") and ((w[0] == modelName) or (modelName == "ALL")):
			model = w[0]
			fpSparsity = int(w[1])
			bpSparsity = int(w[2])
			dwSparsity = int(w[3])
			wuSparsity = int(w[4])			
			print "Model:" + model + " FP:" + str(fpSparsity) + " BP:" + str(bpSparsity) + " DW:" + str(dwSparsity) + " WU:" + str(wuSparsity)
			
			
			
def main():
	
	argLen = len(sys.argv)
	if	(argLen < 2):
		print "Usage: sys.argv[0] <SparsityFile> [Model]"
		quit()
	
	sparsityFile = sys.argv[1]
	if (argLen > 2):
		modelName = sys.argv[2]
	else:
		modelName = "ALL"

	print "Running canonical compute SparsityFile=" + sparsityFile + " Model="+modelName
	ComputeModel(sparsityFile, modelName)
	
main()