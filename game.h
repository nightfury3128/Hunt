#ifndef GAME_H
#define GAME_H
/* All the lines mention where GPT has been used along with the relevent conversations, As mentioned in the main file, This code is to be put on a public repository before the due date for the extra credit*/
#include "graph.h"
#include "player.h"
#include <iostream>
#include <random>
#include <cctype>
#include <array>
#include <numeric>

using namespace std;

class Game
{
private:
    Graph cave;
    Player *player;
    int lastroomID;
    void initializeCave()
    {
        array<string, 20> roomNames = {
            "Cipher", "Chimera", "Leviathan", "Pegasus", "Mjolnir",
            "Odyssey", "Atlantis", "Nebula", "Vortex", "Phoenix",
            "Quantum", "Zephyr", "Aurora", "Galaxy", "Eclipse",
            "Orion", "Titan", "Merlin", "Valkyrie", "Hydra"};

        // Initialize rooms
        for (int i = 0; i < 20; i++)
        {
            cave.addRoom(i, roomNames[i]);
        }

        // Decided the map to be circular to avoid checking for edges and if the room is valid, we gave the user the room they can travel to and the entire map is a circle
        // https://chat.openai.com/share/10e87e05-6687-4a89-838b-6b87ba8db81d Could not figure out a logic while using loops (or else it would be around 20-40 lines extra)
        for (int i = 0; i < 20; i++)
        {
            int nextRoom = (i + 1) % 20; // Calculate the index of the next room in a circular manner
            cave.addEdge(i, nextRoom);
        }
        placeHazards(); // This can be within this function but I decided against it because I had to GPT and citations were making it look clusterd
    }

    // https://chat.openai.com/share/e5e3ff5d-6c04-4c2e-a39e-d93db17ff24d This is our conversation with GPT
    /* NOT used to the random generator so used GPT for the same */
    void placeHazards()
    {
        const int totalRooms = 20;
        array<int, totalRooms> roomIndices;
        iota(roomIndices.begin(), roomIndices.end(), 0);

        random_device rd;
        mt19937 gen(rd());
        shuffle(roomIndices.begin(), roomIndices.end(), gen); // Shuffle the room indices

        const int numRoomsWithHazards = totalRooms / 4;
        for (int i = 0; i < numRoomsWithHazards; i++)
        {
            if (i != 0) // Exclude the starting room
            {
                // Adjust the probability of placing different hazards as desired
                cave.getRoom(roomIndices[i]).hasWumpus = true;
                cave.getRoom(roomIndices[i]).hasPit = true;
                cave.getRoom(roomIndices[i]).hasBats = true;
            }
        }

        // There will be better way to solve this but We could not figure it out at all
    }

    string
    trimlower(const string &input)
    { // helper function to make sure that the input is not case and space senestive
        string result;
        for (char c : input)
        {
            if (!isspace(c))
            {
                result += tolower(c);
            }
        }
        return result;
    }

    /* void moveWumpus()
     {
         // Find the current room of the Wumpus
         int currentWumpusRoom = -1;
         for (int i = 0; i < 20; i++)
         {
             if (cave.getRoom(i).hasWumpus)
             {
                 currentWumpusRoom = i;
                 break;
             }
         }

     } */
    // This is our old code for movewumpus() but we could figure out to how move it randomly since it was getting a little difficult so we used
    // https://chat.openai.com/share/90453b21-0cb4-4fc5-a32b-49cf51b6ca6a for the function

    void moveWumpus()
    {
        // Find the current room of the Wumpus
        int currentWumpusRoomId = -1;
        for (int i = 0; i < 20; i++)
        {
            if (cave.getRoom(i).hasWumpus)
            {
                currentWumpusRoomId = i;
                break;
            }
        }

        // If the Wumpus is found in a room
        if (currentWumpusRoomId != -1)
        {
            // Initialize random number generation
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> roomDist(0, cave.getRoom(currentWumpusRoomId).edges.size() - 1);

            // Select a random connected room
            int newRoomIndex = roomDist(gen);
            int newRoomId = cave.getRoom(currentWumpusRoomId).edges[newRoomIndex];

            // Move the Wumpus
            cave.getRoom(currentWumpusRoomId).hasWumpus = false;
            cave.getRoom(newRoomId).hasWumpus = true;
        }
    }

