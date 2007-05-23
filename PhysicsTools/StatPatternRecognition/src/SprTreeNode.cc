//$Id: SprTreeNode.cc,v 1.5 2007/02/05 21:49:46 narsky Exp $

#include "PhysicsTools/StatPatternRecognition/interface/SprExperiment.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprTreeNode.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprAbsTwoClassCriterion.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprAbsFilter.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprPoint.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprUtils.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprIntegerBootstrap.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprTrainedNode.hh"

#include <iostream>
#include <set>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cmath>

using namespace std;


int SprTreeNode::counter_ = 0;

struct STNCmpPairFirst 
  : public binary_function<pair<double,int>,pair<double,int>,bool> {
  bool operator()(const pair<double,int>& l, const pair<double,int>& r)
    const {
    return (l.first < r.first);
  }
};


SprTreeNode::~SprTreeNode()
{
  delete data_;
  delete left_;
  delete right_;
}


SprTreeNode::SprTreeNode(const SprAbsTwoClassCriterion* crit,
			 const SprAbsFilter* data,
			 bool allLeafsSignal,
			 int nmin,
			 bool discrete,
			 bool canHavePureNodes,
			 SprIntegerBootstrap* bootstrap)
  :
  crit_(crit),
  data_(new SprBoxFilter(data)),
  allLeafsSignal_(allLeafsSignal),
  nmin_(nmin),
  discrete_(discrete),
  canHavePureNodes_(canHavePureNodes),
  cls0_(0),
  cls1_(1),
  parent_(0),
  left_(0),
  right_(0),
  fom_(0),
  w0_(0),
  w1_(0),
  n0_(0),
  n1_(0),
  limits_(),
  id_(0),
  nodeClass_(-1),
  d_(-1),
  cut_(0),
  bootstrap_(bootstrap)
{
  assert( crit_ != 0 );
  assert( data_->size() > nmin_ );
  this->setClasses();
  counter_ = 0;// if no parent specified, starting a new tree from scratch
}


SprTreeNode::SprTreeNode(const SprAbsTwoClassCriterion* crit,
			 const SprBoxFilter& data, 
			 bool allLeafsSignal,
			 int nmin,
			 bool discrete,
			 bool canHavePureNodes,
			 const SprClass& cls0,
			 const SprClass& cls1,
			 const SprTreeNode* parent,
			 int d,
			 const SprCut& cut,
			 const SprBox& limits,
			 //			 double w0, double w1,
			 //			 unsigned n0, unsigned n1,
			 SprIntegerBootstrap* bootstrap)
  :
  crit_(crit),
  data_(new SprBoxFilter(&data)),
  allLeafsSignal_(allLeafsSignal),
  nmin_(nmin),
  discrete_(discrete),
  canHavePureNodes_(canHavePureNodes),
  cls0_(cls0),
  cls1_(cls1),
  parent_(parent),
  left_(0),
  right_(0),
  fom_(0),
  w0_(0),
  w1_(0),
  n0_(0),
  n1_(0),
  limits_(limits),
  id_(++counter_),
  nodeClass_(-1),
  d_(-1),
  cut_(0),
  bootstrap_(bootstrap)
{
  assert( crit_ != 0 );
  assert( parent_ != 0 );
  bool status = data_->setCut(d,cut);
  assert( status );
  status = data_->irreversibleFilter();
  assert( status );
}


void SprTreeNode::setClasses() 
{
  vector<SprClass> classes;
  data_->classes(classes);
  int size = classes.size();
  if( size > 0 ) cls0_ = classes[0];
  if( size > 1 ) cls1_ = classes[1];
  //  cout << "Classes for decision tree are set to " 
  //       << cls0_ << " " << cls1_ << endl;
}


std::pair<double,double> SprTreeNode::limits(int d) const 
{
  // if out of range return zeros
  if( d<0 || d>=limits_.size() ) 
    return pair<double,double>(0,0);

  // find the cut
  SprBox::const_iterator iter = limits_.find(d);

  // if not found, infty range
  if( iter == limits_.end() ) 
    return std::pair<double,double>(SprUtils::min(),SprUtils::max());

  // exit
  return iter->second;
}


