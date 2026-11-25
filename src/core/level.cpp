#include "level.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//here im loading the map and the items in the map
bool loadBuildingMap(const char* filename,char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
ifstream file(filename);
if(!file) return false;

//im storing all the  allowed item symbols in a variable so i can check other items later using it
string line,items="<)\\4*~;(!%$27^@&";
int floor=NUM_FLOORS-1;

// im using this loop to read map from top floor to bottom
while(floor>=0 && getline(file,line)){

// im checking if the floor heading isnt found and if its  not found then i am skipping the line
if(line.find("F"+to_string(floor+1)+"-R1")==string::npos) continue;

//here im making sure line is long enough
if(line.length()<57) return false;

//using this loop im saivng the connections
for(int j=0;j<ROOMS_PER_FLOOR-1;j++){


char c=line[20+j*12];

// im seeing if there is a vaid symbol for connection
if(string("DWL#1").find(c)==string::npos) return false;
horizontalConn[floor][j]=c;
}

// im reading next line which has the room items
if(!getline(file,line) || line.length()<57) return false;

// im saving items for all the rooms
for(int j=0;j<ROOMS_PER_FLOOR;j++){

entityCounts[floor][j]=0;
for(int k=0;k<MAX_ENTITIES_PER_ROOM;k++) roomEntities[floor][j][k]=' ';

// im going through the positions inside each room
for(int k=9+j*12;k<20+j*12;k++){

if(line[k]==' ') continue;

//im using the string::npos keyword with the .find()function because it lets me check if item symbol was not found, npos means not ffound
if(items.find(line[k])==string::npos) return false;


//i made a n variable which counts how many number of items the room has already
int n=entityCounts[floor][j];

if(n>=MAX_ENTITIES_PER_ROOM) return false;

//im adding the itme in the next free index and incrementing count
roomEntities[floor][j][n]=line[k];
entityCounts[floor][j]++;
}
}
floor--;
}

//i made the floor -1 when all floors are read
return floor==-1;
}



// im loading the starting game values in this function
bool loadInitialState(const char* filename,int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft)
{
ifstream file(filename);
if(!file) return false;

string word,line;

//i initialized all values to -1 so that i can check at the end which of them were not loaded which will let me know if theres any error
energy=-1;
time=-1;
score=-1;
inventoryCount=0;
emergencyTripsLeft=3;

for(int i=0;i<MAX_INVENTORY_CAPACITY;i++) inventory[i]=' ';

for(int i=0;i<NUM_OBJECTIVES;i++) objectives[i]=false;


while(file>>word){

//im reading value once the word matches condition
if(word=="INITIAL_ENERGY:"){if(!(file>>energy)) return false;}
else if(word=="INITIAL_TIME:"){if(!(file>>time)) return false;}
else if(word=="INITIAL_SCORE:"){if(!(file>>score)) return false;}

//im gettign the next line if the word isnt needed
else getline(file,line);
}

// im seeing if all the new values are inside limits
if(energy<0 || energy>MAX_ENERGY || time<0 || time>MAX_TIME || score<0) return false;

// im asking player where he/she should start
cout<<"enter floor: ";
cin>>playerFloor;

cout<<"enter room: ";
cin>>playerRoom;

//im checking if input has failed or no
if(!cin) return false;

if(playerFloor<1 || playerFloor>NUM_FLOORS) return false;

if(playerRoom<1 || playerRoom>ROOMS_PER_FLOOR) return false;

return true;
}


//in this function im seeing if room and floor numbers are valid
bool validateStartingRoom(int startFloor,int startRoom,const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1])
{
// i wrpte this line so i dont get the warning "unused parameter horizontalConn"
(void)horizontalConn;

if(startFloor<1 || startFloor>NUM_FLOORS) return false;

if(startRoom<1 || startRoom>ROOMS_PER_FLOOR) return false;

return true;
}


// this function will let me get the the name of an item by using its symbol
const char* getEntityName(char entitySymbol){
switch(entitySymbol){
case '<':return "master key";

case ')': return "energy pack";

case '\\':return "emergency toolkit";

case '4' :return "clue digit 4";

case '*':return "fuse";

case '~':return "cat";

case ';':return "control panel";

case '(' :return "lift pass";

case '!':return "passenger";

case '%':return "medical kit";

case '$':return "emergency access card";

case '2': return "clue digit 2";

case '7':return "clue digit 7";

case '^' :return "cabinet / train key";

case '@':return "cat carrier";

case '&':return "observation token";

default: return "unknown";
}
}
