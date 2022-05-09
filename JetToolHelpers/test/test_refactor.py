import ROOT

histFile = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/JetUncertainties/CalibArea-08/rel21/Summer2019/R4_AllComponents.root"
histName1D = "EffectiveNP_1_AntiKt4EMPFlow"
histName2D = "EtaIntercalibration_Modelling_AntiKt4EMPFlow"

myH1D = ROOT.HistoInput("myH1D", histFile, histName1D, "pt", "float", True)

if not myH1D.initialize():
    print("Failed to initialise HistoInput of name", histName1D)

import array

jc = ROOT.JetContext()
jc.setValue("pt", 30)
jc.setValue("abseta", 2.5)
jc.setValue("funny_key", -20000.5000)
print(jc.sayHello())
#jc.getValue("abseta", val)
#print(val)

# jet = ROOT.xAOD.Jet()
# val1D = array.array('d',[0])
# print(myH1D.getFileName())
# print(myH1D.getValue(jet, jc, val1D))