#ifndef Common_RefToBaseVector_h
#define Common_RefToBaseVector_h
/**\class RefToBaseVector
 *
 * \author Luca Lista, INFN
 *
 */

#include "DataFormats/Common/interface/RefToBase.h"
#include "FWCore/Utilities/interface/EDMException.h"

namespace edm {
  namespace reftobase {
    template <class T>
    class BaseVectorHolder {
    public:
      BaseVectorHolder() {}
      virtual ~BaseVectorHolder() {}
      virtual BaseVectorHolder<T>* clone() const = 0;
      virtual RefToBase<T> const at(unsigned int idx) const = 0;
      virtual bool empty() const = 0;
      virtual unsigned int size() const = 0;
      virtual unsigned int capacity() const = 0;
      virtual void reserve(unsigned int n) = 0;
      virtual void clear() = 0;
      virtual ProductID id() const = 0;
    protected:
      struct const_iterator_imp {
	typedef ptrdiff_t difference_type;
	const_iterator_imp() { } 
	virtual ~const_iterator_imp() { }
	virtual const_iterator_imp * clone() const = 0;
	virtual void increase() = 0;
	virtual void decrease() = 0;
	virtual void increase( difference_type d ) = 0;
	virtual void decrease( difference_type d ) = 0;
	virtual bool equal_to( const const_iterator_imp * ) const = 0;
	virtual bool less_than( const const_iterator_imp * ) const = 0;
	virtual void assign( const const_iterator_imp * ) = 0;
	virtual const T & deref() const = 0;
	virtual difference_type difference( const const_iterator_imp * ) const = 0;
      };
    public:
      struct const_iterator {
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;
	typedef ptrdiff_t difference_type;
	const_iterator() { }
	const_iterator( const_iterator_imp * it ) : i( it ) { }
	const_iterator( const const_iterator & it ) : i( it.i->clone() ) { }
	~const_iterator() { delete i; }
	const_iterator & operator=( const const_iterator & it ) { i->assign( it.i ); return *this; }
	const_iterator& operator++() { i->increase(); return *this; }
	const_iterator operator++( int ) { const_iterator ci = *this; i->increase(); return ci; }
	const_iterator& operator--() { i->decrease(); return *this; }
	const_iterator operator--( int ) { const_iterator ci = *this; i->decrease(); return ci; }
	difference_type operator-( const const_iterator & o ) const { return i->difference( o.i ); }
	const_iterator operator+( difference_type n ) const { 
	  const_iterator_imp * ii = i->clone(); ii->increase( n );
	  return const_iterator( ii ); 
	}
	const_iterator operator-( difference_type n ) const { 
	  const_iterator_imp * ii = i->clone(); ii->decrease( n );
	  return const_iterator( ii ); 
	}
	bool operator<( const const_iterator & o ) const { return i->less_than( o.i ); }
	bool operator==( const const_iterator& ci ) const { return i->equal_to( ci.i ); }
	bool operator!=( const const_iterator& ci ) const { return ! i->equal_to( ci.i ); }
	const T & operator * () const { return i->deref(); }
	const T * operator->() const { return & ( operator*() ); }
	const_iterator & operator +=( difference_type d ) { i->increase( d ); return *this; }
	const_iterator & operator -=( difference_type d ) { i->decrease( d ); return *this; }
      private:
	const_iterator_imp * i;
      };

      virtual const_iterator begin() const = 0;
      virtual const_iterator end() const = 0;
    };

    template <class T, class TRefVector>
    class VectorHolder : public BaseVectorHolder<T> {
    public:
      VectorHolder() {}
      explicit VectorHolder(const TRefVector& iRefVector) : refVector_(iRefVector) {}
      virtual ~VectorHolder() {}
      virtual BaseVectorHolder<T>* clone() const { return new VectorHolder<T,TRefVector>(*this); }
      RefToBase<T> const at(unsigned int idx) const { return RefToBase<T>( refVector_.at( idx ) ); }
      bool empty() const { return refVector_.empty(); }
      unsigned int size() const { return refVector_.size(); }
      unsigned int capacity() const { return refVector_.capacity(); }
      void reserve(unsigned int n) { refVector_.reserve(n); }
      void clear() { refVector_.clear(); }
      ProductID id() const { return refVector_.id(); } 
      typedef typename BaseVectorHolder<T>::const_iterator const_iterator;
      const_iterator begin() const { 
	return const_iterator( new const_iterator_imp_specific( refVector_.begin() ) ); 
      }
      const_iterator end() const { 
	return const_iterator( new const_iterator_imp_specific( refVector_.end() ) ); 
      }