bool SprTreeNode::split(std::vector<SprTreeNode*>& nodesToSplit, 
			std::vector<std::pair<int,double> >& countTreeSplits,
			int verbose)
{
  // sanity check
  if( data_ == 0 ) return true;

  // message
  if( (id_%100)==0 && verbose>1 ) 
    cout << "Splitting node " << id_ << " ..." << endl;

  // check weights
  /*
  double w0actual = data_->weightInClass(cls0_);
  double w1actual = data_->weightInClass(cls1_);
  w0_ = ( w0_<w0actual ? w0actual : w0_ );
  w1_ = ( w1_<w1actual ? w1actual : w1_ );
  */
  w0_ = data_->weightInClass(cls0_);
  w1_ = data_->weightInClass(cls1_);

  // check numbers of events
  /*
  unsigned n0actual = data_->ptsInClass(cls0_);
  unsigned n1actual = data_->ptsInClass(cls1_);
  n0_ = ( n0_<n0actual ? n0actual : n0_ );
  n1_ = ( n1_<n1actual ? n1actual : n1_ );
  */
  n0_ = data_->ptsInClass(cls0_);
  n1_ = data_->ptsInClass(cls1_);

  // get totals and FOM
  int ntot = n0_ + n1_;
  double wtot = w0_ + w1_;
  if( ntot<nmin_ || wtot<SprUtils::eps() ) {
    if( verbose > 2 ) {
      cout << "Ignore node " << id_ << " with " 
	   << ntot << " events and " << wtot << " total weight." << endl;
    }
    return this->prepareExit(true);
  }

  // compute fom
  fom_ = crit_->fom(0,w0_,w1_,0);
  double invertedFom = crit_->fom(w0_,0,0,w1_);
  if( verbose > 3 ) {
    cout << "===================" << endl;
    cout << "Direct FOM=" << fom_ 
	 << "  Inverted FOM=" << invertedFom << endl;
  }
  if( n1_>0 && 
      ( allLeafsSignal_ ||
	( crit_->symmetric() && (w1_+SprUtils::eps())>w0_ ) ||
	( !crit_->symmetric() && 
	  ( fom_>invertedFom || 
	    ( fabs(fom_-invertedFom)<SprUtils::eps() 
	      && (w1_+SprUtils::eps())>w0_ ) ) 
	  ) 
	) 
      ) {
    nodeClass_ = 1;
  }
  else {
    nodeClass_ = 0;
    fom_ = invertedFom;
  }

  // check weights
  if( w0_<SprUtils::eps() || w1_<SprUtils::eps() ) {
    if( verbose > 3 ) {
      cout << "Node " << id_ << " missing one of categories." << endl;
    }
    return this->prepareExit(true);
  }

  // check if minimal number of events
  if( (n0_+n1_) == nmin_ ) {
    if( verbose > 3 ) {
      cout << "Node " << id_ << " has minimal number of events." 
	   << " Will exit without splitting." << endl;
    }
    return this->prepareExit(true);
  }

  // message
  if( verbose > 3 ) {
    cout << "Splitting node " << id_ << " of class " << nodeClass_ 
	 << " with " << w0_ << " background and " 
	 << w1_ << " signal weights and " << ntot << " events." << endl;
    cout << "Starting FOM=" << fom_ << endl;
  }

  // select features
  set<unsigned> dims;
  if(      bootstrap_ == 0 ) {
    for( int d=0;d<data_->dim();d++ ) dims.insert(d);
  }
  else if( !bootstrap_->replica(dims) ) {
    cerr << "Unable to select features." << endl;
    return this->prepareExit(false);
  }
  if( verbose > 2 ) {
    cout << "Selected dimensions: ";
    for( set<unsigned>::const_iterator 
	   iter=dims.begin();iter!=dims.end();iter++ ) cout << *iter << " ";
    cout << endl;
  }

  // set limits
  if( parent_ == 0 ) {
    limits_.clear();
  }
  if( verbose > 3 ) {
    cout << "Limits:" << endl;
    for( SprBox::const_iterator 
	   iter=limits_.begin();iter!=limits_.end();iter++ ) {
      cout << "Dimension " << iter->first << "    " 
	   << iter->second.first << " " << iter->second.second << endl;
    }
  }

  // init
  unsigned dim = data_->dim();
  vector<double> fom(dim,SprUtils::min());
  vector<double> cut(dim,SprUtils::min());
  //  vector<double> w0best(dim,0), w1best(dim,0);
  //  vector<unsigned> n0best(dim,0), n1best(dim,0);

  // loop through dimensions
  for( set<unsigned>::const_iterator 
	 iter=dims.begin();iter!=dims.end();iter++ ) {
    // get dimension
    unsigned d = *iter;
    assert( d < dim );

    // sort
    vector<int> sorted;
    vector<double> division;
    if( !this->sort(d,sorted,division) ) {
      cerr << "Unable to sort tree node in dimension " << d << endl;
      return this->prepareExit(false);
    }

    // check divisions
    if( division.empty() ) continue;

    // init
    double wmis0 = w0_;
    double wcor1 = w1_;
    double wmis1(0), wcor0(0);
    int nmis0 = n0_;
    int ncor1 = n1_;
    int nmis1(0), ncor0(0);
    vector<double> flo, fhi;
    //    vector<double> w0lo, w1lo;
    //    vector<unsigned> n0lo, n1lo;

    // loop through points
    int ndiv = division.size();
    int istart(0), isplit(0);
    bool lbreak = true;
    for( int k=0;k<ndiv;k++ ) {
      double z = division[k];
      lbreak = false;
      for( isplit=istart;isplit<sorted.size();isplit++ ) {
	if( (*data_)[sorted[isplit]]->x_[d] > z ) {
	  lbreak = true;
	  break;
	}
      }
      if( !lbreak ) isplit = sorted.size();
      for( int i=istart;i<isplit;i++ ) {
	const SprPoint* p = (*data_)[sorted[i]];
	double w = data_->w(sorted[i]);
	if(      p->class_ == cls0_ ) {
	  wmis0 -= w;
	  wcor0 += w;
	  nmis0--;
	  ncor0++;
	}
	else if( p->class_ == cls1_ ) {
	  wcor1 -= w;
	  wmis1 += w;
	  ncor1--;
	  nmis1++;
	}
      }
      istart = isplit;
      if( crit_->symmetric() ) {  // need to compute FOM for one side only
	if( (ncor1+nmis0)>=nmin_ && (nmis1+ncor0)>=nmin_ 
	    && (wcor1+wmis0)>0 && (wmis1+wcor0)>0 
	    && ( canHavePureNodes_ 
		 || ((ncor1*nmis0)>0 && (nmis1*ncor0)>0 
		     && (wcor1*wmis0)>0 && (wmis1*wcor0)>0) ) )
	  flo.push_back(crit_->fom(wcor0,wmis0,wcor1,wmis1));
	else
	  flo.push_back(SprUtils::min());
      }
      else { // take both sides into account
	if( (ncor1+nmis0)>=nmin_ && (wcor1+wmis0)>0 
	    && ( canHavePureNodes_ || ((ncor1*nmis0)>0 && (wcor1*wmis0)>0) ) )
	  flo.push_back(crit_->fom(wcor0,wmis0,wcor1,wmis1));
	else
	  flo.push_back(SprUtils::min());
	if( (nmis1+ncor0)>=nmin_ && (wmis1+wcor0)>0
	    && (canHavePureNodes_ || ((nmis1*ncor0)>0 && (wmis1*wcor0)>0) ) )
	  fhi.push_back(crit_->fom(wmis0,wcor0,wmis1,wcor1));
	else
	  fhi.push_back(SprUtils::min());
      }
      //      w0lo.push_back(wmis0);
      //      w1lo.push_back(wcor1);
      //      n0lo.push_back( (nmis0<0 ? 0 : nmis0) );
      //      n1lo.push_back( (ncor1<0 ? 0 : ncor1) );
    }

    // find optimal point and sign of cut
    vector<double>::iterator ilo = max_element(flo.begin(),flo.end());
    vector<double>::iterator ihi = max_element(fhi.begin(),fhi.end());
    if( crit_->symmetric() || *ilo>*ihi ) {
      int k = ilo - flo.begin();
      cut[d] = division[k]; 
      fom[d] = *ilo;
      //      w0best[d] = w0lo[k];
      //      w1best[d] = w1lo[k];
      //      n0best[d] = n0lo[k];
      //      n1best[d] = n1lo[k];
    }
    else {
      int k = ihi - fhi.begin();
      cut[d] = division[k]; 
      fom[d] = *ihi;
      //      w0best[d] = w0lo[k];
      //      w1best[d] = w1lo[k];
      //      n0best[d] = n0lo[k];
      //      n1best[d] = n1lo[k];
    }
  }

  // find optimal fom
  vector<double>::iterator imax = max_element(fom.begin(),fom.end());
  double newFom = *imax;

  // split the node
  if( newFom > fom_ ) {
    d_ = imax - fom.begin();// dimension on which we split
    cut_ = cut[d_];

    // print out
    if( verbose > 2 ) {
      cout << "Splitting node " << id_ << " of class " << nodeClass_
	   << " in dimension " << d_ << " with " 
	   << n1_ << " signal and " << n0_ << " background events" 
	   << "     FOM=" << newFom 
	   << "   Split=" << cut_ << endl;
    }
    if( verbose > 3 ) {
      double w0l(0), w0r(0), w1l(0), w1r(0);
      int n0l(0), n0r(0), n1l(0), n1r(0);
      for( int i=0;i<data_->size();i++ ) {
	const SprPoint* p = (*data_)[i];
	double w = data_->w(i);
	if(      p->class_ == cls0_ ) {
	  if( p->x_[d_] < cut_ ) {
	    w0l += w;
	    n0l++;
	  }
	  else {
	    w0r += w;
	    n0r++;
	  }
	}
	else if( p->class_ == cls1_ ) {
	  if( p->x_[d_] < cut_ ) {
	    w1l += w;
	    n1l++;
	  }
	  else {
	    w1r += w;
	    n1r++;
	  }
	}
      }
      cout << "Splitting node "<< id_ 
	   << " into nodes " << counter_+1 << "  " << counter_+2
	   << "   Left  (0/1)= " << w0l << "/" << w1l 
	   << " " << n0l << "/" << n1l 
	   << "   Right (0/1)= " << w0r << "/" << w1r 
	   << " " << n0r << "/" << n1r 
	   << endl;
    }

    // get limits
    SprBox leftLims = limits_;
    SprBox rightLims = limits_;

    // update limits
    SprBox::iterator iter = limits_.find(d_);
    if( iter == limits_.end() ) {
      pair<double,double> leftcut(SprUtils::min(),cut_);
      leftLims.insert(pair<const unsigned,pair<double,double> >(d_,leftcut));
      pair<double,double> rightcut(cut_,SprUtils::max());
      rightLims.insert(pair<const unsigned,pair<double,double> >(d_,rightcut));
    }
    else {
      leftLims[d_].second = cut_;
      rightLims[d_].first = cut_;
    }

    // make new nodes
    left_ = new SprTreeNode(crit_,*data_,allLeafsSignal_,nmin_,
			    discrete_,canHavePureNodes_,cls0_,cls1_,
			    this,d_,SprUtils::upperBound(cut_),leftLims,
			    //    w0_-w0best[d_],w1_-w1best[d_],
			    //    n0_-n0best[d_],n1_-n1best[d_],
			    bootstrap_);
    right_ = new SprTreeNode(crit_,*data_,allLeafsSignal_,nmin_,
			     discrete_,canHavePureNodes_,cls0_,cls1_,
			     this,d_,SprUtils::lowerBound(cut_),rightLims,
			     //			     w0best[d_],w1best[d_],
			     //			     n0best[d_],n1best[d_],
			     bootstrap_);

    // add new nodes to the split list
    nodesToSplit.push_back(left_);
    nodesToSplit.push_back(right_);

    // update split counter
    if( countTreeSplits.size() == data_->dim() ) {
      countTreeSplits[d_].first++;
      countTreeSplits[d_].second += (newFom - fom_);
    }

    // exit
    return this->prepareExit(true);
  }

  // message
  if( verbose > 2 ) {
    cout << "Failed to split node " << id_ 
	 << " with " << n1_ << " signal and " 
	 << n0_ << " background events." << endl;
  }
  if( verbose > 3 )
    cout << "===================" << endl;

  // exit
  return this->prepareExit(true);
}


