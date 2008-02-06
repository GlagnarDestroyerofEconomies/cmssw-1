#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibration.h"
#include "FWCore/Utilities/interface/Exception.h"

//
// Constructors
//
SiPixelGainCalibration::SiPixelGainCalibration() :
  minPed_(0.),
  maxPed_(255.),
  minGain_(0.),
  maxGain_(255.)
{
}
//
SiPixelGainCalibration::SiPixelGainCalibration(float minPed, float maxPed, float minGain, float maxGain) :
  minPed_(minPed),
  maxPed_(maxPed),
  minGain_(minGain),
  maxGain_(maxGain)
{
}

bool SiPixelGainCalibration::put(const uint32_t& DetId, Range input, const int& nCols) {
  // put in SiPixelGainCalibration of DetId

  Registry::iterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,SiPixelGainCalibration::StrictWeakOrdering());
  if (p!=indexes.end() && p->detid==DetId)
    return false;
  
  size_t sd= input.second-input.first;
  DetRegistry detregistry;
  detregistry.detid=DetId;
  detregistry.ncols=nCols;
  detregistry.ibegin=v_pedestals.size();
  detregistry.iend=v_pedestals.size()+sd;
  indexes.insert(p,detregistry);

  v_pedestals.insert(v_pedestals.end(),input.first,input.second);
  return true;
}

const int SiPixelGainCalibration::getNCols(const uint32_t& DetId) const {
  // get number of columns of DetId
  RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,SiPixelGainCalibration::StrictWeakOrdering());
  if (p==indexes.end()|| p->detid!=DetId) 
    return 0;
  else
    return p->ncols; 
}

const SiPixelGainCalibration::Range SiPixelGainCalibration::getRange(const uint32_t& DetId) const {
  // get SiPixelGainCalibration Range of DetId
  
  RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,SiPixelGainCalibration::StrictWeakOrdering());
  if (p==indexes.end()|| p->detid!=DetId) 
    return SiPixelGainCalibration::Range(v_pedestals.end(),v_pedestals.end()); 
  else 
    return SiPixelGainCalibration::Range(v_pedestals.begin()+p->ibegin,v_pedestals.begin()+p->iend);
}

const std::pair<const SiPixelGainCalibration::Range, const int>
SiPixelGainCalibration::getRangeAndNCols(const uint32_t& DetId) const {
  RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,SiPixelGainCalibration::StrictWeakOrdering());
  if (p==indexes.end()|| p->detid!=DetId) 
    return std::make_pair(SiPixelGainCalibration::Range(v_pedestals.end(),v_pedestals.end()), 0); 
  else 
    return std::make_pair(SiPixelGainCalibration::Range(v_pedestals.begin()+p->ibegin,v_pedestals.begin()+p->iend), p->ncols);
}
  

void SiPixelGainCalibration::getDetIds(std::vector<uint32_t>& DetIds_) const {
  // returns vector of DetIds in map
  SiPixelGainCalibration::RegistryIterator begin = indexes.begin();
  SiPixelGainCalibration::RegistryIterator end   = indexes.end();
  for (SiPixelGainCalibration::RegistryIterator p=begin; p != end; ++p) {
    DetIds_.push_back(p->detid);
  }
}

void SiPixelGainCalibration::setData(float ped, float gain, std::vector<char>& vped){
  
  float theEncodedGain  = encodeGain(gain);
  float theEncodedPed   = encodePed (ped);

  unsigned int ped_   = (static_cast<unsigned int>(theEncodedPed))  & 0xFF; 
  unsigned int gain_  = (static_cast<unsigned int>(theEncodedGain)) & 0xFF;

  unsigned int data = (ped_ << 8) | gain_ ;
  vped.resize(vped.size()+2);
  // insert in vector of char
  ::memcpy((void*)(&vped[vped.size()-2]),(void*)(&data),2);
}

float SiPixelGainCalibration::getPed(const int& col, const int& row, const Range& range, const int& nCols) const {

  int nRows = (range.second-range.first)/2 / nCols;
  const DecodingStructure & s = (const DecodingStructure & ) *(range.first+(col*nRows + row)*2);
  if (col >= nCols || row >= nRows){
    throw cms::Exception("CorruptedData")
      << "[SiPixelGainCalibration::getPed] Pixel out of range: col " << col << " row " << row;
  }  
  return decodePed(s.ped & 0xFF);  
}

float SiPixelGainCalibration::getGain(const int& col, const int& row, const Range& range, const int& nCols) const {

  int nRows = (range.second-range.first)/2 / nCols;
  const DecodingStructure & s = (const DecodingStructure & ) *(range.first+(col*nRows + row)*2);
  if (col >= nCols || row >= nRows){
    throw cms::Exception("CorruptedData")
      << "[SiPixelGainCalibration::getPed] Pixel out of range: col " << col << " row " << row;
  }  
  return decodeGain(s.gain & 0xFF);
}

float SiPixelGainCalibration::encodeGain( const float& gain ) {
  
  if(gain < minGain_ || gain > maxGain_ ) {
    throw cms::Exception("InsertFailure")
      << "[SiPixelGainCalibration::encodeGain] Trying to encode gain out of range\n";
  } else {
    double precision   = (maxGain_-minGain_)/255.;
    float  encodedGain = (float)((gain-minGain_)/precision);
    return encodedGain;
  }

}

float SiPixelGainCalibration::encodePed( const float& ped ) {

  if(ped < minPed_ || ped > maxPed_ ) {
    throw cms::Exception("InsertFailure")
      << "[SiPixelGainCalibration::encodePed] Trying to encode pedestal out of range\n";
  } else {
    double precision   = (maxPed_-minPed_)/255.;
    float  encodedPed = (float)((ped-minPed_)/precision);
    return encodedPed;
  }

}

float SiPixelGainCalibration::decodePed( unsigned int ped ) const {

  double precision = (maxPed_-minPed_)/255.;
  float decodedPed = (float)(ped*precision + minPed_);
  return decodedPed;

}

float SiPixelGainCalibration::decodeGain( unsigned int gain ) const {

  double precision = (maxGain_-minGain_)/255.;
  float decodedGain = (float)(gain*precision + minGain_);
  return decodedGain;

}


// functions for template compatibility with other payloads. should never run.
float SiPixelGainCalibration::getPed(const int& col, const Range& range, const int& nCols) const {
   throw cms::Exception("ConfigurationError")
      << "[SiPixelGainCalibration::getPed(col, range)] Data is stored at pixel granularity in this payload.  Please use getPed(col, row, range, ncols)";
   return -1.;
}

float SiPixelGainCalibration::getGain(const int& col, const Range& range, const int& nCols) const {
   throw cms::Exception("ConfigurationError")
      << "[SiPixelGainCalibration::getGain(col, range)] Data is stored at pixel granularity in this payload.  Please use getGain(col, row, range, ncols)";
   return -1.;
}
