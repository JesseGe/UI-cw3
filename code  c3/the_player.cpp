
#include "the_player.h"
#include <QHelpEvent>
#include <QToolTip>
#include <QTime>

using namespace std;

ThePlayer::ThePlayer() : QMediaPlayer(NULL)
{


    videoWidget->setMinimumHeight(480);
    this->setVideoOutput(videoWidget);
    videoWidget->installEventFilter(this);



    mainLayout->addWidget(videoWidget);
    mainLayout->addLayout(timeLayout);
    mainLayout->addLayout(controlsLayout);

    controlsLayout->addWidget(backBtn);
    controlsLayout->addWidget(playBtn);
    controlsLayout->addWidget(fBtn);
    controlsLayout->addWidget(nextBtn);
    controlsLayout->addWidget(restartButton);
    setfunctions();
    display->setLayout(mainLayout);
}

void ThePlayer::setfunctions()
{
    playBtn->setCheckable(true);
    connect(playBtn, SIGNAL(clicked(bool)), this, SLOT(Play(bool)));
    connect(fBtn, SIGNAL(released()), this, SLOT(Sender()));
    connect(backBtn, SIGNAL(released()), this, SLOT(Sender()));
    connect(nextBtn, SIGNAL(released()), this, SLOT(nextEvent()));
    connect(restartButton, SIGNAL (released()), this, SLOT (restartClicked()));
    connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this,
            SLOT(State(QMediaPlayer::State)));
}

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton *> *b, std::vector<TheButtonInfo> *i)
{
    buttons = b;
    infos = i;
    currentInfo = buttons->at(0)->infomation;
    setMedia(*currentInfo->url);
    play();

    QString data = currentInfo->url->toString();
    QFileInfo file(data);
    data = file.baseName();
}


void ThePlayer::State(QMediaPlayer::State newstate)
{

    switch (newstate)
    {
    case QMediaPlayer::State::StoppedState:
        playBtn->setChecked(false);
        playBtn->name = "play";
        playBtn->setIcon(QIcon((":/playback_images/play.png")));
        break;
    case QMediaPlayer::State::PlayingState:
        playBtn->name = "pause";
        playBtn->setIcon(QIcon((":/playback_images/pause.png")));
        playBtn->setChecked(true);
        break;
    case QMediaPlayer::State::PausedState:
        playBtn->name = "play";
        playBtn->setIcon(QIcon((":/playback_images/play.png")));
        playBtn->setChecked(false);
        break;

    }
}


void ThePlayer::Play(bool flag)
{
    if (flag)
    {
        play();
    }
    else
    {
        pause();
    }
}
void ThePlayer::playEvent()
{

    if (playbackRate() != 1)
    {
        setPlaybackRate(1);
        playBtn->setIcon(QIcon((":/playback_images/pause.png")));
        return;
    }

    switch (this->state())
    {
    case QMediaPlayer::State::StoppedState:
        play();
        playBtn->name = "pause";
        playBtn->setIcon(QIcon((":/playback_images/pause.png")));
        break;
    case QMediaPlayer::State::PlayingState:
        pause();
        playBtn->name = "play";
        playBtn->setIcon(QIcon((":/playback_images/play.png")));
        break;
    case QMediaPlayer::State::PausedState:
        play();
        playBtn->name = "pause";
        playBtn->setIcon(QIcon((":/playback_images/pause.png")));
        break;
    }
}

void ThePlayer::Sender()
{
    if (this->state() == QMediaPlayer::StoppedState)
        return;

    QPushButton *btn = (QPushButton *)sender();
    qint64 time = this->position();
    if (backBtn == btn)
    {
        time -= 5000;
        if (time < 0)
        {
            this->stop();
            return;
        }
    }
    else
    {
        time += 5000;

        if (time > this->duration())
        {
            this->stop();
            return;
        }
    }

    this->setPosition(time);
}

void ThePlayer::fEvent()
{
    playBtn->setIcon(QIcon((":/playback_images/play.png")));
    setPlaybackRate(2);
}

void ThePlayer::backEvent()
{
    playBtn->setIcon(QIcon((":/playback_images/play.png")));
    setPlaybackRate(-2);
}

void ThePlayer::nextEvent()
{
    jumpTo(buttons->at(1)->infomation);
}

void ThePlayer::restartClicked() {
    setMedia(*currentInfo->url);
    setPlaybackRate(1);
    play();
}

void ThePlayer::jumpTo(TheButtonInfo *button)
{
    // swap clicked button info with current playing info
    if (currentInfo == button)
        return;

    for (unsigned long long i = 0; i < buttons->size(); i++)
    {
        if (buttons->at(i)->infomation == button)
        {
            for (unsigned long long j = i; j < buttons->size() - 1; j++)
                buttons->at(j)->init(buttons->at(j + 1)->infomation);
            break;
        }
    }
    buttons->at(buttons->size() - 1)->init(currentInfo);

    currentInfo = button;
    buttons->at(0)->init(currentInfo);
    setMedia(*currentInfo->url);
    setPlaybackRate(1);
    play();
    playBtn->name = "pause";
    playBtn->setIcon(QIcon((":/playback_images/pause.png")));
    QString data = button->url->toString();
    QFileInfo file(data);
    data = file.baseName();
    emit nextclick(currentInfo);
}
