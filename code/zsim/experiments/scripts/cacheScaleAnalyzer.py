#! c:\python27\python.exe

import sys
import os
import subprocess
import collections



def PrintResults(cacheResults):
	resultLines = dict()
	for scale, result in sorted(cacheResults.iteritems()):
		for thread, latency in sorted(result.iteritems()):
			if resultLines == None:
				resultLines[thread] = "{0:0.2f}".format(latency)
			elif not resultLines.has_key(thread):
				resultLines[thread] = "{0:0.2f}".format(latency)
			else:
				resultLines[thread] += "\t" + "{0:0.2f}".format(latency)
	
	for thread in sorted(resultLines):
		print str(thread) + " " + resultLines[thread]
		
def UpdateResults(cacheResults, inputFile):
	flog = open(inputFile, "r")
	currentPass = None
	thread = None
	scale = None
	latency = 0
	for row in flog:
		w = row.split()
		length = len(w)
		if length < 2:
			continue
		if w[0] == "Pass:":
			currentPass = w[1] + ":"
		elif w[0] == currentPass:
			latency = float(w[1])
		elif w[0] == "ThreadCount:":
			thread = int(w[1])
		elif w[length - 2] == "sim.l3CacheScale":
			scale = int(w[length-1])
	
	if not cacheResults.has_key(scale):
		cacheResults[scale] = dict()
		
	scaleResults = cacheResults[scale]	
	scaleResults[thread] = latency
		
		
def GetResults(inputFolder):
	cacheResults = dict()
	
	for cacheDir in os.listdir(inputFolder):
		cacheDirPath = inputFolder + os.sep + cacheDir
		for threadDir in os.listdir(cacheDirPath):
			threadDirPath = cacheDirPath + os.sep + threadDir
			if os.path.isdir(threadDirPath):
				for file in os.listdir(threadDirPath):
					if file == "run.txt":
						filePath = threadDirPath + os.sep + file
						UpdateResults(cacheResults, filePath)
						
	return cacheResults
	
def main():

	if not (len(sys.argv) == 2):
		print "Usage: " + sys.argv[0] + "<Results Folder>"
		quit()
	
	inputFolder = sys.argv[1]
	if not os.path.isdir(inputFolder):
		print "Error: Folder " + inputFolder + " not found ... exiting"
		quit()
		
	print "# Analyze Cache & Thread Scalability"
		
	cacheResults = GetResults(inputFolder)
	
	if not cacheResults == None:
		PrintResults(cacheResults)
	
main()
