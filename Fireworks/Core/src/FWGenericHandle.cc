// -*- C++ -*-
//
// Package:     Framework
// Class  :     FWGenericHandle
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Thu Mar 30 15:48:37 EST 2006
//

// system include files

// user include files
#include "Fireworks/Core/src/FWGenericHandle.h"

namespace edm {
void convert_handle(BasicHandle const& orig,
                    Handle<FWGenericObject>& result)
{
  if(orig.failedToGet()) {
    result.setWhyFailed(orig.whyFailed());
    return;
  }

  WrapperHolder originalWrap = orig.wrapperHolder();
  if(!originalWrap.isValid()) {
    throw edm::Exception(edm::errors::InvalidReference,"NullPointer")
      << "edm::BasicHandle has null pointer to Wrapper";
  }
  
  edm::ObjectWithDict wrap(edm::TypeWithDict(originalWrap.wrappedTypeInfo()), const_cast<void*>(originalWrap.wrapper()));
  assert(bool(wrap));
  
  edm::ObjectWithDict product(wrap.get("obj"));
  
  if(!product){
    throw edm::Exception(edm::errors::LogicError)<<"FWGenericObject could not find 'obj' member";
  }
  if(product.typeOf().isTypedef()){
    //For a 'edm::TypeWithDictdef' the 'toType' method returns the actual type
    // this is needed since you are now allowed to 'invoke' methods of a 'Typedef'
    // only for a 'real' class
    product = edm::ObjectWithDict(product.typeOf().toType(), product.address());
    assert(!product.typeOf().isTypedef());
  }
  // NOTE: comparing on type doesn't seem to always work! The problem appears to be if we have a typedef
  if(product.typeOf()!=result.type() &&
     !product.typeOf().isEquivalentTo(result.type()) &&
     product.typeOf().typeInfo()!= result.type().typeInfo()){
        std::cerr << "FWGenericObject asked for "<<result.type().name()
         <<" but was given a " << product.typeOf().name();
    throw edm::Exception(edm::errors::LogicError)<<"FWGenericObject asked for "<<result.type().name()
    <<" but was given a "<<product.typeOf().name();
  }
  
  Handle<FWGenericObject> h(product, orig.provenance(), orig.id());
  h.swap(result);
}

///Specialize the getByLabel method to work with a Handle<FWGenericObject>
template<>
bool
edm::EventBase::getByLabel<FWGenericObject>(edm::InputTag const& tag,
                                             Handle<FWGenericObject>& result) const
{
   std::string dataTypeName = result.type().name(edm::TypeNameHandling::Scoped);
   if (dataTypeName[dataTypeName.size() -1] == '>')
      dataTypeName += " ";
   std::string wrapperName = "edm::Wrapper<" + dataTypeName + ">";

   edm::TypeWithDict wrapperType(edm::TypeWithDict::byName(wrapperName));

   BasicHandle bh = this->getByLabelImpl(wrapperType.typeInfo(),
                                         result.type().typeInfo(),
                                         tag);
   convert_handle(bh, result);  // throws on conversion error
   if(bh.failedToGet()) 
      return false;
   return true;
}

}
