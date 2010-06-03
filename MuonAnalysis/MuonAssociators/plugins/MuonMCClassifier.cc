// -*- C++ -*-
//
// Package:    MuonMCClassifier
// Class:      MuonMCClassifier
// 
/**\class MuonMCClassifier MuonMCClassifier.cc MuonAnalysis/MuonAssociators/src/MuonMCClassifier.cc


 CLASSIFICATION: For each RECO Muon, match to SIM particle, and then:
  - If the SIM is not a Muon, label as Punchthrough (1)
  - If the SIM is a Muon, then look at it's provenance.
     A) the SIM muon is also a GEN muon, whose parent is NOT A HADRON AND NOT A TAU
        -> classify as "primary" (4).
     B) the SIM muon is also a GEN muon, whose parent is HEAVY FLAVOURED HADRON OR A TAU
        -> classify as "heavy flavour" (3)
     C) classify as "light flavour/decay" (2)

  In any case, if the TP is not preferentially matched back to the same RECO muon,
  label as Ghost (flip the classification)


 FLAVOUR: 
  - for non-muons: 0
  - for primary muons: 13
  - for non primary muons: flavour of the mother: abs(pdgId) of heaviest quark, or 15 for tau
     
*/
//
// Original Author:  Nov 16 16:12 (lxplus231.cern.ch)
//         Created:  Sun Nov 16 16:14:09 CET 2008
// $Id: MuonMCClassifier.cc,v 1.3 2010/05/24 22:17:04 gpetrucc Exp $
//
//


// system include files
#include <memory>
#include <set>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "SimMuon/MCTruth/interface/MuonAssociatorByHits.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include <SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h>

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

//
// class decleration
class MuonMCClassifier : public edm::EDProducer {
    public:
        explicit MuonMCClassifier(const edm::ParameterSet&);
        ~MuonMCClassifier();

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&);
        /// The RECO objects
        edm::InputTag muons_;

        /// A preselection cut for the muon. 
        /// I pass through pat::Muon so that I can access muon id selectors
        bool hasMuonCut_;
        StringCutObjectSelector<pat::Muon> muonCut_;
 
        /// Track to use
        MuonAssociatorByHits::MuonTrackType trackType_;

        /// The TrackingParticle objects 
        edm::InputTag trackingParticles_;

        /// The Associations
        std::string associatorLabel_;

        TrackingParticleRef getParticleWithGen(TrackingParticleRef tp) const ;

        /// Returns the flavour given a pdg id code
        int flavour(int pdgId) const ;

        /// Write a ValueMap<int> in the event
        template<typename T>
        void writeValueMap(edm::Event &iEvent,
                const edm::Handle<edm::View<reco::Muon> > & handle,
                const std::vector<T> & values,
                const std::string    & label) const ;

        TrackingParticleRef getTpMother(TrackingParticleRef tp) {
            if (tp.isNonnull() && tp->parentVertex().isNonnull() && !tp->parentVertex()->sourceTracks().empty()) {
                return tp->parentVertex()->sourceTracks()[0];
            } else {
                return TrackingParticleRef();
            }
        }

        const HepMC::GenParticle * getGpMother(const HepMC::GenParticle *gp) {
            if (gp != 0) {
                const HepMC::GenVertex *vtx = gp->production_vertex();
                if (vtx != 0 && vtx->particles_in_size() > 0) {
                    return *vtx->particles_in_const_begin();
                }
            }
            return 0;
        }

};

MuonMCClassifier::MuonMCClassifier(const edm::ParameterSet &iConfig) :
    muons_(iConfig.getParameter<edm::InputTag>("muons")),
    hasMuonCut_(iConfig.existsAs<std::string>("muonPreselection")),
    muonCut_(hasMuonCut_ ? iConfig.getParameter<std::string>("muonPreselection") : ""),
    trackingParticles_(iConfig.getParameter<edm::InputTag>("trackingParticles")),
    associatorLabel_(iConfig.getParameter< std::string >("associatorLabel"))
{
    std::string trackType = iConfig.getParameter< std::string >("trackType");
    if (trackType == "inner") trackType_ = MuonAssociatorByHits::InnerTk;
    else if (trackType == "outer") trackType_ = MuonAssociatorByHits::OuterTk;
    else if (trackType == "global") trackType_ = MuonAssociatorByHits::GlobalTk;
    else if (trackType == "segments") trackType_ = MuonAssociatorByHits::Segments;
    else throw cms::Exception("Configuration") << "Track type '" << trackType << "' not supported.\n";

    produces<edm::ValueMap<int> >(); 
    produces<edm::ValueMap<int> >("flav"); 
    produces<edm::ValueMap<int> >("hitsPdgId"); 
    produces<edm::ValueMap<int> >("momPdgId"); 
    produces<edm::ValueMap<int> >("momFlav"); 
    produces<edm::ValueMap<int> >("gmomPdgId"); 
    produces<edm::ValueMap<int> >("gmomFlav"); 
    produces<edm::ValueMap<int> >("tpId");
    produces<edm::ValueMap<float> >("prodRho"); 
    produces<edm::ValueMap<float> >("prodZ"); 
    produces<edm::ValueMap<float> >("tpAssoQuality");
}

