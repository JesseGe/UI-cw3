#include "myslider.h"
#include <QMouseEvent>
#include <QDebug>

MySlider::MySlider(QWidget *parent):QSlider (parent)
{

}

MySlider::~MySlider()
{

}

void MySlider::mousePressEvent(QMouseEvent *ev)
{
    //Get current click position
    int currentX = ev->pos().x();

    //Get the current click as a percentage of the entire Slider
    double per = currentX *1.0 /this->width();

    //Use the calculated percentage to get a specific figure
    int value = per*(this->maximum() - this->minimum()) + this->minimum();

//    qDebug() << value;

    //Set slider position
    this->setValue(value);

//    The mousePressEvent is also used in events such as slider movement events,
//    and this phrase is added so that it does not affect them,
//    and so that the Slider can respond normally to other mouse events.
    QSlider::mousePressEvent(ev);
}
