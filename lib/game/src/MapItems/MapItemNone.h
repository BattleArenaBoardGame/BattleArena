/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/

#ifndef BABG_MAPITEMNONE_H
#define BABG_MAPITEMNONE_H

#include <Arduino.h>
#include <MapItems/MapItem.h>


class MapItemNone : public MapItem {
  public:
    MapItemNone(Map *map) : MapItem(map, MapItemType_None) {}
    uint32_t getColor() { return 0xb596; }
    Team getTeam() { return Team_Unknown; }
};


#endif