
#ifndef _magnifierfilter_h_
#define _magnifierfilter_h_
#include <QEasingCurve>
#include <QImage>
#include "wsmatrix.h"
namespace namespace_wslibs {
namespace namespace_wsbaswidgets {
class MagnifierFilter
{
public:
    MagnifierFilter(int _w, int _h, int _factor, qreal curve);
    ~MagnifierFilter();
    void init(int _w, int _h, int _factor, qreal curve);
    bool selfCheck();
    QString filterVersion();
    QString filterName();
    QString filterDescription();
    QString filterHomeUrl();
    bool filter(const QImage &source, QImage &dest, QRect region);
private:
    WsMatrix *m_matrixX;
    WsMatrix *m_matrixY;
    QEasingCurve m_easingCurve;
    int m_width;
    int m_height;
    int m_factor;
    qreal m_curve;
};

}//wslibs
}//wsbaswidgets
#endif
