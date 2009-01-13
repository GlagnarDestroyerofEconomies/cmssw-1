
#include "CSCGeometryESModule.h"
#include "Geometry/CSCGeometryBuilder/src/CSCGeometryBuilderFromDDD.h"
#include "Geometry/CSCGeometryBuilder/src/CSCGeometryBuilder.h"
#include "Geometry/CSCGeometry/interface/CSCChamberSpecs.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/MuonNumberingRecord.h"

// #include "CondFormats/DataRecord/interface/RecoIdealGeometryRcd.h"
// #include "CondFormats/DataRecord/interface/CSCRecoDigiParametersRcd.h"
#include "Geometry/Records/interface/RecoIdealGeometryRcd.h"
#include "Geometry/Records/interface/CSCRecoDigiParametersRcd.h"
#include "CondFormats/RecoGeometryObjects/interface/RecoIdealGeometry.h"
#include "CondFormats/RecoGeometryObjects/interface/CSCRecoDigiParameters.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"

// Alignments
#include "CondFormats/Alignment/interface/DetectorGlobalPosition.h"
#include "CondFormats/Alignment/interface/AlignmentErrors.h"
#include "CondFormats/AlignmentRecord/interface/GlobalPositionRcd.h"
#include "CondFormats/AlignmentRecord/interface/CSCAlignmentRcd.h"
#include "CondFormats/AlignmentRecord/interface/CSCAlignmentErrorRcd.h"
#include "Geometry/TrackingGeometryAligner/interface/GeometryAligner.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"

#include <memory>

using namespace edm;

CSCGeometryESModule::CSCGeometryESModule(const edm::ParameterSet & p)
  : alignmentsLabel_(p.getParameter<std::string>("alignmentsLabel")),
    myLabel_(p.getParameter<std::string>("appendToDataLabel"))
{

  setWhatProduced(this, dependsOn(&CSCGeometryESModule::geometryCallback_) );

  // Choose wire geometry modelling
  // We now _require_ some wire geometry specification in the CSCOrcaSpec.xml file
  // in the DDD Geometry.
  // Default as of transition to CMSSW is to use real values.
  // Alternative is to use pseudo-values which match reasonably closely
  // the calculated geometry values used up to and including ORCA_8_8_1.
  // (This was the default in ORCA.)

  useRealWireGeometry =   p.getParameter<bool>("useRealWireGeometry");

  // Suppress strips altogether in ME1a region of ME11?

  useOnlyWiresInME1a =    p.getParameter<bool>("useOnlyWiresInME1a");

  // Allow strips in ME1a region of ME11 but gang them?
  // Default is now to treat ME1a with ganged strips (e.g. in clusterizer)

  useGangedStripsInME1a = p.getParameter<bool>("useGangedStripsInME1a");

  if ( useGangedStripsInME1a ) useOnlyWiresInME1a = false; // override possible inconsistentcy

  // Use the backed-out offsets that correct the CTI
  useCentreTIOffsets = p.getParameter<bool>("useCentreTIOffsets"); 

  // Debug printout etc. in CSCGeometry etc.

  debugV = p.getUntrackedParameter<bool>("debugV", false);

  // Find out if using the DDD or CondDB Geometry source.
  useDDD_ = p.getParameter<bool>("useDDD");

  // Feed these value to where I need them
  applyAlignment_ = p.getParameter<bool>("applyAlignment");


  edm::LogInfo("Geometry") << "@SUB=CSCGeometryESModule" 
			   << "Label '" << myLabel_ << "' "
			   << (applyAlignment_ ? "looking for" : "IGNORING")
			   << " alignment labels '" << alignmentsLabel_ << "'.";
}


CSCGeometryESModule::~CSCGeometryESModule(){}


boost::shared_ptr<CSCGeometry> CSCGeometryESModule::produce(const MuonGeometryRecord& record) {

  // Called whenever the alignments or alignment errors change

  if ( applyAlignment_ ) {
    // applyAlignment_ is scheduled for removal. 
    // Ideal geometry obtained by using 'fake alignment' (with applyAlignment_ = true)
    edm::ESHandle<Alignments> globalPosition;
    record.getRecord<GlobalPositionRcd>().get(alignmentsLabel_, globalPosition);
    edm::ESHandle<Alignments> alignments;
    record.getRecord<CSCAlignmentRcd>().get(alignmentsLabel_, alignments);
    edm::ESHandle<AlignmentErrors> alignmentErrors;
// <<<<<<< CSCGeometryESModule.cc
//     record.getRecord<CSCAlignmentErrorRcd>().get( alignmentErrors );
//     GeometryAligner aligner;
//     aligner.applyAlignments<CSCGeometry>( &(*_cscGeometry),
// 					  &(*alignments), &(*alignmentErrors),
// 	 align::DetectorGlobalPosition(*globalPositionRcd, DetId(DetId::Muon)));
// =======
    record.getRecord<CSCAlignmentErrorRcd>().get(alignmentsLabel_,  alignmentErrors);
    // Only apply alignment if values exist
    if (alignments->empty() && alignmentErrors->empty() && globalPosition->empty()) {
      edm::LogInfo("Config") << "@SUB=CSCGeometryRecord::produce"
			     << "Alignment(Error)s and global position (label '"
			     << alignmentsLabel_ << "') empty: Geometry producer (label "
			     << "'" << myLabel_ << "') assumes fake and does not apply.";
    } else {
      GeometryAligner aligner;
      aligner.applyAlignments<CSCGeometry>( &(*cscGeometry), &(*alignments), &(*alignmentErrors),
	                    align::DetectorGlobalPosition(*globalPosition, DetId(DetId::Muon)) );
    }
// >>>>>>> 1.8
  }

  return cscGeometry;
}


void CSCGeometryESModule::geometryCallback_( const MuonNumberingRecord& record )
{
  // Called whenever the muon numbering (or ideal geometry) changes

  cscGeometry = boost::shared_ptr<CSCGeometry>( new CSCGeometry );

  cscGeometry->setUseRealWireGeometry( useRealWireGeometry );
  cscGeometry->setOnlyWiresInME1a( useOnlyWiresInME1a );
  cscGeometry->setGangedStripsInME1a( useGangedStripsInME1a );
  cscGeometry->setUseCentreTIOffsets( useCentreTIOffsets );
  cscGeometry->setDebugV( debugV );
  if ( debugV ) cscGeometry->queryModelling();

  // Called whenever the muon numbering (or ideal geometry) changes
  //
  if ( useDDD_ ) {
    edm::ESHandle<DDCompactView> cpv;
    edm::ESHandle<MuonDDDConstants> mdc;
    record.getRecord<IdealGeometryRecord>().get(cpv);
    record.get( mdc );
    CSCGeometryBuilderFromDDD builder;
    //    _cscGeometry = boost::shared_ptr<CSCGeometry>(builder.build(_cscGeometry, &(*cpv), *mdc));
    builder.build(cscGeometry, &(*cpv), *mdc);
  } else {
    edm::ESHandle<RecoIdealGeometry> rig;
    edm::ESHandle<CSCRecoDigiParameters> rdp;
    record.getRecord<RecoIdealGeometryRcd>().get(rig);
    record.getRecord<CSCRecoDigiParametersRcd>().get(rdp);
    CSCGeometryBuilder cscgb;
    //    _cscGeometry = boost::shared_ptr<CSCGeometry>(cscgb.build(_cscGeometry, *rig, *rdp));
    cscgb.build(cscGeometry, *rig, *rdp);
  }

}

DEFINE_FWK_EVENTSETUP_MODULE(CSCGeometryESModule);
