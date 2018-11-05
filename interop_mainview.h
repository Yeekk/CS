// i_interop_mainview, _ctxmainview():
//  must be implemented by main view class,
//  which is dependent on both GUI system used
//  and cloth simulator application (interop_app2.h).

#ifndef _interop_mainview_H
#define _interop_mainview_H

#include "interop_base.h"
#include "bmdx_cpiomt.h"
#include <string>

struct i_interop_mainview : i_interop_base
{
  virtual void notify_set_title(const std::string& title) const = 0;
  virtual void notify_set_cursor_v(pixelpoint vpos) const  = 0; // vpos in virt. desktop coords.
  virtual pixelrect geometry_view() const = 0;
};

bmdx::cref_t<i_interop_mainview> _ctxmainview();

#endif
