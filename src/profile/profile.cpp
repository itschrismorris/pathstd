#include "pathlib/pathlib.h"
#include "mimalloc/mimalloc.h"

//---
namespace Pathlib { _Internal::Profile profile; }

namespace Pathlib::_Internal {

//---
bool Profile::initiate()
{
  LOGT(u8"Initiating profiling.");
  #ifdef _DEBUG
    mi_option_enable(mi_option_verbose);
  #endif
  return true;
}

//---
void Profile::shutdown()
{
  LOGT(u8"Shutting down profiling.");
}
}