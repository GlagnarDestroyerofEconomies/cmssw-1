

#include "FWCore/PluginManager/interface/ProblemTracker.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"

#include <string>

namespace edm
{

  // -----------------------------------------------

  bool ProblemTracker::dead_ = true;
  //edmplugin::DebugAids::AssertHook ProblemTracker::old_assert_hook_ = 0;

  ProblemTracker::ProblemTracker()
  {
    dead_ = false;
    //old_assert_hook_ = edmplugin::DebugAids::failHook(&failure);
    if(not edmplugin::PluginManager::isAvailable()) {
      edmplugin::PluginManager::Config config(edmplugin::standard::config());

      edmplugin::PluginManager::configure(config);
    }
  }

  ProblemTracker::~ProblemTracker()
  {
    // since this is a singleton, we will not restore the old handle
    dead_ = true;
  }

  ProblemTracker* ProblemTracker::instance()
  {
    static ProblemTracker pt;
    return &pt;
  }

  // ---------------------------------------------

  AssertHandler::AssertHandler():pt_(ProblemTracker::instance())
  {
  }

  AssertHandler::~AssertHandler()
  {
  }


}
