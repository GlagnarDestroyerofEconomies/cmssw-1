#include "Validation/GlobalDigis/interface/GlobalDigisProducer.h"

GlobalDigisProducer::GlobalDigisProducer(const edm::ParameterSet& iPSet) :
  fName(""), verbosity(0), frequency(0), label(""), getAllProvenances(false),
  printProvenanceInfo(false), count(0)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_GlobalDigisProducer";

  // get information from parameter set
  fName = iPSet.getUntrackedParameter<std::string>("Name");
  verbosity = iPSet.getUntrackedParameter<int>("Verbosity");
  frequency = iPSet.getUntrackedParameter<int>("Frequency");
  label = iPSet.getParameter<std::string>("Label");
  edm::ParameterSet m_Prov =
    iPSet.getParameter<edm::ParameterSet>("ProvenanceLookup");
  getAllProvenances = 
    m_Prov.getUntrackedParameter<bool>("GetAllProvenances");
  printProvenanceInfo = 
    m_Prov.getUntrackedParameter<bool>("PrintProvenanceInfo");

  //get Labels to use to extract information
  ECalEBSrc_ = iPSet.getParameter<edm::InputTag>("ECalEBSrc");
  ECalEESrc_ = iPSet.getParameter<edm::InputTag>("ECalEESrc");
  ECalESSrc_ = iPSet.getParameter<edm::InputTag>("ECalESSrc");
  HCalSrc_ = iPSet.getParameter<edm::InputTag>("HCalSrc");
  SiStripSrc_ = iPSet.getParameter<edm::InputTag>("SiStripSrc");  

  // use value of first digit to determine default output level (inclusive)
  // 0 is none, 1 is basic, 2 is fill output, 3 is gather output
  verbosity %= 10;

  // create persistent object
  produces<PGlobalDigi>(label);

  // print out Parameter Set information being used
  if (verbosity >= 0) {
    edm::LogInfo(MsgLoggerCat) 
      << "\n===============================\n"
      << "Initialized as EDProducer with parameter values:\n"
      << "    Name          = " << fName << "\n"
      << "    Verbosity     = " << verbosity << "\n"
      << "    Frequency     = " << frequency << "\n"
      << "    Label         = " << label << "\n"
      << "    GetProv       = " << getAllProvenances << "\n"
      << "    PrintProv     = " << printProvenanceInfo << "\n"
      << "    ECalEBSrc     = " << ECalEBSrc_.label() 
      << ":" << ECalEBSrc_.instance() << "\n"
      << "    ECalEESrc     = " << ECalEESrc_.label() 
      << ":" << ECalEESrc_.instance() << "\n"
      << "    ECalESSrc     = " << ECalESSrc_.label() 
      << ":" << ECalESSrc_.instance() << "\n"
      << "    HCalSrc       = " << HCalSrc_.label() 
      << ":" << HCalSrc_.instance() << "\n"
      << "    SiStripSrc    = " << SiStripSrc_.label() 
      << ":" << SiStripSrc_.instance() << "\n"      
      << "===============================\n";
  }

  // set default constants
  // ECal
  ECalgainConv_[0] = 0.;
  ECalgainConv_[1] = 1.;
  ECalgainConv_[2] = 2.;
  ECalgainConv_[3] = 12.;  
  ECalbarrelADCtoGeV_ = 0.035;
  ECalendcapADCtoGeV_ = 0.06;

}

GlobalDigisProducer::~GlobalDigisProducer() 
{
}

void GlobalDigisProducer::beginJob(const edm::EventSetup& iSetup)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_beginJob";

  // setup calorimeter constants from service
  edm::ESHandle<EcalADCToGeVConstant> pAgc;
  iSetup.get<EcalADCToGeVConstantRcd>().get(pAgc);
  const EcalADCToGeVConstant* agc = pAgc.product();
  
  EcalMGPAGainRatio * defaultRatios = new EcalMGPAGainRatio();

  ECalgainConv_[0] = 0.;
  ECalgainConv_[1] = 1.;
  ECalgainConv_[2] = defaultRatios->gain12Over6() ;
  ECalgainConv_[3] = ECalgainConv_[2]*(defaultRatios->gain6Over1()) ;

  delete defaultRatios;

  ECalbarrelADCtoGeV_ = agc->getEBValue();
  ECalendcapADCtoGeV_ = agc->getEEValue();

  if (verbosity >= 0) {
    edm::LogInfo(MsgLoggerCat) 
      << "Modified Calorimeter gain constants: g0 = " << ECalgainConv_[0]
      << ", g1 = " << ECalgainConv_[1] << ", g2 = " << ECalgainConv_[2]
      << ", g3 = " << ECalgainConv_[3];
    edm::LogInfo(MsgLoggerCat)
      << "Modified Calorimeter ADCtoGeV constants: barrel = " 
      << ECalbarrelADCtoGeV_ << ", endcap = " << ECalendcapADCtoGeV_;
  }

  // clear storage vectors
  clear();
  return;
}

void GlobalDigisProducer::endJob()
{
  std::string MsgLoggerCat = "GlobalDigisProducer_endJob";
  if (verbosity >= 0)
    edm::LogInfo(MsgLoggerCat) 
      << "Terminating having processed " << count << " events.";
  return;
}

