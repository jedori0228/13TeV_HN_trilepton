import os

#filenames=['SR_rebins.txt', 'SR_xaxis.txt', 'SR_yaxis.txt'] 
filenames=['CR_rebins.txt', 'CR_xaxis.txt', 'CR_yaxis.txt'] 

for filename in filenames:

  tempfile = open('test_'+filename,'w')

  lines = open(filename).readlines()

  savethem=[]

  for line in lines:
    if "_ZZ_4mu0el" in line:
      savethem.append(line)
    tempfile.write(line)

  for line in savethem:
    line = line.replace('_ZZ_4mu0el','_ZZ_2mu2el')
    tempfile.write(line)

  tempfile.close()
  os.system('mv test_'+filename+' '+filename)
  
