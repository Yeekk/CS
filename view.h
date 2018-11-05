#ifndef VIEW_H
#define VIEW_H

#include <QtOpenGL/QGLWidget>
#include <QtCore/QTimer>

#include "interop_mainview.h"
#include "interop_app2.h"

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QGLFormat format, QWidget *parent);
    ~View();

private:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
//    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void tick1ms();

signals:
    void changeTitle(const QString);

private:
    friend struct interop_mainview;
    QTimer _timer1ms;
    i_interop_app2::EvMouse ev(QMouseEvent& e) const;
    i_interop_app2::EvKbd ev(QKeyEvent& e) const;
    void notify_set_title(const std::string& title);
};

#endif // VIEW_H

