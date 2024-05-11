#include "log/log.h"
#include "errors/errors.h"

namespace Pathlib::Errors {

/**/
bool log_on_error(Error error)
{
  if (error < ERROR_NONE) {
    //LOG.error(L"Error: %S.", eng_error_strings[-error]);
    return true;
  }
  return false;
}

/**/
bool popup_on_error(Error error)
{
  if (error < ERROR_NONE) {
    // TODO: Print the 'additional error info' string.
    //LOG.error(L"Error: %S.", eng_error_strings[-error]);
    // wnd_msg(L"Error: %S.", error_strings[-error]);
    return true;
  }
  return false;
}
} 