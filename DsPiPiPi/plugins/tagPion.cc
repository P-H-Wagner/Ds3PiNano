//#include "RDs-tools-private/RDsNano/src/helper.h" 
//#include "RDs-tools-private/RDsNano/src/classes.h" 

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "controlChannel/DsPiPiPi/interface/TriggerCandInfo.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerAlgorithm.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/BasicTransientTrack.h"

//B field 
#include "MagneticField/ParametrizedEngine/src/OAEParametrizedMagneticField.h"
// for the cut as string
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include <TLorentzVector.h>
#include "helper.h" 

using namespace std;

class tagPion : public edm::EDProducer {

public:

    //define the transienttrackcollection type, does not exist!
    //typedef std::vector<reco::TransientTrack> TransientTrackCollection;
    
    //constructor, takes a reference to the ParameterSet 
    explicit tagPion(const edm::ParameterSet&);
    //destructor
    ~tagPion() override {};
    virtual void endJob() override; // NEW! 

private:

    int nEvents        = 0;
    int passesHLT      = 0;
    int passesHLTFilter= 0;
    int patTriggerCand = 0;
    int trgCand        = 0;

   
    OAEParametrizedMagneticField *paramField = new OAEParametrizedMagneticField("3_8T");

    // pion selection
    const StringCutObjectSelector<pat::PackedCandidate> pionSelection_;
 
    //physics heart ---- check the constant override stuff!!!
    void produce(edm::Event&, const edm::EventSetup&) override;

    //define tokens to access data later
    const edm::InputTag pionTag;
    const edm::EDGetTokenT<pat::PackedCandidateCollection> pionSrc_;

    //lost tracks for isolation
    const edm::InputTag tracksLostTag;
    const edm::EDGetTokenT<pat::PackedCandidateCollection> tracksLost_;


    const edm::InputTag triggerBitTag;
    const edm::EDGetTokenT<edm::TriggerResults> triggerBits_;

    const edm::InputTag triggerObjectTag;
    const edm::EDGetTokenT<std::vector<pat::TriggerObjectStandAlone>> triggerObjects_;

    const edm::InputTag triggerPrescaleTag;
    const edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;

    const edm::InputTag vertexSrcTag;
    const edm::EDGetTokenT<reco::VertexCollection> vertexSrc_;

    //the maximal dR you allow between pat muon and trigger muon candidate
    const string trgFilterLabelMu7_4_ ;
    const string trgFilterLabelMu8_3_ ;
    const string trgFilterLabelMu8p5_3p5_ ;
    const string trgFilterLabelMu8_5_ ;
    const string trgFilterLabelMu8_6_ ;
    const string trgFilterLabelMu9_4_ ;
    const string trgFilterLabelMu9_5_ ;
    const string trgFilterLabelMu9_6_ ;
    const string trgFilterLabelMu10p5_3p5_ ;
    const string trgFilterLabelMu12_6_;

    const string hlt_7_4_ ;
    const string hlt_8_3_ ;
    const string hlt_8p5_3p5_ ;
    const string hlt_8_5_ ;
    const string hlt_8_6_ ;
    const string hlt_9_4_ ;
    const string hlt_9_5_ ;
    const string hlt_9_6_ ;
    const string hlt_10p5_3p5_ ;
    const string hlt_12_6_;
 
    const double maxdR_; 

};

