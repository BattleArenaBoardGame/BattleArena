/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_MOVEMENTSCREEN_H
#define BABG_MOVEMENTSCREEN_H

#include <Arduino.h>
#include <Screens/Screen.h>
#include <Map.h>

class HeroStats;


class MovementScreen : public Screen {
  public:
    MovementScreen(TFT_eSPI *tft, Game *game);
    ~MovementScreen();



    Screen *refresh();
    Screen *touch(uint16_t x, uint16_t y);

  private:
    void showColorMatrix();
    void showHeroStats();

    Buttons *m_buttons;
    HeroStats *m_heroStats[MAX_HEROES_COUNT];
};


#endif