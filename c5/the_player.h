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
    funcBtn *restartButton = new funcBtn("restart");
    funcBtn *fBtn = new funcBtn("forward");
    funcBtn *backBtn = new funcBtn("back");
    funcBtn *nextBtn = new funcBtn("next");
    funcBtn* listBtn = new funcBtn("hidden");
    funcBtn* volumeBtn = new funcBtn("volume");
    funcBtn* fullScreenBtn = new funcBtn("full");
    QSlider* volumeSlider = new MySlider;
    QSlider* timeSlider = new MySlider;
    QWidget *display = new QWidget;
    QWidget *space = new QWidget;
    QLabel *name = new  QLabel();
    QLabel* timelabel = new QLabel();

    bool isPress = false;
public:
    ThePlayer();
    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    QWidget* getDisplay() const { return display; }
    TheButtonInfo* getInfo() { return currentInfo; }
    void setfunctions();

    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void sigOpenList(bool flag);
    void nextclick(TheButtonInfo*);
private slots:
    void playEvent();
    void fEvent();
    void backEvent();
    void nextEvent();
    void restartClicked();
    void Position(qint64 time);
    void Duration(qint64 time);
    void Press();
    void Release();
    void Mute();
    void VolumeChanged(int val);


    void Sender();

    void State(QMediaPlayer::State newstate);
    void Play(bool flag);
public slots:
    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
};

#endif //CW2_THE_PLAYER_H
