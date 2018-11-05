#include <exception>

#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>

#include "view.h"

using namespace bmdx;

struct exc_mainview_numobj : std:: exception { const char* what() const throw() { return "Main view: bad number of views."; } };


View* pmv = 0;

View::View(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent)
{
  if (1)
  {
    critsec_t<View> __lock(10, -1); if (sizeof(__lock)) {}
    if (pmv) { throw exc_mainview_numobj(); }
    pmv = this;
  }
  setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);
  connect(&_timer1ms, SIGNAL(timeout()), this, SLOT(tick1ms()));
}

View::~View()
{
  if (1)
  {
    critsec_t<View> __lock(10, -1); if (sizeof(__lock)) {}
    if (pmv == this) { pmv = 0; }
  }
}

void View::initializeGL()
{
  cref_t<i_interop_app2> app = _ctxapp();
  app.ref().notify_app_create();
  app.ref().notify_mainview_GL_initialize();
  _timer1ms.start(1);
}
void View::paintGL() { _ctxapp().ref().notify_mainview_GL_paint(); }
void View::resizeGL(int w, int h) { _ctxapp().ref().notify_mainview_GL_resize(w, h); }
void View::mouseMoveEvent(QMouseEvent* e) { if (!e) { return; } _ctxapp().ref().notify_mainview_mouse_moved(ev(*e)); }
void View::leaveEvent(QEvent *e) { if (!e) { return; } _ctxapp().ref().notify_mainview_mouse_left(); }
void View::enterEvent(QEvent *e) { if (!e) { return; } _ctxapp().ref().notify_mainview_mouse_entered(); }
void View::mousePressEvent(QMouseEvent *e) { if (!e) { return; } _ctxapp().ref().notify_mainview_mouse_button_down(ev(*e)); }
void View::mouseReleaseEvent(QMouseEvent *e) { if (!e) { return; } _ctxapp().ref().notify_mainview_mouse_button_up(ev(*e)); }
void View::keyPressEvent(QKeyEvent *e) { if (!e) { return; } _ctxapp().ref().notify_mainview_key_down(ev(*e)); }
void View::keyReleaseEvent(QKeyEvent *e) { if (!e) { return; } _ctxapp().ref().notify_mainview_key_up(ev(*e)); }

void View::tick1ms()
{
  cref_t<i_interop_app2> app = _ctxapp();
  try {
    if (app.ref().b_app_quit())
    {
      app->notify_app_destroy();
      QApplication::quit();
      return;
    }
    app.ref().notify_tick_1ms();
  } catch (...) {}
  this->update();
}

i_interop_app2::EvMouse View::ev(QMouseEvent& e0) const
{
  i_interop_app2::EvMouse e;

  e.pos.x = e0.x();
  e.pos.y = e0.y();

  QPoint g = this->mapToGlobal(QPoint(0, 0));
    e.wrect.topleft.x = g.x();
    e.wrect.topleft.y = g.y();
    e.wrect.w = this->size().width();
    e.wrect.h = this->size().height();

  QDesktopWidget* d = QApplication::desktop();
  if (d)
  {
    QRect r = d->geometry();
      e.vscreen.topleft.x = r.x();
      e.vscreen.topleft.y = r.y();
      e.vscreen.w = r.width();
      e.vscreen.h = r.height();
  }
  else
  {
    e.vscreen.w = 10;
    e.vscreen.h = 10;
  }

  QScreen* s = QGuiApplication::screenAt(g);
    if (!s) { s = QGuiApplication::screens().front(); }
  if (s)
  {
    QRect r = s->geometry();
      e.ev_screen.topleft.x = r.x();
      e.ev_screen.topleft.y = r.y();
      e.ev_screen.w = r.width();
      e.ev_screen.h = r.height();
  }
  else
  {
    e.ev_screen.w = 10;
    e.ev_screen.h = 10;
  }

  e.mbut = e0.buttons() & i_interop_app2::_MaskMBut;
  e.mbrel = e0.button();
  e.kbpref = e0.modifiers() & i_interop_app2::_MaskPrefixKeys;

  return e;
}

i_interop_app2::EvKbd View::ev(QKeyEvent& e0) const
{
  i_interop_app2::EvKbd e;

  e.key = e0.key();
  e.kbpref = e0.modifiers() & i_interop_app2::_MaskPrefixKeys;
  if (e.kbpref & i_interop_app2::Shift)
  {
    switch (e.key)
    {
      case '~': e.key = i_interop_app2::Key_QuoteLeft; break;
      case '!': e.key = i_interop_app2::Key_1; break;
      case '@': e.key = i_interop_app2::Key_2; break;
      case '#': e.key = i_interop_app2::Key_3; break;
      case '$': e.key = i_interop_app2::Key_4; break;
      case '%': e.key = i_interop_app2::Key_5; break;
      case '^': e.key = i_interop_app2::Key_6; break;
      case '&': e.key = i_interop_app2::Key_7; break;
      case '*': e.key = i_interop_app2::Key_8; break;
      case '(': e.key = i_interop_app2::Key_9; break;
      case ')': e.key = i_interop_app2::Key_0; break;
      case '_': e.key = i_interop_app2::Key_Minus; break;
      case '+': e.key = i_interop_app2::Key_Equal; break;

      case '{': e.key = i_interop_app2::Key_BracketLeft; break;
      case '}': e.key = i_interop_app2::Key_BracketRight; break;
      case '|': e.key = i_interop_app2::Key_Backslash; break;

      case ':': e.key = i_interop_app2::Key_Semicolon; break;
      case '"': e.key = i_interop_app2::Key_QuoteDbl; break;

      case '<': e.key = i_interop_app2::Key_Less; break;
      case '>': e.key = i_interop_app2::Key_Greater; break;

      case '?': e.key = i_interop_app2::Key_Slash; break;

      default: break;
    }
  }

  return e;
}

void View::notify_set_title(const std::string& title)
{
  emit changeTitle(QString::fromStdString(title));
}

  // NOTE Each method that needs pmv, must access and check it under critsec_t<View>.
struct interop_mainview : i_interop_mainview
{
  virtual void notify_set_title(const std::string& title) const
  {
    critsec_t<View> __lock(10, -1); if (sizeof(__lock)) {}
    if (!pmv) { return; }
    pmv->notify_set_title(title);
  }
  virtual void notify_set_cursor_v(i_interop_app2::pixelpoint vpos) const
  {
    QCursor::setPos(QPoint(vpos.x, vpos.y));
  }
  virtual i_interop_app2::pixelrect geometry_view() const
  {
    i_interop_app2::pixelrect wrect;
    if (1)
    {
      critsec_t<View> __lock(10, -1); if (sizeof(__lock)) {}
      if (!pmv) { throw exc_mainview_numobj(); }
      QPoint g = pmv->mapToGlobal(QPoint(0, 0));
        wrect.topleft.x = g.x();
        wrect.topleft.y = g.y();
        wrect.w = pmv->size().width();
        wrect.h = pmv->size().height();
    }
    return wrect;
  }
};

cref_t<i_interop_mainview> _ctxmainview()
{
  static cref_t<i_interop_mainview> x;
  if (!x) { cref_t<i_interop_mainview>::t_lock __lock(x); if (sizeof(__lock)) {} if (!x) { x = cref_t<interop_mainview>::iref<i_interop_mainview>::create0(1); } }
  return x;
}
