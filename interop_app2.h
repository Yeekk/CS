// i_interop_app2:
//  This is complete interaface that is necessary to control cloth simulator application.
//  Application instance is created automatically and returned by _ctxapp().

#ifndef _interop_app2_H
#define _interop_app2_H

#include "interop_app1.h"
#include "bmdx_cpiomt.h"

  // Implemented in particleapp.cpp.
struct i_interop_app2 : i_interop_app1
{
    // Codes from Qt::KeyboardModifier and Qt::MouseButtons.
  enum ECtrlKeys
  {
    Shift = 0x02000000,  Ctrl = 0x04000000, Alt = 0x08000000, _MaskPrefixKeys = 0x0e000000,
    MButLeft = 0x00000001, MButRight = 0x00000002, MButMid = 0x00000004, _MaskMBut = 0x7,
  };

    // Codes from Qt key defs.
  enum EKey
  {
      // 7 bit printable ASCII.
    Key_Space = 0x20,                 Key_Exclam = 0x21, Key_QuoteDbl = 0x22, Key_NumberSign = 0x23, Key_Dollar = 0x24, Key_Percent = 0x25, Key_Ampersand = 0x26, Key_Apostrophe = 0x27, Key_ParenLeft = 0x28, Key_ParenRight = 0x29, Key_Asterisk = 0x2a, Key_Plus = 0x2b, Key_Comma = 0x2c, Key_Minus = 0x2d, Key_Period = 0x2e, Key_Slash = 0x2f, Key_0 = 0x30, Key_1 = 0x31, Key_2 = 0x32, Key_3 = 0x33, Key_4 = 0x34, Key_5 = 0x35, Key_6 = 0x36, Key_7 = 0x37, Key_8 = 0x38, Key_9 = 0x39, Key_Colon = 0x3a, Key_Semicolon = 0x3b, Key_Less = 0x3c, Key_Equal = 0x3d, Key_Greater = 0x3e, Key_Question = 0x3f, Key_At = 0x40, Key_A = 0x41, Key_B = 0x42, Key_C = 0x43, Key_D = 0x44, Key_E = 0x45, Key_F = 0x46, Key_G = 0x47, Key_H = 0x48, Key_I = 0x49, Key_J = 0x4a, Key_K = 0x4b, Key_L = 0x4c, Key_M = 0x4d, Key_N = 0x4e, Key_O = 0x4f, Key_P = 0x50, Key_Q = 0x51, Key_R = 0x52, Key_S = 0x53, Key_T = 0x54, Key_U = 0x55, Key_V = 0x56, Key_W = 0x57, Key_X = 0x58, Key_Y = 0x59, Key_Z = 0x5a, Key_BracketLeft = 0x5b, Key_Backslash = 0x5c, Key_BracketRight = 0x5d, Key_AsciiCircum = 0x5e, Key_Underscore = 0x5f, Key_QuoteLeft = 0x60, Key_BraceLeft = 0x7b, Key_Bar = 0x7c, Key_BraceRight = 0x7d, Key_AsciiTilde = 0x7e,

      // Function and special keys.
    Key_Escape = 0x01000000, Key_Tab = 0x01000001, Key_Backtab = 0x01000002, Key_Backspace = 0x01000003, Key_Return = 0x01000004, Key_Enter = 0x01000005, Key_Insert = 0x01000006, Key_Delete = 0x01000007, Key_Pause = 0x01000008, Key_Print = 0x01000009, Key_SysReq = 0x0100000a, Key_Clear = 0x0100000b, Key_Home = 0x01000010, Key_End = 0x01000011, Key_Left = 0x01000012, Key_Up = 0x01000013, Key_Right = 0x01000014, Key_Down = 0x01000015, Key_PageUp = 0x01000016, Key_PageDown = 0x01000017, Key_CapsLock = 0x01000024, Key_NumLock = 0x01000025, Key_ScrollLock = 0x01000026,
    Key_F1 = 0x01000030, Key_F2 = 0x01000031, Key_F3 = 0x01000032, Key_F4 = 0x01000033, Key_F5 = 0x01000034, Key_F6 = 0x01000035, Key_F7 = 0x01000036, Key_F8 = 0x01000037, Key_F9 = 0x01000038, Key_F10 = 0x01000039, Key_F11 = 0x0100003a, Key_F12 = 0x0100003b,
    Key_Menu = 0x01000055,

