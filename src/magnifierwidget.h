
#ifndef _magnifier_h_
#define _magnifier_h_
#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QRegion>
#include "magnifierfilter.h"
namespace namespace_wslibs {
namespace namespace_wsbaswidgets {
class DrawWidget;
class MagnifierWidget;

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    DrawWidget(MagnifierWidget *parent);
    ~DrawWidget() {}
    void paintEvent(QPaintEvent *_event);
private:
    MagnifierWidget *m_magifier;

};

class MagnifierWidget : public QWidget
{
    Q_OBJECT
    friend class DrawWidget;
public:
    MagnifierWidget(const QRegion _region, QWidget *parent = NULL);
    MagnifierWidget(QWidget *parent = NULL);
    virtual ~MagnifierWidget();
    void setMaskRegion(const QRegion _region);
protected:
    void focusInEvent(QFocusEvent *_event);
    void paintEvent(QPaintEvent *_event);
    void enterEvent(QEvent *_event);
    void leaveEvent(QEvent *_event);
    void mousePressEvent(QMouseEvent *_event);
    void moveEvent(QMoveEvent *_event);
    void mouseReleaseEvent(QMouseEvent *_event);
    void mouseMoveEvent(QMouseEvent *_event);
    void resizeEvent(QResizeEvent *_event);
private slots:
    void slotFocusChanged(QWidget *, QWidget *);
    void slotTimer();
protected:
    QPixmap m_capturePixmap;
private:
    QRegion m_maskRegion;
    QImage source;
    QImage dest;
    QPoint dragPosition;
    DrawWidget *m_draw;
    QTimer m_timer;
    QPoint m_pos;
    QRect m_rect;
    MagnifierFilter *m_filter;
};


}//wslibs
}//wsbaswidgets
#endif
