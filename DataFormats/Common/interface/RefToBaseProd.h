#ifndef DataFormats_Common_RefToBaseProd_h
#define DataFormats_Common_RefToBaseProd_h

/* \class edm::RefToBaseProd<T>
 *
 * \author Luca Lista, INFN
 *
 * $Id: RefToBaseProd.h,v 1.2 2007/07/23 11:08:55 llista Exp $
 *
 */
  
#include "DataFormats/Common/interface/EDProductfwd.h"
#include "DataFormats/Common/interface/RefCore.h"
#include "DataFormats/Provenance/interface/ProductID.h"
// circular dependency to be removed !!! L.L.
#include "DataFormats/Common/interface/View.h"

namespace edm {

  template <typename T>
  class RefToBaseProd {
  public:
    typedef View<T> product_type;

    /// Default constructor needed for reading from persistent store. Not for direct use.
    RefToBaseProd() : product_() {}

    /// General purpose constructor from handle-like object.
    // The templating is artificial.
    // HandleC must have the following methods:
    //   id(),      returning a ProductID,
    //   product(), returning a C*.
    template <class HandleC>
    explicit RefToBaseProd(HandleC const& handle);
    explicit RefToBaseProd(Handle<View<T> > const& handle);
    /// Constructor from Ref<C,T,F>
    template <typename C, typename F>
    explicit RefToBaseProd(Ref<C, T, F> const& ref);
    explicit RefToBaseProd( const View<T> & );
    RefToBaseProd( const RefToBaseProd<T> & );
    template<typename C>
    RefToBaseProd( const RefProd<C> & );

    /// Destructor
    ~RefToBaseProd() {}

    /// Dereference operator
    product_type const&  operator*() const;

    /// Member dereference operator
    product_type const* operator->() const;

    /// Returns C++ pointer to the product
    /// Will attempt to retrieve product
    product_type const* get() const {
      return isNull() ? 0 : this->operator->();
    }

    /// Returns C++ pointer to the product
    /// Will attempt to retrieve product
    product_type const* product() const {
      return isNull() ? 0 : this->operator->();
    }

    RefCore const& refCore() const {
      return product_;
    }

    /// Checks for null
    bool isNull() const {return !isNonnull(); }

    /// Checks for non-null
    bool isNonnull() const {return id().isValid(); }

    /// Checks for null
    bool operator!() const {return isNull(); }

    /// Accessor for product ID.
    ProductID id() const {return product_.id();}

    /// Accessor for product getter.
    EDProductGetter const* productGetter() const {return product_.productGetter();}

    /// Checks if product is in memory.
    bool hasCache() const {return product_.productPtr() != 0;}

    RefToBaseProd<T> & operator=( const RefToBaseProd<T> & other );

  private:
    RefCore product_;
    mutable std::auto_ptr<View<T> > view_;
  };
}

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefCoreGet.h"
#include "DataFormats/Common/interface/FillView.h"
#include "DataFormats/Common/interface/RefVectorHolder.h"
#include "DataFormats/Common/interface/RefVector.h"

namespace edm {

  namespace refhelper {
    template<typename C>
    struct RefToBaseProdTrait {
      typedef RefVector<C> ref_vector_type;
    };

    template<typename C>
    struct RefToBaseProdTrait<RefVector<C> > {
      typedef RefVector<C> ref_vector_type;
    };
  }

  template <typename T>
  template <class HandleC>
  inline
  RefToBaseProd<T>::RefToBaseProd(HandleC const& handle) :
    product_(handle.id(), handle.product(), 0) { 
    std::vector<void const*> pointers;
    typedef typename refhelper::RefToBaseProdTrait<typename HandleC::product_type>::ref_vector_type ref_vector;
    typedef reftobase::RefVectorHolder<ref_vector> holder_type;
    helper_vector_ptr helpers( new holder_type );
    detail::reallyFillView( * handle, handle.id(), pointers, * helpers );
    view_.reset( new View<T>( pointers, helpers ) );
  }

  template <typename T>
  inline
  RefToBaseProd<T>::RefToBaseProd(Handle<View<T> > const& handle) :
    product_(handle->id(), 0, handle->productGetter() ),
    view_( new View<T>( *handle ) ) {
   }


  /// Constructor from Ref.
  template <typename T>
  template <typename C, typename F>
  inline
  RefToBaseProd<T>::RefToBaseProd(Ref<C, T, F> const& ref) :
      product_(ref.id(), 
	       ref.hasProductCache() ? 
	       ref.product() : 
	       0, ref.productGetter()) {
    std::vector<void const*> pointers;
    typedef typename refhelper::RefToBaseProdTrait<C>::ref_vector_type ref_vector;
    typedef reftobase::RefVectorHolder<ref_vector> holder_type;
    helper_vector_ptr helpers( new holder_type );
    detail::reallyFillView( * ref.product(), ref.id(), pointers, * helpers );
    view_.reset( new View<T>( pointers, helpers ) );
  }

  template <typename T>
  inline
  RefToBaseProd<T>::RefToBaseProd( const View<T> & view ) :
    product_( view.id(), 0, view.productGetter() ),
    view_( new View<T>( view ) ) {
  }

  template <typename T>
  inline
  RefToBaseProd<T>::RefToBaseProd( const RefToBaseProd<T> & ref ) :
    product_( ref.product_ ),
    view_( ref.view_ ) {
  }

  template <typename T>
  template <typename C>
  inline
  RefToBaseProd<T>::RefToBaseProd( const RefProd<C> & ref ) :
    product_( ref.recCore() ) {
    std::vector<void const*> pointers;
    typedef typename refhelper::RefToBaseProdTrait<C>::ref_vector_type ref_vector;
    typedef reftobase::RefVectorHolder<ref_vector> holder_type;
    helper_vector_ptr helpers( new holder_type );
    detail::reallyFillView( * ref.product(), ref.id(), pointers, * helpers );
    view_.reset( new View<T>( pointers, helpers ) );
   
  }

  template <typename T>
  inline
  RefToBaseProd<T> & RefToBaseProd<T>::operator=( const RefToBaseProd<T> & other ) {
    product_ = other.product_;
    view_ = other.view_;
    return *this;
  }


  /// Dereference operator
  template <typename T>
  inline
  View<T> const& RefToBaseProd<T>::operator*() const {
    return * operator->();
  }

  /// Member dereference operator
  template <typename T>
  inline
  View<T> const* RefToBaseProd<T>::operator->() const {
    return view_.get();
  } 

  template <typename T>
  inline
  bool
  operator== (RefToBaseProd<T> const& lhs, RefToBaseProd<T> const& rhs) {
    return lhs.refCore() == rhs.refCore();
  }

  template <typename T>
  inline
  bool
  operator!= (RefToBaseProd<T> const& lhs, RefToBaseProd<T> const& rhs) {
    return !(lhs == rhs);
  }

  template <typename T>
  inline
  bool
  operator< (RefToBaseProd<T> const& lhs, RefToBaseProd<T> const& rhs) {
    return (lhs.refCore() < rhs.refCore());
  }
}
#endif
