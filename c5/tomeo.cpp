/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include "the_player.h"
#include "the_button.h"
#include <QScrollArea>
#include <QPalette>
#include <QLineEdit>
#include <QFileDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>

TheButtonInfo getInfo(QString thumb, QString f, QString filename, QString device,
                      QString time, QString position, QString size)
{
    if (QFile(thumb).exists())
    { // if a png thumbnail exists
        QImageReader *imageReader = new QImageReader(thumb);
        QImage sprite = imageReader->read(); // read the thumbnail
        if (!sprite.isNull())
        {
            // create an icon for the button
            QIcon *icon = new QIcon(QPixmap::fromImage(sprite));
            // convert the file location to a generic url
            QUrl *url = new QUrl(QUrl::fromLocalFile(f));
            // add to the output list
            return TheButtonInfo(url, icon, filename, device, time, position, size);
        }
        else
            qDebug() << "1 warning: skipping video because I couldn't process output "
                     << thumb << endl;
    }
    else
    {
        QString missingThumb = ":/playback_images/missing.png";

        QImageReader *missingThumbReader = new QImageReader(missingThumb);
        QImage missingThumbSprite = missingThumbReader->read(); // read the thumbnail
        if (!missingThumbSprite.isNull())
        {
            // voodoo to create an icon for the button
            QIcon *icon = new QIcon(QPixmap::fromImage(missingThumbSprite));
            // convert the file location to a generic url
            QUrl *url = new QUrl(QUrl::fromLocalFile(f));
            // add to the output list
            return TheButtonInfo(url, icon, filename, device, time, position, size);
        }
        else
            qDebug() << "2 warning: skipping video because I couldn't process thumbnail "
                     << missingThumb << endl;
    }
}

vector<TheButtonInfo> getInfoIn(string loc)
{
    vector<TheButtonInfo> output = vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc));
    QDirIterator it(dir);

    while (it.hasNext())
    { // for all files
        QString f = it.next();
        if (f.contains("."))
#if defined(_WIN32)
            if (f.contains(".wmv"))
            { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))
            { // mac/linux
#endif
                QString thumb = f.left(f.length() - 4) + ".png";
                QFileInfo fi(f);
                QString text_file = fi.baseName() + ".txt";
                QString init = ":/initfile/" + fi.baseName() + ".txt";
                QString filename, position, time, device, size;

                if (QFile(init).exists() == false)
                {
                    QFile *file = new QFile(text_file);
                    file->open(QIODevice::WriteOnly | QIODevice::Text);
                    QTextStream stream(file);
                    filename = position = time = device = size = "None";
                    stream << device << " " << time << " " << position << " " << size;
                }
                else
                {
                    QFile *file = new QFile(init);
                    file->open(QIODevice::ReadOnly | QIODevice::Text);
                    QTextStream stream(file);
                    stream >> filename >> device >> time >> position >> size;
                }
                output.push_back(getInfo(thumb, f, filename, device, time, position, size));
            }
    }
    return output;
}

int main(int argc, char *argv[])
{
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;
    // create the Qt Application / the QMediaPlayer which controls the playback
    QApplication app(argc, argv);
    ThePlayer *videoplay = new ThePlayer;

    // a row of buttons, a list of the buttons
    QWidget *buttonW = new QWidget();
    vector<TheButton *> buttons_widget;
    // the buttons are arranged vertically
    QVBoxLayout *layout = new QVBoxLayout();
    buttonW->setLayout(layout);

    QWidget *classW = new QWidget();
    QVBoxLayout *classlayout = new QVBoxLayout();
    classW->setLayout(classlayout);

    QHBoxLayout *searchbar = new QHBoxLayout();
    QPushButton *search = new QPushButton();
    search->setText("Find");
    QLineEdit *edit = new QLineEdit();
    edit->setPlaceholderText("Please enter video name");
    searchbar->addWidget(edit);
    searchbar->addWidget(search);
    classlayout->addLayout(searchbar);

    // create the main window and layout
    QWidget window;
    QVBoxLayout *mainContainer = new QVBoxLayout();
    window.setLayout(mainContainer);
    window.setWindowTitle("Tomeo");
    window.setMinimumSize(1200, 800);

    QWidget *playershow = videoplay->getDisplay();
    QHBoxLayout *videofirstview = new QHBoxLayout();
    // add the video and the buttons to the top level
    videofirstview->addWidget(playershow);
    videofirstview->addWidget(classW);
    mainContainer->addLayout(videofirstview);

    QObject::connect(videoplay, &ThePlayer::sigOpenList, [=](bool flag)
                     { classW->setVisible(!flag); });
    // collect all the videos in the folder
    vector<TheButtonInfo> videos;
    if (argc == 2)
        videos = getInfoIn(string(argv[1]));
    if (videos.size() == 0)
    {
        const int result = QMessageBox::question(0, "Tomeo", "There is no video in the folder, "
                                                             "please select the folder which contains video!");
        if (result == QMessageBox::Yes)
        {
            QString path = QFileDialog::getExistingDirectory(0);
            if (path != "")
                videos = getInfoIn(path.toStdString());
            else
                exit(-1);
        }
        else
            exit(-1);
    }

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(buttonW);
    scrollArea->setFixedWidth(250);

    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    classlayout->addWidget(scrollArea);

    // make buttons
    for (int i = 0; i < static_cast<int>(videos.size()); i++)
    {
        TheButton *vbuttons = new TheButton(buttonW);
        vbuttons->connect(vbuttons, SIGNAL(jumpTo(TheButtonInfo *)), videoplay, SLOT(jumpTo(TheButtonInfo *)));
        vbuttons->setStyleSheet("text-align:left;");
        buttons_widget.push_back(vbuttons);
        layout->addWidget(vbuttons);
        vbuttons->init(&videos.at(i));
    }

    // buttons details
    videoplay->setContent(&buttons_widget, &videos);
    window.show();
    return app.exec(); // wait for terminate
}
