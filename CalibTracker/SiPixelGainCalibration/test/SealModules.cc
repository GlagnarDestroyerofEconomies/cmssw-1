#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CalibTracker/SiPixelGainCalibration/test/SimpleTestPrintOutPixelCalibAnalyzer.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE(SimpleTestPrintOutPixelCalibAnalyzer);
