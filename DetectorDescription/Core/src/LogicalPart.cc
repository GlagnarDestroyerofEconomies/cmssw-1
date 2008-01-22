
#include <algorithm>
#include "DetectorDescription/Core/src/LogicalPart.h"
#include "DetectorDescription/Core/interface/DDPartSelection.h"
#include "DetectorDescription/Base/interface/DDdebug.h"
//#include "DetectorDescription/Core/interface/DDSpecifics.h"


using DDI::LogicalPart;

LogicalPart::LogicalPart(const DDMaterial & m,
                         const DDSolid & s,
			 DDEnums::Category c)
 : material_(m), solid_(s), cat_(c), weight_(0), specifics_(0), hasDDValue_(1,false)
{ }




const DDMaterial & LogicalPart::material() const { return material_; }
const DDSolid & LogicalPart::solid() const { return solid_; }
DDEnums::Category LogicalPart::category() const { return cat_; }
void LogicalPart::stream(std::ostream & os) 
{
  os << std::endl << " mat=" << material().ddname() << std::endl << " solid=" << solid();
}

double & LogicalPart::weight()  { return weight_; }

void LogicalPart::addSpecifics(const std::pair<DDPartSelection*,DDsvalues_type*> & s)
{
  if ( ! (s.first && s.second) ) {
    // FIXME
    std::cerr << "LogicalPart::addSpecific error pointer 0 " 
	      << s.first << "," << s.second << std::endl;
    return;
  }
  specifics_.push_back(s);
  DDsvalues_type::const_iterator it = s.second->begin();
  DDsvalues_type::const_iterator ed = s.second->end();
  for (; it != ed; ++it) {
    unsigned int id = it->first;
    if ( id < hasDDValue_.size() ) {
      hasDDValue_[id] = true;
    }
    else {
      hasDDValue_.resize(id+1,false);
      hasDDValue_[id] = true;
    }
    
    DCOUT('S', "hasValue_.size()=" << hasDDValue_.size() << " DDValue_id=" << id << std::flush
          << " DDValue_name=" << DDValue(id).name() << std::flush 
	   << " DDValue_string=" << DDValue(id).strings().size() );
  }
}

bool LogicalPart::hasDDValue(const DDValue & v) const
{
   bool result = false;
   unsigned int id = v.id();
   if ( id < hasDDValue_.size()) {
     result = hasDDValue_[id];
   }
   return result; 
}

void LogicalPart::removeSpecifics(const std::pair<DDPartSelection*,DDsvalues_type*> & s)
{
   std::vector<std::pair<DDPartSelection*,DDsvalues_type* > >::iterator it = 
     std::find(specifics_.begin(),specifics_.end(),s);
   specifics_.erase(it);
}


std::vector<const DDsvalues_type*> LogicalPart::specifics() const
{
   std::vector<const DDsvalues_type*> result;
   specificsV(result);
   return result;

}

void LogicalPart::specificsV(std::vector<const DDsvalues_type*> & result) const
{
  typedef std::vector<std::pair<DDPartSelection*,DDsvalues_type* > > sp_type;
  sp_type::const_iterator it = specifics_.begin();
  sp_type::const_iterator ed = specifics_.end();
  for (; it != ed; ++it) {
    DDPartSelection & ps = *(it->first);
    if (ps.size()==1 && ps[0].selectionType_==ddanylogp) {
      result.push_back(it->second);
    }
  }
}

DDsvalues_type LogicalPart::mergedSpecifics() const {
  DDsvalues_type merged;
  mergedSpecificsV(merged);  
  return merged;
}


void LogicalPart::mergedSpecificsV(DDsvalues_type & merged) const {
  merged.clear();
  std::vector<const DDsvalues_type *> unmerged; specificsV(unmerged);
  if (unmerged.size()==1) {
    merged = *(unmerged[0]);
  }
  else if (unmerged.size()>1) {
    std::vector<const DDsvalues_type *>::const_iterator it = unmerged.begin();
    std::vector<const DDsvalues_type *>::const_iterator ed = unmerged.end();
    for (; it != ed; ++it) {
      merge(merged, **it);
    } 
  }
}  


/*
const std::vector<DDPartSelection*> & LogicalPart::partSelections(const DDValue &) const{
  static std::vector<DDPartSelection*> empty_;
  std::vector<DDPartSelection*> * result_ = &empty;
  std::map<DDValue, std::vector<DDPartSelection*> >::const_iterator it = valToPartsel_.find(v);
  if (it != valToPartsel_.end()) {
     result = &(it->second);
  }
  return *result;
}
*/
