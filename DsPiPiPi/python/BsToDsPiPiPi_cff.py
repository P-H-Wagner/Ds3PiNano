import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import Var, CandVars
from controlChannel.DsPiPiPi.common_cff import RDsCandVars, ufloat, uint, ubool
from controlChannel.DsPiPiPi.rds_common_cff import TableDefaultVariables, TableDefault
from controlChannel.DsPiPiPi.variables_cff import * 
#from PhysicsTools.RDsNano.primaryVertices_cff import *

#BsToDsPhiKKPiMuCfg = BuilderDefaultCfg.clone()
#BTo3MuCfg.dileptons             = cms.InputTag('JpsiMuonPairs')
#BTo3MuCfg.leptonTransientTracks = JpsiMuonPairs.transientTracksSrc

#cuts for ma thesis at:https://github.com/rmanzoni/Bmmm/blob/main/Bmmm/Analysis/test/rds/cuts.py

BsToDsPiPiPi = cms.EDProducer(
    'BsToDsPiPiPiBuilder',
    offBeamSpot = cms.InputTag('offlineBeamSpot'),
    pfCand = cms.InputTag('packedPFCandidates'),
    trgInfo = cms.InputTag('pionTagSelector', 'triggerInfo'),
    pvCand = cms.InputTag("offlineSlimmedPrimaryVertices"),
    tracks = cms.InputTag('packedPFCandidates'), # for isolation
    lostTracks = cms.InputTag("lostTracks"),     # consider also lost pf cands!
    prunedCand = cms.InputTag("prunedGenParticles"),
    packedCand = cms.InputTag("packedGenParticles"),
    photonCand = cms.InputTag("reducedEgamma"),
    unpackedTracksCand = cms.InputTag("unpackedTracksAndVertices"),
    vtxWithBsCand = cms.InputTag("primaryVertexRefit","WithBS", "RDsNANO"),
    hadSelection = cms.string(' &&  '.join([
#'abs(pdgId) != 11',
#'abs(pdgId) != 13',
'abs(pdgId) == 211',
'charge != 0',
'pt > 0.7', 
'eta > -2.4',
'eta < 2.4',
'hasTrackDetails'])), # pre-selection of hadrons (k1,k2 and pion)

    threePionSelection = cms.string(' &&  '.join([
'abs(pdgId) == 211',
'charge != 0',
'pt > 0.7', 
'eta > -2.4',
'eta < 2.4',
'hasTrackDetails'])), # pre-selection of pi2 and pi3 


    gSelection = cms.string(' &&  '.join([
'(abs(pdgId) == 22)'])), #pre-selection of photons

    hadSelectionGen = cms.string(' &&  '.join([
'charge != 0',
])), # pre-selection of Gen hadrons (k1,k2 and pion), allow some tolerance w.r.t. hadSelection
maxdRHadTrgPion = cms.double( 1.2),       # max dR between hadron and muon
mindRHadTrgPion = cms.double( 0.005),    # min dR "
maxdRPhotonDs    = cms.double( 1.0),       # max dR between photon and Ds
maxdzDiffHadTrgPion = cms.double( 1.0),       # max difference in dz between muon/pv and had/pv
maxdxyHadPv      = cms.double( 0.6),
phiMassAllowance = cms.double( 0.02),    # allow 20 MeV when collecting candidates for phi 
dsMassAllowance  = cms.double( 0.06),    # allow 60 MeV when collecting candidates for ds
dsStarMassAllowance  = cms.double( 0.1), # allow 100 MeV when collecting candidates for dsstar
drMatchGen       = cms.double( 0.1),         # allow 0.1, (0.05 would also be reasonable) in dR when gen matching
maxBsMass        = cms.double( 8.0 ),  
piMass           = cms.double( 0.13957039),      # pi mass
piMassSigma      = cms.double( 0.00000018),      # pi mass
kMass            = cms.double( 0.493677),         # kaon mass
kMassSigma       = cms.double( 0.000016),         # kaon mass
phiMass          = cms.double( 1.019461),       # phi mass
constrainPhiMass = cms.bool(True),    # constrain phi mass in the vtx fit?
minPhiVtxProb    = cms.double( 0.01),
dsMass           = cms.double( 1.96834),         # ds mass
constrainDsMass  = cms.bool(False),     # constrain Ds mass in the vtx fit?
minDsVtxProb     = cms.double( 0.01),
dsStarMass       = cms.double( 2.112204),    # ds star mass
muMass           = cms.double( 0.105658),        # mu ma
muMassSigma      = cms.double( 0.0000000023),        # mu mass
bsMass           = cms.double( 5.36688),         # bs mass
isoCone          = cms.double( 0.5)             # cut on dR for the mu isolation cone
)

print( " ========> Parameters used:")
print(BsToDsPiPiPi.dumpPython)

#BsToDsPhiKKPiMuVariables.extend(vertexVariables)
combined_variables = cms.PSet(
  triggerVariables,
  prefitBasicVariables,
  #genVariables,
  vertexVariables,
  postfitBasicVariables,
  helicityVariables,
  bsMomentumVariables,
  photonVariables

)
  
BsToDsPiPiPiTable = cms.EDProducer('SimpleCompositeCandidateFlatTableProducer',

    src = cms.InputTag("BsToDsPiPiPi:bs"),
    cut = cms.string(""),
    name = cms.string(""),
    doc  = cms.string("BsToDsPiPiPi"),
    singleton = cms.bool(False), # the number of entries is variable
    extension = cms.bool(False), # this is the main table for the muons
    variables = combined_variables

)



BsToDsPiPiPiSequence = cms.Sequence(BsToDsPiPiPi + BsToDsPiPiPiTable)

