#include "log/log.h"
#include "errors/errors.h"

namespace Pathlib::Errors {

/**/
bool error(Error error)
{
  if (error < 0) {
    // TODO: Print the 'additional error info' string.
    //LOG.error(L"Error: %S.", eng_error_strings[-error]);
    // wnd_msg(L"Error: %S.", error_strings[-error]);
    return true;
  }
  return false;
}
} 