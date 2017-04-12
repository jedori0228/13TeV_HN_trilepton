import os

filenames=['SR_rebins.txt', 'SR_xaxis.txt', 'SR_yaxis.txt'] 
masses = [5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000]

for filename in filenames:

  ftest = open('test_'+filename,'aw')

  lines = open(filename).readlines()

  savethem_low=[]
  savethem_high=[]

  for line in lines:
    if "cutWlow" in line:
      savethem_low.append(line)
    if "cutWhigh" in line:
      savethem_high.append(line)

  for mass in masses:
    if mass < 80:
      for line in savethem_low:
        line = line.replace('cutWlow','cutHN'+str(mass))
        ftest.write(line)

  ftest.close()
  
