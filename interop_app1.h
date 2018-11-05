// i_interop_app1:
//  this is minimal possible interface that is exposed by main application to physics simulator.
//  It is passed by reference into i_interop_ptclsim::simStep() function.
//  Physics simulator may be used by any kind of application, even non-GUI or shared library,
//  if this application/library implements i_interop_app1.

#ifndef _interop_app1_H
#define _interop_app1_H

#include "interop_base.h"

#if defined(GLM_INCLUDED) && !defined(GLM_FORCE_RADIANS)
  #error "glm must be initially included with GLM_FORCE_RADIANS defined"
#endif
#if !defined(GLM_INCLUDED)
  #undef GLM_FORCE_RADIANS
  #define GLM_FORCE_RADIANS
#endif
#include <glm.hpp>

struct i_interop_app1 : i_interop_base
{
  virtual glm::mat4 maincam_getProjectionViewMatrix() const = 0;
  virtual pixelrect appview_geometry() const = 0;
  virtual pixelpoint appview_mouse_pos_rel() const = 0;
  virtual EMouseAction appview_mouse_action() const = 0;
};

#endif