    // As mentioned, the random generator confuses me a lot so https://chat.openai.com/share/dae53dbd-7ef0-4e81-8bd1-850c194f0657
    bool shootArrow(const string &targetRoomName)
    {
        int currentRoomId = player->getCurrentRoomId();
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(2, 4); // Random number of rooms to travel
        int numRoomsToTravel = dist(gen);

        int currentRoom = currentRoomId;
        for (int i = 0; i < numRoomsToTravel; ++i)
        {
            uniform_int_distribution<> roomDist(0, cave.getRoom(currentRoom).edges.size() - 1);
            int nextRoomIndex = roomDist(gen);
            currentRoom = cave.getRoom(currentRoom).edges[nextRoomIndex];

            if (cave.getRoom(currentRoom).hasWumpus)
            {
                cout << "You've killed the Wumpus! You win!" << endl;
                return true;
            }
        }
        if (!cave.getRoom(currentRoom).hasBats && !cave.getRoom(currentRoom).hasPit)
        { // Check if the Arrow went to a room where there is a pit or a swarm of bat
            lastroomID = currentRoom;
        }

        cout << "Arrow missed. The Wumpus might move!" << endl;
        moveWumpus(); // Move the Wumpus after the arrow shot

        return false;
    }

public:
    Game()
    {
        initializeCave();
        player = new Player(&cave.getRoom(0)); // Start in Room 0
    }

    ~Game()
    {
        delete player; // gaming ending destructor (Or just deleting the player but keep everything else which the cave and stuff)
    }

