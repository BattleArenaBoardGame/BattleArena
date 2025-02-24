#include "Screens/Screen.h"
#include "Screen.h"

Button::Button(uint8_t id, const char *title, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color) :
m_id(id), m_title(title), m_x(x), m_y(y), m_w(w), m_h(h), m_color(color) {
}

void Button::refresh(TFT_eSPI *tft) {
	tft->setTextSize(1);
	tft->setTextFont(4);
	// Print SD test button
	tft->setTextColor(TFT_BLACK, m_color);
    tft->fillRoundRect(m_x, m_y, m_w, m_h, 3, m_color);
    tft->setTextDatum(MC_DATUM);
	tft->drawString(m_title, m_x + m_w / 2, m_y + m_h / 2);
}

bool Button::touch(uint16_t x, uint16_t y) {
    if (x > m_x && y > m_y && x < m_x + m_w && y < m_y + m_h) {
        return true;
    }
    return false;
}

Buttons::Buttons(TFT_eSPI *tft) : m_tft(tft) {
    for (int i=0; i<MAX_BUTTONS; i++) {
        m_buttons[i] = nullptr;
    }
}

Buttons::~Buttons() {
    Button *b;
    for (int i=0; i<MAX_BUTTONS; i++) {
        b = m_buttons[i];
        if (b == nullptr) {
            continue;
        }
        delete b;
    }
}

void Buttons::addButton(Button *b) {
    for (int i=0; i<MAX_BUTTONS; i++) {
        if (m_buttons[i] == nullptr) {
            m_buttons[i] = b;
            break;
        }
    }
}

void Buttons::refresh() {
    Button *b;
    for (int i=0; i<MAX_BUTTONS; i++) {
        b = m_buttons[i];
        if (b == nullptr) {
            continue;
        }
        b->refresh(m_tft);
    }
}

Button *Buttons::touch(uint16_t x, uint16_t y) {
    Button *b;
    for (int i=0; i<MAX_BUTTONS; i++) {
        b = m_buttons[i];
        if (b == nullptr) {
            continue;
        }

        if (b->touch(x, y)) {
            return b;
        }
    }
    return nullptr;
}