void GlobalDigisProducer::produce(edm::Event& iEvent, 
				  const edm::EventSetup& iSetup)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_produce";

  // keep track of number of events processed
  ++count;

  // get event id information
  int nrun = iEvent.id().run();
  int nevt = iEvent.id().event();

  if (verbosity > 0) {
    edm::LogInfo(MsgLoggerCat)
      << "Processing run " << nrun << ", event " << nevt
      << " (" << count << " events total)";
  } else if (verbosity == 0) {
    if (nevt%frequency == 0 || nevt == 1) {
      edm::LogInfo(MsgLoggerCat)
	<< "Processing run " << nrun << ", event " << nevt
	<< " (" << count << " events total)";
    }
  }

  // clear event holders
  clear();

  // look at information available in the event
  if (getAllProvenances) {

    std::vector<const edm::Provenance*> AllProv;
    iEvent.getAllProvenance(AllProv);

    if (verbosity >= 0)
      edm::LogInfo(MsgLoggerCat)
	<< "Number of Provenances = " << AllProv.size();

    if (printProvenanceInfo && (verbosity >= 0)) {
      TString eventout("\nProvenance info:\n");      

      for (unsigned int i = 0; i < AllProv.size(); ++i) {
	eventout += "\n       ******************************";
	eventout += "\n       Module       : ";
	eventout += AllProv[i]->moduleLabel();
	eventout += "\n       ProductID    : ";
	eventout += AllProv[i]->productID().id();
	eventout += "\n       ClassName    : ";
	eventout += AllProv[i]->className();
	eventout += "\n       InstanceName : ";
	eventout += AllProv[i]->productInstanceName();
	eventout += "\n       BranchName   : ";
	eventout += AllProv[i]->branchName();
      }
      eventout += "\n       ******************************\n";
      edm::LogInfo(MsgLoggerCat) << eventout << "\n";
      printProvenanceInfo = false;
    }
    getAllProvenances = false;
  }

  // call fill functions
  // gather Ecal information from event
  fillECal(iEvent, iSetup);
  // gather Hcal information from event
  fillHCal(iEvent, iSetup);
  // gather Track information from event
  fillTrk(iEvent, iSetup);

  if (verbosity > 0)
    edm::LogInfo (MsgLoggerCat)
      << "Done gathering data from event.";

  // produce object to put into event
  std::auto_ptr<PGlobalDigi> pOut(new PGlobalDigi);

  if (verbosity > 2)
    edm::LogInfo (MsgLoggerCat)
      << "Saving event contents:";

  // call store functions
  // store ECal information in produce
  storeECal(*pOut);
  // store HCal information in produce
  storeHCal(*pOut);
  // store Track information in produce
  storeTrk(*pOut);

  // store information in event
  iEvent.put(pOut,label);

  return;
}