MuonMCClassifier::~MuonMCClassifier() 
{
}

void
MuonMCClassifier::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::LogVerbatim("MuonMCClassifier") <<"\n sono in MuonMCClassifier !";
  
    edm::Handle<edm::View<reco::Muon> > muons; 
    iEvent.getByLabel(muons_, muons);

    edm::Handle<TrackingParticleCollection> trackingParticles;
    iEvent.getByLabel(trackingParticles_, trackingParticles);

    edm::ESHandle<TrackAssociatorBase> associatorBase;
    iSetup.get<TrackAssociatorRecord>().get(associatorLabel_, associatorBase);
    const MuonAssociatorByHits * assoByHits = dynamic_cast<const MuonAssociatorByHits *>(associatorBase.product());
    if (assoByHits == 0) throw cms::Exception("Configuration") << "The Track Associator with label '" << associatorLabel_ << "' is not a MuonAssociatorByHits.\n";

    MuonAssociatorByHits::MuonToSimCollection recSimColl;
    MuonAssociatorByHits::SimToMuonCollection simRecColl;
    edm::LogVerbatim("MuonMCClassifier") <<"\n ***************************************************************** ";
    edm::LogVerbatim("MuonMCClassifier") <<  " RECO MUON association, type:  "<< trackType_;
    edm::LogVerbatim("MuonMCClassifier") <<  " ***************************************************************** \n";

    edm::RefToBaseVector<reco::Muon> selMuons;
    if (!hasMuonCut_) {
        // all muons
        selMuons = muons->refVector();
    } else {
        // filter, fill refvectors, associate
        // I pass through pat::Muon so that I can access muon id selectors
        for (size_t i = 0, n = muons->size(); i < n; ++i) {
            edm::RefToBase<reco::Muon> rmu = muons->refAt(i);
            if (muonCut_(pat::Muon(rmu))) selMuons.push_back(rmu);
        }
    }

    edm::RefVector<TrackingParticleCollection> allTPs;
    for (size_t i = 0, n = trackingParticles->size(); i < n; ++i) {
        allTPs.push_back(TrackingParticleRef(trackingParticles,i));
    }

    assoByHits->associateMuons(recSimColl, simRecColl, selMuons, trackType_, allTPs, &iEvent, &iSetup);

    // for global muons without hits on muon detectors, look at the linked standalone muon
    MuonAssociatorByHits::MuonToSimCollection UpdSTA_recSimColl;
    MuonAssociatorByHits::SimToMuonCollection UpdSTA_simRecColl;
    if (trackType_ == MuonAssociatorByHits::GlobalTk) {
      edm::LogVerbatim("MuonMCClassifier") <<"\n ***************************************************************** ";
      edm::LogVerbatim("MuonMCClassifier") <<  " STANDALONE (UpdAtVtx) MUON association ";
      edm::LogVerbatim("MuonMCClassifier") <<  " ***************************************************************** \n";
      assoByHits->associateMuons(UpdSTA_recSimColl, UpdSTA_simRecColl, selMuons, MuonAssociatorByHits::OuterTk, 
				 allTPs, &iEvent, &iSetup);
    }

    typedef MuonAssociatorByHits::MuonToSimCollection::const_iterator r2s_it;
    typedef MuonAssociatorByHits::SimToMuonCollection::const_iterator s2r_it;

    size_t nmu = muons->size();
    edm::LogVerbatim("MuonMCClassifier") <<"\n There are "<<nmu<<" reco::Muons.";

    std::vector<int> classif(nmu, 0);
    std::vector<int> hitsPdgId(nmu, 0), momPdgId(nmu, 0), gmomPdgId(nmu, 0);
    std::vector<int> flav(nmu, 0),      momFlav(nmu, 0),  gmomFlav(nmu, 0);
    std::vector<int> tpId(nmu, -1);
    std::vector<float> prodRho(nmu, 0.0), prodZ(nmu, 0.0);
    std::vector<float> tpAssoQuality(nmu, -1);
    for(size_t i = 0; i < nmu; ++i) {
        edm::LogVerbatim("MuonMCClassifier") <<"\n reco::Muons # "<<i;
        edm::RefToBase<reco::Muon> mu = muons->refAt(i);
        if (hasMuonCut_ && (std::find(selMuons.begin(), selMuons.end(), mu) == selMuons.end()) ) {
            edm::LogVerbatim("MuonMCClassifier") <<"\t muon didn't pass the selection. classified as -99 and skipped";
            classif[i] = -99; continue;
        }

        TrackingParticleRef        tp;
        edm::RefToBase<reco::Muon> muMatchBack;
        r2s_it match = recSimColl.find(mu);
        s2r_it matchback;
        if (match != recSimColl.end()) {
            edm::LogVerbatim("MuonMCClassifier") <<"\t RtS matched Ok...";
            // match->second is vector, front is first element, first is the ref (second would be the quality)
            tp = match->second.front().first;
            tpId[i]          = tp.isNonnull() ? tp.key() : -1; // we check, even if null refs should not appear here at all
            tpAssoQuality[i] = match->second.front().second;
            s2r_it matchback = simRecColl.find(tp);
            if (matchback != simRecColl.end()) {
                muMatchBack = matchback->second.front().first;
            } else {
                edm::LogWarning("MuonMCClassifier") << "\n***WARNING:  This I do NOT understand: why no match back? *** \n";
            }
        } else if ((trackType_ == MuonAssociatorByHits::GlobalTk) &&
                    mu->isGlobalMuon()) {
            // perform a second attempt, matching with the standalone muon
            r2s_it matchSta = UpdSTA_recSimColl.find(mu);
            if (matchSta != UpdSTA_recSimColl.end()) {
                edm::LogVerbatim("MuonMCClassifier") <<"\t RtS matched Ok... from the UpdSTA_recSimColl ";
                tp    = matchSta->second.front().first;
                tpId[i]          = tp.isNonnull() ? tp.key() : -1; // we check, even if null refs should not appear here at all
                tpAssoQuality[i] = matchSta->second.front().second;
                s2r_it matchback = UpdSTA_simRecColl.find(tp);
                if (matchback != UpdSTA_simRecColl.end()) {
                    muMatchBack = matchback->second.front().first;
                } else {
                    edm::LogWarning("MuonMCClassifier") << "\n***WARNING:  This I do NOT understand: why no match back in UpdSTA? *** \n";
                }
            }
        } 
        if (tp.isNonnull()) {
            bool isGhost = muMatchBack != mu;
            if (isGhost) edm::LogVerbatim("MuonMCClassifier") <<"\t This seems a GHOST ! classif[i] will be < 0";

            hitsPdgId[i] = tp->pdgId();
            prodRho[i]   = tp->vertex().Rho(); 
            prodZ[i]     = tp->vertex().Z();
	    edm::LogVerbatim("MuonMCClassifier") <<"\t TP pdgId = "<<hitsPdgId[i] << ", vertex rho = " << prodRho[i] << ", z = " << prodZ[i];

            // Try to extract mother and grand mother of this muon.
            // Unfortunately, SIM and GEN histories require diffent code :-(
            if (!tp->genParticle().empty()) {
                const HepMC::GenParticle * genMom = getGpMother(tp->genParticle()[0].get());
                if (genMom) {
                    momPdgId[i] = genMom->pdg_id();
                    edm::LogVerbatim("MuonMCClassifier") << "\t Particle pdgId = "<<hitsPdgId[i] << " produced at rho = " << prodRho[i] << ", z = " << prodZ[i] << ", has GEN mother pdgId = " << momPdgId[i];
                    const HepMC::GenParticle * genGMom = getGpMother(genMom);
                    if (genGMom) gmomPdgId[i] = genGMom->pdg_id();
                }
            } else {
                TrackingParticleRef simMom = getTpMother(tp);
                if (simMom.isNonnull()) {
                    momPdgId[i] = simMom->pdgId();
                    edm::LogVerbatim("MuonMCClassifier") << "\t Particle pdgId = "<<hitsPdgId[i] << " produced at rho = " << prodRho[i] << ", z = " << prodZ[i] << ", has SIM mother pdgId = " << momPdgId[i];
                    if (!simMom->genParticle().empty()) {
                        const HepMC::GenParticle * genGMom = getGpMother(simMom->genParticle()[0].get());
                        if (genGMom) gmomPdgId[i] = genGMom->pdg_id();
                    } else {
                        TrackingParticleRef simGMom = getTpMother(simMom);
                        if (simGMom.isNonnull()) gmomPdgId[i] = simGMom->pdgId();
                    }
                } else {
                  edm::LogVerbatim("MuonMCClassifier") << "\t Particle pdgId = "<<hitsPdgId[i] << " produced at rho = " << prodRho[i] << ", z = " << prodZ[i] << ", has NO mother!";
                }
            }
            momFlav[i]  = flavour(momPdgId[i]);
            gmomFlav[i] = flavour(gmomPdgId[i]);

            // Check first IF this is a muon at all
            if (abs(tp->pdgId()) != 13) {
                classif[i] = isGhost ? -1 : 1;
                edm::LogVerbatim("MuonMCClassifier") <<"\t This is not a muon. Sorry. classif[i] = " << classif[i];
                continue;
            }

            // Is this SIM muon also a GEN muon, with a mother?
            if (!tp->genParticle().empty() && (momPdgId[i] += 0)) {
                if (abs(momPdgId[i]) < 100 && (abs(momPdgId[i]) != 15)) {
                    classif[i] = isGhost ? -4 : 4;
                    flav[i] = (abs(momPdgId[i]) == 15 ? 15 : 13);
                    edm::LogVerbatim("MuonMCClassifier") <<"\t This seems PRIMARY MUON ! classif[i] = " << classif[i];
                } else if (momFlav[i] == 4 || momFlav[i] == 5 || momFlav[i] == 15) {
                    classif[i] = isGhost ? -3 : 3;
                    flav[i]    = momFlav[i];
                    edm::LogVerbatim("MuonMCClassifier") <<"\t This seems HEAVY FLAVOUR ! classif[i] = " << classif[i];
                } else {
                    classif[i] = isGhost ? -2 : 2;
                    flav[i]    = momFlav[i];
                    edm::LogVerbatim("MuonMCClassifier") <<"\t This seems LIGHT FLAVOUR ! classif[i] = " << classif[i];
                }
            } else {
                classif[i] = isGhost ? -2 : 2;
                flav[i]    = momFlav[i];
                edm::LogVerbatim("MuonMCClassifier") <<"\t This seems LIGHT FLAVOUR ! classif[i] = " << classif[i];
            }
	}
    }
    
    writeValueMap(iEvent, muons, classif,   "");
    writeValueMap(iEvent, muons, flav,      "flav");
    writeValueMap(iEvent, muons, tpId,      "tpId");
    writeValueMap(iEvent, muons, hitsPdgId, "hitsPdgId");
    writeValueMap(iEvent, muons, momPdgId,  "momPdgId");
    writeValueMap(iEvent, muons, momFlav,   "momFlav");
    writeValueMap(iEvent, muons, gmomPdgId, "gmomPdgId");
    writeValueMap(iEvent, muons, gmomFlav,  "gmomFlav");
    writeValueMap(iEvent, muons, prodRho,   "prodRho");
    writeValueMap(iEvent, muons, prodZ,     "prodZ");
    writeValueMap(iEvent, muons, tpAssoQuality, "tpAssoQuality");
}    

