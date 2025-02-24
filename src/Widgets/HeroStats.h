/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_HEROSTATS_H
#define BABG_HEROSTATS_H

#include <Arduino.h>
#include <TFT_eSPI.h>

class Hero;
class Game;
class ProgressBar;


class HeroStats {
  public:
    HeroStats(Game *game, Hero *hero, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    ~HeroStats();

    void refresh(TFT_eSPI *tft);
    bool touch(uint16_t x, uint16_t y);
    void setSelected(bool selected) { m_selected = selected; }
    bool getSelected() { return m_selected; }
    Hero *getHero() { return m_hero; }

  private:
    uint16_t m_x;
    uint16_t m_y;
    uint16_t m_w;
    uint16_t m_h;
    Hero *m_hero;
    Game *m_game;
    ProgressBar *m_health;
    ProgressBar *m_mana;
    bool m_selected;
};


#endif