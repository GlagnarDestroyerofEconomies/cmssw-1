/*----------------------------------------------------------------------
  
$Id: ProductRegistryHelper.cc,v 1.1 2005/12/28 00:49:48 wmtan Exp $

----------------------------------------------------------------------*/

#include "FWCore/Framework/interface/ProductRegistryHelper.h"
#include "DataFormats/Common/interface/ProductRegistry.h"
#include "DataFormats/Common/interface/BranchDescription.h"

namespace edm {
  ProductRegistryHelper::~ProductRegistryHelper() { }

  ProductRegistryHelper::TypeLabelList & ProductRegistryHelper::typeLabelList() {
    return typeLabelList_;
  }

  void
  ProductRegistryHelper::addToRegistry(TypeLabelList::const_iterator const& iBegin,
                             TypeLabelList::const_iterator const& iEnd,
                             ModuleDescription const& iDesc,
                             ProductRegistry& iReg,
                             bool iIsListener) {
    for (TypeLabelList::const_iterator p = iBegin; p != iEnd; ++p) {
      BranchDescription pdesc(iDesc,
                              p->typeID_.userClassName(),
                              p->typeID_.friendlyClassName(), 
                              p->productInstanceName_,
                              p->productPtr_);
      iReg.addProduct(pdesc, iIsListener);
    }//for
  }
}
  
