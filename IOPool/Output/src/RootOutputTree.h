#ifndef IOPool_Input_RootOutputTree_h
#define IOPool_Input_RootOutputTree_h

/*----------------------------------------------------------------------

RootOutputTree.h // used by ROOT input sources

$Id: RootOutputTree.h,v 1.13 2007/06/14 22:02:15 wmtan Exp $

----------------------------------------------------------------------*/

#include <memory>
#include <string>

#include "boost/shared_ptr.hpp"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/Registry.h"
#include "DataFormats/Provenance/interface/BranchDescription.h"
#include "DataFormats/Provenance/interface/BranchEntryDescription.h"
#include "DataFormats/Provenance/interface/BranchKey.h"
#include "DataFormats/Provenance/interface/BranchType.h"
#include "DataFormats/Provenance/interface/ConstBranchDescription.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "TBranch.h"
#include "TTree.h"
class TFile;

namespace edm {

  class RootOutputTree {
  public:
    template <typename T>
    RootOutputTree(BranchType const& branchType, T const*& pAux, int bufSize, int splitLevel) :
      tree_(new TTree(BranchTypeToProductTreeName(branchType).c_str(), "", splitLevel)),
      metaTree_(new TTree(BranchTypeToMetaDataTreeName(branchType).c_str(), "", 0)),
      auxBranch_(tree_->Branch(BranchTypeToAuxiliaryBranchName(branchType).c_str(), &pAux, bufSize, 0)),
      basketSize_(bufSize),
      splitLevel_(splitLevel),
      branchNames_()
    {}
    ~RootOutputTree() {}
    
    bool isValid() const;
    void addBranch(BranchDescription const& prod, bool selected, BranchEntryDescription const*& pProv, void const*& pProd);
    std::vector<std::string> const& branchNames() const {return branchNames_;}
    void fillTree() const {
      tree_->Fill();
      metaTree_->Fill();
    }
    void writeTree() const {
      tree_->Write(tree_->GetName(), TObject::kWriteDelete);
      metaTree_->Write(metaTree_->GetName(), TObject::kWriteDelete);
    }
    TTree *const tree() const {
      return tree_;
    }
  private:
// We use bare pointers for pointers to some ROOT entities.
// Root owns them and uses bare pointers internally.
// Therefore,using smart pointers here will do no good.
    TTree *const tree_;
    TTree *const metaTree_;
    TBranch *const auxBranch_;
    int basketSize_;
    int splitLevel_;
    std::vector<std::string> branchNames_;
  };
}
#endif
