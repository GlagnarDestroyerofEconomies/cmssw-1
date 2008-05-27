#include "RecoParticleFlow/PFClusterTools/interface/SpaceManager.h"
#include <cassert>
#include <algorithm>

using namespace pftools;

SpaceManager::SpaceManager() {
}

SpaceManager::~SpaceManager() {
	
}

void SpaceManager::clear() {
	for (std::map<SpaceVoxelPtr, CalibratorPtr>::iterator it = myAddressBook.begin(); it
			!= myAddressBook.end(); ++it) {
		SpaceVoxelPtr s = (*it).first;
		CalibratorPtr c = (*it).second;
	}
}

void SpaceManager::createCalibrators(const Calibrator& toClone,
		const double etaSeg, const double phiSeg, const double energySeg) {
	std::cout << __PRETTY_FUNCTION__ << ": this method has not yet been implemented!\n";
	PFToolsException me("Unimplemented method! Sorry!");
	throw me;

}

void SpaceManager::createCalibrators(const Calibrator& toClone,
		const unsigned nEta, const double etaMin, const double etaMax,
		const unsigned nPhi, const double phiMin, const double phiMax,
		const unsigned nEnergy, const double energyMin, const double energyMax) throw(
		PFToolsException&) {
	clear();

	if (nEta == 0|| nPhi ==0|| nEnergy == 0) {
		PFToolsException
				me("Can't create calibrators with zero values for nEta, nPhi or nEnergy!");
		throw me;
	}

	double etaSeg = (etaMax - etaMin) / nEta;
	double phiSeg = (phiMax - phiMin) / nPhi;
	double energySeg = (energyMax - energyMin) / nEnergy;

	double eta1, eta2, phi1, phi2, energy1, energy2;
	for (unsigned k(0); k < nEta; ++k) {
		for (unsigned l(0); l < nPhi; ++l) {
			for (unsigned m(0); m < nEnergy; ++m) {
				eta1 = etaMin + k * etaSeg;
				eta2 = eta1 + etaSeg;
				
				phi1 = phiMin + l * phiSeg;
				phi2 = phi1 + phiSeg;
				
				energy1 = energyMin + m * energySeg;
				energy2 = energy1 + energySeg;
				SpaceVoxelPtr sv(new SpaceVoxel(eta1, eta2, phi1, phi2, energy1, energy2));
				myKnownSpaceVoxels.push_back(sv);
				CalibratorPtr c(toClone.clone());
				myAddressBook[sv] = c;
			}
		}
	}
	unsigned nCalibrators =  nEta * nPhi * nEnergy;
	std::cout << "Created " << nCalibrators << " calibrators.\n";
	std::cout << "Address book size: \t\t" << myAddressBook.size() << "\n";
	std::cout << "Known space voxels size: \t" << myKnownSpaceVoxels.size() << "\n";
	assert(myAddressBook.size() == myKnownSpaceVoxels.size());

}
CalibratorPtr SpaceManager::createCalibrator(const Calibrator& toClone,
		SpaceVoxelPtr s) {
	CalibratorPtr c;
	int known = count(myKnownSpaceVoxels.begin(), myKnownSpaceVoxels.end(), s);
	if (known == 0) {
		myKnownSpaceVoxels.push_back(s);
		c.reset(toClone.clone());
		myAddressBook[s] = c;
	} else {
		c = myAddressBook[s];
	}
	assert(c != 0);
	return c;

}

CalibratorPtr SpaceManager::findCalibrator(const double eta, const double phi,
		const double energy) const {
	CalibratorPtr answer;
	for (std::vector<SpaceVoxelPtr>::const_iterator
			cit = myKnownSpaceVoxels.begin(); cit != myKnownSpaceVoxels.end(); ++cit) {
		SpaceVoxelPtr s = *cit;
		if (s->contains(eta, phi, energy)) {
			assert(count(myKnownSpaceVoxels.begin(), myKnownSpaceVoxels.end(), s) != 0);
			answer = (*myAddressBook.find(s)).second;
			break;
		} else {
			//assert(count(myKnownSpaceVoxels.begin(), myKnownSpaceVoxels.end(), s) == 0);
		}
	}
	return answer;
}

