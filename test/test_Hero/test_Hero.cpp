#include <Arduino.h>
#include <unity.h>
#include <Game.h>
#include <Map.h>
#include <MapItems/Hero.h>
#include <MapItems/HeroSpawnPoint.h>

Game game;
Map *game_map = nullptr;

void setUp(void) {
    game_map = game.getMap();
    game_map->initializeMap(1, 1);
}

void tearDown(void) {
    // clean stuff up here
}

void test_moveDirection() {
    game_map->handleGameRoundStarted(1);
    Hero *h = game_map->getHero(0);

    TEST_ASSERT_EQUAL(0, h->getX());
    TEST_ASSERT_EQUAL(MAP_H - 1, h->getY());

    h->move(UP);
    TEST_ASSERT_EQUAL(0, h->getX());
    TEST_ASSERT_EQUAL(MAP_H - 2, h->getY());

    h->move(UP_LEFT);
    TEST_ASSERT_EQUAL(0, h->getX());
    TEST_ASSERT_EQUAL(MAP_H - 3, h->getY());
}

void run_tests() {
    UNITY_BEGIN();
    RUN_TEST(test_moveDirection);
    UNITY_END();
}

void setup() {
    delay(2000); // service delay
    run_tests();
}

void loop() {
}

int main(int argc, char **argv) {
    run_tests();
}
