
#include "SimCalorimetry/EcalSimProducers/interface/EcalDigiProducer.h"

EcalDigiProducer::EcalDigiProducer(const edm::ParameterSet& params) 
:  theGeometry(0)
{
  produces<EBDigiCollection>();
  produces<EEDigiCollection>();
  produces<ESDigiCollection>();


  // initialize the default valuer for hardcoded parameters and the EB/EE shape

  double simHitToPhotoelectronsBarrel = params.getUntrackedParameter<double>("simHitToPhotoelectronsBarrel", 2250.);
  double simHitToPhotoelectronsEndcap = params.getUntrackedParameter<double>("simHitToPhotoelectronsEndcap", 1800.);
  double photoelectronsToAnalogBarrel = params.getUntrackedParameter<double>("photoelectronsToAnalogBarrel", 1./2250.);
  double photoelectronsToAnalogEndcap = params.getUntrackedParameter<double>("photoelectronsToAnalogEndcap", 1./1800.);
  double samplingFactor = params.getUntrackedParameter<double>("samplingFactor", 1.);
  double timePhase = params.getUntrackedParameter<double>("timePhase", 47.6683);
  int readoutFrameSize = params.getUntrackedParameter<int>("readoutFrameSize", 10);
  int binOfMaximum = params.getUntrackedParameter<int>("binOfMaximum", 5);
  bool doPhotostatistics = params.getUntrackedParameter<bool>("doPhotostatistics", true);
  theParameterMap = new EcalSimParameterMap(simHitToPhotoelectronsBarrel, simHitToPhotoelectronsEndcap, 
                                            photoelectronsToAnalogBarrel, photoelectronsToAnalogEndcap, 
                                            samplingFactor, timePhase, readoutFrameSize, binOfMaximum,
                                            doPhotostatistics);
  theEcalShape = new EcalShape(timePhase);

  int ESGain = params.getUntrackedParameter<int>("ESGain", 1);
  theESShape = new ESShape(ESGain);

  theEcalResponse = new CaloHitResponse(theParameterMap, theEcalShape);
  theESResponse = new CaloHitResponse(theParameterMap, theESShape);
  
  bool addNoise = params.getUntrackedParameter<bool>("doNoise" , true); 
  theCoder = new EcalCoder(addNoise);
  bool applyConstantTerm = params.getUntrackedParameter<bool>("applyConstantTerm", true);
  double rmsConstantTerm = params.getUntrackedParameter<double> ("ConstantTerm", 0.003);
  theElectronicsSim = new EcalElectronicsSim(theParameterMap, theCoder, applyConstantTerm, rmsConstantTerm);

  bool addESNoise = params.getUntrackedParameter<bool>("doESNoise" , true);
  double ESNoiseSigma = params.getUntrackedParameter<double> ("ESNoiseSigma", 3.);
  int ESBaseline = params.getUntrackedParameter<int>("ESBaseline", 1000);
  double ESMIPADC = params.getUntrackedParameter<double>("ESMIPADC", 9.);
  double ESMIPkeV = params.getUntrackedParameter<double>("ESMIPkeV", 78.47);
  theESElectronicsSim = new ESElectronicsSim(addESNoise, ESNoiseSigma, ESGain, ESBaseline, ESMIPADC, ESMIPkeV);

  theBarrelDigitizer = new EBDigitizer(theEcalResponse, theElectronicsSim, addNoise);
  theEndcapDigitizer = new EEDigitizer(theEcalResponse, theElectronicsSim, addNoise);
  theESDigitizer = new ESDigitizer(theESResponse, theESElectronicsSim, addESNoise);

}


EcalDigiProducer::~EcalDigiProducer() 
{
  delete theParameterMap;
  delete theEcalShape;
  delete theESShape;
  delete theEcalResponse;
  delete theESResponse;
  delete theCoder;
}


void EcalDigiProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) 
{

  // Step A: Get Inputs

  checkGeometry(eventSetup);
  checkCalibrations(eventSetup);

  // Get input
  edm::Handle<CrossingFrame> crossingFrame;
  event.getByType(crossingFrame);

  // test access to SimHits
  const std::string barrelHitsName("EcalHitsEB");
  const std::string endcapHitsName("EcalHitsEE");
  const std::string preshowerHitsName("EcalHitsES");

  bool isEB = true;
  MixCollection<PCaloHit> * EBHits = 0 ;
  try {
    EBHits = new MixCollection<PCaloHit>(crossingFrame.product(), barrelHitsName);
  } catch ( cms::Exception &e ) { isEB = false; }
  
  //  std::auto_ptr<MixCollection<PCaloHit> > 
  //    barrelHits( new MixCollection<PCaloHit>(crossingFrame.product(), barrelHitsName) );

  bool isEE = true;
  MixCollection<PCaloHit> * EEHits = 0 ;
  try {
    EEHits = new MixCollection<PCaloHit>(crossingFrame.product(), endcapHitsName);
  } catch ( cms::Exception &e ) { isEE = false; }

  //  std::auto_ptr<MixCollection<PCaloHit> > 
  //    endcapHits( new MixCollection<PCaloHit>(crossingFrame.product(),endcapHitsName) );

  bool isES = true;
  MixCollection<PCaloHit> * ESHits = 0 ;
  try {
    ESHits = new MixCollection<PCaloHit>(crossingFrame.product(), preshowerHitsName);
  } catch ( cms::Exception &e ) { isES = false; }

  //    std::auto_ptr<MixCollection<PCaloHit> >
  //      preshowerHits( new MixCollection<PCaloHit>(crossingFrame.product(), preshowerHitsName) ); 
  
  // Step B: Create empty output
  auto_ptr<EBDigiCollection> barrelResult(new EBDigiCollection());
  auto_ptr<EEDigiCollection> endcapResult(new EEDigiCollection());
  auto_ptr<ESDigiCollection> preshowerResult(new ESDigiCollection());

  // run the algorithm

  CaloDigiCollectionSorter sorter(5);

  if ( isEB ) {
    std::auto_ptr<MixCollection<PCaloHit> >  barrelHits( EBHits );
    theBarrelDigitizer->run(*barrelHits, *barrelResult);
    edm::LogInfo("DigiInfo") << "EB Digis: " << barrelResult->size();

    std::vector<EBDataFrame> sortedDigisEB = sorter.sortedVector(*barrelResult);
    LogDebug("DigiDump") << "Top 10 EB digis";
    for(int i = 0; i < std::min(10,(int) sortedDigisEB.size()); ++i) 
      {
        LogDebug("DigiDump") << sortedDigisEB[i];
      }
  }

  if ( isEE ) {
    std::auto_ptr<MixCollection<PCaloHit> >  endcapHits( EEHits );
    theEndcapDigitizer->run(*endcapHits, *endcapResult);
    edm::LogInfo("DigiInfo") << "EE Digis: " << endcapResult->size();

    std::vector<EEDataFrame> sortedDigisEE = sorter.sortedVector(*endcapResult);
    LogDebug("DigiDump")  << "Top 10 EE digis";
    for(int i = 0; i < std::min(10,(int) sortedDigisEE.size()); ++i) 
      {
        LogDebug("DigiDump") << sortedDigisEE[i];
      }
  }

  if ( isES ) {
    std::auto_ptr<MixCollection<PCaloHit> >  preshowerHits( ESHits );
    theESDigitizer->run(*preshowerHits, *preshowerResult);
    edm::LogInfo("DigiInfo") << "ES Digis: " << preshowerResult->size();
    
//   CaloDigiCollectionSorter sorter_es(7);
//   std::vector<ESDataFrame> sortedDigis_es = sorter_es.sortedVector(*preshowerResult);
//   LogDebug("DigiDump") << "List all ES digis";
//   for(int i = 0; i < sortedDigis_es.size(); ++i) 
//     {
//       LogDebug("DigiDump") << sortedDigis_es[i];
//     }
  }

  // Step D: Put outputs into event
  event.put(barrelResult);
  event.put(endcapResult);
  event.put(preshowerResult);

}



void  EcalDigiProducer::checkCalibrations(const edm::EventSetup & eventSetup) 
{

  // Fake pedestals for initial tests

  setupFakePedestals();

}


void EcalDigiProducer::checkGeometry(const edm::EventSetup & eventSetup) 
{
  // TODO find a way to avoid doing this every event
  edm::ESHandle<CaloGeometry> hGeometry;
  eventSetup.get<IdealGeometryRecord>().get(hGeometry);

  const CaloGeometry * pGeometry = &*hGeometry;
  
  // see if we need to update
  if(pGeometry != theGeometry) {
    theGeometry = pGeometry;
    updateGeometry();
  }
}


void EcalDigiProducer::updateGeometry() {
  theEcalResponse->setGeometry(theGeometry);
  theESResponse->setGeometry(theGeometry);

  theBarrelDets.clear();
  theEndcapDets.clear();
  theESDets.clear();

  theBarrelDets =  theGeometry->getValidDetIds(DetId::Ecal, EcalBarrel);
  theEndcapDets =  theGeometry->getValidDetIds(DetId::Ecal, EcalEndcap);
  theESDets     =  theGeometry->getValidDetIds(DetId::Ecal, EcalPreshower);

  LogInfo("DigiInput") << "deb geometry: " << "\n" 
                       << "\t barrel: " << theBarrelDets.size () << "\n"
                       << "\t endcap: " << theEndcapDets.size () << "\n"
                       << "\t preshower: " << theESDets.size();

  theBarrelDigitizer->setDetIds(theBarrelDets);
  theEndcapDigitizer->setDetIds(theEndcapDets);
  theESDigitizer->setDetIds(theESDets);
}


void EcalDigiProducer::setupFakePedestals() 
{
  thePedestals.m_pedestals.clear();
  // make pedestals for each of these
  EcalPedestals::Item item;
  item.mean_x1 = 201.0;
  item.rms_x1 = 0.62;
  item.mean_x6 = 199.4;
  item.rms_x6 = 0.9;
  item.mean_x12 = 198.8;
  item.rms_x12 = 1.10;

  // make one vector of all det ids
  vector<DetId> detIds(theBarrelDets.begin(), theBarrelDets.end());
  detIds.insert(detIds.end(), theEndcapDets.begin(), theEndcapDets.end());

  // make a pedestal entry for each one 
  for(std::vector<DetId>::const_iterator detItr = detIds.begin();
       detItr != detIds.end(); ++detItr)
  {
    thePedestals.m_pedestals[detItr->rawId()] = item;
  }

  theCoder->setPedestals(&thePedestals);
}

