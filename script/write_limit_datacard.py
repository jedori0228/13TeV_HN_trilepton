import os,math
import ROOT

Lumi = 35863.308

WORKING_DIR = os.environ["PLOTTER_WORKING_DIR"]
dataset = os.environ["CATANVERSION"]
LIMIT_PATH = WORKING_DIR+"/plots/"+dataset+"/Limit/"

WHICHCUTOPDIR = "170524_mumue_previous"
LIMIT_PATH = LIMIT_PATH+WHICHCUTOPDIR+"/"

DATACARDS_PATH = LIMIT_PATH+"/datacards/"

os.system("mkdir -p "+DATACARDS_PATH)

f = ROOT.TFile(LIMIT_PATH+"/hist.root")

hist_mass = f.Get("hist_mass")
hist_xsec = f.Get("hist_xsec")
hist_fake = f.Get("hist_fake")
hist_prompt = f.Get("hist_prompt")
hist_signal = f.Get("hist_signal")
hist_signal_weighted = f.Get("hist_signal_weighted")

kfactor_haneol = [
    1.08, 1.08, 1.08, 1.09,
    1.09, 1.09, 1.09, 1.09,
    1.11, 1.12, 1.14, 1.15,
    1.17, 1.18, 1.18, 1.19,
    1.19]


for i in range(0,17):

  mass = str(int(hist_mass.GetBinContent(i+1)))

  print ""
  print "#### HN"+mass+" ####"
  print ""

  xsec = hist_xsec.GetBinContent(i+1)

  fake = hist_fake.GetBinContent(i+1)
  fake_err = hist_fake.GetBinError(i+1)

  prompt = hist_prompt.GetBinContent(i+1)
  prompt_err = hist_prompt.GetBinError(i+1)

  signal = hist_signal.GetBinContent(i+1)
  signal_err = hist_signal.GetBinError(i+1)

  signal_weighted = hist_signal_weighted.GetBinContent(i+1)*200.
  signal_weighted_err = hist_signal_weighted.GetBinError(i+1)*200.

  if hist_mass.GetBinContent(i+1) <= 80:
    signal_weighted *= 0.00001
    signal_weighted_err *= 0.00001
  elif hist_mass.GetBinContent(i+1) <= 100:
    signal_weighted *= 0.0001
    signal_weighted_err *= 0.0001
  elif hist_mass.GetBinContent(i+1) <= 700:
    signal_weighted *= 0.001
    signal_weighted_err *= 0.001
  else:
    signal_weighted *= 0.001
    signal_weighted_err *= 0.001

  signal_weighted *= kfactor_haneol[i]
  signal_weighted_err *= kfactor_haneol[i]

  N_MC = 100000.;
  if mass=="200":
    N_MC = 96193.;
  if mass=="400":
    N_MC = 99070.;

  signal_yield = 0.01*signal*N_MC*200.
  signal_yield_err = 1.+(0.01*signal_err*N_MC*200.)/signal_yield

  bkg_yield = fake+prompt
  bkg_yield_err = 1.+math.sqrt(fake_err*fake_err+prompt_err*prompt_err)/bkg_yield

  out = open(DATACARDS_PATH+"/HN"+mass+".txt","w")

  #print "bin\t1\t1"
  #print "process\tHN"+mass+"\tbkgd"
  #print "process\t0\t1"
  #print "rate\t"+str(signal_weighted)+"\t"+str(bkg_yield)
  #print "------------"
  #print "lumi\tlnN\t1.026\t1.026"
  #print "sigerr\tlnN\t"+str(signal_yield_err)+"\t-"
  #print "bkgerr\tlnN\t-\t"+str(bkg_yield_err)

  out.write("imax 1  number of channels\n") 
  out.write("jmax 1  number of backgrounds\n") 
  out.write("kmax 2  number of nuisance parameters (sources of systematical uncertainties)\n") 
  out.write("------------\n") 
  out.write("# we have just one channel, in which we observe 0 events\n") 
  out.write("bin 1\n") 
  out.write("observation "+str(bkg_yield)+"\n") 
  out.write("------------\n") 
  out.write("# now we list the expected events for signal and all backgrounds in that bin\n") 
  out.write("# the second 'process' line must have a positive number for backgrounds, and 0 for signal\n") 
  out.write("# then we list the independent sources of uncertainties, and give their effect (syst. error)\n") 
  out.write("# on each process and bin\n") 
  out.write("bin\t1\t1\n")
  out.write("process\tHN"+mass+"\tbkgd\n")
  out.write("process\t0\t1\n")
  out.write("rate\t"+str(signal_weighted)+"\t"+str(bkg_yield)+"\n")
  out.write("------------\n")
  #out.write("lumi\tlnN\t1.026\t1.026\n")
  out.write("sigerr\tlnN\t"+str(signal_yield_err)+"\t-\n")

  if "mumue" in WHICHCUTOPDIR:
    out.write("bkgerr\tlnN\t-\t"+str(1.30)+"\n")
  else:
    out.write("bkgerr\tlnN\t-\t"+str(bkg_yield_err)+"\n")

  out.close()




#  KEY: TH1D  hist_mass;1  
#  KEY: TH1D  hist_xsec;1  
#  KEY: TH1D  hist_fake;1  
#  KEY: TH1D  hist_prompt;1  
#  KEY: TH1D  hist_signal;1

  

