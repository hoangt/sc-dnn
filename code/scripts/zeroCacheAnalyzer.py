#! c:\python27\python.exe

import sys
import os
import subprocess
import collections
from array import array

g_CacheNames = ["l0_cntrl1.Dcache", "l0_cntrl1.Zcache", 
				"l1_cntrl1.cache", "l1_cntrl1.zcache", 
				"l2_cntrl0.L2cache", "l2_cntrl0.ZL2cache"]

class CachePerf:
	def __init__(self, name):
		self.name = name
		self.hits = 0
		self.misses = 0
		self.accesses = 0
		
	def Update(self, w):
		if self.name in w[0]:
			stats = int(w[1])
			if "hits" in w[0]:
				self.hits = stats
			elif "misses" in w[0]:
				self.misses = stats
			elif "accesses" in w[0]:
				self.accesses = stats

	def IntStat(self, stats):
		return "{:>10}".format(stats)
	
	
	def Print(self):
		hitRate = self.hits*100.0/max(1, self.accesses)
		missRate = self.misses*100.0/max(1, self.accesses)
		
		resultString = "{:<20}".format(self.name) + "\t" + self.IntStat(self.hits) + "\t" + self.IntStat(self.misses) 
		resultString += "\t" + self.IntStat(self.accesses) 
		resultString += "\t" + "{0:5.2f}".format(hitRate)
		resultString += "\t" + "{0:5.2f}".format(missRate)
		print resultString

		
def GetCachePerf(inputFile):
	global g_CacheNames
	
	caches = []
	for i in range(0, len(g_CacheNames)):
		caches.append(CachePerf(g_CacheNames[i]))

	flog = open(inputFile, "r")
	for row in flog:
		w = row.split()
		if len(w) > 3:
			for i in range(0, len(caches)):
				caches[i].Update(w)
	
	for i in range(0, len(caches)):
		caches[i].Print()
				
def main():
	if not (len(sys.argv) == 2):
		print "Usage: " + sys.argv[0] + "<InputFile.txt>"
		quit()
		
	inputFile = sys.argv[1]
	
	if not os.path.exists(inputFile):
		print "Error: " + inputFile + " not found"
		quit()
	
	GetCachePerf(inputFile)


main()