//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include "myslider.h"
#include <vector>
#include <QTimer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QToolTip>
#include<QLabel>
#include "funcBtn.h"
#include<QFileInfo>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 0;
    TheButtonInfo* currentInfo; //info of the video currently playing

    //set up the layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVideoWidget *videoWidget = new QVideoWidget;
    QHBoxLayout *timeLayout = new QHBoxLayout();
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    QHBoxLayout *sLayout = new QHBoxLayout();

    funcBtn *playBtn = new funcBtn("play");
    funcBtn *nextBtn = new funcBtn("next");
    QWidget *display = new QWidget;
    QWidget *space = new QWidget;
    QLabel *name = new  QLabel();


    bool isPress = false;
public:
    ThePlayer();
    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    QWidget* getDisplay() const { return display; }
    TheButtonInfo* getInfo() { return currentInfo; }
    void setfunctions();


signals:
    void sigOpenList(bool flag);
    void nextclick(TheButtonInfo*);
private slots:
    void playEvent();
    void fEvent();
    void backEvent();
    void nextEvent();




    void State(QMediaPlayer::State newstate);
    void Play(bool flag);
public slots:
    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
};

#endif //CW2_THE_PLAYER_H
