/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include "Screens/PopUpScreen.h"
#include "Map.h"
#include "Game.h"


PopUpScreen::PopUpScreen(TFT_eSPI *tft, Game *game, const char *msg, Screen *nextScreen)
: Screen(tft, game), m_msg(msg), m_nextScreen(nextScreen) {
}

PopUpScreen::~PopUpScreen() {
}

Screen *PopUpScreen::refresh() {
	m_tft->setTextSize(2);
	m_tft->setTextFont(4);
	m_tft->setTextColor(TFT_WHITE, TFT_BLACK);
    m_tft->setTextDatum(MC_DATUM);
	m_tft->drawString(m_msg, 240, 160);

    return nullptr;
}

Screen *PopUpScreen::touch(uint16_t x, uint16_t y) {
    m_tft->fillScreen(TFT_BLACK);
    return m_nextScreen;
}

