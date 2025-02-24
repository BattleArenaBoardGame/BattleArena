/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_SCREEN_H
#define BABG_SCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>


class Game;

class Button {
  public:
    Button(uint8_t id, const char *title, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);

    void refresh(TFT_eSPI *tft);
    bool touch(uint16_t x, uint16_t y);
    uint8_t getId() { return m_id; }

  private:
    uint m_id;
    const char *m_title;
    uint16_t m_x;
    uint16_t m_y;
    uint16_t m_w;
    uint16_t m_h;
    uint32_t m_color;

};

#define MAX_BUTTONS 20

class Buttons{
  public:
    Buttons(TFT_eSPI *tft);
    ~Buttons();

    void addButton(Button *b);
    void refresh();
    Button *touch(uint16_t x, uint16_t y);

  private:
    Button *m_buttons[MAX_BUTTONS];
    TFT_eSPI *m_tft;
};

class Screen {
  public:
    Screen(TFT_eSPI *tft, Game *game) : m_tft(tft), m_game(game) {}
    virtual Screen *refresh() = 0;
    virtual Screen *touch(uint16_t x, uint16_t y) = 0;

  protected:
    TFT_eSPI *m_tft;
    Game *m_game;
};


#endif