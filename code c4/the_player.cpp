//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include <QHelpEvent>
#include <QToolTip>
#include <QTime>

using namespace std;

ThePlayer::ThePlayer() : QMediaPlayer(NULL)
{
    setVolume(80); // be slightly less annoying
    setNotifyInterval(1);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(80);

    videoWidget->setMinimumHeight(480);
    this->setVideoOutput(videoWidget);
    videoWidget->installEventFilter(this);

    timeLayout->addWidget(timeSlider);
    timeSlider->setOrientation(Qt::Horizontal);
    timelabel->setText("00:00/00:00");
    timeLayout->addWidget(timelabel);

    name->setStyleSheet("font-weight: bold;font: 15pt Arial Bold");

    mainLayout->addWidget(name, 1, Qt::AlignCenter);
    mainLayout->addWidget(videoWidget);
    mainLayout->addLayout(timeLayout);
    mainLayout->addLayout(controlsLayout);

    controlsLayout->addWidget(restartButton);
    controlsLayout->addWidget(backBtn);
    controlsLayout->addWidget(playBtn);
    controlsLayout->addWidget(fBtn);
    controlsLayout->addWidget(nextBtn);
    controlsLayout->addWidget(volumeBtn);
    controlsLayout->addWidget(volumeSlider);
    mainLayout->addLayout(sLayout);
    controlsLayout->addWidget(fullScreenBtn);
    controlsLayout->addWidget(listBtn);
    volumeSlider->setOrientation(Qt::Horizontal);
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
    listBtn->setCheckable(true);
    connect(listBtn, SIGNAL(clicked(bool)), this, SIGNAL(sigOpenList(bool)));
    fullScreenBtn->setCheckable(true);
    connect(fullScreenBtn, &QPushButton::clicked, [=](bool)
            { videoWidget->setFullScreen(true); });
    connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(Position(qint64)));
    connect(this, SIGNAL(durationChanged(qint64)), this, SLOT(Duration(qint64)));
    connect(timeSlider, SIGNAL(sliderPressed()), this, SLOT(Press()));
    connect(timeSlider, SIGNAL(sliderReleased()), this, SLOT(Release()));
    connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(VolumeChanged(int)));
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
    name->setText(data);
}

bool ThePlayer::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == videoWidget)
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            if (videoWidget->isFullScreen())
            {
                videoWidget->setFullScreen(false);
            }
            else
            {
                videoWidget->setFullScreen(true);
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            playEvent();
        }
        else if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyevent = (QKeyEvent *)event;
            if (keyevent->key() == Qt::Key_Escape && videoWidget->isFullScreen())
            {
                videoWidget->setFullScreen(false);
            }
        }
    }

    return QMediaPlayer::eventFilter(watched, event);
}


void ThePlayer::VolumeChanged(int val)
{
    this->setVolume(val);
    if (val == 0)
        this->setMuted(true);
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

void ThePlayer::State(QMediaPlayer::State newstate)
{

    switch (newstate)
    {
    case QMediaPlayer::State::StoppedState:
        playBtn->setChecked(false);
        playBtn->name = "play";
        playBtn->setIcon(QIcon((":/playback_images/play.png")));
        timeSlider->setValue(0);
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

void ThePlayer::Position(qint64 time)
{
    if (!isPress)
        timeSlider->setValue(time);

    timelabel->setText(QString("%1/%2").arg(QTime::fromMSecsSinceStartOfDay(time)
                                                .toString("mm:ss"))
                           .arg(QTime::fromMSecsSinceStartOfDay(timeSlider->maximum())
                                    .toString("mm:ss")));
}

void ThePlayer::Duration(qint64 time)
{
    timeSlider->setRange(0, time);

    timelabel->setText(QString("%1/%2").arg(QTime::fromMSecsSinceStartOfDay(0).toString("mm:ss")).arg(QTime::fromMSecsSinceStartOfDay(time).toString("mm:ss")));
}

void ThePlayer::Press()
{
    isPress = true;
}

void ThePlayer::Release()
{
    isPress = false;
    this->setPosition(timeSlider->value());
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

void ThePlayer::restartClicked() {
    setMedia(*currentInfo->url);
    setPlaybackRate(1);
    play();
}

void ThePlayer::nextEvent()
{
    jumpTo(buttons->at(1)->infomation);
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
    name->setText(data);
    emit nextclick(currentInfo);
}
