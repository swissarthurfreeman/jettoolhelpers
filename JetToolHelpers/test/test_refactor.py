import ROOT

histFile = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/JetUncertainties/CalibArea-08/rel21/Summer2019/R4_AllComponents.root"
histName1D = "EffectiveNP_1_AntiKt4EMPFlow"
histName2D = "EtaIntercalibration_Modelling_AntiKt4EMPFlow"


import array

#jc = ROOT.JetContext()
#jc.setValue("pt", 30)
#jc.setValue("abseta", 2.5)
#jc.setValue("funny_key", -20000.5000)
#jc.getValue("abseta", val)
#print(val)

import ctypes

myH1D = ROOT.HistoInput("myH1D", histFile, histName1D, "pt", "float", False)

if not myH1D.initialize():
    print("Failed to initialise HistoInput of name", histName1D)

jc = ROOT.JetContext()
jc.setValue("pt", 30.0)
jc.setValue("abseta", 2.5)
jet = ROOT.xAOD.Jet()
val1D = array.array('d',[0])

print(val1D)
print(myH1D.getFileName())

val1D = ctypes.c_double()
print(myH1D.getValue(jet, jc, val1D))
print(val1D)

#print(myH1D.getValue(jet, jc))