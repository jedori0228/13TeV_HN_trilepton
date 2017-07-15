import os

#filenames=['SR_rebins.txt', 'SR_xaxis.txt', 'SR_yaxis.txt'] 
filenames=['CR_rebins.txt', 'CR_xaxis.txt', 'CR_yaxis.txt'] 

From = "_ZGamma_3mu0el"
To = "_LowMassResonance_3mu0el"

for filename in filenames:

  tempfile = open('test_'+filename,'w')

  lines = open(filename).readlines()

  savethem=[]

  for line in lines:
    if From in line:
      savethem.append(line)
    tempfile.write(line)

  for line in savethem:
    line = line.replace(From,To)
    tempfile.write(line)

  tempfile.close()
  os.system('mv test_'+filename+' '+filename)
  
