#include "DataFormats/V0Candidate/interface/V0Candidate.h"

using namespace reco;

void V0Candidate::setVertex( const Vertex & vtxIn ) {
  recoVertex = vtxIn;
  Particle::setVertex( vtxIn.position() );
}
