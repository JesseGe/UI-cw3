//
// Created by twak on 11/11/2019.
//

#include "the_button.h"
#include "the_player.h"
#include <QMediaPlayer>

void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    info =  i;
}


void TheButton::clicked() {
    emit jumpTo(info);
}

void Pause_Button::clicked(){
    emit Pause(info);
}

void Stop_Button::clicked(){
    emit Stop(info);
}
