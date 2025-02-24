/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#include "Screens/MovementScreen.h"
#include "Screens/PopUpScreen.h"
#include "Screens/AttackScreen.h"
#include "Map.h"
#include "Game.h"
#include "MapItems/Hero.h"
#include "Widgets/ProgressBar.h"
#include "Widgets/HeroStats.h"
#include "MovementScreen.h"

enum ButtonAction{
    ATTACK=50,
    END_TURN
};


MovementScreen::MovementScreen(TFT_eSPI *tft, Game *game): Screen(tft, game) {
    m_buttons = new Buttons(tft);
    // End Turn.
    m_buttons->addButton(new Button(END_TURN, "End turn", 350, 10, 130, 60, TFT_RED));

    // Up Left;
    m_buttons->addButton(new Button(UP_LEFT, "", 200, 100, 80, 60, 0xffffff));
    // Up.
    m_buttons->addButton(new Button(UP, "Up", 300, 100, 80, 60, 0xffffff));
    // Up Right.
    m_buttons->addButton(new Button(UP_RIGHT, "", 400, 100, 80, 60, 0xffffff));
    // Left.
    m_buttons->addButton(new Button(LEFT, "Left", 200, 180, 80, 60, 0xffffff));
    // Attack.
    m_buttons->addButton(new Button(ATTACK, "Attack", 300, 180, 80, 60, TFT_RED));
    // Right.
    m_buttons->addButton(new Button(RIGHT, "Right", 400, 180, 80, 60, 0xffffff));
    // Down Left.
    m_buttons->addButton(new Button(DOWN_LEFT, "", 200, 260, 80, 60, 0xffffff));
    // Down.
    m_buttons->addButton(new Button(DOWN, "Down", 300, 260, 80, 60, 0xffffff));
    // Down Right.
    m_buttons->addButton(new Button(DOWN_RIGHT, "", 400, 260, 80, 60, 0xffffff));

    for (int i = 0; i < MAX_HEROES_COUNT; i++) {
        m_heroStats[i] = new HeroStats(m_game, m_game->getHero(i), 0, i*45, 180, 45);
    }
}

MovementScreen::~MovementScreen() {
    delete m_buttons;
    for (int i = 0; i < MAX_HEROES_COUNT; i++) {
        delete m_heroStats[i];
    }
}

void MovementScreen::showHeroStats() {
    for (int i = 0; i < MAX_HEROES_COUNT; i++) {
        m_heroStats[i]->setSelected(m_heroStats[i]->getHero() == m_game->getCurrentHero());
        m_heroStats[i]->refresh(m_tft);
    }
}

void MovementScreen::showColorMatrix() {
	ColorMatrix *cm = m_game->getMap()->getColorMatrix();
	for(int x = 0; x < MAP_W; x++) {
		for(int y = 0; y < MAP_H; y++) {
			uint32_t color = (*cm)[y][x];
			m_tft->fillRect(200 + x*5, y*5, 5, 5, color);
		}
	}
}

Screen *MovementScreen::refresh() {
    showColorMatrix();
    showHeroStats();
    m_buttons->refresh();

    return nullptr;
}

Screen *MovementScreen::touch(uint16_t x, uint16_t y) {
    Button *b = m_buttons->touch(x, y);
    AttackInfo ai;
    if (b) {
        switch(b->getId()) {
            case ATTACK:
                return new AttackScreen(m_tft, m_game);
                // ai = m_game->getCurrentHero()->attack(m_game->getHero(1));
                // if (ai != ATTACK_OK) {
                //     Screen *scr = new MovementScreen(m_tft, m_game);
                //     return new PopUpScreen(m_tft, m_game, "Target not visible", scr);
                // }
                break;
            case END_TURN:
                m_game->endTurn();
            default:
                m_game->getCurrentHero()->move((Direction) b->getId());
                // Serial.print("Moving to direction ");
                // Serial.print(b->getId(), DEC);
                // Serial.println();

                // Serial.print("New coordinates are [");
                // Serial.print(m_game->getCurrentHero()->getX(), DEC);
                // Serial.print(", ");
                // Serial.print(m_game->getCurrentHero()->getY(), DEC);
                // Serial.println("]");
                ColorMatrix *cm = m_game->getMap()->getColorMatrix();
                // for(int x = 0; x < MAP_W; x++) {
                //     for(int y = 0; y < MAP_H; y++) {
                //         uint32_t color = (*cm)[x][y];
                //         Serial.print(color, DEC);
                //         Serial.print(" ");
                //     }
                //     Serial.println();
                // }
                // Serial.println("---");
                break;
        }
    }
    return nullptr;
}

