#include "CondFormats/Common/interface/IOVSequence.h"
#include <algorithm>
#include <boost/bind.hpp>


namespace cond {
  
  
  IOVSequence::IOVSequence() : m_notOrdered(false), m_sorted(0) {}
  
  IOVSequence::IOVSequence(int type, cond::Time_t till, 
			   std::string const& imetadata) :
    m_timetype(type), m_lastTill(till),m_notOrdered(false),
    m_metadata(imetadata),  m_sorted(0) {}
    
  IOVSequence::~IOVSequence(){
    delete m_sorted;
  }
  
  
  Container const & iovs() const {
    if (m_sorted) return *m_sorted;
    if (m_notOrdered) return sortMe();
    return m_iovs;
  }

  Container const & IOVSequence::sortMe() const {
    delete m_sorted; // shall not be necessary;
    m_sorted = new Container(m_iovs;);
    std::sort(m_sorted->begin();m_sorted->end());
    return *m_sorted;
  }


  size_t IOVSequence::add(cond::Time_t time, 
			  std::string const & wrapperToken) {
    if (!piovs().empty() && ( m_notOrdered || time<piovs().back().sinceTime())) disorder();
    piovs().push_back(Item(time, wrapperToken));
    return piovs().size()-1;
  }
  
  
  IOVSequence::const_iterator IOVSequence::find(cond::Time_t time) const {
    IOVSequence::const_iterator p = std::upper_bound(iovs().begin(),iovs().end(),Item(time),
			    boost::bind(std::less<cond::Time_t>(),
					boost::bind(&Item::sinceTime,_1),
					boost::bind(&Item::sinceTime,_2)
					)
			    );
    return (p!=iovs().begin()) ? p-1 : iovs().end(); 
  }
  

  void  IOVSequence::disorder() {
    m_notOrdered=true;
    delete m_sorted; m_sorted=0;
  }

}