void GlobalDigisProducer::fillECal(edm::Event& iEvent, 
				   const edm::EventSetup& iSetup)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_fillECal";

  TString eventout;
  if (verbosity > 0)
    eventout = "\nGathering info:";  

  // extract crossing frame from event
  edm::Handle<CrossingFrame> crossingFrame;
  iEvent.getByType(crossingFrame);
  if (!crossingFrame.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find crossingFrame in event!";
    return;
  }

  ////////////////////////
  //extract EB information
  ////////////////////////
  bool isBarrel = true;
  edm::Handle<EBDigiCollection> EcalDigiEB;  
  const EBDigiCollection *EBdigis = 0;
  iEvent.getByLabel(ECalEBSrc_, EcalDigiEB);
  if (!EcalDigiEB.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find EcalDigiEB in event!";
    return;
  }  
  EBdigis = EcalDigiEB.product();
  if (EBdigis->size() == 0) isBarrel = false;

  if (isBarrel) {

    // loop over simhits
    const std::string barrelHitsName("EcalHitsEB");
    std::auto_ptr<MixCollection<PCaloHit> >
      barrelHits(new MixCollection<PCaloHit>
		 (crossingFrame.product(), barrelHitsName));

    // keep track of sum of simhit energy in each crystal
    MapType ebSimMap;
    for (MixCollection<PCaloHit>::MixItr hitItr 
	   = barrelHits->begin();
	 hitItr != barrelHits->end();
	 ++hitItr) {

      EBDetId ebid = EBDetId(hitItr->id());

      uint32_t crystid = ebid.rawId();
      ebSimMap[crystid] += hitItr->energy();
    }

    // loop over digis
    const EBDigiCollection *barrelDigi = EcalDigiEB.product();

    std::vector<double> ebAnalogSignal;
    std::vector<double> ebADCCounts;
    std::vector<double> ebADCGains;
    ebAnalogSignal.reserve(EBDataFrame::MAXSAMPLES);
    ebADCCounts.reserve(EBDataFrame::MAXSAMPLES);
    ebADCGains.reserve(EBDataFrame::MAXSAMPLES);

    int i = 0;
    for (std::vector<EBDataFrame>::const_iterator digis =
	   barrelDigi->begin();
	 digis != barrelDigi->end();
	 ++digis) {

      ++i;

      EBDetId ebid = digis->id();

      double Emax = 0;
      int Pmax = 0;
      double pedestalPreSample = 0.;
      double pedestalPreSampleAnalog = 0.;
        
      for (int sample = 0; sample < digis->size(); ++sample) {
	ebAnalogSignal[sample] = 0.;
	ebADCCounts[sample] = 0.;
	ebADCGains[sample] = -1.;
      }
  
      // calculate maximum energy and pedestal
      for (int sample = 0; sample < digis->size(); ++sample) {
	ebADCCounts[sample] = (digis->sample(sample).adc());
	ebADCGains[sample] = (digis->sample(sample).gainId());
	ebAnalogSignal[sample] = 
	  (ebADCCounts[sample] * ECalgainConv_[(int)ebADCGains[sample]]
	   * ECalbarrelADCtoGeV_);
	if (Emax < ebAnalogSignal[sample]) {
	  Emax = ebAnalogSignal[sample];
	  Pmax = sample;
	}
	if ( sample < 3 ) {
	  pedestalPreSample += ebADCCounts[sample] ;
	  pedestalPreSampleAnalog += 
	    ebADCCounts[sample] * ECalgainConv_[(int)ebADCGains[sample]]
	    * ECalbarrelADCtoGeV_ ;
	}
	
      }
      pedestalPreSample /= 3. ; 
      pedestalPreSampleAnalog /= 3. ; 

      // calculate pedestal subtracted digi energy in the crystal
      double Erec = Emax - pedestalPreSampleAnalog
	* ECalgainConv_[(int)ebADCGains[Pmax]];

      // gather necessary information
      EBCalAEE.push_back(Erec);
      EBCalSHE.push_back(ebSimMap[ebid.rawId()]);
      EBCalmaxPos.push_back(Pmax);
    }
    
    if (verbosity > 1) {
      eventout += "\n          Number of EBDigis collected:.............. ";
      eventout += i;
    }
  }

  /////////////////////////
  //extract EE information
  ////////////////////////
  bool isEndCap = true;
  edm::Handle<EEDigiCollection> EcalDigiEE;  
  const EEDigiCollection *EEdigis = 0;
  iEvent.getByLabel(ECalEESrc_, EcalDigiEE);
  if (!EcalDigiEE.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find EcalDigiEE in event!";
    return;
  }  
  EEdigis = EcalDigiEE.product();
  if (EEdigis->size() == 0) isEndCap = false;

  if (isEndCap) {

    // loop over simhits
    const std::string endcapHitsName("EcalHitsEE");
    std::auto_ptr<MixCollection<PCaloHit> >
      endcapHits(new MixCollection<PCaloHit>
		 (crossingFrame.product(), endcapHitsName));

    // keep track of sum of simhit energy in each crystal
    MapType eeSimMap;
    for (MixCollection<PCaloHit>::MixItr hitItr 
	   = endcapHits->begin();
	 hitItr != endcapHits->end();
	 ++hitItr) {

      EEDetId eeid = EEDetId(hitItr->id());

      uint32_t crystid = eeid.rawId();
      eeSimMap[crystid] += hitItr->energy();
    }

    // loop over digis
    const EEDigiCollection *endcapDigi = EcalDigiEE.product();

    std::vector<double> eeAnalogSignal;
    std::vector<double> eeADCCounts;
    std::vector<double> eeADCGains;
    eeAnalogSignal.reserve(EEDataFrame::MAXSAMPLES);
    eeADCCounts.reserve(EEDataFrame::MAXSAMPLES);
    eeADCGains.reserve(EEDataFrame::MAXSAMPLES);

    int i = 0;
    for (std::vector<EEDataFrame>::const_iterator digis =
	   endcapDigi->begin();
	 digis != endcapDigi->end();
	 ++digis) {

      ++i;

      EEDetId eeid = digis->id();

      double Emax = 0;
      int Pmax = 0;
      double pedestalPreSample = 0.;
      double pedestalPreSampleAnalog = 0.;
        
      for (int sample = 0; sample < digis->size(); ++sample) {
	eeAnalogSignal[sample] = 0.;
	eeADCCounts[sample] = 0.;
	eeADCGains[sample] = -1.;
      }
  
      // calculate maximum enery and pedestal
      for (int sample = 0; sample < digis->size(); ++sample) {
	eeADCCounts[sample] = (digis->sample(sample).adc());
	eeADCGains[sample] = (digis->sample(sample).gainId());
	eeAnalogSignal[sample] = 
	  (eeADCCounts[sample] * ECalgainConv_[(int)eeADCGains[sample]]
	   * ECalbarrelADCtoGeV_);
	if (Emax < eeAnalogSignal[sample]) {
	  Emax = eeAnalogSignal[sample];
	  Pmax = sample;
	}
	if ( sample < 3 ) {
	  pedestalPreSample += eeADCCounts[sample] ;
	  pedestalPreSampleAnalog += 
	    eeADCCounts[sample] * ECalgainConv_[(int)eeADCGains[sample]]
	    * ECalbarrelADCtoGeV_ ;
	}
	
      }
      pedestalPreSample /= 3. ; 
      pedestalPreSampleAnalog /= 3. ; 

      // calculate pedestal subtracted digi energy in the crystal
      double Erec = Emax - pedestalPreSampleAnalog
	* ECalgainConv_[(int)eeADCGains[Pmax]];

      // gather necessary information
      EECalAEE.push_back(Erec);
      EECalSHE.push_back(eeSimMap[eeid.rawId()]);
      EECalmaxPos.push_back(Pmax);
    }
    
    if (verbosity > 1) {
      eventout += "\n          Number of EEDigis collected:.............. ";
      eventout += i;
    }
  }

  /////////////////////////
  //extract ES information
  ////////////////////////
  bool isPreshower = true;
  edm::Handle<ESDigiCollection> EcalDigiES;  
  const ESDigiCollection *ESdigis = 0;
  iEvent.getByLabel(ECalESSrc_, EcalDigiES);
  if (!EcalDigiES.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find EcalDigiES in event!";
    return;
  }  
  ESdigis = EcalDigiES.product();
  if (ESdigis->size() == 0) isPreshower = false;

  if (isPreshower) {

    // loop over simhits
    const std::string preshowerHitsName("EcalHitsES");
    std::auto_ptr<MixCollection<PCaloHit> >
      preshowerHits(new MixCollection<PCaloHit>
		 (crossingFrame.product(), preshowerHitsName));

    // keep track of sum of simhit energy in each crystal
    MapType esSimMap;
    for (MixCollection<PCaloHit>::MixItr hitItr 
	   = preshowerHits->begin();
	 hitItr != preshowerHits->end();
	 ++hitItr) {

      ESDetId esid = ESDetId(hitItr->id());

      uint32_t crystid = esid.rawId();
      esSimMap[crystid] += hitItr->energy();
    }

    // loop over digis
    const ESDigiCollection *preshowerDigi = EcalDigiES.product();

    std::vector<double> esADCCounts;
    esADCCounts.reserve(ESDataFrame::MAXSAMPLES);

    int i = 0;
    for (std::vector<ESDataFrame>::const_iterator digis =
	   preshowerDigi->begin();
	 digis != preshowerDigi->end();
	 ++digis) {

      ++i;

      ESDetId esid = digis->id();
        
      for (int sample = 0; sample < digis->size(); ++sample) {
	esADCCounts[sample] = 0.;
      }
  
      // gether ADC counts
      for (int sample = 0; sample < digis->size(); ++sample) {
	esADCCounts[sample] = (digis->sample(sample).adc());
      }

      ESCalADC0.push_back(esADCCounts[0]);
      ESCalADC1.push_back(esADCCounts[1]);
      ESCalADC2.push_back(esADCCounts[2]);
      ESCalSHE.push_back(esSimMap[esid.rawId()]);
    }
    
    if (verbosity > 1) {
      eventout += "\n          Number of ESDigis collected:.............. ";
      eventout += i;
    }
  }

  if (verbosity > 0)
    edm::LogInfo(MsgLoggerCat) << eventout << "\n";

  return;
}

