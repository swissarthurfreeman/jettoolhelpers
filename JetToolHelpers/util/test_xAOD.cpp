#include "TFile.h"
#include "TChain.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"
#include "xAODTracking/Vertex.h"
#include "xAODTracking/VertexContainer.h"

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/HistoInput1D.h"
#include "JetToolHelpers/HistoInput2D.h"

int main (int argc, char* argv[])
{

    // Prepare to read the input file(s)
    xAOD::TEvent event;
    xAOD::TStore store;

    // Check the arguments
    if (argc != 3)
    {
        printf("USAGE: %s <num events, -1 for all> <input file>\n",argv[0]);
        exit(1);
    }
    if (TString(argv[1]).Contains(".root"))
    {
        printf("ERROR: First argument should be a number, but instead is a root file, check argument ordering\n");
        exit(1);
    }
    if (!TString(argv[2]).Contains(".root"))
    {
        printf("ERROR: Second argument should be a root file, but doesn't appear to be\n");
        exit(1);
    }

    // Hard-code the ROOT histogram file for now (for our tool tests)
    const std::string histFile {"/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/JetUncertainties/CalibArea-08/rel21/Summer2019/R4_AllComponents.root"};
    const std::string histName1D {"EffectiveNP_1_AntiKt4EMPFlow"};
    const std::string histName2D {"EtaIntercalibration_Modelling_AntiKt4EMPFlow"};
    std::unique_ptr<IInputBase> myH1D = std::make_unique<HistoInput1D>("myH1D",histFile,histName1D,"pt","float",true);
    std::unique_ptr<IInputBase> myH2D = std::make_unique<HistoInput2D>("myH2D",histFile,histName2D,"pt","float",true,"abseta","float",true);
    if (!myH1D->initialize())
    {
        std::cout << "Failed to initialise HistoInput1D of name " << histName1D << "\n";
        return 1;
    }
    else if (!myH2D->initialize())
    {
        std::cout << "Failed to initialise HistoInput2D of name " << histName2D << "\n";
        return 1;
    }
    else
    {
        std::cout << "Successfully initialized both 1D and 2D test histograms\n";
    }


    // Parse the arguments
    Long64_t numEvents = atoll(argv[1]);
    TChain* chain = new TChain("CollectionTree");
    chain->AddFile(argv[2]);
    if (event.readFrom(chain).isFailure())
    {
        printf("Failed to read from the input file\n");
        exit(1);
    }

    // Check the number of events
    if (numEvents < 0)
        numEvents = event.getEntries();
    else if (numEvents > event.getEntries())
    {
        printf("Asked for %lld events, but only %lld are available, capping to number of available events\n",numEvents,event.getEntries());
        numEvents = event.getEntries();
    }
    printf("Processing %lld events...\n",numEvents);

    // Loop over the events
    for (Long64_t iEntry = 0; iEntry < numEvents; ++iEntry)
    {
        // Get the event
        event.getEntry(iEntry);
        if (iEntry%100 == 0)
            printf("Processing entry %lld/%lld (%lld available)\n",iEntry,numEvents,event.getEntries());

        // Get EventInfo and metadata
        const xAOD::EventInfo* eInfo = nullptr;
        if(event.retrieve(eInfo,"EventInfo").isFailure() || !eInfo)
        {
            printf("Failed to retrieve EventInfo\n");
            exit(1);
        }
        unsigned long long eventNumber  = eInfo->eventNumber();
        float muAve                     = eInfo->averageInteractionsPerCrossing();
        float muAct                     = eInfo->actualInteractionsPerCrossing();

        // Get pileup info and only continue if there is a PV
        const xAOD::VertexContainer* vertices = nullptr;
        if ( event.retrieve(vertices,"PrimaryVertices").isFailure() || !vertices)
        {
            printf("Failed to retrieve PV container for event %lld (entry %lld)\n",eInfo->eventNumber(),iEntry);
            continue;
        }
        xAOD::VertexContainer::const_iterator vtx_itr = vertices->begin();
        xAOD::VertexContainer::const_iterator vtx_end = vertices->end();
        unsigned NPV = 0;
        for ( ; vtx_itr != vtx_end; ++vtx_itr ) 
            if ( (*vtx_itr)->nTrackParticles() >= 2 ) ++NPV;
        if (!NPV) continue;

        // Get jets
        const xAOD::JetContainer* jets = nullptr;
        if (event.retrieve(jets,"AntiKt4EMPFlowJets").isFailure() || !jets)
        {
            printf("Failed to retrive jets for event %lld (entry %lld)\n",eInfo->eventNumber(),iEntry);
            continue;
        }

        // Make an empty JetContext object
        JetContext jc;

        // Print some info
        printf("Event %llu:\n",eventNumber);
        printf("\tmu (average) = %f, mu (actual) = %f\n",muAve,muAct);
        printf("\t#PV (no cut) = %zu, #PV (>=2 tracks) = %d\n",vertices->size(),NPV);
        printf("\t#jets = %zu",jets->size());
        if (jets->size())
        {
            printf(", leading jet (pt,eta,phi,m) = (%.1f,%.1f,%.1f,%.1f)\n",jets->at(0)->pt()/1.e3,jets->at(0)->eta(),jets->at(0)->phi(),jets->at(0)->m()/1.e3);
            printf("Histogram values are %f (H1D) and %f (H2D)\n",myH1D->getValue(*jets->at(0),jc),myH2D->getValue(*jets->at(0),jc));
        }
        else
            printf(", no leading jet to print\n");


        // End of the event, clear the store to free memory
        store.clear();
    }

    // Done!

    return 0;
}

