/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_PROGRESSBAR_H
#define BABG_PROGRESSBAR_H

#include <Arduino.h>
#include <TFT_eSPI.h>

class Game;

class ProgressBar {
  public:
    ProgressBar(const char *title, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t bgColor, u_int32_t textColor);

    void setValue(uint16_t value);
    void setMaxValue(uint16_t maxValue);

    void refresh(TFT_eSPI *tft);

  private:
    const char *m_title;
    uint16_t m_x;
    uint16_t m_y;
    uint16_t m_w;
    uint16_t m_h;
    uint32_t m_bgColor;
    uint32_t m_textColor;
    uint16_t m_value;
    uint16_t m_maxValue;

};


#endif