void GlobalDigisProducer::storeECal(PGlobalDigi& product)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_storeECal";

  if (verbosity > 2) {
    TString eventout("\n         nEBDigis     = ");
    eventout += EBCalmaxPos.size();
    for (unsigned int i = 0; i < EBCalmaxPos.size(); ++i) {
      eventout += "\n      (maxPos, AEE, SHE) = (";
      eventout += EBCalmaxPos[i];
      eventout += ", ";
      eventout += EBCalAEE[i];
      eventout += ", ";
      eventout += EBCalSHE[i];
      eventout += ")";
    }
    eventout += "\n         nEEDigis     = ";
    eventout += EECalmaxPos.size();
    for (unsigned int i = 0; i < EECalmaxPos.size(); ++i) {
      eventout += "\n      (maxPos, AEE, SHE) = (";
      eventout += EECalmaxPos[i];
      eventout += ", ";
      eventout += EECalAEE[i];
      eventout += ", ";
      eventout += EECalSHE[i];
      eventout += ")";
    }
    eventout += "\n         nESDigis          = ";
    eventout += ESCalADC0.size();
    for (unsigned int i = 0; i < ESCalADC0.size(); ++i) {
      eventout += "\n      (ADC0, ADC1, ADC2, SHE) = (";
      eventout += ESCalADC0[i];
      eventout += ", ";
      eventout += ESCalADC1[i];
      eventout += ", ";
      eventout += ESCalADC2[i];
      eventout += ", ";
      eventout += ESCalSHE[i];
      eventout += ")";
    }
    edm::LogInfo(MsgLoggerCat) << eventout << "\n";
  }

  product.putEBCalDigis(EBCalmaxPos,EBCalAEE,EBCalSHE);
  product.putEECalDigis(EECalmaxPos,EECalAEE,EECalSHE);
  product.putESCalDigis(ESCalADC0,ESCalADC1,ESCalADC2,ESCalSHE);

  return;
}

