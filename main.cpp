#include "game.h"  
// The main function has no fuction other than start the game, but we had already created the header file so decided to keep it as is

// The main function could contain the UI but since we would the main function to be as a minimal as possible 
/*This entire code is on a public reprosity before the deadline for the extra credit assignemnt for two reasons 
1) We as team working on the code use github to access the files and work on it at our time. 
2) This is code can act as a good personal project for all the members of the team and since we would like to work on improve the functioanlity of the code. 
3) We have the kept the reprository public to have other people work on the same (after we have submitted the code) to get more ideas on how the game can be improved */


/*Important Aspect of the code and the data structure used 
The graph is undirected to allow more easy coding for the trasversal between room and allow us to make a circular map
The first room is Cipher and the Last room is Hydra, The room names are in a vector in the game.h file, feel free to change them to your need
I have the coded it in such a way that the graph is circular (with help from GPT (Conversation is mentioned)) to allow for all rooms to be connected and allow the player to traverse through each and every room
*/

/*Future Improvements to work on: 
Picking up the arrow (I could not get it to work) -- Got it working
Using Ascii Art instead of a text based mode -- Used text base since it is extra credit and I have finals .
Adding more levels/enemies -- Will add in next version if we can finish all features by then.
Improving the respwan mechanics to have a more rogue like struture instead of what I am doing it (which is basically move the player back to the start) but in the rougelike, the entire cave system would also reset
Getting 16 or 8 bit graphics making the game more realistic -- I was planning on using Python or java for the same but currently I am not that smart
*/

// The map/diagram is in the pdf 
// The code is not peak efficency in any way but as you can see it is a work in progress
#include "graph.h"
int main() {
    Game game;  
    game.play();  // Start the game
    
    return 0;
}