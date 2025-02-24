#include <Arduino.h>
#include <unity.h>
#include <Game.h>
#include <Map.h>
#include <MapItems/Hero.h>

Game game;

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_initializeMap() {
    Map *map = game.getMap();
    map->initializeMap(1, 1);
    MapItem *item = map->getMapItem(0, MAP_H - 1);
    TEST_ASSERT_EQUAL(MapItemType_HeroSpawnPoint, item->getType());
}

void test_refreshColorMatrix() {
    Map *map = game.getMap();
    map->initializeMap(1, 1);
    map->handleGameStarted();
    map->handleGameRoundStarted(1);

    ColorMatrix *cm = map->getColorMatrix();
    TEST_ASSERT_EQUAL(0xffffff, (*cm)[MAP_H - 1][0]);
    TEST_ASSERT_EQUAL(0xb596, (*cm)[MAP_H - 1][1]);
    TEST_ASSERT_EQUAL(0xb596, (*cm)[MAP_H - 1][2]);
    TEST_ASSERT_EQUAL(0xb596, (*cm)[MAP_H - 1][3]);
    TEST_ASSERT_EQUAL(0x52aa, (*cm)[MAP_H - 1][4]);
}

void test_refreshColorMatrixAnotherHero() {
    Map *map = game.getMap();
    map->initializeMap(1, 1);
    map->handleGameStarted();
    map->handleGameRoundStarted(1);
    Hero *hero = map->getHero(1);
    hero->move(2, MAP_H - 1);

    ColorMatrix *cm = map->getColorMatrix();
    TEST_ASSERT_EQUAL(0xffffff, (*cm)[MAP_H - 1][0]);
    TEST_ASSERT_EQUAL(0xb596, (*cm)[MAP_H - 1][1]);
    TEST_ASSERT_EQUAL(0xffffff, (*cm)[MAP_H - 1][2]);
    TEST_ASSERT_EQUAL(0xb596, (*cm)[MAP_H - 1][3]);
    TEST_ASSERT_EQUAL(0x52aa, (*cm)[MAP_H - 1][4]);
    TEST_ASSERT_EQUAL(true, map->isItemVisible(hero));
}

void test_refreshColorMatrixAnotherHeroNotInViewport() {
    Map *map = game.getMap();
    map->initializeMap(1, 1);
    map->handleGameStarted();
    map->handleGameRoundStarted(1);
    Hero *hero = map->getHero(1);
    hero->move(4, MAP_H - 1);

    ColorMatrix *cm = map->getColorMatrix();
    TEST_ASSERT_EQUAL(0xffffff, (*cm)[MAP_H - 1][0]);
    TEST_ASSERT_EQUAL(0xb596, (*cm)[MAP_H - 1][1]);
    TEST_ASSERT_EQUAL(0xb596, (*cm)[MAP_H - 1][2]);
    TEST_ASSERT_EQUAL(0xb596, (*cm)[MAP_H - 1][3]);
    TEST_ASSERT_EQUAL(0x52aa, (*cm)[MAP_H - 1][4]);
    TEST_ASSERT_EQUAL(false, map->isItemVisible(hero));
}

void run_tests() {
    UNITY_BEGIN();
    RUN_TEST(test_initializeMap);
    RUN_TEST(test_refreshColorMatrix);
    RUN_TEST(test_refreshColorMatrixAnotherHero);
    RUN_TEST(test_refreshColorMatrixAnotherHeroNotInViewport);
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