void GlobalDigisProducer::fillHCal(edm::Event& iEvent, 
				   const edm::EventSetup& iSetup)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_fillHCal";

  TString eventout;
  if (verbosity > 0)
    eventout = "\nGathering info:";  

  // get calibration info
  edm::ESHandle<HcalDbService> HCalconditions;
  iSetup.get<HcalDbRecord>().get(HCalconditions);
  if (!HCalconditions.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find HCalconditions in event!";
    return;
  } 
  const HcalQIEShape *shape = HCalconditions->getHcalShape();
  HcalCalibrations calibrations;
  CaloSamples tool;

  ///////////////////////
  // extract simhit info
  //////////////////////
  edm::Handle<edm::PCaloHitContainer> hcalHits;
  iEvent.getByLabel(HCalSrc_,hcalHits);
  if (!hcalHits.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find hcalHits in event!";
    return;
  }  
  const edm::PCaloHitContainer *simhitResult = hcalHits.product();
  
  MapType fHBEnergySimHits;
  MapType fHEEnergySimHits;
  MapType fHOEnergySimHits;
  MapType fHFEnergySimHits;
  for (std::vector<PCaloHit>::const_iterator simhits = simhitResult->begin();
       simhits != simhitResult->end();
       ++simhits) {
    
    HcalDetId detId(simhits->id());
    uint32_t cellid = detId.rawId();

    if (detId.subdet() == sdHcalBrl){  
      fHBEnergySimHits[cellid] += simhits->energy(); 
    }
    if (detId.subdet() == sdHcalEC){  
      fHEEnergySimHits[cellid] += simhits->energy(); 
    }    
    if (detId.subdet() == sdHcalOut){  
      fHOEnergySimHits[cellid] += simhits->energy(); 
    }    
    if (detId.subdet() == sdHcalFwd){  
      fHFEnergySimHits[cellid] += simhits->energy(); 
    }    
  }

  ////////////////////////
  // get HBHE information
  ///////////////////////
  edm::Handle<edm::SortedCollection<HBHEDataFrame> > hbhe;
  iEvent.getByType(hbhe);
  if (!hbhe.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find HBHEDataFrame in event!";
    return;
  }    
  edm::SortedCollection<HBHEDataFrame>::const_iterator ihbhe;
  
  int iHB = 0;
  int iHE = 0; 
  for (ihbhe = hbhe->begin(); ihbhe != hbhe->end(); ++ihbhe) {
    HcalDetId cell(ihbhe->id()); 

    if ((cell.subdet() == sdHcalBrl) || (cell.subdet() == sdHcalEC)) {
      
      HCalconditions->makeHcalCalibration(cell, &calibrations);
      const HcalQIECoder *channelCoder = HCalconditions->getHcalCoder(cell);
      HcalCoderDb coder(*channelCoder, *shape);
      coder.adc2fC(*ihbhe, tool);
      
      // get HB info
      if (cell.subdet() == sdHcalBrl) {

	++iHB;
	float fDigiSum = 0;
	for  (int ii = 0; ii < tool.size(); ++ii) {
	  // default ped is 4.5
	  int capid = (*ihbhe)[ii].capid();
	  fDigiSum += (tool[ii] - calibrations.pedestal(capid));
	}
	
	HBCalAEE.push_back(fDigiSum);
	HBCalSHE.push_back(fHBEnergySimHits[cell.rawId()]);
      }
	
      // get HE info
      if (cell.subdet() == sdHcalEC) {
	
	++iHE;
	float fDigiSum = 0;
	for  (int ii = 0; ii < tool.size(); ++ii) {
	  int capid = (*ihbhe)[ii].capid();
	  fDigiSum += (tool[ii]-calibrations.pedestal(capid));
	}
	
	HECalAEE.push_back(fDigiSum);
	HECalSHE.push_back(fHEEnergySimHits[cell.rawId()]);
      }
    }
  }

  if (verbosity > 1) {
    eventout += "\n          Number of HBDigis collected:.............. ";
    eventout += iHB;
  }
  
  if (verbosity > 1) {
    eventout += "\n          Number of HEDigis collected:.............. ";
    eventout += iHE;
  }

  ////////////////////////
  // get HO information
  ///////////////////////
  edm::Handle<edm::SortedCollection<HODataFrame> > ho;
  iEvent.getByType(ho);
  if (!ho.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find HODataFrame in event!";
    return;
  }    
  edm::SortedCollection<HODataFrame>::const_iterator iho;
  
  int iHO = 0; 
  for (iho = ho->begin(); iho != ho->end(); ++iho) {
    HcalDetId cell(iho->id()); 

    if (cell.subdet() == sdHcalOut) {
      
      HCalconditions->makeHcalCalibration(cell, &calibrations);
      const HcalQIECoder *channelCoder = HCalconditions->getHcalCoder(cell);
      HcalCoderDb coder (*channelCoder, *shape);
      coder.adc2fC(*iho, tool);

      ++iHO;
      float fDigiSum = 0;
      for  (int ii = 0; ii < tool.size(); ++ii) {
	// default ped is 4.5
	int capid = (*iho)[ii].capid();
	fDigiSum += (tool[ii] - calibrations.pedestal(capid));
      }
	
      HOCalAEE.push_back(fDigiSum);
      HOCalSHE.push_back(fHOEnergySimHits[cell.rawId()]);
    }
  }

  if (verbosity > 1) {
    eventout += "\n          Number of HODigis collected:.............. ";
    eventout += iHO;
  }

  ////////////////////////
  // get HF information
  ///////////////////////
  edm::Handle<edm::SortedCollection<HFDataFrame> > hf;
  iEvent.getByType(hf);
  if (!hf.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find HFDataFrame in event!";
    return;
  }    
  edm::SortedCollection<HFDataFrame>::const_iterator ihf;
  
  int iHF = 0; 
  for (ihf = hf->begin(); ihf != hf->end(); ++ihf) {
    HcalDetId cell(ihf->id()); 

    if (cell.subdet() == sdHcalFwd) {
      
      HCalconditions->makeHcalCalibration(cell, &calibrations);
      const HcalQIECoder *channelCoder = HCalconditions->getHcalCoder(cell);
      HcalCoderDb coder (*channelCoder, *shape);
      coder.adc2fC(*ihf, tool);

      ++iHF;
      float fDigiSum = 0;
      for  (int ii = 0; ii < tool.size(); ++ii) {
	// default ped is 1.73077
	int capid = (*ihf)[ii].capid();
	fDigiSum += (tool[ii] - calibrations.pedestal(capid));
      }
	
      HFCalAEE.push_back(fDigiSum);
      HFCalSHE.push_back(fHOEnergySimHits[cell.rawId()]);
    }
  }

  if (verbosity > 1) {
    eventout += "\n          Number of HFDigis collected:.............. ";
    eventout += iHF;
  }

  if (verbosity > 0)
    edm::LogInfo(MsgLoggerCat) << eventout << "\n";

  return;
}

