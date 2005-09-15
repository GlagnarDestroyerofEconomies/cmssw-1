
#include "IOPool/Streamer/interface/ClassFiller.h"
#include "IOPool/Streamer/interface/StreamedProducts.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/ProductRegistry.h"
#include "FWCore/Framework/interface/ProductDescription.h"
#include "FWCore/Framework/src/DebugMacros.h"

#include "PluginManager/PluginCapabilities.h"
#include "RootStorageSvc/CINTType.h"
#include "StorageSvc/IOODatabaseFactory.h"
#include "StorageSvc/IClassLoader.h"
#include "StorageSvc/DbType.h"

#include "Reflection/Class.h"
#include "TClass.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <iterator>

using namespace std;

namespace edm {

  TClass* loadClass(pool::IClassLoader* cl, const std::type_info& ti);

  // ---------------------
  pool::IClassLoader* getClassLoader()
  {
    pool::IOODatabaseFactory* dbf = pool::IOODatabaseFactory::get();
    pool::IOODatabase* db=dbf->create(pool::ROOT_StorageType.storageName());

    if(db==0)
      {
        throw cms::Exception("Configuration","EventStreamerImpl")
          << "could not get the IOODatabase from the IOODatabaseFactory\n"
          << "for storageName = " << pool::ROOT_StorageType.storageName()
          << "\n";
      }

    pool::IClassLoader* cl = db->classLoader();
    
    if(cl==0)
      {
        throw cms::Exception("Configuration","EventStreamerImpl")
          << "could not get the classloader from the IOODatabase\n";
      }

    return cl;
  }

  // ---------------------
  void fillChildren(pool::IClassLoader* cl, const seal::reflect::Class* cc)
  {
    FDEBUG(9) << "JBK: parent - " << cc->fullName() << endl;

    if(cc->isContainer())
      {
        const seal::reflect::Class* ct = cc->getComponentType();
	if(ct->isPrimitive()==false) fillChildren(cl,ct);
      }

    std::vector<const seal::reflect::Field*> fs =
      cc->fields(seal::reflect::PRIVATE);
    std::vector<const seal::reflect::Field*> fs2 =
      cc->fields(seal::reflect::PUBLIC);
    copy(fs2.begin(),fs2.end(),back_inserter(fs));

    std::vector<const seal::reflect::Field*>::iterator
      beg(fs.begin()),end(fs.end());

    for(;beg!=end;++beg)
      {
	const seal::reflect::Class* ft = (*beg)->type();
	
	// check for isPrimitive() here
	if(ft->isPrimitive()) continue;

	if(ft==0)
	  {
	    cerr << "Error: could not find Class object for "
		 << ft->fullName() << endl;
	    return;
	  }

        FDEBUG(9) << "JBK: child - " << ft->fullName() << endl;
	fillChildren(cl, ft);

	
      }

    if(cl->loadClass(cc->fullName())!=pool::DbStatus::SUCCESS)
      {
	cerr << "Error: could not loadClass for " << cc->fullName() << endl;
	return;
      }

    TClass* ttest = TClass::GetClass(cc->fullName().c_str());

    if(ttest==0)
      {
        cerr << "EventStreamImpl: "
             << "Could not get the TClass for " << cc->fullName()
             << endl;
	return;
      }

    FDEBUG(9) << "JBK: parent complete loadClass - " << cc->fullName() << endl;

  }


  static void loadCap(const string& name)
  {
   // string name = i->second.fullClassName_;
	
   string fname("LCGDict/");
   fname+=name;
   FDEBUG(7) << "JBK: cap loading " << fname << endl;
   seal::PluginCapabilities::get()->load(fname);
	
   const seal::reflect::Class* cc = seal::reflect::Class::forName(name);

   if(cc==0)
     {
       cerr << "Error: could not find Class object for " << name << endl;
       return;
     }
  }

  // ---------------------
  void fillStreamers(ProductRegistry const& reg)
  {
    FDEBUG(5) << "In fillStreamers" << endl;
    pool::IClassLoader* cl = getClassLoader();

    typedef ProductRegistry::ProductList Prods;
    const Prods& prods = reg.productList();
    Prods::const_iterator i(prods.begin()),e(prods.end());

    // first go through and load all capabilities

    for(;i!=e;++i)
      {
        loadCap(i->second.fullClassName_);
      }

    for(i=prods.begin();i!=e;++i)
      {
	string name = i->second.fullClassName_;
	FDEBUG(7) << "JBK: class loading " << name << endl;
	const seal::reflect::Class* cc = seal::reflect::Class::forName(name);

        if(cc==0)
	  {
	    cerr << "Error: could not find Class object for " << name << endl;
	    continue;
	  }

        fillChildren(cl,cc);
      }
  }

#if 0
  string prodpair_name("LCGDict/edm::ProdPair");
  string vprodpair_name("LCGDict/std::vector<edm::ProdPair>");
  string sendevent_name("LCGDict/edm::SendEvent");
#endif


  namespace
  {
    seal::reflect::Class const * getReflectClass(const std::type_info& ti)
    {
      seal::reflect::Class const * typ = 
	seal::reflect::Class::forTypeinfo(ti);
      
      if(typ==0)
	{
	  throw cms::Exception("Configuration","getReflectClass")
	    << "could not find reflection class for "
	    << ti.name()
	    << "\n";
	}
      
      return typ;
    }

    TClass* getRootClass(const std::string& name)
    {
      TClass* tc = TClass::GetClass(name.c_str());    
      
      // get ROOT TClass for this product
      // CINT::Type* cint_type = CINT::Type::get(typ_ref);
      // tc_ = cint_type->rootClass();
      // TClass* tc = TClass::GetClass(typeid(se));
      // tc_ = TClass::GetClass("edm::SendEvent");
      
      if(tc==0)
	{
	  throw cms::Exception("Configuration","getRootClass")
	    << "could not find TClass for " << name
	    << "\n";
	}
      
      return tc;
    }
  }

  // ---------------------
  TClass* getTClass(const std::type_info& ti)
  {
    seal::reflect::Class const* typ = getReflectClass(ti);
    return getRootClass(typ->fullName());
  }
   
  // ---------------------
  TClass* loadClass(pool::IClassLoader* cl, const std::type_info& ti)
  {
    seal::reflect::Class const * typ = getReflectClass(ti);

    string fname("LCGDict/");
    fname+=typ->fullName();
    seal::PluginCapabilities::get()->load(fname);

    if(cl->loadClass(typ->fullName())!=pool::DbStatus::SUCCESS)
      {
        throw cms::Exception("Configuration","edm::loadClass")
          << "could not do loadClass for " << fname
          << "\n";
      }
    
    TClass* tc = getRootClass(typ->fullName()); 
    // cerr << "TClass name " << tc->GetName() << endl;

    return tc;
  }

  void loadExtraClasses()
  {
    pool::IClassLoader* cl = getClassLoader();
    loadClass(cl,typeid(ProdPair));
    loadClass(cl,typeid(SendProds));
    loadClass(cl,typeid(SendEvent));
    loadClass(cl,typeid(SendDescs));
    loadClass(cl,typeid(SendJobHeader));
    loadClass(cl,typeid(ProductDescription));
    loadClass(cl,typeid(BranchEntryDescription));
  }
  
  
}
