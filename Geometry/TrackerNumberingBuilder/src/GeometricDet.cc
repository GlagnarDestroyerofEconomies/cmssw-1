#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "Geometry/TrackerNumberingBuilder/interface/TrackerShapeToBounds.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include "DetectorDescription/Core/interface/DDMaterial.h"

#include <boost/bind.hpp>

#include <cfloat>
#include <vector>
#include <string>

namespace {

  template<typename DDView>
  double  getDouble(const std::string & s,  DDView const & ev) {
    DDValue val(s);
    std::vector<const DDsvalues_type *> result;
    ev.specificsV(result);
    std::vector<const DDsvalues_type *>::iterator it = result.begin();
    bool foundIt = false;
    for (; it != result.end(); ++it)
      {
	foundIt = DDfetch(*it,val);
	if (foundIt) break;
      }    
    if (foundIt)
      { 
	const std::vector<std::string> & temp = val.strings(); 
	if (temp.size() != 1)
	  {
	    throw cms::Exception("Configuration") << "I need 1 "<< s << " tags";
	  }
	return double(::atof(temp[0].c_str())); 
      }
    return 0;
  }



}


/**
 * What to do in the destructor?
 * destroy all the daughters!
 */
GeometricDet::~GeometricDet(){
  deleteComponents();
}

GeometricDet::GeometricDet(nav_type navtype, GeometricEnumType type) : _ddd(navtype), _type(type){ 
  //
  // I need to find the params by myself :(
  //
  DDCompactView cpv;
  DDExpandedView ev(cpv);
  ev.goTo(_ddd);
  _params = ((ev.logicalPart()).solid()).parameters();
  _trans = ev.translation();
  _phi = _trans.Phi();
  _rho = _trans.Rho();
  _rot = ev.rotation();
  _shape = ((ev.logicalPart()).solid()).shape();
  _ddname = ((ev.logicalPart()).ddname()).name();
  _parents = ev.geoHistory();
  _volume   = ((ev.logicalPart()).solid()).volume();
  _density  = ((ev.logicalPart()).material()).density();
  //  _weight  = (ev.logicalPart()).weight();
  _weight   = _density * ( _volume / 1000.); // volume mm3->cm3
  _copy     = ev.copyno();
  _material = ((ev.logicalPart()).material()).name();
  _radLength = getDouble("TrackerRadLength",ev);
  _xi = getDouble("TrackerXi",ev);

}

GeometricDet::GeometricDet(DDExpandedView* fv, GeometricEnumType type) : _type(type) {
  //
  // Set by hand the _ddd
  //
  _ddd = fv->navPos();
  _params = ((fv->logicalPart()).solid()).parameters();  
  _trans = fv->translation();
  _phi = _trans.Phi();
  _rho = _trans.Rho();
  _rot = fv->rotation();
  _shape = ((fv->logicalPart()).solid()).shape();
  _ddname = ((fv->logicalPart()).ddname()).name();
  _parents = fv->geoHistory();
  _volume   = ((fv->logicalPart()).solid()).volume();  
  _density  = ((fv->logicalPart()).material()).density();
  //  _weight   = (fv->logicalPart()).weight();  
  _weight   = _density * ( _volume / 1000.); // volume mm3->cm3
  _copy     = fv->copyno();
  _material = ((fv->logicalPart()).material()).name();
  _radLength = getDouble("TrackerRadLength",*fv);
  _xi = getDouble("TrackerXi",*fv);

}

GeometricDet::GeometricDet(DDFilteredView* fv, GeometricEnumType type) : _type(type){
  //
  // Set by hand the _ddd
  //
  _ddd = fv->navPos();
  _params = ((fv->logicalPart()).solid()).parameters();
  _trans = fv->translation();
  _phi = _trans.Phi();
  _rho = _trans.Rho();
  _rot = fv->rotation();
  _shape = ((fv->logicalPart()).solid()).shape();
  _ddname = ((fv->logicalPart()).ddname()).name();
  _parents = fv->geoHistory();
  _volume   = ((fv->logicalPart()).solid()).volume();
  _density  = ((fv->logicalPart()).material()).density();
  //  _weight   = (fv->logicalPart()).weight();
  _weight   = _density * ( _volume / 1000.); // volume mm3->cm3
  _copy     = fv->copyno();
  _material = ((fv->logicalPart()).material()).name();
  _radLength = getDouble("TrackerRadLength",*fv);
  _xi = getDouble("TrackerXi",*fv);

}

GeometricDet::ConstGeometricDetContainer GeometricDet::deepComponents() const {
  //
  // iterate on all the components ;)
  //
  ConstGeometricDetContainer _temp;
  deepComponents(_temp);
  return _temp;
}

void GeometricDet::deepComponents(GeometricDetContainer & cont) const {
  if (isLeaf())
    cont.push_back(const_cast<GeometricDet*>(this));
  else 
    std::for_each(_container.begin(),_container.end(), 
		  boost::bind(&GeometricDet::deepComponents,_1,boost::ref(cont))
		  );
}


void GeometricDet::addComponents(GeometricDetContainer const & cont){
  if (_container.empty()) {
    _container=cont;
    return;
  }
  _container.reserve(_container.size()+cont.size());
  std::copy(cont.begin(), cont.end(), back_inserter(_container));
}


void GeometricDet::addComponent(GeometricDet* det){
  _container.push_back(det);
}

namespace {
  struct Deleter {
    void operator()(GeometricDet const* det) const { delete const_cast<GeometricDet*>(det);}
  };
}

void GeometricDet::deleteComponents(){
  std::for_each(_container.begin(),_container.end(),Deleter()); 
  _container.clear();
}


GeometricDet::Position GeometricDet::positionBounds() const{

  Position _pos(float(_trans.x()/cm), 
		float(_trans.y()/cm), 
		float(_trans.z()/cm));
  return _pos;
}

GeometricDet::Rotation GeometricDet::rotationBounds() const{
  DD3Vector x, y, z;
  _rot.GetComponents(x, y, z);
  Rotation _rotation(float(x.X()),float(x.Y()),float(x.Z()),
		     float(y.X()),float(y.Y()),float(y.Z()),
		     float(z.X()),float(z.Y()),float(z.Z())); 
  return _rotation;
}

const Bounds * GeometricDet::bounds() const{
  const std::vector<double>& par = _params;
  Bounds * bounds = 0;
  TrackerShapeToBounds shapeToBounds;
  bounds = shapeToBounds.buildBounds(_shape,par);
  return bounds;
}