void GlobalDigisProducer::storeHCal(PGlobalDigi& product)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_storeHCal";

  if (verbosity > 2) {
    TString eventout("\n         nHBDigis     = ");
    eventout += HBCalAEE.size();
    for (unsigned int i = 0; i < HBCalAEE.size(); ++i) {
      eventout += "\n      (AEE, SHE) = (";
      eventout += HBCalAEE[i];
      eventout += ", ";
      eventout += HBCalSHE[i];
      eventout += ")";
    }
    eventout += "\n         nHEDigis     = ";
    eventout += HECalAEE.size();
    for (unsigned int i = 0; i < HECalAEE.size(); ++i) {
      eventout += "\n      (AEE, SHE) = (";
      eventout += HECalAEE[i];
      eventout += ", ";
      eventout += HECalSHE[i];
      eventout += ")";
    }
    eventout += "\n         nHODigis     = ";
    eventout += HOCalAEE.size();
    for (unsigned int i = 0; i < HOCalAEE.size(); ++i) {
      eventout += "\n      (AEE, SHE) = (";
      eventout += HOCalAEE[i];
      eventout += ", ";
      eventout += HOCalSHE[i];
      eventout += ")";
    }
    eventout += "\n         nHFDigis     = ";
    eventout += HFCalAEE.size();
    for (unsigned int i = 0; i < HFCalAEE.size(); ++i) {
      eventout += "\n      (AEE, SHE) = (";
      eventout += HFCalAEE[i];
      eventout += ", ";
      eventout += HFCalSHE[i];
      eventout += ")";
    }

    edm::LogInfo(MsgLoggerCat) << eventout << "\n";
  }

  product.putHBCalDigis(HBCalAEE,HBCalSHE);
  product.putHECalDigis(HECalAEE,HECalSHE);
  product.putHOCalDigis(HOCalAEE,HOCalSHE);
  product.putHFCalDigis(HFCalAEE,HFCalSHE);

  return;
}

void GlobalDigisProducer::fillTrk(edm::Event& iEvent, 
				   const edm::EventSetup& iSetup)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_fillTrk";

  TString eventout;
  if (verbosity > 0)
    eventout = "\nGathering info:";  

  // get strip information
  edm::Handle<edm::DetSetVector<SiStripDigi> > stripDigis;  
  iEvent.getByLabel(SiStripSrc_, stripDigis);
  if (!stripDigis.isValid()) {
    edm::LogWarning(MsgLoggerCat)
      << "Unable to find stripDigis in event!";
    return;
  }  

  int nBrl = 0, nFwd = 0;
  edm::DetSetVector<SiStripDigi>::const_iterator DSViter;
  for (DSViter = stripDigis->begin(); DSViter != stripDigis->end(); 
       ++DSViter) {
    unsigned int id = DSViter->id;
    DetId detId(id);
    edm::DetSet<SiStripDigi>::const_iterator begin = DSViter->data.begin();
    edm::DetSet<SiStripDigi>::const_iterator end = DSViter->data.end();
    edm::DetSet<SiStripDigi>::const_iterator iter;
    
    // get TIB
    if (detId.subdetId() == sdSiTIB) {
      TIBDetId tibid(id);
      for (iter = begin; iter != end; ++iter) {
	++nBrl;
	if (tibid.layer() == 1) {
	  TIBL1ADC.push_back((*iter).adc());
	  TIBL1Strip.push_back((*iter).strip());
	}
	if (tibid.layer() == 2) {
	  TIBL2ADC.push_back((*iter).adc());
	  TIBL2Strip.push_back((*iter).strip());
	}	
	if (tibid.layer() == 3) {
	  TIBL3ADC.push_back((*iter).adc());
	  TIBL3Strip.push_back((*iter).strip());
	}
	if (tibid.layer() == 4) {
	  TIBL4ADC.push_back((*iter).adc());
	  TIBL4Strip.push_back((*iter).strip());
	}
      }
    }
    
    // get TOB
    if (detId.subdetId() == sdSiTOB) {
      TOBDetId tobid(id);
      for (iter = begin; iter != end; ++iter) {
	++nBrl;
	if (tobid.layer() == 1) {
	  TOBL1ADC.push_back((*iter).adc());
	  TOBL1Strip.push_back((*iter).strip());
	}
	if (tobid.layer() == 2) {
	  TOBL2ADC.push_back((*iter).adc());
	  TOBL2Strip.push_back((*iter).strip());
	}	
	if (tobid.layer() == 3) {
	  TOBL3ADC.push_back((*iter).adc());
	  TOBL3Strip.push_back((*iter).strip());
	}
	if (tobid.layer() == 4) {
	  TOBL4ADC.push_back((*iter).adc());
	  TOBL4Strip.push_back((*iter).strip());
	}
      }
    }    
    
    // get TID
    if (detId.subdetId() == sdSiTID) {
      TIDDetId tidid(id);
      for (iter = begin; iter != end; ++iter) {
	++nFwd;
	if (tidid.wheel() == 1) {
	  TIDW1ADC.push_back((*iter).adc());
	  TIDW1Strip.push_back((*iter).strip());
	}
	if (tidid.wheel() == 2) {
	  TIDW2ADC.push_back((*iter).adc());
	  TIDW2Strip.push_back((*iter).strip());
	}
	if (tidid.wheel() == 3) {
	  TIDW3ADC.push_back((*iter).adc());
	  TIDW3Strip.push_back((*iter).strip());
	}
      }
    }   

    // get TEC
    if (detId.subdetId() == sdSiTEC) {
      TECDetId tecid(id);
      for (iter = begin; iter != end; ++iter) {
	++nFwd;
	if (tecid.wheel() == 1) {
	  TECW1ADC.push_back((*iter).adc());
	  TECW1Strip.push_back((*iter).strip());
	}
	if (tecid.wheel() == 2) {
	  TECW2ADC.push_back((*iter).adc());
	  TECW2Strip.push_back((*iter).strip());
	}
	if (tecid.wheel() == 3) {
	  TECW3ADC.push_back((*iter).adc());
	  TECW3Strip.push_back((*iter).strip());
	}
	if (tecid.wheel() == 4) {
	  TECW4ADC.push_back((*iter).adc());
	  TECW4Strip.push_back((*iter).strip());
	}
	if (tecid.wheel() == 5) {
	  TECW5ADC.push_back((*iter).adc());
	  TECW5Strip.push_back((*iter).strip());
	}
	if (tecid.wheel() == 6) {
	  TECW6ADC.push_back((*iter).adc());
	  TECW6Strip.push_back((*iter).strip());
	}
	if (tecid.wheel() == 7) {
	  TECW7ADC.push_back((*iter).adc());
	  TECW7Strip.push_back((*iter).strip());
	}
	if (tecid.wheel() == 8) {
	  TECW8ADC.push_back((*iter).adc());
	  TECW8Strip.push_back((*iter).strip());
	}
      }
    }     
  } // end loop over DataSetVector

  if (verbosity > 1) {
    eventout += "\n          Number of BrlStripDigis collected:........ ";
    eventout += nBrl;
  }

  if (verbosity > 1) {
    eventout += "\n          Number of FrwdStripDigis collected:....... ";
    eventout += nFwd;
  }

  // get pixel digis

  if (verbosity > 0)
    edm::LogInfo(MsgLoggerCat) << eventout << "\n";

  return;
}

