#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <vector>
#include <map>
#include <random>
#include <string>

#include "graph.h"

class Player
{
public:
    Graph::Room *currentRoom;
    int arrows = 3; // Number of arrows player
    int lives = 3;  // we decided the player should get a chance to respawn and play our game more than once(because it is pretty cool), we hardcoded it to three

    Player(Graph::Room *startRoom, int startArrows = 3)
        : currentRoom(startRoom), arrows(startArrows) {}

    void moveTo(Graph::Room *room)
    {
        currentRoom = room;
    }

    bool shootArrow()
    {
        if (arrows > 0)
        {
            arrows--;
            return true;
        }
        return false;
    }

    // Get the current room as a reference
    Graph::Room *getCurrentRoom()
    {
        return currentRoom;
    }

    // Get the current room ID
    int getCurrentRoomId() const
    {
        return currentRoom->id;
    }

    // Shoot an arrow
    bool shootAtRoom(Graph::Room *targetRoom)
    {
        if (arrows <= 0)
        {
            return false;
        }
        arrows--; // Decrement the total arrow count

        if (targetRoom->hasWumpus)
        {
            targetRoom->hasWumpus = false;
            return true;
        }
        return false;
    }
};

#endif // PLAYER_H