    void play()
    { // The actual gameplay loop
        string input;
        bool gameRunning = true;
        // cave.printGraph(cave); This line was just as a error check for the graph and make sure the room are connected circularly

        while (gameRunning)
        {
            cout << "You are in " << player->getCurrentRoom()->name << "." << endl;
            Graph::Room *currentRoom = player->getCurrentRoom(); // This is for the observe (we were having trouble using the RoomID directly so decided to use pointer)
            int currentRoomId = player->getCurrentRoomId();
            string respawn = "";
            // Harzard check
            if (cave.getRoom(currentRoomId).hasPit)
            {
                cout << "Oh no! You fell into a bottomless pit!" << endl;
                player->lives--;
                if (player->lives <= 0)
                {
                    gameRunning = false;
                    break;
                }
                else
                { // The respawn mechanics was just something that I thought of in the spur of the moment, can be improved upon
                    cout << "You died, Do you wish to respawn (y/n) and try again? "; 
                    cin >> respawn;
                    trimlower(respawn);
                    if (respawn == "y")
                    {
                        player->moveTo(&cave.getRoom(0)); // Send the player back to start
                        player->lives--;
                        continue;
                    }
                    else
                    {
                        cout << "Out of lives \n ";
                        cout << "Credits:\n" // This is just for adding a more gamey effect (and for fun)
                             << "Nipun Chandra -- Lead Developer \n"
                             << "Ishaa Jain -- Lead Room Design \n"
                             << "Shivam Kharangate -- Lead tester \n";
                        break;
                    }
                    continue;
                }
            }

            if (cave.getRoom(currentRoomId).hasBats)
            {
                cout << "Oh no! You've encountered bats! They snatch you away to another room!" << endl;
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<> roomDist(0, 19);
                player->moveTo(&cave.getRoom(roomDist(gen)));
                continue;
            }

            if (cave.getRoom(currentRoomId).hasWumpus)
            {
                cout << "You've encountered the Wumpus!" << endl;

                if (!player->shootArrow())
                {
                    cout << "You have no arrows left to defend yourself. The Wumpus eats you!" << endl;
                    player->lives--;
                    if (player->lives <= 0)
                    {
                        gameRunning = false;
                        break;
                    }
                    else
                    {
                        cout << "You died, Do you wish to respawn (y/n) and try again? ";
                        cin >> respawn;
                        trimlower(respawn);
                        if (respawn == "y")
                        {
                            player->moveTo(&cave.getRoom(0)); // Send the player back to start
                            player->lives--;
                            continue;
                        }
                        else
                        {
                            cout << "Out of lives \n";
                            cout << "Credits:\n"
                                 << "Nipun Chandra -- Lead Developer \n"
                                 << "Ishaa Jain -- Lead Room Design\n"
                                 << "Shivam Kharangate -- Lead tester\n";
                            break;
                        }
                        continue;
                    }
                }

                cout << "You scare the Wumpus away with an arrow. It moves to another room." << endl;
                moveWumpus();
            }

            // Actual game
            cout << "What would you like to do? (move/shoot/observe): ";
            cin >> input;
            input = trimlower(input);
            if (input == "move")
            {
                cout << "Available moves: \n";
                int i = 1;
                for (int edge : cave.getRoom(currentRoomId).edges)
                {

                    cout << "Room " << i << " is " << cave.getRoom(edge).name << " \n";
                    i++;
                }
                cout << endl;

                cout << "Enter room name to move: ";
                cin.ignore();
                getline(cin, input);
                input = trimlower(input);
                int moveID = player->getCurrentRoomId();
                bool moved = false;

                if (lastroomID == player->getCurrentRoomId())
                {
                    cout << "You found your arrow and retrieved it.\n";
                    player->arrows++; // Increase the arrow count if it is retrived
                    cout << "You now have " << player->arrows << " arrows";
                }

                for (int edge : cave.getRoom(currentRoomId).edges)
                {
                    if (trimlower(cave.getRoom(edge).name) == input)
                    {
                        player->moveTo(&cave.getRoom(edge));
                        moved = true;
                        break;
                    }
                }
                if (!moved)
                {
                    cout << "Invalid room selection." << endl;
                }
            }
            else if (input == "shoot")
            {
                if (player->shootArrow())
                {
                    cout << "The neighboring rooms are: ";
                    for (int edge : cave.getRoom(currentRoomId).edges)
                    {
                        cout << cave.getRoom(edge).name << " ";
                    }
                    cout << endl;

                    cout << "Choose direction to shoot (room name): ";
                    cin >> input;
                    if (shootArrow(input))
                    {
                        gameRunning = false;
                    }
                    else
                    {
                        moveWumpus();
                    }
                }
                else
                {
                    cout << "You are out of arrows." << endl;
                }
            }
            else if (input == "observe")
            {
                cout << "Observing tunnel" << endl;

                for (int roomId : currentRoom->edges)
                {
                    Graph::Room &room = cave.getRoom(roomId);
                    string hazards;
                    // Since it is only one statment, the bracket are not neccessary but we used VScode to format the document and it added the brackets
                    if (room.hasPit)
                    {
                        cout << "You feel a gust of wind and hear an echo, there must be a pit nearby \n";
                        break;
                    }

                    if (room.hasBats)
                    {
                        cout << "You hear screeching, there must be bats nearby \n";
                        break;
                    }

                    if (room.hasWumpus)
                    {
                        cout << "There is a foul smell, the wumpus must be nearby \n";
                        break;
                    }

                    else
                    {
                        cout << "Nothing to observe" << endl;
                        break;
                    }
                }
            }

            else if (input == "exit")
            {
                cout << "Exiting game..." << endl;
                cout << "\n";
                cout << "Credits:\n"
                     << "Nipun Chandra -- Lead Developer\n"
                     << "Ishaa Jain -- Lead Room Design\n "
                     << "Shivam Kharangate -- Lead tester\n";
                gameRunning = false;
            }
            else
            {
                cout << "Invalid command." << endl;
            }
        }
    }
};

#endif // GAME_H
