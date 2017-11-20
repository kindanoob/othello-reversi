


///This file includes misc ideas and TODOs related to the game.

//Implement My_button class
//Implement Bounding_box for Entity class
//Add a separate class for Sprite Manager(?)

//when debugging infinite loop, put random number (current time, timer etc) to cout
//in order to make sure that something is happening

//add ammo ejecting from the gun after shooting

//add bullet hit-the-wall animation so it visible when bullet collides with obstacle

//Add Steam achievements

//Add Steam trading cards

//Create a Gun class

//Implement pathfinding

//Implement save/load game possibility

//How to properly handle states push/pop stuff? Should we create a new Options_state before
//pushing it onto the stack or just keep one pointer to it? Etc.

//Add layer field to all drawables which will indicate the priority of objects:
//those with lowest priority are drawn first, with highest - last

//Use Prim's algorithm for procedural generation of rooms

//Make init() method common to all game states, add it as pure virtual to Game_state class

//Experiment with spatialized sound via sf::Listener etc

//Create a Level class. It should contain a Game_map, vector of enemies, vector of bonuses,
//a Boss, vector of locations of other important things (e.g., the locations of spawns of
//all entities), locations of entries and exits.

//Create test suite for map generator. It should create a lot of maps (e.g., 1000) and
//check if there are any defects/bugs, then produce statistics containing percentage of
//good and bad maps.
