//
// Created by twak on 11/11/2019.
//

#include "the_button.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QHelpEvent>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QDialog>
#include <QFile>

void TheButton::init(TheButtonInfo *i)
{
    setIcon(*(i->icon));
    infomation = i;
}

void TheButton::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
        clicked();
}

void TheButton::clicked()
{
    emit jumpTo(infomation);
}

void TheButton::enterEvent(QEvent *e)
{
    QEnterEvent *enterEvent = static_cast<QEnterEvent *>(e);
    infomation->data = "Video:\t" + infomation->filename + "\n" + "Device:\t" + infomation->device + "\n" + "Time:\t" + infomation->time + "\n" + "Position:  " + infomation->position + "\n" + "Size:\t" + infomation->size;
    QToolTip::showText(enterEvent->globalPos(), infomation->data, this);
}
void TheButton::leaveEvent()
{
    return;
}
