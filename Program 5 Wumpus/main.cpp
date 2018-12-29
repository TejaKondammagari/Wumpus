/* prog5Wumpus.cpp
 
 Description: Navigate a maze to kill the Wumpus.
 Implementation of a 1972 game by Gregory Yob.
 
 Results of a program run are shown below:
 
 
 */

/***Mallavarapu:
 * 1) No header information
 * 2) avoid use of Global variables
 * 3) the comments shouldd have details about the parameter s the function uses.
 * ***/
#include <iostream>
#include <iomanip>   // for setw()
#include <cctype>    // for toupper()
#include <cstdlib>   // for rand() and srand()
#include <ctime> // to seed random number generator with time(0)
#include <string>
#include <vector>
using namespace std;

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

//--------------------------------------------------------------------------------
void displayCave()
{
    cout<< "       ______18______             \n"
    << "      /      |       \\           \n"
    << "     /      _9__      \\          \n"
    << "    /      /    \\      \\        \n"
    << "   /      /      \\      \\       \n"
    << "  17     8        10     19       \n"
    << "  | \\   / \\      /  \\   / |    \n"
    << "  |  \\ /   \\    /    \\ /  |    \n"
    << "  |   7     1---2     11  |       \n"
    << "  |   |    /     \\    |   |      \n"
    << "  |   6----5     3---12   |       \n"
    << "  |   |     \\   /     |   |      \n"
    << "  |   \\       4      /    |      \n"
    << "  |    \\      |     /     |      \n"
    << "  \\     15---14---13     /       \n"
    << "   \\   /            \\   /       \n"
    << "    \\ /              \\ /        \n"
    << "    16---------------20           \n"
    << endl;
}//end displayCave()


//--------------------------------------------------------------------------------
void displayInstructions()
{
    cout<< "Hunt the Wumpus:                                             \n"
    << "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    << "room has 3 tunnels leading to other rooms.                   \n"
    << "                                                             \n"
    << "Hazards:                                                     \n"
    << "1. Two rooms have bottomless pits in them.  If you go there you fall and die.  \n"
    << "2. Two other rooms have super-bats.  If you go there, the bats grab you and    \n"
    << "   fly you to some random room, which could be troublesome.  Then those bats go\n"
    << "   to a new room different from where they came from and from the other bats.  \n"
    << "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and   \n"
    << "   is too heavy for bats to lift.  Usually he is asleep.  Two things wake      \n"
    << "   him up: Anytime you shoot an arrow, or you entering his room.  When he      \n"
    << "   wakes he moves if he is in an odd-numbered room, but stays still otherwise. \n"
    << "   After that, if he is in your room, he eats you and you die!                 \n"
    << "                                                                               \n"
    << "Moves:                                                                         \n"
    << "On each move you can do the following, where input can be upper or lower-case: \n"
    << "1. Move into an adjacent room.  To move enter 'M' followed by a space and      \n"
    << "   then a room number.                                                         \n"
    << "2. Shoot your guided arrow through a list of up to three adjacent rooms, which \n"
    << "   you specify.  Your arrow ends up in the final room.                         \n"
    << "   To shoot enter 'S' followed by the number of rooms (1..3), and then the     \n"
    << "   list of the desired number (up to 3) of adjacent room numbers, separated    \n"
    << "   by spaces. If an arrow can't go a direction because there is no connecting  \n"
    << "   tunnel, it ricochets and moves to the lowest-numbered adjacent room and     \n"
    << "   continues doing this until it has traveled the designated number of rooms.  \n"
    << "   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You \n"
    << "   automatically pick up the arrow if you go through a room with the arrow in  \n"
    << "   it.                                                                         \n"
    << "3. Enter 'R' to reset the person and hazard locations, useful for testing.     \n"
    << "4. Enter 'C' to cheat and display current board positions.                     \n"
    << "5. Enter 'D' to display this set of instructions.                              \n"
    << "6. Enter 'P' to print the maze room layout.                                    \n"
    << "7. Enter 'X' to exit the game.                                                 \n"
    << "                                                                               \n"
    << "Good luck!                                                                     \n"
    << " \n"
    << endl;
}//end displayInstructions()


//--------------------------------------------------------------------------------

bool hazardsGenerated = false; // boolean to test if hazards were generated
int moveCounter =1; // to count number of moves user makes

