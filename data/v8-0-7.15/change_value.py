import os

#filenames=['CR_yaxis.txt']
#cut = '_ZGamma'

filenames=['SR_rebins.txt', 'SR_xaxis.txt', 'SR_yaxis.txt']
FromCut = ["_cut0", "_cutWlow", "_cutWhigh", "_MuMuE", "_cutHN"]
ToCut = ["_cut_MuMuMu_Presel", "_cut_MuMuMu_low", "_cut_MuMuMu_high", "_cut_SSSF_MuMuE_Presel", "_cut_MuMuMu_HN"]

for filename in filenames:

  tempfile = open('test_'+filename,'w')

  lines = open(filename).readlines()

  for line in lines:

    words = line.split()

    ## change value
    #if words[0]==cut:
    #  value = int(words[2])
    #  if not abs(value) == 999:
    #    value = 2*value
    #  print words[0]+'\t'+words[1]+'\t'+str(value)
    #  tempfile.write( words[0]+'\t'+words[1]+'\t'+str(value)+'\n' )

    ## change cut
    counter = 0
    for cut in FromCut:
      if cut in words[0]:
        line = line.replace(cut, ToCut[counter])
      counter = counter+1

    tempfile.write(line)

  tempfile.close()
  os.system('mv test_'+filename+' '+filename)
  
