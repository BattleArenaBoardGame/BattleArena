/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_POPUPSCREEN_H
#define BABG_POPUPSCREEN_H

#include <Arduino.h>
#include <Screens/Screen.h>

class PopUpScreen : public Screen {
  public:
    PopUpScreen(TFT_eSPI *tft, Game *game, const char *msg, Screen *nextScreen);
    ~PopUpScreen();

    Screen *refresh();
    Screen *touch(uint16_t x, uint16_t y);

  private:
    Screen *m_nextScreen;
    const char *m_msg;
};


#endif