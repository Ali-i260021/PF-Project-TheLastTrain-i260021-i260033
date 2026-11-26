#include "simulator.h"
#include "level.h"
#include <iostream>
using namespace std;

int main(){

int floor=1;

int score=INITIAL_SCORE;

char inventory[MAX_INVENTORY_CAPACITY];
int inventorycount=0;
int trips=0;

int room=1;
int energy=INITIAL_ENERGY;
int time=INITIAL_TIME;
bool objectives[NUM_OBJECTIVES]={false};

char conn[NUM_FLOORS][ROOMS_PER_FLOOR-1];
char entities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
int counts[NUM_FLOORS][ROOMS_PER_FLOOR]={0};

// im loading the building
if(!loadBuildingMap("data/building.txt",conn,entities,counts)){
   cout <<"map loading failed"<<endl;
return 1;
}

// im loading game states
if(!loadInitialState("data/initial-state.txt",floor,room,energy,time,score,inventory,inventorycount,objectives,trips)){
   cout <<"loading failed"<<endl;
   return 1;
}

cout <<"-_The Last Train_-"<<endl;

// im starting game
runSimulation(floor,room,energy,time,score,inventory,inventorycount,objectives,trips,conn,entities,counts);


}