// function prototyping
void roomChecker(int[][3],int,int[]);
void checkHazards(int[][3],int [], int);
int generateUniqueRandom(int []);

int rooms[2]; // stores random numbers generated for bat and player
bool firstPlayer = true; // indicates if player need a new random number after carried by super bats
bool firstBat = true; // indicates if the bat needs a new random number after carrying player to
// another room

// generates random numbers for hazards and stores them in an array and generate random number
// for player and bat when player enters the bat room
int randomNumberGenerator(int hazards[], bool randomPlayerLocation, bool randomBatLocation)
{
    if(!hazardsGenerated)
    {
        int temporary[7]; // stores initial random numbers generated
        for (int i=1; i<7; i++){ // loop to generate random numbers
            int randomNumber1 = (rand()%20)+1;
            for (int j=0; j<6; j++){
                while (randomNumber1 == temporary[j]){ // makes sure that a unique random number is
                    // generated
                    randomNumber1 = (rand()%20)+1;
                    j=0; // resets the index to 0 to check if new random number is unique
                }
            }
            temporary[i] = randomNumber1; // assigns random number generated to temporary array
            
            temporary[0] = -1; // initializes arrow location at beginning of the game
            hazardsGenerated = true; // indicates that random numbers for hazards are generated
        }
        // reassign the order of the random numbers in another array
        hazards[0] = temporary[6];
        hazards[1] = temporary[5];
        hazards[2] = temporary[1];
        hazards[3] = temporary[2];
        hazards[4] = temporary[3];
        hazards[5] = temporary[4];
        hazards[6] = temporary[0];
    }
    // set random number for player location
    if(randomPlayerLocation && firstPlayer)
    {
        firstPlayer = false;
        return rooms[0];
    }
    // set random number for bat location
    else if (randomBatLocation && firstBat)
    {
        firstBat = false;
        return rooms[1];
    }
    // generates unique random rooms for bat and player after bats transport player
    for(int k=0;k<2;k++){
        rooms[k] = (rand()%20)+1;
        for (int j=0; j<6; j++){
            while (rooms[k] == hazards[j]){
                rooms[k] = (rand()%20)+1;
                j=0;
            }
        }
    }
    // returns the random room generated for the player and the bat
    if(randomPlayerLocation || randomBatLocation)
    {
        return rooms[0];
    }
    
    return 0;
}
// prints out the locations of hazards if player decides to cheat
void printCheatSheet(int hazards[])
{
    cout << "Cheating! Game elements are in the following rooms: " << endl;
    cout<<"Player "<<"Wumpus "<<"Bats1 "<<"Bats2 "<<"Pit1 "<<"Pit2 "<<"Arrow"<<endl;
    cout<<"  "<<hazards[0]<<"      "<<hazards[1]<<"      "<<hazards[2]<<"     "<<hazards[3]<<
    "   "<<hazards[4]<<"    "<<hazards[5]<<"    "<<hazards[6]<<endl;
    cout<<"You are in room "<<hazards[0]<<"."<<endl;
}
// prints location of player and checks hazards
void printLocation(int arr[][3],int hazards[], int player){
    checkHazards(arr, hazards, hazards[0]);
    cout<<endl;
}
// checks to see if player entered a room with a hazard
void checkHazards(int arr[][3],int hazards[], int player)
{
    int minimum;
    
    //If player enters odd numbered room of wumpus, wumpus moves to the lowest adjacent room
    if (hazards[0] == hazards[1]){
        if (hazards[1]%2 != 0){ // checks if wumpus is in an odd numbered room
            if (arr[player][0]< arr[player][1]){ //find the minimum adjacent room
                minimum = arr[player][0];
            }
            else{
                minimum = arr[player][1];
            }
            if(minimum > arr[player][2]){
                minimum = arr[player][2];
            }
            hazards[1] = minimum; // set minImum adjacent room to wumpus
            cout << endl;
            cout << "You hear a slithering sound, as the Wumpus slips away." << endl;
            cout << "Whew, that was close!" << endl;
        }
        // if player enters even numbered room of wumpus, wumpus kills player
        else{
            cout << endl;
            cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
            cout << "It is over." << endl << endl;
            cout << "Exiting Program ..." << endl;
            exit (0); // Terminate program
        }
    }
    
    // if player enters either of the bat room, then new random numbers are generated for player and bat
    if (hazards[0] == hazards[2] || hazards[0] == hazards[3]){
        int playerLocation = randomNumberGenerator(hazards, true, false); // generate random number to update player location
        int bat = randomNumberGenerator(hazards, false, true); // generate random number to update bat location
        if (hazards[0] == hazards[2]){
            hazards[2] = bat; // update bat1 location
        }
        else{
            hazards[3] = bat; // update bat2 location
        }
        hazards[0] = playerLocation; // update player location
        cout << "Woah... you're flying!" << endl;
        cout << "You've just been transported by bats to room " << hazards[0] << endl;
    }
    
    // if player enters pit room, player dies
    if (hazards[0] == hazards[4] || hazards[0] == hazards[5]){ // checks if player enters pit1 or pit2 rooms
        cout << "Aaaaaaaaahhhhhh...." << endl;
        cout << "    You fall into a pit and die." << endl << endl;
        cout << "Exiting Program ..." << endl;
        exit (0); // Terminate program
    }
    
    cout << "You are in room " << hazards[0]<<"."; // Display player location
    
    // checks if player is near hazard and warns player
    for (int j=0; j<3; j++){
        if (arr[hazards[0]][j] == hazards[1]){ // checks for wumpus
            cout << " You smell a stench." ;
        }
        if(arr[hazards[0]][j] == hazards[4] || arr[hazards[0]][j] == hazards[5]){ // checks for pit
            cout << " You feel a draft.";
        }
        if(arr[hazards[0]][j] == hazards[2] || arr[hazards[0]][j] == hazards[3]){ // checks for bats
            cout << " You hear rustling of bat wings.";
        }
    }
}
// checks if player enters a room with hazards and also if the move is valid or not
void roomChecker(int arr [][3], int number, int hazards[]){
    for(int i=0; i<3; i++){ // if move is valid
        if (number == arr[hazards[0]][i]){
            hazards[0] = number;
            checkHazards(arr, hazards, number); // check for hazards
            cout<<endl;
            return;
        }
    }
    cout <<"Invalid move.  Please retry." << endl; // if move is invalid
    checkHazards(arr, hazards, hazards[0]); // check for hazards
    cout<<endl;
    moveCounter--; // decrements move counter since move isn't valid
}
// checks the path of the arrow is valid and if arrow kills wumpus or player
void arrowChecker(int arrowPath[], int cave[][3], int hazards[], int size){
    //Check if path is valid
    int start = hazards[0];
    int minimum; // to store minimbus adjacent room
    bool pathValid; // indicates if arrow path is valid
    for (int j=0; j<size; j++)
    {
        pathValid = false;
        // checks if rooms are adjacent
        for(int k=0; k<3; k++)
        {
            if(arrowPath[j] == cave[start][k])
            {
                pathValid = true;
                hazards[6] = arrowPath[j];
                //valid path - checks if it has wumpus or player
                if(arrowPath[j] == hazards[1]) // checks if arrow hits wumpus
                {
                    cout<<"Wumpus has just been pierced by your deadly arrow!"<<endl;
                    cout<<"Congratulations on your victory, you awesome hunter you."<<endl;
                    cout<<"Exiting Program ...";
                    exit (0); // Terminate program
                }
                if(arrowPath[j] == hazards[0]) // checks if arrow hits player
                {
                    cout<<"You just shot yourself."<<endl;
                    cout<<"Maybe Darwin was right.  You're dead."<<endl;
                    cout<<"Exiting Program ...";
                    exit (0); // Terminate program
                }
                break;
            }
        }
        
        // if path is invalid
        if(!pathValid)
        {
            cout<<endl;
            // if room is not adjacent, the arrow ricochets to minimum adjacent room
            cout<<"Room "<< arrowPath[j] << " is not adjacent.  Arrow ricochets..."<<endl;
            // find minimum adjacent room value
            if (cave[start][0]< cave[start][1]){
                minimum = cave[start][0];
            }
            else{
                minimum = cave[start][1];
            }
            if(minimum > cave[start][2]){
                minimum = cave[start][2];
            }
            
            hazards[6] = minimum; // Set arrow to minimum adjacent room
            arrowPath[j] = minimum;
            if(arrowPath[j] == hazards[1]) // if arrow kills wumpus while ricocheting
            {
                cout<<"Your arrow ricochet killed the Wumpus, you lucky dog!"<<endl;
                cout<<"Accidental victory, but still you win!"<<endl;
                cout<<"Exiting Program ...";
                exit (0); // Terminate program
            }
            if(arrowPath[j] == hazards[0]) // if arrow kills player while ricocheting
            {
                cout<<"You just shot yourself, and are dying."<<endl;
                cout<<"It didn't take long, you're dead."<<endl;
                cout<<"Exiting Program ...";
                exit (0); // Terminate program
            }
        }
        start = arrowPath[j];
    }
    
    // moves wumpus to minimum adjacent room if arrow is shot and misses wumpus
    int min = cave[hazards[1]][0]; // store minimum adjacent room value
    if (min > cave[hazards[1]][1]){
        min = cave[hazards[1]][1];
    }
    if(min > cave[hazards[1]][2]){
        min = cave[hazards[1]][2];
    }
    hazards[1] = min;
    cout<<"You are in room "<<hazards[0]<<"."<<endl;
}

