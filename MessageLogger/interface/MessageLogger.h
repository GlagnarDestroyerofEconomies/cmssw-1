#ifndef MessageLogger_MessageLogger_h
#define MessageLogger_MessageLogger_h

// -*- C++ -*-
//
// Package:     MessageLogger
// Class  :     <none>
// Functions:   LogError, LogWarning, LogInfo, LogDebug
//

//
// Original Author:  W. Brown and M. Fischler
//         Created:  Fri Nov 11 16:38:19 CST 2005
//     Major Split:  Tue Feb 14 11:00:00 CST 2006
//		     See MessageService/interface/MessageLogger.h
// $Id$
//

// system include files

#include <memory>
#include <string>

// user include files

// forward declarations

#include "FWCore/MessageLogger/interface/MessageSender.h"
#include "FWCore/MessageLogger/interface/MessageDrop.h"
#include "FWCore/MessageLogger/interface/ErrorObj.h"

namespace edm  {

class LogWarning
{
public:
  explicit LogWarning( std::string const & id ) 
    : ap( new MessageSender(ELwarning,id) )
  { }

  template< class T >
    LogWarning & 
    operator<< (T const & t)  { (*ap) << t; return *this; }
  LogWarning & 
  operator<< ( std::ostream&(*f)(std::ostream&))  
    				      { (*ap) << f; return *this; }
  LogWarning & 
  operator<< ( std::ios_base&(*f)(std::ios_base&) )  
    				      { (*ap) << f; return *this; }     
private:
  std::auto_ptr<MessageSender> ap; 
  
};  // LogWarning

class LogError
{
public:
  explicit LogError( std::string const & id ) 
    : ap( new MessageSender(ELerror,id) )
  { }

  template< class T >
    LogError & 
    operator<< (T const & t)  { (*ap) << t; return *this; }
  LogError & 
  operator<< ( std::ostream&(*f)(std::ostream&))  
    				      { (*ap) << f; return *this; }
  LogError & 
  operator<< ( std::ios_base&(*f)(std::ios_base&) )  
    				      { (*ap) << f; return *this; }     

private:
  std::auto_ptr<MessageSender> ap; 

};  // LogError


class LogInfo
{
public:
  explicit LogInfo( std::string const & id ) 
    : ap( new MessageSender(ELinfo,id) )
  { }

  template< class T >
    LogInfo & 
    operator<< (T const & t)  { (*ap) << t; return *this; }
  LogInfo & 
  operator<< ( std::ostream&(*f)(std::ostream&))  
    				      { (*ap) << f; return *this; }
  LogInfo & 
  operator<< ( std::ios_base&(*f)(std::ios_base&) )  
    				      { (*ap) << f; return *this; }     

private:
  std::auto_ptr<MessageSender> ap; 
  
};  // LogInfo

static 
std::string
onlyLowestDirectory(const std::string & file) {
  std::string::size_type lastSlash = file.find_last_of('/');
  if (lastSlash == std::string::npos) return file;
  if (lastSlash == file.size()-1)     return file;
  return file.substr(lastSlash+1, file.size()-lastSlash-1);
}

void LogStatistics(); 

class LogDebug_
{
public:
  explicit LogDebug_( std::string const & id, std::string const & file, int line ) 
    : ap( new MessageSender(ELsuccess,id) )
  { *this << onlyLowestDirectory(file) << ":" << line << "\n"; }

  template< class T >
    LogDebug_ & 
    operator<< (T const & t)  { (*ap) << t; return *this; }
  LogDebug_ & 
  operator<< ( std::ostream&(*f)(std::ostream&))  
    				      { (*ap) << f; return *this; }
  LogDebug_ & 
  operator<< ( std::ios_base&(*f)(std::ios_base&) )  
    				      { (*ap) << f; return *this; }     

private:
  std::auto_ptr<MessageSender> ap; 

};  // LogDebug_

class Suppress_LogDebug_ 
{ 
  // With any decent optimization, use of Suppress_LogDebug_ (...)
  // including streaming of items to it via operator<<
  // will produce absolutely no executable code.
public:
  template< class T >
    Suppress_LogDebug_ & 
    operator<< (T const & t)  { return *this; }
};  // Suppress_LogDebug_

}  // namespace edm


// If ML_DEBUG is defined, LogDebug is active.  
// Otherwise, LogDebug is supressed if either ML_NDEBUG or NDEBUG is defined.
#undef EDM_MESSAGELOGGER_SUPPRESS_LOGDEBUG
#ifdef NDEBUG
#define EDM_MESSAGELOGGER_SUPPRESS_LOGDEBUG
#endif
#ifdef ML_NDEBUG
#define EDM_MESSAGELOGGER_SUPPRESS_LOGDEBUG
#endif
#ifdef ML_DEBUG
#undef EDM_MESSAGELOGGER_SUPPRESS_LOGDEBUG
#endif

#ifdef EDM_MESSAGELOGGER_SUPPRESS_LOGDEBUG 
#define LogDebug(id) edm::Suppress_LogDebug_();
#else
#define LogDebug(id)                                            \
  if (edm::MessageDrop::instance()->debugEnabled )               \
          edm::LogDebug_(id, __FILE__, __LINE__)                               
#endif
#undef EDM_MESSAGELOGGER_SUPPRESS_LOGDEBUG

#endif  // MessageLogger_MessageLogger_h

