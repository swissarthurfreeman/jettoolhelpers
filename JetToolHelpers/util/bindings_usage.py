import ROOT
import ctypes as ct

ROOT.gSystem.Load('../build/libJetToolHelpers.so')
jc = ROOT.JetContext()
jc.setValue("machuPichu", 10)
print("JetContext MachuPichu value = ", jc.getValue[int]("machuPichu"))

histo = ROOT.MakeHistoInput("Test Histogram", "./data/R4_AllComponents.root", "Zjet_MuStat12_AntiKt4EMTopo", "pt", "float", True)
print("Created histogram @", histo)

histo.initialize()

value = ct.c_double()
jet = ROOT.xAOD.Jet(ct.c_double(50), ct.c_double(50), ct.c_double(50), ct.c_double(50))
histo.getValue(jet, jc, value)
print("HistoGram readout value = ", value)
