#ifndef IOPool_Output_RootOutputTree_h
#define IOPool_Output_RootOutputTree_h

/*----------------------------------------------------------------------

RootOutputTree.h // used by ROOT output modules

$Id: RootOutputTree.h,v 1.22 2008/02/25 16:38:10 wmtan Exp $

----------------------------------------------------------------------*/

#include <memory>
#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/Registry.h"
#include "DataFormats/Provenance/interface/BranchDescription.h"
#include "DataFormats/Provenance/interface/BranchEntryDescription.h"
#include "DataFormats/Provenance/interface/BranchKey.h"
#include "DataFormats/Provenance/interface/BranchType.h"
#include "DataFormats/Provenance/interface/ConstBranchDescription.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "DataFormats/Provenance/interface/Selections.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "TBranch.h"
#include "TTree.h"
class TFile;

namespace edm {

  class RootOutputTree {
  public:
    // Constructor for trees with no fast cloning
    template <typename T>
    RootOutputTree(boost::shared_ptr<TFile> filePtr,
		   BranchType const& branchType,
		   T const*& pAux,
		   int bufSize,
		   int splitLevel) :
      filePtr_(filePtr),
      tree_(makeTTree(filePtr.get(), BranchTypeToProductTreeName(branchType), splitLevel)),
      metaTree_(makeTTree(filePtr.get(), BranchTypeToMetaDataTreeName(branchType), 0)),
      auxBranch_(0),
      branches_(),
      metaBranches_(),
      clonedBranches_(),
      clonedMetaBranches_(),
      currentlyFastCloning_(),
      basketSize_(bufSize),
      splitLevel_(splitLevel),
      branchNames_() {

      auxBranch_ = tree_->Branch(BranchTypeToAuxiliaryBranchName(branchType).c_str(), &pAux, bufSize, 0);
      clonedBranches_.push_back(auxBranch_);
    }

    ~RootOutputTree() {}
    
    static void fastCloneTTree(TTree *in, TTree *out);

    static TTree * makeTTree(TFile *filePtr, std::string const& name, int splitLevel);

    static TTree * assignTTree(TFile *file, TTree * tree);

    static void writeTTree(TTree *tree);

    bool isValid() const;

    void addBranch(BranchDescription const& prod, bool selected, BranchEntryDescription const*& pProv, void const*& pProd, bool inInput);

    std::vector<std::string> const& branchNames() const {return branchNames_;}

    void fastCloneTree(TTree *tree, TTree *metaTree);

    void fillTree() const;

    void writeTree() const;

    TTree *const tree() const {
      return tree_;
    }

    TTree *const metaTree() const {
      return metaTree_;
    }

    void setEntries() {
      if (tree_->GetNbranches() != 0) tree_->SetEntries(-1);
      if (metaTree_->GetNbranches() != 0) metaTree_->SetEntries(-1);
    }

    void beginInputFile(bool fastCloning) {currentlyFastCloning_ = fastCloning;}

  private:
    static void fillTTree(TTree *tree, std::vector<TBranch *> const& branches);
// We use bare pointers for pointers to some ROOT entities.
// Root owns them and uses bare pointers internally.
// Therefore,using smart pointers here will do no good.
    boost::shared_ptr<TFile> filePtr_;
    TTree *const tree_;
    TTree *const metaTree_;
    TBranch * auxBranch_;
    std::vector<TBranch *> branches_; // does not include cloned branches
    std::vector<TBranch *> metaBranches_; // does not include cloned branches
    std::vector<TBranch *> clonedBranches_;
    std::vector<TBranch *> clonedMetaBranches_;
    bool currentlyFastCloning_;
    int basketSize_;
    int splitLevel_;
    std::vector<std::string> branchNames_;
  };
}
#endif