//constructor definition (outsde class)
tagPion::tagPion(const edm::ParameterSet& iConfig):
 
  //for the muons
  pionSelection_(iConfig.getParameter<std::string>                   ("pionSelection")), 
  pionTag(iConfig.getParameter<edm::InputTag>                        ("pfCand")),
  pionSrc_(consumes<pat::PackedCandidateCollection>                  (pionTag)),

  tracksLostTag(iConfig.getParameter<edm::InputTag>     ("lostTracks"         )),
  tracksLost_(consumes<pat::PackedCandidateCollection>  (tracksLostTag        )),

  //for trigger info
  triggerBitTag(iConfig.getParameter<edm::InputTag>                  ("trgResultsCollection")),
  triggerBits_(consumes<edm::TriggerResults>                         (triggerBitTag)),

  triggerObjectTag(iConfig.getParameter<edm::InputTag>               ("trgObjectsCollection")),
  triggerObjects_(consumes<std::vector<pat::TriggerObjectStandAlone>>(triggerObjectTag)),

  triggerPrescaleTag(iConfig.getParameter<edm::InputTag>             ("trgPrescaleCollection")),
  triggerPrescales_(consumes<pat::PackedTriggerPrescales>            (triggerPrescaleTag)),
  //vertex info
  vertexSrcTag(iConfig.getParameter<edm::InputTag>                   ("vtxCollection")),
  vertexSrc_(consumes<reco::VertexCollection>(vertexSrcTag)), 



  //parameters
  trgFilterLabelMu7_4_ (iConfig.getParameter<string>                    ("trgFilterLabelMu7_4" )),
  trgFilterLabelMu8_3_ (iConfig.getParameter<string>                    ("trgFilterLabelMu8_3" )),
  trgFilterLabelMu8p5_3p5_ (iConfig.getParameter<string>                ("trgFilterLabelMu8p5_3p5" )),
  trgFilterLabelMu8_5_ (iConfig.getParameter<string>                    ("trgFilterLabelMu8_5" )),
  trgFilterLabelMu8_6_ (iConfig.getParameter<string>                    ("trgFilterLabelMu8_6" )),
  trgFilterLabelMu9_4_ (iConfig.getParameter<string>                    ("trgFilterLabelMu9_4" )),
  trgFilterLabelMu9_5_ (iConfig.getParameter<string>                    ("trgFilterLabelMu9_5" )),
  trgFilterLabelMu9_6_ (iConfig.getParameter<string>                    ("trgFilterLabelMu9_6" )),
  trgFilterLabelMu10p5_3p5_ (iConfig.getParameter<string>               ("trgFilterLabelMu10p5_3p5" )),
  trgFilterLabelMu12_6_(iConfig.getParameter<string>                    ("trgFilterLabelMu12_6")),

  hlt_7_4_     (iConfig.getParameter<string>                           ("hlt_7_4")),
  hlt_8_3_     (iConfig.getParameter<string>                           ("hlt_8_3")),
  hlt_8p5_3p5_ (iConfig.getParameter<string>                           ("hlt_8p5_3p5")),
  hlt_8_5_     (iConfig.getParameter<string>                           ("hlt_8_5")),
  hlt_8_6_     (iConfig.getParameter<string>                           ("hlt_8_6")),
  hlt_9_4_     (iConfig.getParameter<string>                           ("hlt_9_4")),
  hlt_9_5_     (iConfig.getParameter<string>                           ("hlt_9_5")),
  hlt_9_6_     (iConfig.getParameter<string>                           ("hlt_9_6")),
  hlt_10p5_3p5_(iConfig.getParameter<string>                           ("hlt_10p5_3p5")),
  hlt_12_6_    (iConfig.getParameter<string>                           ("hlt_12_6")),

  maxdR_(iConfig.getParameter<double>                                ("maxdR_matching"))

{
  // produces 
  //produces<pat::PackedCandidateCollection>         ("trgPions");  
  produces<std::vector<TriggerCandInfo>>("triggerInfo");
}