int main()
{
    // Seed the random number generator.  Change seed to time(0) to change output each time.
    // srand(time(0));
    srand(1);   // Use this version in what you turn in
    // Lots more code goes here...
    // ...
    
    // creating the array for the cave
    int cave [21] [3] = {{0,0,0}, // index number is room number along and the contents are the rooms
        // the path can lead you to
        {2,5,8},
        {1,3,10},
        {2,4,12},
        {3,5,14},
        {1,4,6},
        {5,7,15},
        {6,8,17},
        {1,7,9},
        {8,10,18},
        {11,9,2},
        {10,12,19},
        {11,3,13},
        {12,14,20},
        {4,13,15},
        {6,14,16},
        {15,17,20},
        {7,16,18},
        {9,17,19},
        {11,18,20},
        {13,16,19}
    };
    
    char selection; // taking input choice from user
    int roomNumber; // taking input for room number that player should enter from user
    
    // initializes hazard array elements to 0 before random numberr are generated
    int hazards[7];
    for (int j=0; j<7; j++){
        hazards[j] = 0;
    }
    
    randomNumberGenerator(hazards, false, false); // initially generates random numbers for hazards
    // and player
    printLocation(cave, hazards, hazards[0]); // prints location of player at beginning of game
    int roomNumbers; // to take room numbers for the arrow path from the user
    // loop to take input from user
    do{
        cout << moveCounter<< ". Enter your move (or 'D' for directions): ";
        cin >> selection;
        moveCounter++; // increments room number
        selection = toupper(selection);
        
        if (selection == 'M'){
            cin >> roomNumber; // input for room number
            cout<<endl;
            if (roomNumber == hazards[6]){ // checks if player enters room with arrow
                hazards[6] = -1;
                cout<<"Congratulations, you found the arrow and can once again shoot.";
            }
            roomChecker(cave,roomNumber,hazards); // checks for hazards
        }
        else{
            if (selection == 'D'){ // displays instructions
                displayInstructions();
            }
            if (selection == 'P'){ // displays cave map
                displayCave();
            }
            if (selection == 'C'){ // displays locations of player, hazards and arrow
                printCheatSheet(hazards);
                moveCounter--;
            }
            if (selection == 'R'){ // resets the locations of player, hazards and arrow
                int input;
                cout <<endl<< "(Remember arrow value of -1 means it is with the person.)" << endl;
                cout << "Enter the 7 room locations (1..20) for player, wumpus, bats1, bats2, pit1, pit2, and arrow:" << endl;
                for (int j=0; j<7; j++){
                    cin >> input;
                    hazards[j] = input;
                }
                printLocation(cave, hazards, hazards[0]); // prints player location
                moveCounter=1; // resets move counter to 1
            }
            if (selection == 'S'){
                if(hazards[6]!=-1) // checks to see that player doesn't have arrow
                {
                    cout<<"Sorry, you can't shoot an arrow you don't have.  Go find it."<<endl;
                    cout<<"You are in room "<<hazards[0]<<"."<<endl;
                    continue;
                }
                cout << "Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ";
                int numRooms;
                cin >> numRooms; // taking arrow shoot path
                int numRoomsToShoot[numRooms];
                for (int j=0; j<numRooms; j++){
                    cin >> roomNumbers;
                    numRoomsToShoot[j] = roomNumbers;
                }
                arrowChecker(numRoomsToShoot, cave, hazards, numRooms); // checks if arrow ricochets or
                // hits something
            }
        }
    } while (selection != 'X'); // loop until selection is X
    cout << "\nExiting Program ..." << endl;
    return 0;
}





