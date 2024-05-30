#include "pathlib/pathlib.h"
#include "../src/third_party/mimalloc/mimalloc.h"

//---
namespace Pathlib { _Internal::Profile profile; }

namespace Pathlib::_Internal {

//---
bool Profile::initiate()
{
  log.logt(u8"Initiating profiling.");
  #ifdef _DEBUG
    mi_option_enable(mi_option_verbose);
  #endif
  return true;
}

//---
void Profile::shutdown()
{
  log.logt(u8"Shutting down profiling.");
}
}