      // Prefix keys for tracking keyup/keydown events.
    Key_Shift = 0x01000020, Key_Control = 0x01000021, Key_Meta = 0x01000022, Key_Alt = 0x01000023
  };

  struct EvMouse
  {
    pixelpoint pos; // event position, related to widget client area
    pixelrect wrect; // widget client area, in virtual desktop coords.
    pixelrect vscreen; // virtual screen rect., in virtual desktop coords.
    pixelrect ev_screen; // monitor rect. on which the event occurred, in virtual desktop coords.
    int mbut; // mouse buttons currently pressed (ECtrlKeys)
    int mbrel; // mouse buttons just released (ECtrlKeys)
    int kbpref; // keyboard prefix keys pressed (ECtrlKeys)
    EvMouse() : mbut(0), kbpref(0) {}
    pixelpoint vpos() { return pixelpoint(wrect.topleft.x + pos.x, wrect.topleft.y + pos.y); } // event position, in virtual desktop coords.
  };
  struct EvKbd
  {
    int key; // key pressed (EKey)
    int kbpref; // keyboard prefix keys pressed (ECtrlKeys)
    EvKbd() : key(0), kbpref(0) {}
  };


    // Functions for calling from application main function.
    //    notify_app_create once creates single instance of the application.
    //    notify_app_quit notifies about program exit nearing (i.e. notify_app_destroy will follow).
    //      b_app_quit becomes true.
    //    notify_app_destroy deletes the application instance.
    //    notify_tick_1ms performs GL simulation steps and rendering according to fixed FPS.
  virtual void notify_app_create() const = 0;
  virtual void notify_app_quit_intended() const = 0;
  virtual bool b_app_quit() const = 0;
  virtual void notify_app_destroy() const = 0;
  virtual void notify_tick_1ms() const = 0;

    // Functions for calling from main view (i.e. from GUI impl.).
  virtual void notify_mainview_GL_initialize() const = 0;
  virtual void notify_mainview_GL_paint() const = 0;
  virtual void notify_mainview_GL_resize(int w, int h) const = 0;
  virtual void notify_mainview_mouse_button_down(i_interop_app2::EvMouse e) const = 0;
  virtual void notify_mainview_mouse_button_up(i_interop_app2::EvMouse e) const = 0;
  virtual void notify_mainview_mouse_moved(i_interop_app2::EvMouse e) const = 0;
  virtual void notify_mainview_mouse_entered() const = 0;
  virtual void notify_mainview_mouse_left() const = 0;
  virtual void notify_mainview_key_down(i_interop_app2::EvKbd e) const = 0;
  virtual void notify_mainview_key_up(i_interop_app2::EvKbd e) const = 0;

    // Variables, related to live performance tracking and information display.
  virtual double  fps() const = 0;
  virtual bmdx_meta::s_ll  ndropped() const = 0;
  virtual bmdx_meta::s_ll  t_sim_ms() const = 0;
  virtual bmdx_meta::s_ll  t_render_ms() const = 0;
  virtual int  n_particles() const = 0;
    // ORed flags of user toggling display items:
    //  0x1 - (i) number of particles and hit-test info
    //  0x2 - (h) help text
    //  0x4 - (l) links between particles
    //  0x8 - (v) particle velocity vectors
    //  0x10 - (p) particles
    //  0x20 - (s) surfaces
  virtual int  dspl_flags() const = 0;
  virtual std::string scene_name() const = 0;
  virtual std::string info_helpmsg() const = 0;
  virtual std::string info_time_line() const = 0;
  virtual std::string info_numptcl_line() const = 0;
};

  // Single instance of the main application.
  //    Implemented in particleapp.cpp.
bmdx::cref_t<i_interop_app2> _ctxapp();

  // The current physical simulator instance ("world"), that is the single part of single application instance.
  //    Implemented in particleapp.cpp.
  //  The returned world is recreated (reference changes) each time the user enables some new scene.
  //  On error (no world currently exists), _ctxptclsim returns an empty reference.
namespace _interop_ptclsim { namespace decl { struct i_interop_ptclsim; } }
bmdx::cref_t<_interop_ptclsim::decl::i_interop_ptclsim> _ctxptclsim();

#endif
