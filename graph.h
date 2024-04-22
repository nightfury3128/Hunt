#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // This was something that we needed for the "addEdge" and "hasedge"

using namespace std;
/* This is just a way to define the graph data structure that is used, a circular linked list can be used for the code
but the assginment required us to graphs instead so that is exactly what we did*/
class Graph
{
public:
    struct Room
    { // The room can be a class on it's own and that was the plan before but I changed to avoid using way too many header files
        int id;
        string name;
        vector<int> edges; // Stores IDs of adjacent rooms

        // No hazard in any room before the game start
        bool hasPit = false;
        bool hasBats = false;
        bool hasWumpus = false;

        Room(int id, string name) : id(id), name(name) {} // Constructor for the struct Room (again can be a class)
    };

    vector<Room> rooms; // Used vector for the graph 

    // Every function is named according to the actual usage which is addroom to add a cave(room), addedge follows the same rule and so on
    void addRoom(int id, string name)
    {
        rooms.push_back(Room(id, name));
    }
 
    Room &getRoom(int id)
    {
        return rooms[id];
    }

    //https://chat.openai.com/share/0c28135d-1f98-47c6-b118-426fb15c3aaa our code for edges was not working as intended so ChatGPT for the same
    void addEdge(int roomId1, int roomId2)
    {
        // Ensure that roomId1 and roomId2 are valid indices before adding the edge
        if (roomId1 < rooms.size() && roomId2 < rooms.size()) {
            Room &room1 = rooms[roomId1];
            Room &room2 = rooms[roomId2];

            // Check if an edge already exists before adding
            if (find(room1.edges.begin(), room1.edges.end(), roomId2) == room1.edges.end()) {
                room1.edges.push_back(roomId2);
            }
            if (find(room2.edges.begin(), room2.edges.end(), roomId1) == room2.edges.end()) {
                room2.edges.push_back(roomId1);
            }
        }
    }

    bool hasEdge(int roomId1, int roomId2)
    {
        // Check if two rooms are connected
        if (roomId1 < rooms.size() && roomId2 < rooms.size()) {
            Room &room1 = rooms[roomId1];
            return find(room1.edges.begin(), room1.edges.end(), roomId2) != room1.edges.end();
        }
        return false;
    }


    // The next three functions are setters for the hazards
    void setHasWumpus(int id, bool status)
    {
        rooms[id].hasWumpus = status;
    }

    void setHasBats(int id, bool status)
    {
        rooms[id].hasBats = status;
    }

    void setHasPit(int id, bool status)
    {
        rooms[id].hasPit = status;
    }

    /* void printGraph(const Graph& graph) {
     cout << "Graph:\n";
     for (const auto& room : graph.rooms) {
         cout << "Room " << room.id << " (" << room.name << "):";
         for (int connectedRoomId : room.edges) {
             const Graph::Room& connectedRoom = graph.rooms[connectedRoomId]; // Directly accessing rooms vector
             cout << " -> Room " << connectedRoom.id << " (" << connectedRoom.name << ")";
         }
         cout << endl;
     }*/
   
    // As mentioned in the Game.h file, this function was mainly to make sure that the graph is properly intialized, I have commented it out since it distrupts the gameplay

    /*string getRoomNameById(int roomId)
    { // HELPER FUNCTION
        for (const auto &room : rooms)
        {
            if (room.id == roomId)
            {
                return room.name;
            }
        }
        return "";
    }*/

    // The last commented function is a logic that I had used for the retrieve arrow (I have kept it in because I would like to revist that idea to improve on the functionality)
};

#endif // GRAPH_H
