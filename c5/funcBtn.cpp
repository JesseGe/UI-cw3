#include "funcBtn.h"

funcBtn::funcBtn(QString x)
{
    name = x;
    QString icon = ":/playback_images/"+name+".png";
    setStyleSheet(ButtonStyleSheet);
    setIconSize(QSize(80, 80));
    setIcon(QIcon((icon)));
}

void funcBtn::enterEvent(QEvent *event)
{
   QEnterEvent *enterEvent = static_cast<QEnterEvent*>(event);
   QToolTip::showText(enterEvent->globalPos(),name,this);
   setCursor(Qt::ArrowCursor);
   update();

}

void funcBtn::leaveEvent(QEvent *event)
{
   setCursor(Qt::PointingHandCursor);
   update();
}