#ifndef Candidate_ShallowCloneCandidate_h
#define Candidate_ShallowCloneCandidate_h
/** \class reco::ShallowCloneCandidate
 *
 * shallow clone of a particle candidate keepint a reference
 * to the master clone
 *
 * \author Luca Lista, INFN
 *
 * \version $Id: ShallowCloneCandidate.h,v 1.12 2007/10/15 11:51:26 llista Exp $
 *
 */
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/iterator_imp_specific.h"

namespace reco {
  class ShallowCloneCandidate : public Candidate {
  public:
    /// collection of daughter candidates
    typedef CandidateCollection daughters;
    /// default constructor
    ShallowCloneCandidate() : Candidate() {  }
    /// constructor from Particle
    explicit ShallowCloneCandidate( const CandidateBaseRef & masterClone ) : 
      Candidate( * masterClone ), 
      masterClone_( masterClone->hasMasterClone() ? 
		    masterClone->masterClone() : 
		    masterClone ) { 
    }
    /// constructor from values
    ShallowCloneCandidate( const CandidateBaseRef & masterClone, 
			   Charge q, const LorentzVector & p4, const Point & vtx = Point( 0, 0, 0 ) ) : 
      Candidate( q, p4, vtx ), masterClone_( masterClone ) { }
    /// constructor from values
    ShallowCloneCandidate( const CandidateBaseRef & masterClone, 
			   Charge q, const PolarLorentzVector & p4, const Point & vtx = Point( 0, 0, 0 ) ) : 
      Candidate( q, p4, vtx ), masterClone_( masterClone ) { }
    /// destructor
    virtual ~ShallowCloneCandidate();
    /// returns a clone of the Candidate object
    virtual ShallowCloneCandidate * clone() const;
    /// first daughter const_iterator
    virtual const_iterator begin() const;
    /// last daughter const_iterator
    virtual const_iterator end() const;
    /// first daughter iterator
    virtual iterator begin();
    /// last daughter iterator
    virtual iterator end();
    /// number of daughters
    virtual size_t numberOfDaughters() const;
    /// number of daughters
    virtual size_t numberOfMothers() const;
    /// return daughter at a given position (throws an exception)
    virtual const Candidate * daughter( size_type i ) const;
    /// return daughter at a given position (throws an exception)
    virtual const Candidate * mother( size_type i ) const;
    /// return daughter at a given position (throws an exception)
    virtual Candidate * daughter( size_type i );
    /// has master clone
    virtual bool hasMasterClone() const;
    /// returns reference to master clone
    virtual const CandidateBaseRef & masterClone() const;

  private:
    // const iterator implementation
    typedef candidate::const_iterator_imp_specific<daughters> const_iterator_imp_specific;
    // iterator implementation
    typedef candidate::iterator_imp_specific<daughters> iterator_imp_specific;
    /// check overlap with another Candidate
    virtual bool overlap( const Candidate & c ) const { return masterClone_->overlap( c ); }
    /// CandidateBaseReference to master clone
    CandidateBaseRef masterClone_;
  };

}

#endif
