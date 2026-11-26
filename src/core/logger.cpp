#include "logger.h"
#include "util.h"
#include <fstream>
#include <iostream>
using namespace std;

//in this function im saving game state in the textfile
bool saveGameStateToFile(const char* filename,int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,const bool objectives[NUM_OBJECTIVES],int emergencyTripsLeft,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
ofstream file(filename);

if(!file) return false;

file<<playerFloor<<" "<<playerRoom<<endl;
file<<energy<<" "<<time<<" "<<score<<endl;
file<<emergencyTripsLeft<<endl;
file<<inventoryCount<<endl;

// im saving all the items
for(int i=0;i<inventoryCount;i++) file<<inventory[i]<<" ";

file<<endl;

// here im saving objectives
for(int i=0;i<NUM_OBJECTIVES;i++) file<<objectives[i]<<" ";

file<<endl;

// im saving the items taht were in each room
for(int i=0;i<NUM_FLOORS;i++){
for(int j=0;j<ROOMS_PER_FLOOR;j++){

file<<entityCounts[i][j]<<" ";

for(int k=0;k<entityCounts[i][j];k++) file<<roomEntities[i][j][k]<<" ";

file<<endl;
}
}

file.close();
return !file.fail();
}


// here im loading the saved game states
bool loadGameStateFromFile(const char* filename,int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
ifstream file(filename);

if(!file) return false;

file>>playerFloor>>playerRoom;
file>>energy>>time>>score;
file>>emergencyTripsLeft;
file>>inventoryCount;

if(!file || inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY) return false;


for(int i=0;i<inventoryCount;i++) file>>inventory[i];


for(int i=0;i<NUM_OBJECTIVES;i++) file>>objectives[i];


for(int i=0;i<NUM_FLOORS;i++){
for(int j=0;j<ROOMS_PER_FLOOR;j++){

file>>entityCounts[i][j];

if(!file || entityCounts[i][j]<0 || entityCounts[i][j]>MAX_ENTITIES_PER_ROOM) return false;

for(int k=0;k<entityCounts[i][j];k++) file>>roomEntities[i][j][k];
}
}

return !file.fail();
}


// im writing the game results into file
void writeGameSummaryReport(const char* filename,int score,int remainingEnergy,int remainingTime,const bool objectives[NUM_OBJECTIVES],bool victory)
{
ofstream file(filename);

if(!file) return;

file<<"game summary"<<endl;
file<<"score: "<<score<<endl;
file<<"energy: "<<remainingEnergy<<endl;
file<<"time: "<<remainingTime<<endl;

if(victory) file<<"mission won"<<endl;
else file<<"mission failed"<<endl;

file<<"objectives: "<<endl;

for(int i=0;i<NUM_OBJECTIVES;i++){

file<<"objective"<<i+1<<":  ";

//im seeing if objective is completed
if(objectives[i]) file<<"yes"<<endl;
else file<<"no"<<endl;
}

file.close();
}