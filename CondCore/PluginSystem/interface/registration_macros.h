#ifndef PLUGINSYSTEM_REGISTRATION_MACROS_H
#define PLUGINSYSTEM_REGISTRATION_MACROS_H
// -*- C++ -*-
//
// Package:     PluginSystem
// Class  :     registration_macros
// 
/**\class registration_macros registration_macros.h TestCondDB/PluginSystem/interface/registration_macros.h

 Description: CPP macros used to simplify registration of plugins

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Mon Jul 25 06:47:37 EDT 2005
//

// system include files

// user include files
#include "CondCore/PluginSystem/interface/ProxyFactory.h"
#include "CondCore/PluginSystem/interface/DataProxy.h"

// forward declarations

// macros
#define INSTANTIATE_PROXY(record_, type_) template class DataProxyWrapper<record_, type_>;

#define ONLY_REGISTER_PLUGIN(record_,type_)\
typedef DataProxyWrapper<record_, type_> EDM_PLUGIN_SYM(Proxy , __LINE__ ); \
DEFINE_EDM_PLUGIN( cond::ProxyFactory, EDM_PLUGIN_SYM(Proxy , __LINE__ ), #record_ "@NewProxy")


#define REGISTER_PLUGIN(record_, type_ ) \
INSTANTIATE_PROXY(record_, type_ ) \
ONLY_REGISTER_PLUGIN(record_, type_ )


// source_ is the record name of the keyed objects
#define REGISTER_KEYLIST_PLUGIN(record_, type_, source_) \
template class DataProxyWrapper<record_, type_>; \
struct EDM_PLUGIN_SYM(Proxy , record_ ) : public  DataProxyWrapper<record_, type_> { EDM_PLUGIN_SYM(Proxy , record_ ) (cond::DbSession& a, std::pair< std::string, std::string> const & b) :  DataProxyWrapper<record_, type_>(a,b.first,b.second, #source_){};}; \
DEFINE_EDM_PLUGIN( cond::ProxyFactory, EDM_PLUGIN_SYM(Proxy , record_ ), #record_ "@NewProxy") 


#endif /* PLUGINSYSTEM_REGISTRATION_MACROS_H */