bool SprTreeNode::sort(unsigned d, std::vector<int>& sorted,
		       std::vector<double>& division)
{
  // init
  assert( d < data_->dim() );
  int size = data_->size();
  sorted.clear();
  sorted.resize(size,-1);
  division.clear();

  // prepare vector
  vector<pair<double,int> > r(size);
  
  // loop through points
  for( int j=0;j<size;j++ )
    r[j] = pair<double,int>((*data_)[j]->x_[d],j);
  
  // sort
  SprSort(r.begin(),r.end(),STNCmpPairFirst());
  
  // fill out sorted indices
  double xprev = r[0].first;
  sorted[0] = r[0].second;
  for( int j=1;j<size;j++ ) {
    sorted[j] = r[j].second;
    double xcurr = r[j].first;
    if( (xcurr-xprev) > SprUtils::eps() ) {
      division.push_back(0.5*(xcurr+xprev));
      xprev = xcurr;
    }
  }

  // exit
  return true;
}


bool SprTreeNode::prepareExit(bool status)
{
  delete data_;
  data_ = 0;
  return status;
}


SprTrainedNode* SprTreeNode::makeTrained() const
{
  SprTrainedNode* t = new SprTrainedNode;
  t->id_ = id_;
  if( discrete_ )
    t->score_ = nodeClass_;
  else {
    assert( (w0_+w1_) > 0 );
    t->score_ = w1_/(w0_+w1_);
  }
  t->d_ = d_;
  t->cut_ = cut_;
  return t;
}


bool SprTreeNode::setClasses(const SprClass& cls0, const SprClass& cls1)
{
  if( left_!=0 || right_!=0 ) {
    cerr << "Unable to reset classes for the tree node with daughters." 
	 << endl;
    return false;
  }
  cls0_ = cls0; 
  cls1_ = cls1;
  cout << "Classes for tree node reset to " << cls0_ << " " << cls1_ << endl;
  return true;
}
