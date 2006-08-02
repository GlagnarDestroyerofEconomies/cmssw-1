// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     L1EmParticle
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Werner Sun
//         Created:  Tue Jul 25 15:56:47 EDT 2006
// $Id: L1EmParticle.cc,v 1.1 2006/07/26 00:05:39 wsun Exp $
//

// system include files

// user include files
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"

using namespace l1extra ;

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1EmParticle::L1EmParticle()
{
}

L1EmParticle::L1EmParticle( const LorentzVector& p4,
			    const L1Ref& aRef )
//   : ParticleKinematics( p4 ),
   : L1PhysObjectBase( ( char ) 0, p4, aRef )
{
   if( triggerObjectRef().isNonnull() )
   {
      type_ = gctEmCand()->isolated() ? kIsolated : kNonIsolated ;
   }
}

L1EmParticle::L1EmParticle( const LorentzVector& p4,
			    EmType type )
   : L1PhysObjectBase( ( char ) 0, p4, L1Ref() ),
     type_( type )
{
}

// L1EmParticle::L1EmParticle(const L1EmParticle& rhs)
// {
//    // do actual copying here;
// }

L1EmParticle::~L1EmParticle()
{
}

//
// assignment operators
//
// const L1EmParticle& L1EmParticle::operator=(const L1EmParticle& rhs)
// {
//   //An exception safe implementation is
//   L1EmParticle temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//

const L1GctEmCand*
L1EmParticle::gctEmCand() const
{
   return dynamic_cast< const L1GctEmCand* >( triggerObjectPtr() ) ;
}

//
// const member functions
//

//
// static member functions
//
