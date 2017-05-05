import os

filenames=['SR_rebins.txt', 'SR_xaxis.txt', 'SR_yaxis.txt'] 
masses = [5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000]

hists = ['n_events', 'HN_mass_class', 'W_pri_lowmass_mass', 'W_pri_highmass_mass', 'W_sec_highmass_mass', 'deltaR_OS_min', 'gamma_star_mass', 'z_candidate_mass']

for filename in filenames:

  tempfile = open('test_'+filename,'w')

  lines = open(filename).readlines()

  for line in lines:

    for hist in hists:
      if hist in line:
        newhist = "h_"+hist
        if hist == 'n_events':
          newhist = "h_Nevents"
        line = line.replace(hist,newhist)

    tempfile.write(line)

  tempfile.close()
  os.system('mv test_'+filename+' '+filename)
  
