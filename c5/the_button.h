//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H

#include <QPushButton>
#include <QUrl>
#include <QToolTip>
#include <QToolButton>

class TheButtonInfo
{

public:
    QUrl *url;    // video file to play
    QIcon *icon;  // video icon
    QString data; // video information
    QString filename;
    QString device;
    QString time;
    QString position;
    QString size;

    TheButtonInfo(QUrl *url, QIcon *icon, QString filename, QString device, QString time, QString position, QString size) : url(url), icon(icon), filename(filename), device(device), time(time), position(position), size(size) {}
};

class TheButton : public QToolButton
{
    Q_OBJECT

public:
    TheButtonInfo *infomation;

    TheButton(QWidget *parent) : QToolButton(parent)
    {
        setIconSize(QSize(200, 110));
        connect(this, SIGNAL(released()), this, SLOT(clicked())); // if QPushButton clicked...then run clicked() below
    }
    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEvent *e);
    void leaveEvent();
    void init(TheButtonInfo *i);
private slots:
    void clicked();
signals:
    void jumpTo(TheButtonInfo *);
};

#endif // CW2_THE_BUTTON_H
