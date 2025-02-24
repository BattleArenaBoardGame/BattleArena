#include "Widgets/ProgressBar.h"

ProgressBar::ProgressBar(const char *title, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t bgColor, uint32_t textColor) :
m_title(title), m_x(x), m_y(y), m_w(w), m_h(h), m_bgColor(bgColor), m_textColor(textColor), m_value(0) {
}

void ProgressBar::setValue(uint16_t v) {
    m_value = v;
}

void ProgressBar::setMaxValue(uint16_t v) {
    m_maxValue = v;
}

void ProgressBar::refresh(TFT_eSPI *tft) {
    int w = (m_w - 2)  * ((float) m_value / m_maxValue);
    tft->drawRect(m_x, m_y, m_w, m_h, TFT_WHITE);
    tft->fillRect(m_x + 1, m_y + 1, m_w - 2, m_h - 2, TFT_BLACK);
    tft->fillRect(m_x + 1, m_y + 1, w, m_h - 2, m_bgColor);
	tft->setTextSize(1);
	tft->setTextFont(2);
	tft->setTextColor(m_textColor);
    tft->setTextDatum(MC_DATUM);
	tft->drawString(m_title, m_x + m_w / 2, m_y + m_h / 2);
}
