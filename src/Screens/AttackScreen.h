/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_ATTACKSCREEN_H
#define BABG_ATTACKSCREEN_H

#include <Arduino.h>
#include <Screens/Screen.h>
#include <Map.h>

class HeroStats;
class Hero;

class AttackScreen : public Screen {
  public:
    AttackScreen(TFT_eSPI *tft, Game *game);
    ~AttackScreen();

    Hero *getSelectedHero();

    Screen *refresh();
    Screen *touch(uint16_t x, uint16_t y);

  private:
    HeroStats *m_heroStats[MAX_HEROES_COUNT];
    Buttons *m_buttons;
};


#endif