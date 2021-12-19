#include "myslider.h"
#include <QMouseEvent>
#include <QDebug>

MySlider::MySlider(QWidget *parent) : QSlider(parent)
{
}

MySlider::~MySlider()
{
}

void MySlider::mousePressEvent(QMouseEvent *ev)
{
    // Gets the current click location
    int currentX = ev->pos().x();

    // Gets the percentage of the entire Slider where the current click is
    double per = currentX * 1.0 / this->width();

    // Specific figures are obtained using the calculated percentages
    int value = per * (this->maximum() - this->minimum()) + this->minimum();

    //    qDebug() << value;

    // Sets the slider position
    this->setValue(value);

    // Events such as slider movement events also use mousePressEvent, and this sentence is added so as not to affect it, and slider can respond normally to other mouse events
    QSlider::mousePressEvent(ev);
}