void tagPion::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
 
  //Define handles
  //std::cout << "New event!" << std::endl;

  edm::Handle<reco::VertexCollection> vertexHandle;
  iEvent.getByToken(vertexSrc_, vertexHandle);

  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
  iEvent.getByToken(triggerPrescales_, triggerPrescales);

  edm::Handle<edm::TriggerResults> triggerBits;
  iEvent.getByToken(triggerBits_, triggerBits);

  edm::Handle<pat::PackedCandidateCollection> tracksLost;
  iEvent.getByToken(tracksLost_,tracksLost);
  

  // for every event, get the name list of the triggers
  const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);

      
  unsigned int trgBitsMax = triggerBits->size();

  unsigned int index_7_4       = trgBitsMax; 
  unsigned int index_8_3       = trgBitsMax; 
  unsigned int index_8p5_3p5   = trgBitsMax; 
  unsigned int index_8_5       = trgBitsMax; 
  unsigned int index_8_6       = trgBitsMax; 
  unsigned int index_9_4       = trgBitsMax; 
  unsigned int index_9_5       = trgBitsMax; 
  unsigned int index_9_6       = trgBitsMax; 
  unsigned int index_10p5_3p5  = trgBitsMax; 
  unsigned int index_12_6      = trgBitsMax; 

  bool pass_7_4         = false; 
  bool pass_8_3         = false; 
  bool pass_8p5_3p5     = false; 
  bool pass_8_5         = false; 
  bool pass_8_6         = false; 
  bool pass_9_4         = false; 
  bool pass_9_5         = false; 
  bool pass_9_6         = false; 
  bool pass_10p5_3p5    = false; 
  bool pass_12_6        = false; 

  int prescale_7_4      = 0;
  int prescale_8_3      = 0;
  int prescale_8p5_3p5  = 0;
  int prescale_8_5      = 0;
  int prescale_8_6      = 0;
  int prescale_9_4      = 0;
  int prescale_9_5      = 0;
  int prescale_9_6      = 0;
  int prescale_10p5_3p5 = 0;
  int prescale_12_6     = 0;
  
  
  /////////// Debugging:
  for (unsigned int i = 0; i < names.size(); ++i) {

    std::string trgName = names.triggerName(i);

    if (trgName.find(hlt_7_4_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_7_4 = names.triggerIndex(trgName);

      if ((index_7_4 < trgBitsMax) && (triggerBits->accept(index_7_4))){
        //std::cout << "index is: "  << index_7_4 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_7_4) << std::endl;
        pass_7_4 = true;
        prescale_7_4  = triggerPrescales->getPrescaleForIndex (index_7_4 );
        prescale_7_4 *= 5;
      }

    }
    if (trgName.find(hlt_8_3_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_8_3 = names.triggerIndex(trgName);
      if ((index_8_3 < trgBitsMax) && (triggerBits->accept(index_8_3))){
        //std::cout << "index is: "  << index_8_3 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_8_3) << std::endl;
        pass_8_3      = true;
        prescale_8_3  = triggerPrescales->getPrescaleForIndex (index_8_3 );
        prescale_8_3 *= 5;
      }

    }
    if (trgName.find(hlt_8_5_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_8_5 = names.triggerIndex(trgName);
      if ((index_8_5 < trgBitsMax) && (triggerBits->accept(index_8_5))){
        //std::cout << "index is: "  << index_8_5 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_8_5) << std::endl;
        pass_8_5 = true;
        prescale_8_5  = triggerPrescales->getPrescaleForIndex (index_8_5 );
        prescale_8_5 *= 5;
      }

    }
    if (trgName.find(hlt_8_6_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_8_6 = names.triggerIndex(trgName);
      if ((index_8_6 < trgBitsMax) && (triggerBits->accept(index_8_6))){
        //std::cout << "index is: "  << index_8_6 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_8_6) << std::endl;
        pass_8_6 = true;
        prescale_8_6  = triggerPrescales->getPrescaleForIndex (index_8_6 );
        prescale_8_6 *= 5;
      }

    }


    if (trgName.find(hlt_8p5_3p5_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_8p5_3p5 = names.triggerIndex(trgName);
      if ((index_8p5_3p5 < trgBitsMax) && (triggerBits->accept(index_8p5_3p5))){
        //std::cout << "index is: "  << index_8p5_3p5 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_8p5_3p5) << std::endl;
        pass_8p5_3p5 = true;
        prescale_8p5_3p5  = triggerPrescales->getPrescaleForIndex (index_8p5_3p5 );
        prescale_8p5_3p5 *= 5;
      }

    }


    if (trgName.find(hlt_9_4_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_9_4 = names.triggerIndex(trgName);
      if ((index_9_4 < trgBitsMax) && (triggerBits->accept(index_9_4))){
        //std::cout << "index is: "  << index_9_4 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_9_4) << std::endl;
        pass_9_4 = true;
        prescale_9_4  = triggerPrescales->getPrescaleForIndex (index_9_4 );
        prescale_9_4 *= 5;
      }

    }
    if (trgName.find(hlt_9_5_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_9_5 = names.triggerIndex(trgName);
      if ((index_9_5 < trgBitsMax) && (triggerBits->accept(index_9_5))){
        //std::cout << "index is: "  << index_9_5 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_9_5) << std::endl;
        pass_9_5 = true;
        prescale_9_5  = triggerPrescales->getPrescaleForIndex (index_9_5 );
        prescale_9_5 *= 5;
      }

    }
    if (trgName.find(hlt_9_6_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_9_6 = names.triggerIndex(trgName);
      if ((index_9_6 < trgBitsMax) && (triggerBits->accept(index_9_6))){
        //std::cout << "index is: "  << index_9_6 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_9_6) << std::endl;
        pass_9_6 = true;
        prescale_9_6  = triggerPrescales->getPrescaleForIndex (index_9_6 );
        prescale_9_6 *= 5;
      }

    }

    if (trgName.find(hlt_10p5_3p5_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_10p5_3p5 = names.triggerIndex(trgName);
      if ((index_10p5_3p5 < trgBitsMax) && (triggerBits->accept(index_10p5_3p5))){
        //std::cout << "index is: "  << index_10p5_3p5 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_10p5_3p5) << std::endl;
        pass_10p5_3p5 = true;
        prescale_10p5_3p5  = triggerPrescales->getPrescaleForIndex (index_10p5_3p5 );
        prescale_10p5_3p5 *= 5;
      }

    }




    if (trgName.find(hlt_12_6_) != std::string::npos){

      //std::cout << trgName << std::endl;
      index_12_6 = names.triggerIndex(trgName);
      if ((index_12_6 < trgBitsMax) && (triggerBits->accept(index_12_6))){
        //std::cout << "index is: "  << index_12_6 << std::endl;
        //std::cout << "accepted ? " << triggerBits->accept(index_12_6) << std::endl;
        pass_12_6 = true;
        prescale_12_6  = triggerPrescales->getPrescaleForIndex (index_12_6 );
        prescale_12_6 *= 5;
      }

    }


  }
  /////////////

  nEvents++; 

  //taken from https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2016#Trigger
  edm::Handle<std::vector<pat::TriggerObjectStandAlone>> triggerObjects;
  iEvent.getByToken(triggerObjects_, triggerObjects);

  //pions
  edm::Handle<pat::PackedCandidateCollection> pions;
  iEvent.getByToken(pionSrc_, pions);

  //to save
  std::unique_ptr<pat::PackedCandidateCollection> trgPions(new pat::PackedCandidateCollection);
  auto triggerInfo = std::make_unique<std::vector<TriggerCandInfo>>();
 
  //std::cout << "before passing " << muons->size() << std::endl;
  //only continue when we the event passes one of the triggers
  if (pass_7_4 || pass_8_3 || pass_8_5 || pass_8_6 || pass_9_4 || pass_9_5 || pass_9_6 || pass_12_6) {

    //std::cout << "passed :) " << pions->size() << std::endl;
    
    passesHLT++;

    ////////////////////////////////////////////////////////////////////////////
    // Make sure that you can find a PF cand (pion) matching the HLT object   //
    // and take the best match in dR as candidate                             //
    ////////////////////////////////////////////////////////////////////////////
  
    // count the nr of PF pions which fired the trigger -> reset to 0 in every event
    int counter = 0;
  
    //std::cout << "#pions in this event " << pions->size() + tracksLost->size()  << std::endl;

    for (unsigned int piIdx=0; piIdx < pions->size() + tracksLost->size() ; ++piIdx){
      //if (iEvent.id().luminosityBlock() != 2007) continue;  
      //std::cout << iEvent.id().event() << std::endl;
      //std::cout << "event found! "<< std::endl;

      //define a pointer to the pf cand at position pfIdx
      edm::Ptr<pat::PackedCandidate> piPtr;
      if (piIdx < pions->size()) piPtr = edm::Ptr<pat::PackedCandidate>(pions, piIdx); //normal tracks
      else piPtr = edm::Ptr<pat::PackedCandidate>(tracksLost, piIdx - pions->size()); //lost tracks
      
      // selection 
      if (!pionSelection_(*piPtr)) continue; 
     
      //std::cout << "found pion with pt,eta,id:" << piPtr->pt() <<" " << piPtr->eta() << std::endl;
 
      patTriggerCand++;
  
      // initialize start values
      float drPionTrgObj   = 0.0;

      int   trgObjIdx      = -1;
      int   iTrg           = 0 ;
      int   iMatch         = 0 ;

      int   filterLabelMu7_4  = -1;
      int   filterLabelMu8_3  = -1;
      int   filterLabelMu8p5_3p5  = -1;
      int   filterLabelMu8_5  = -1;
      int   filterLabelMu8_6  = -1;
      int   filterLabelMu9_4  = -1;
      int   filterLabelMu9_5  = -1;
      int   filterLabelMu9_6  = -1;
      int   filterLabelMu10p5_3p5  = -1;
      int   filterLabelMu12_6 = -1;

      for(unsigned int objIdx=0; objIdx < triggerObjects->size(); ++objIdx){
  
        iTrg++;
  
        pat::TriggerObjectStandAlone trgObj = (*triggerObjects)[objIdx];      

        //unpack trigger labels
        trgObj.unpackFilterLabels(iEvent, *triggerBits);
  
        std::vector<std::string> filterLabels = trgObj.filterLabels();
    
        // check if the trigger object was actually firing our trigger

        const bool passTrigger =
            (pass_7_4  && trgObj.hasFilterLabel(trgFilterLabelMu7_4_))  ||
            (pass_8_3  && trgObj.hasFilterLabel(trgFilterLabelMu8_3_))  ||
            (pass_8p5_3p5  && trgObj.hasFilterLabel(trgFilterLabelMu8p5_3p5_))  ||
            (pass_8_5  && trgObj.hasFilterLabel(trgFilterLabelMu8_5_))  ||
            (pass_8_6  && trgObj.hasFilterLabel(trgFilterLabelMu8_6_))  ||
            (pass_9_4  && trgObj.hasFilterLabel(trgFilterLabelMu9_4_))  ||
            (pass_9_5  && trgObj.hasFilterLabel(trgFilterLabelMu9_5_))  ||
            (pass_9_6  && trgObj.hasFilterLabel(trgFilterLabelMu9_6_))  ||
            (pass_10p5_3p5  && trgObj.hasFilterLabel(trgFilterLabelMu10p5_3p5_))  ||
            (pass_12_6 && trgObj.hasFilterLabel(trgFilterLabelMu12_6_)) ;
       
        // CHANGE ME 
        //if (!passTrigger) continue;
 
        passesHLTFilter++;   
        iMatch++;

        float dr = reco::deltaR(trgObj, *piPtr);
     
        if( ((dr < drPionTrgObj) || (trgObjIdx < 0))  && (dr < maxdR_))
        {
              //modify dR and the reco and triggering muon index
  	      drPionTrgObj = dr;
  	      trgObjIdx    = objIdx;
              filterLabelMu7_4      = trgObj.hasFilterLabel(trgFilterLabelMu7_4_) ;
              filterLabelMu8_3      = trgObj.hasFilterLabel(trgFilterLabelMu8_3_) ;
              filterLabelMu8p5_3p5  = trgObj.hasFilterLabel(trgFilterLabelMu8p5_3p5_) ;
              filterLabelMu8_5      = trgObj.hasFilterLabel(trgFilterLabelMu8_5_) ;
              filterLabelMu8_6      = trgObj.hasFilterLabel(trgFilterLabelMu8_6_) ;
              filterLabelMu9_4      = trgObj.hasFilterLabel(trgFilterLabelMu9_4_) ;
              filterLabelMu9_5      = trgObj.hasFilterLabel(trgFilterLabelMu9_5_) ;
              filterLabelMu9_6      = trgObj.hasFilterLabel(trgFilterLabelMu9_6_) ;
              filterLabelMu10p5_3p5 = trgObj.hasFilterLabel(trgFilterLabelMu10p5_3p5_) ;
              filterLabelMu12_6     = trgObj.hasFilterLabel(trgFilterLabelMu12_6_);

        }
  
      } // closing loop over trg pions
  
      //save pion if we found a matching candidate 
      if(trgObjIdx != -1){
     
        trgCand++;
   
        //the following line does a copy of muon with the name trgMatchedMuon (same properties different adress)
        //pat::CompositeCandidate trgMatchedPion(*piPtr);

        // only take muons with valid tracks
        const reco::TransientTrack ttrackTrgPion(*(piPtr->bestTrack()), paramField);  
        if (!ttrackTrgPion.isValid()) continue;

        //store
        TriggerCandInfo info;

        info.piIdx = piIdx;
        info.trgObjIdx = trgObjIdx;
        
        info.mu7_ip4 = pass_7_4;
        info.prescale_mu7_ip4 = prescale_7_4;
        
        info.mu8_ip3 = pass_8_3;
        info.prescale_mu8_ip3 = prescale_8_3;
        
        info.mu8_ip5 = pass_8_5;
        info.prescale_mu8_ip5 = prescale_8_5;
        
        info.mu8p5_ip3p5 = pass_8p5_3p5;
        info.prescale_mu8p5_ip3p5 = prescale_8p5_3p5;
        
        info.mu8_ip6 = pass_8_6;
        info.prescale_mu8_ip6 = prescale_8_6;
        
        info.mu9_ip4 = pass_9_4;
        info.prescale_mu9_ip4 = prescale_9_4;
        
        info.mu9_ip5 = pass_9_5;
        info.prescale_mu9_ip5 = prescale_9_5;
        
        info.mu9_ip6 = pass_9_6;
        info.prescale_mu9_ip6 = prescale_9_6;
        
        info.mu10p5_ip3p5 = pass_10p5_3p5;
        info.prescale_mu10p5_ip3p5 = prescale_10p5_3p5;
        
        info.mu12_ip6 = pass_12_6;
        info.prescale_mu12_ip6 = prescale_12_6;
        
        info.filter_mu7_4 = filterLabelMu7_4;
        info.filter_mu8_3 = filterLabelMu8_3;
        info.filter_mu8_5 = filterLabelMu8_5;
        info.filter_mu8p5_3p5 = filterLabelMu8p5_3p5;
        info.filter_mu8_6 = filterLabelMu8_6;
        info.filter_mu9_4 = filterLabelMu9_4;
        info.filter_mu9_5 = filterLabelMu9_5;
        info.filter_mu9_6 = filterLabelMu9_6;
        info.filter_mu10p5_3p5 = filterLabelMu10p5_3p5;
        info.filter_mu12_6 = filterLabelMu12_6;

        triggerInfo->push_back(info);
  
        //store
  	//trgMatchedPion.addUserInt("piIdx",                piIdx    );
   	//trgMatchedPion.addUserInt("trgObjIdx",            trgObjIdx);

        //trgMatchedPion.addUserInt("mu7_ip4",              pass_7_4);
        //trgMatchedPion.addUserInt("prescale_mu7_ip4",  prescale_7_4);

        //trgMatchedPion.addUserInt("mu8_ip3",           pass_8_3);
        //trgMatchedPion.addUserInt("prescale_mu8_ip3",  prescale_8_3);

        //trgMatchedPion.addUserInt("mu8_ip5",           pass_8_5);
        //trgMatchedPion.addUserInt("prescale_mu8_ip5",  prescale_8_5);

        //trgMatchedPion.addUserInt("mu8p5_ip3p5",           pass_8p5_3p5);
        //trgMatchedPion.addUserInt("prescale_mu8p5_ip3p5",  prescale_8p5_3p5);


        //trgMatchedPion.addUserInt("mu8_ip6",           pass_8_6);
        //trgMatchedPion.addUserInt("prescale_mu8_ip6",  prescale_8_6);

        //trgMatchedPion.addUserInt("mu9_ip4",           pass_9_4);
        //trgMatchedPion.addUserInt("prescale_mu9_ip4",  prescale_9_4);

        //trgMatchedPion.addUserInt("mu9_ip5",           pass_9_5);
        //trgMatchedPion.addUserInt("prescale_mu9_ip5",  prescale_9_5);

        //trgMatchedPion.addUserInt("mu9_ip6",           pass_9_6);
        //trgMatchedPion.addUserInt("prescale_mu9_ip6",  prescale_9_6);

        //trgMatchedPion.addUserInt("mu10p5_ip3p5",           pass_10p5_3p5);
        //trgMatchedPion.addUserInt("prescale_mu10p5_ip3p5",  prescale_10p5_3p5);

        //trgMatchedPion.addUserInt("mu12_ip6",              pass_12_6);
        //trgMatchedPion.addUserInt("prescale_mu12_ip6",  prescale_12_6);

        //trgMatchedPion.addUserInt("filter_mu7_4"         ,  filterLabelMu7_4);
        //trgMatchedPion.addUserInt("filter_mu8_3"         ,  filterLabelMu8_3);
        //trgMatchedPion.addUserInt("filter_mu8_5"         ,  filterLabelMu8_5);
        //trgMatchedPion.addUserInt("filter_mu8p5_3p5"     ,  filterLabelMu8p5_3p5);
        //trgMatchedPion.addUserInt("filter_mu8_6"         ,  filterLabelMu8_6);
        //trgMatchedPion.addUserInt("filter_mu9_4"         ,  filterLabelMu9_4);
        //trgMatchedPion.addUserInt("filter_mu9_5"         ,  filterLabelMu9_5);
        //trgMatchedPion.addUserInt("filter_mu9_6"         ,  filterLabelMu9_6);
        //trgMatchedPion.addUserInt("filter_mu10p5_3p5"     ,  filterLabelMu10p5_3p5);
        //trgMatchedPion.addUserInt("filter_mu12_6"        ,  filterLabelMu12_6);

        //std::cout << "====> Found TRIGGER OBJECT!" << std::endl; 
        //if (!beamSpotHandle.isValid()) continue;
        //const reco::BeamSpot& beamSpot = *beamSpotHandle;
        //double dxyErr = muon.bestTrack()->dxyError();
        //double ipSig;
        //if (dxyErr > 0) {
        //  ipSig = std::abs(muon.bestTrack()->dxy(beamSpot.position()) / dxyErr);
        //  std::cout << "ip sig is: " << ipSig << std::endl;
        //} else {
        //  ipSig = 0;
        //  std::cout << "dxyError zero or negative" << std::endl;
        //}

        //std::cout << "it has impact parameter: " << ipSig << std::endl;
        counter++;
        //trgPions->emplace_back(trgMatchedPion); 
  
      }
    } //close the loop over pat muons
  } //close if condition of HLT

  //store the new collections in the event
  //iEvent.put(std::move(trgPions),    "trgPions");
  iEvent.put(std::move(triggerInfo), "triggerInfo");

} //close produce function

void tagPion::endJob(){
std::cout << "\n--------- TRIGGER MODULE ----------\n" << std::endl;
std::cout << "#Events                                                   : " << nEvents  << std::endl;
std::cout << "#Events where HLT fired                                   : " << passesHLT  << std::endl;
std::cout << "#Events where HLT fired and HLT Filter passed             : " << passesHLTFilter  << std::endl;
std::cout << "#Muon candidates which possibly fire HLT (unmatched)      : " << patTriggerCand  << std::endl;
std::cout << "#Muon candidates which fired HLT (matched to trg object!) : " << patTriggerCand  << std::endl;

}

DEFINE_FWK_MODULE(tagPion);