template<typename T>
void
MuonMCClassifier::writeValueMap(edm::Event &iEvent,
        const edm::Handle<edm::View<reco::Muon> > & handle,
        const std::vector<T> & values,
        const std::string    & label) const 
{
    using namespace edm; 
    using namespace std;
    auto_ptr<ValueMap<T> > valMap(new ValueMap<T>());
    typename edm::ValueMap<T>::Filler filler(*valMap);
    filler.insert(handle, values.begin(), values.end());
    filler.fill();
    iEvent.put(valMap, label);
}

TrackingParticleRef
MuonMCClassifier::getParticleWithGen(TrackingParticleRef tp) const {
    if (!tp->genParticle().empty()) return tp;
    TrackingParticle::TrackingVertexRef prod = tp->parentVertex();
    if (prod.isNonnull()) {
        foreach(TrackingParticleRef par, prod->sourceTracks()) {
            TrackingParticleRef gp = getParticleWithGen(par);
            if (gp.isNonnull()) return gp;
        }
    }
    return TrackingParticleRef();
}

int
MuonMCClassifier::flavour(int pdgId) const {
    int flav = abs(pdgId);
    // for quarks, leptons and bosons except gluons, take their pdgId
    // muons and taus have themselves as flavour
    if (flav <= 37 && flav != 21) return flav;
    // look for barions
    int bflav = ((flav / 1000) % 10);
    if (bflav != 0) return bflav;
    // look for mesons
    int mflav = ((flav / 100) % 10);
    if (mflav != 0) return mflav;
    return 0;
}


//define this as a plug-in
DEFINE_FWK_MODULE(MuonMCClassifier);
