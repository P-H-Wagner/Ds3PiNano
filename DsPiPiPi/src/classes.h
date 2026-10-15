#ifndef BPHNANO_CLASSES_H
#define BPHNANO_CLASSES_H

#include "DataFormats/Common/interface/Wrapper.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "controlChannel/DsPiPiPi/interface/TriggerCandInfo.h"
#include <vector>


namespace {
  struct dictionary {
      std::vector<reco::TransientTrack> ttv;
      edm::Wrapper<std::vector<reco::TransientTrack> > wttv; 

      TriggerCandInfo triggerCandInfo;
      std::vector<TriggerCandInfo> triggerCandInfoVector;
      edm::Wrapper<std::vector<TriggerCandInfo> > wtriggerCandidateInfo;

  };

}

#endif  
