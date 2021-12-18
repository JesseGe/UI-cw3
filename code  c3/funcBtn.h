#ifndef FUNCBTN_H
#define FUNCBTN_H

#include <QPushButton>
#include <QUrl>
#include <QToolTip>
#include <QHelpEvent>


class funcBtn : public QPushButton
{
    Q_OBJECT
public:
    funcBtn(QString x);

    QString name;
    const QString ButtonStyleSheet = "QPushButton { background-color: #778899; color: white; border-radius: 8px;} QPushButton:hover {background-color: #6f74dd;}";
    
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
private:
    bool leave;
};

#endif
