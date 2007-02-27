#include "DetectorDescription/Core/interface/Division.h"

#include "DetectorDescription/Core/interface/DDSolid.h"

#include <algorithm>
#include <iostream>

using DDI::Division;

Division::Division(const DDLogicalPart & parent,
		   const DDAxes axis,
		   int nReplicas,
		   double width, 
		   double offset )
  : parent_(parent), axis_(axis), nReplicas_(nReplicas), width_(width), offset_(offset)
{ }

Division::Division(const DDLogicalPart & parent,
		   const DDAxes axis,
		   int nReplicas,
		   double offset )
  : parent_(parent), axis_(axis), nReplicas_(nReplicas), width_(0.0), offset_(offset)
{ }

Division::Division(const DDLogicalPart & parent,
		   const DDAxes axis,
		   double width, 
		   double offset )
  : parent_(parent), axis_(axis), nReplicas_(0), width_(width), offset_(offset)
{ }

const DDAxes Division::axis() const { return axis_; }
const int Division::nReplicas() const { return nReplicas_; }
const double Division::width() const { return width_; }
const double Division::offset() const { return offset_; }
const DDLogicalPart & Division::parent() const { return parent_; }

void Division::stream(std::ostream & os) 
{
  os << std::endl;
  os << "  LogicalPart: " << parent_ << std::endl;
  os << "  Solid: " << parent_.solid() << std::endl;
  os << " axis: " << axis() << " nReplicas: " << nReplicas()
     << " width: " << width() << " offset: " << offset() << std::endl; 
}


