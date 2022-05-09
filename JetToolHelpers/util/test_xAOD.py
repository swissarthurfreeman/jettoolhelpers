import ROOT
import sys

if len(sys.argv) != 1:
    print("USAGE: %s"%(sys.argv[0]))
    sys.exit(1)

# Hard-code the ROOT histogram file for now (for our tool tests)
histFile = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/JetUncertainties/CalibArea-08/rel21/Summer2019/R4_AllComponents.root"
histName1D = "EffectiveNP_1_AntiKt4EMPFlow"
histName2D = "EtaIntercalibration_Modelling_AntiKt4EMPFlow"
#myH1D = ROOT.HistoInput1D("myH1D",histFile,histName1D,"pt","float",False)
#myH2D = ROOT.HistoInput2D("myH2D",histFile,histName2D,"pt",    "float",False,"abseta","float",False)
#myH1D = ROOT.HistoInput1D("myH1D",histFile,histName1D,"pt","float",True)
#myH2D = ROOT.HistoInput2D("myH2D",histFile,histName2D,"pt",    "float",True,
#                                                      "abseta","float",True)

"""
if not myH1D.initialize():
    print("Failed to initialise HistoInput1D of name",histName1D)
    sys.exit(1)
elif not myH2D.initialize():
    print("Failed to initialise HistoInput2D of name",histName2D)
    sys.exit(1)
else:
    print("Successfully initialised both 1D and 2D test histograms")
"""

#event = ROOT.xAOD.TEvent()
#store = ROOT.xAOD.TStore()
#jets = ROOT.xAOD.JetContainer()
#jets.setStore(ROOT.xAOD.JetAuxContainer())
#jets.push_back(ROOT.xAOD.Jet())
#fourMom = ROOT.xAOD.JetFourMom_t(30,2.5,0,1) # pt, eta, phi, mass
#jets.at(0).setJetP4(fourMom)
#jc = ROOT.JetContext()
#print("Histogram values are %f (H1D) and %f (H2D)"%(myH1D.getValue(jets.at(0),jc),myH2D.getValue(jets.at(0),jc)))


import array
jc = ROOT.JetContext()
jc.setValue("pt",30.0)
jc.setValue("abseta",2.5)
jet = ROOT.xAOD.Jet()
val1D = array.array('d',[0])
val2D = array.array('d',[0])
myH1D.getValue(jet,jc,val1D)
myH2D.getValue(jet,jc,val2D)
print("Histogram values are %f (H1D) and %f (H2D)"%(val1D[0],val2D[0]))