void GlobalDigisProducer::storeTrk(PGlobalDigi& product)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_storeTrk";
  
  TString eventout("\n         nTIBL1     = ");
  eventout += TIBL1ADC.size();
  for (unsigned int i = 0; i < TIBL1ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TIBL1ADC[i];
    eventout += ", ";
    eventout += TIBL1Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTIBL2     = ";
  eventout += TIBL2ADC.size();
  for (unsigned int i = 0; i < TIBL2ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TIBL2ADC[i];
    eventout += ", ";
    eventout += TIBL2Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTIBL3     = ";
  eventout += TIBL3ADC.size();
  for (unsigned int i = 0; i < TIBL3ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TIBL3ADC[i];
    eventout += ", ";
    eventout += TIBL3Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTIBL4     = ";
  eventout += TIBL4ADC.size();
  for (unsigned int i = 0; i < TIBL4ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TIBL4ADC[i];
    eventout += ", ";
    eventout += TIBL4Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTOBL1     = ";
  eventout += TOBL1ADC.size();
  for (unsigned int i = 0; i < TOBL1ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TOBL1ADC[i];
    eventout += ", ";
    eventout += TOBL1Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTOBL2     = ";
  eventout += TOBL2ADC.size();
  for (unsigned int i = 0; i < TOBL2ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TOBL2ADC[i];
    eventout += ", ";
    eventout += TOBL2Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTOBL3     = ";
  eventout += TOBL3ADC.size();
  for (unsigned int i = 0; i < TOBL3ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TOBL3ADC[i];
    eventout += ", ";
    eventout += TOBL3Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTOBL4     = ";
  eventout += TOBL4ADC.size();
  for (unsigned int i = 0; i < TOBL4ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TOBL4ADC[i];
    eventout += ", ";
    eventout += TOBL4Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTIDW1     = ";
  eventout += TIDW1ADC.size();
  for (unsigned int i = 0; i < TIDW1ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TIDW1ADC[i];
    eventout += ", ";
    eventout += TIDW1Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTIDW2     = ";
  eventout += TIDW2ADC.size();
  for (unsigned int i = 0; i < TIDW2ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TIDW2ADC[i];
    eventout += ", ";
    eventout += TIDW2Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTIDW3     = ";
  eventout += TIDW3ADC.size();
  for (unsigned int i = 0; i < TIDW3ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TIDW3ADC[i];
    eventout += ", ";
    eventout += TIDW3Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTECW1     = ";
  eventout += TECW1ADC.size();
  for (unsigned int i = 0; i < TECW1ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TECW1ADC[i];
    eventout += ", ";
    eventout += TECW1Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTECW2     = ";
  eventout += TECW2ADC.size();
  for (unsigned int i = 0; i < TECW2ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TECW2ADC[i];
    eventout += ", ";
    eventout += TECW2Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTECW3     = ";
  eventout += TECW3ADC.size();
  for (unsigned int i = 0; i < TECW3ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TECW3ADC[i];
    eventout += ", ";
    eventout += TECW3Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTECW4     = ";
  eventout += TECW4ADC.size();
  for (unsigned int i = 0; i < TECW4ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TECW4ADC[i];
    eventout += ", ";
    eventout += TECW4Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTECW5     = ";
  eventout += TECW5ADC.size();
  for (unsigned int i = 0; i < TECW5ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TECW5ADC[i];
    eventout += ", ";
    eventout += TECW5Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTECW6     = ";
  eventout += TECW6ADC.size();
  for (unsigned int i = 0; i < TECW6ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TECW6ADC[i];
    eventout += ", ";
    eventout += TECW6Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTECW7     = ";
  eventout += TECW7ADC.size();
  for (unsigned int i = 0; i < TECW7ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TECW7ADC[i];
    eventout += ", ";
    eventout += TECW7Strip[i];
    eventout += ")";
  }
  eventout += "\n         nTECW8     = ";
  eventout += TECW8ADC.size();
  for (unsigned int i = 0; i < TECW8ADC.size(); ++i) {
    eventout += "\n      (ADC, strip) = (";
    eventout += TECW8ADC[i];
    eventout += ", ";
    eventout += TECW8Strip[i];
    eventout += ")";
  }
  
  product.putTIBL1Digis(TIBL1ADC,TIBL1Strip);
  product.putTIBL2Digis(TIBL2ADC,TIBL2Strip);
  product.putTIBL3Digis(TIBL3ADC,TIBL3Strip);
  product.putTIBL4Digis(TIBL4ADC,TIBL4Strip);
  product.putTOBL1Digis(TOBL1ADC,TOBL1Strip);
  product.putTOBL2Digis(TOBL2ADC,TOBL2Strip);
  product.putTOBL3Digis(TOBL3ADC,TOBL3Strip);
  product.putTOBL4Digis(TOBL4ADC,TOBL4Strip);
  product.putTIDW1Digis(TIDW1ADC,TIDW1Strip);
  product.putTIDW2Digis(TIDW2ADC,TIDW2Strip);
  product.putTIDW3Digis(TIDW3ADC,TIDW3Strip);
  product.putTECW1Digis(TECW1ADC,TECW1Strip);
  product.putTECW2Digis(TECW2ADC,TECW2Strip);
  product.putTECW3Digis(TECW3ADC,TECW3Strip);
  product.putTECW4Digis(TECW4ADC,TECW4Strip);
  product.putTECW5Digis(TECW5ADC,TECW5Strip);
  product.putTECW6Digis(TECW6ADC,TECW6Strip);  
  product.putTECW7Digis(TECW7ADC,TECW7Strip);
  product.putTECW8Digis(TECW8ADC,TECW8Strip);  

  return;
}

void GlobalDigisProducer::clear()
{
  std::string MsgLoggerCat = "GlobalDigisProducer_clear";

  if (verbosity > 0)
    edm::LogInfo(MsgLoggerCat)
      << "Clearing event holders"; 

  // reset electromagnetic info
  // EE info
  EECalmaxPos.clear(); 
  EECalAEE.clear(); 
  EECalSHE.clear();
  // EB info
  EBCalmaxPos.clear(); 
  EBCalAEE.clear(); 
  EBCalSHE.clear();
  // ES info
  ESCalADC0.clear();
  ESCalADC1.clear();
  ESCalADC2.clear();
  ESCalSHE.clear();

  // reset HCal Info
  HBCalAEE.clear();
  HBCalSHE.clear();
  HECalAEE.clear();
  HECalSHE.clear();
  HOCalAEE.clear();
  HOCalSHE.clear();
  HFCalAEE.clear();
  HFCalSHE.clear();  

  // reset Track Info
  TIBL1ADC.clear(); 
  TIBL2ADC.clear(); 
  TIBL3ADC.clear(); 
  TIBL4ADC.clear();
  TIBL1Strip.clear(); 
  TIBL2Strip.clear(); 
  TIBL3Strip.clear(); 
  TIBL4Strip.clear();
  
  TOBL1ADC.clear(); 
  TOBL2ADC.clear(); 
  TOBL3ADC.clear(); 
  TOBL4ADC.clear();
  TOBL1Strip.clear(); 
  TOBL2Strip.clear(); 
  TOBL3Strip.clear(); 
  TOBL4Strip.clear();
  
  TIDW1ADC.clear(); 
  TIDW2ADC.clear(); 
  TIDW3ADC.clear();
  TIDW1Strip.clear(); 
  TIDW2Strip.clear(); 
  TIDW3Strip.clear();
  
  TECW1ADC.clear(); 
  TECW2ADC.clear(); 
  TECW3ADC.clear(); 
  TECW4ADC.clear(); 
  TECW5ADC.clear(); 
  TECW6ADC.clear(); 
  TECW7ADC.clear(); 
  TECW8ADC.clear();
  TECW1Strip.clear(); 
  TECW2Strip.clear(); 
  TECW3Strip.clear(); 
  TECW4Strip.clear(); 
  TECW5Strip.clear(); 
  TECW6Strip.clear(); 
  TECW7Strip.clear(); 
  TECW8Strip.clear();

  return;
}

//define this as a plug-in
DEFINE_FWK_MODULE(GlobalDigisProducer);

/*
void GlobalDigisProducer::fillHCal(edm::Event& iEvent, 
				   const edm::EventSetup& iSetup)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_fillHCal";

  TString eventout;
  if (verbosity > 0)
    eventout = "\nGathering info:";  

  if (verbosity > 0)
    edm::LogInfo(MsgLoggerCat) << eventout << "\n";

  return;
}

void GlobalDigisProducer::storeHCal(PGlobalDigi& product)
{
  std::string MsgLoggerCat = "GlobalDigisProducer_storeHCal";

  return;
}
*/