    private:
      TRefVector refVector_;
      typedef typename BaseVectorHolder<T>::const_iterator_imp const_iterator_imp;
      struct const_iterator_imp_specific : public const_iterator_imp {
	typedef ptrdiff_t difference_type;
	const_iterator_imp_specific() { }
	explicit const_iterator_imp_specific( const typename TRefVector::const_iterator & it ) : i ( it ) { }
	~const_iterator_imp_specific() { }
	const_iterator_imp_specific * clone() const { return new const_iterator_imp_specific( i ); }
	void increase() { ++i; }
	void decrease() { --i; }
	void increase( difference_type d ) { i += d; }
	void decrease( difference_type d ) { i -= d; }
	bool equal_to( const const_iterator_imp * o ) const { return i == dc( o ); }
	bool less_than( const const_iterator_imp * o ) const { return i < dc( o ); }
	void assign( const const_iterator_imp * o ) { i = dc( o ); }
	const T & deref() const { return * * i; }
	difference_type difference( const const_iterator_imp * o ) const { return i - dc( o ); }
      private:
	const typename TRefVector::const_iterator & dc( const const_iterator_imp * o ) const {
	  const const_iterator_imp_specific * oo = dynamic_cast<const const_iterator_imp_specific *>( o );
	  if ( oo == 0 ) 
	    throw edm::Exception( edm::errors::InvalidReference ) 
	      << "In RefToBaseVector<T> trying to cast iterator to wrong type ";
	  return oo->i;
	}    
	typename TRefVector::const_iterator i;
      };
    };

  }

  //--------------------------------------------------------------------
  //
  // Class template RefToBaseVector<T>
  //
  //--------------------------------------------------------------------

  /// RefToBaseVector<T> provides ... ?

  template <class T>
  class RefToBaseVector {
  public:
    typedef RefToBase<T>   value_type;
    typedef T              member_type;
    typedef unsigned int   size_type;
    typedef typename reftobase::BaseVectorHolder<T>::const_iterator const_iterator;

    RefToBaseVector();

    template <class TRefVector> explicit RefToBaseVector(TRefVector const& iRef);

    RefToBaseVector(RefToBaseVector const& iOther);

    RefToBaseVector& operator=(RefToBaseVector const& iRHS);

    void swap(RefToBaseVector& other);

    ~RefToBaseVector();

    value_type const at(size_type idx) const;

    value_type const operator[](size_type idx) const;

    bool empty() const;

    size_type size() const;

    size_type capacity() const;

    void reserve(unsigned int n);

    void clear();

    ProductID id() const;

    const_iterator begin() const;

    const_iterator end() const;

  private:
    reftobase::BaseVectorHolder<T>* holder_;
  };
  
  template <class T>
  inline
  void
  swap(RefToBaseVector<T>& a, RefToBaseVector<T>& b) {
    a.swap(b);
  }

  //--------------------------------------------------------------------
  // Implementation of RefToBaseVector<T>
  //--------------------------------------------------------------------
  
  template <class T>
  inline
  RefToBaseVector<T>::RefToBaseVector() : 
    holder_(0) 
  { }

  template <class T>
  template <class TRefVector>
  inline
  RefToBaseVector<T>::RefToBaseVector(const TRefVector& iRef) :
    holder_(new reftobase::VectorHolder<T,TRefVector>(iRef)) 
  { }

  template <class T>
  inline
  RefToBaseVector<T>::RefToBaseVector(const RefToBaseVector<T>& iOther) : 
    holder_((0==iOther.holder_) 
	    ? static_cast<reftobase::BaseVectorHolder<T>*>(0) 
	    : iOther.holder_->clone()) 
  { }

  template <class T>
  inline
  RefToBaseVector<T>& 
  RefToBaseVector<T>::operator=(const RefToBaseVector& iRHS) {
    RefToBaseVector temp(iRHS);
    this->swap(temp);
    return *this;
  }

  template <class T>
  inline
  void
  RefToBaseVector<T>::swap(RefToBaseVector& other) {
    std::swap(holder_, other.holder_);
  }

  template <class T>
  inline
  RefToBaseVector<T>::~RefToBaseVector() 
  {
    delete holder_; 
  }

  template <class T>
  inline
  typename RefToBaseVector<T>::value_type const
  RefToBaseVector<T>::at(size_type idx) const 
  {
    return holder_->at( idx );
  }

  template <class T>
  inline
  typename RefToBaseVector<T>::value_type const
  RefToBaseVector<T>::operator[](size_type idx) const 
  {
    return at( idx ); 
  }

  template <class T>
  inline
  bool 
  RefToBaseVector<T>::empty() const 
  {
    return holder_->empty(); 
  }

  template <class T>
  inline
  typename RefToBaseVector<T>::size_type
  RefToBaseVector<T>::size() const 
  {
    return holder_->size(); 
  }

  template <class T>
  inline
  typename RefToBaseVector<T>::size_type
  RefToBaseVector<T>::capacity() const 
  {
    return holder_->capacity();
  }


  template <class T>
  inline
  void 
  RefToBaseVector<T>::reserve(unsigned int n)
  {
    holder_->reserve(n);
  }

  template <class T>
  inline
  void 
  RefToBaseVector<T>::clear()
  {
    holder_->clear();
  }

  template <class T>
  inline
  ProductID
  RefToBaseVector<T>::id() const
  {
    return holder_->id();
  }

  template <class T>
  inline
  typename RefToBaseVector<T>::const_iterator
  RefToBaseVector<T>::begin() const
  {
    return holder_->begin();
  }

  template <class T>
  inline
  typename RefToBaseVector<T>::const_iterator
  RefToBaseVector<T>::end() const
  {
    return holder_->end();
  }

}

#endif
