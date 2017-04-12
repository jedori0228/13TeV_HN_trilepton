import os

filenames=['SR_rebins.txt', 'SR_xaxis.txt', 'SR_yaxis.txt'] 
masses = [5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000]

for filename in filenames:

  tempfile = open('test_'+filename,'w')

  lines = open(filename).readlines()

  savethem_low=[]
  savethem_high=[]

  for line in lines:
    if "cutWlow" in line:
      savethem_low.append(line)
    if "cutWhigh" in line:
      savethem_high.append(line)
    tempfile.write(line)

  for mass in masses:
    if mass < 80:
      for line in savethem_low:
        line = line.replace('cutWlow','cutHN'+str(mass))
        tempfile.write(line)
    else:
      for line in savethem_high:
        line = line.replace('cutWhigh','cutHN'+str(mass))
        tempfile.write(line)

  tempfile.close()
  os.system('mv test_'+filename+' '+filename)
  
