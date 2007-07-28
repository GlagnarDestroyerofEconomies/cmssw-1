// DAQ Flag (serialization in Text/Binary stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef DAQ_FLAG_TXT_H
#define DAQ_FLAG_TXT_H

#include <iosfwd>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

class DAQFlagXML;

class DAQFlagTxt: public FlagTxt {
  public:
    DAQFlagTxt() {}
    DAQFlagTxt( const DAQFlagXML *poDAQ_FLAGXML);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new DAQFlagTxt( *this);
    }

    virtual Clonable *cloneXML() const;

    // Used in serialization to get class ID
    ClassIDBase::ID getID() const;

    // Enable children
    virtual int isChildValid( const FlagTxt *poCHILD_CANDIDATE) const;

  private:
    // Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & boost::serialization::base_object<FlagTxt>( *this);
      }
};

std::ostream &
  operator <<( std::ostream &roOut, const DAQFlagTxt &roFLAG);

BOOST_CLASS_EXPORT_GUID( DAQFlagTxt, "DAQFlagTxt")

#endif // DAQ_FLAG_TXT_H
