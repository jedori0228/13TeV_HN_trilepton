import os

filenames=['SR_rebins.txt', 'SR_xaxis.txt', 'SR_yaxis.txt'] 
masses = [5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000]

hists = ['h_mT']
lastvar = 'h_mlll'

for filename in filenames:

  tempfile = open('test_'+filename,'w')

  lines = open(filename).readlines()

  for line in lines:

    tempfile.write(line)

    if lastvar in line:
      

      for hist in hists:
        newline = line.replace(lastvar,hist)

        tempfile.write(newline)

  tempfile.close()
  os.system('mv test_'+filename+' '+filename)
  
