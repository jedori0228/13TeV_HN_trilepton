import os,sys

inputfile = sys.argv[1]

lines = open(inputfile).readlines()

for line in lines:
  if ("(NLimit)" not in line) and ("root [0]" not in line) and ("Processing src/syst_UpDowns" not in line) and (line != "\n"):
    print line.strip('\n')
os.system('rm '+inputfile)

