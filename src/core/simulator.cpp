#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "level.h"
#include "util.h"
#include <iostream>
#include <string>
using namespace std;

//in this function im moving player and reducing its resources
bool handleMoveCommand(int targetFloor,int targetRoom,int& currentFloor,int& currentRoom,int& energy,int& time,const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,int& emergencyTripsLeft,bool lift2Operational)
{

//im using c to store type of connection
char c;

//im checking connection and getting type
if(!isDirectlyConnected(currentFloor,currentRoom,targetFloor,targetRoom,horizontalConn,c)) return false;

//im checking if the player can go through this connection
if(!canTraverse(targetFloor,targetRoom,c,inventory,inventoryCount,emergencyTripsLeft,lift2Operational)) return false;

//im reducing resources depending on connection type
if(c=='W') applyResourceDelta(energy,time,-3,-2);
else if(c=='1' || c=='2') applyResourceDelta(energy,time,-4,-3);
else applyResourceDelta(energy,time,-2,-1);

//im reducing emergency trips if the lift is not repaired
if(c=='2' && !lift2Operational) emergencyTripsLeft--;

//im changing roo,m and floor now
currentFloor=targetFloor;
currentRoom=targetRoom;

return true;
}


//im inspecting room and finding clues
bool handleInspectCommand(int currentFloor,int currentRoom,int& energy,int& time,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCount,bool objectives[NUM_OBJECTIVES],int& score)
{
renderRoomDetails(currentFloor,currentRoom,roomEntities,entityCount);

for(int i=0;i<entityCount;i++){

// im subtracting 1 to get index of room and floor
char c=roomEntities[currentFloor-1][currentRoom-1][i];


//i made this variable n that will store index of the objective that is related to the clue
int n=-1;

//the clue that the player found will tell me what index i need to use 
if(c=='4') n=1;
else if(c=='2') n=2;
else if(c=='7') n=3;

//here im making sure the points are not given more than once by checking if the conditions are met only once and when theyre met ill give the points
if(n!=-1 && !objectives[n]){
objectives[n]=true;
score+=10;
}
}

//inspecting room redduces both again
applyResourceDelta(energy,time,-1,-1);
return true;
}


//in this function im collecting item from the room
bool handleCollectCommand(char itemSymbol,int currentFloor,int currentRoom,int& energy,int& time,int& score,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int& entityCount,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES])
{
//these will store the position that i will use later
int f=currentFloor-1,r=currentRoom-1;
int pos=-1;

//im finding index of the item that the player is trying to collect and if i find it ill break the loop and store the index in that position variable
for(int i=0;i<entityCount;i++){
if(roomEntities[f][r][i]==itemSymbol){
pos=i;
break;
}
}

//if pos is -1 then it means the item isnt found after the full search
if(pos==-1) return false;

//these items like f.e injured passenger cant be picked up normally
if(itemSymbol=='!' || itemSymbol=='~' || itemSymbol==';' || itemSymbol=='4' || itemSymbol=='2' || itemSymbol=='7') return false;

//im making sure trainkey cant be taken if cabinet is locked or key is taken already
if(itemSymbol=='^' && (!objectives[4] || objectives[5])) return false;

//here im applying the bonuses for the itemms
if(itemSymbol==')') applyResourceDelta(energy,time,25,0);
else if(itemSymbol=='\\') applyResourceDelta(energy,time,15,0);
else if(itemSymbol=='&') applyResourceDelta(energy,time,0,15);
else if(!addItem(inventory,inventoryCount,itemSymbol)) return false;

int n=-1,points=0;

//im fixing the points and objective index for different items here
if(itemSymbol=='<'){n=0;points=10;}
else if(itemSymbol=='^'){n=5;points=10;}
else if(itemSymbol=='*') n=6;
else if(itemSymbol=='('){n=8;points=20;}
else if(itemSymbol=='@') n=11;

//im making sure objective exissts here
if(n!=-1 && !objectives[n]){
objectives[n]=true;
score+=points;
}

//im removing the item thats collected 
for(int i=pos;i<entityCount-1;i++) roomEntities[f][r][i]=roomEntities[f][r][i+1];

entityCount--;

//im clearing the last index once ive shifted items
roomEntities[f][r][entityCount]=' ';

applyResourceDelta(energy,time,-1,-1);
return true;
}


//in this function im using the item at its location
bool handleUseCommand(char itemSymbol,int currentFloor,int currentRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES])
{
//im seeing if player has the item
if(!hasItem(inventory,inventoryCount,itemSymbol)) return false;

//im making sure that player is at control panel and fuse is not installed
if(itemSymbol=='*' && currentFloor==3 && currentRoom==4 && !objectives[7]){

if(!installFuseControlPanel(inventory,inventoryCount,objectives,score)) return false;
}

//im rescuing passenger if player uses the medical kit in some allowed room
else if(itemSymbol=='%' && currentFloor==4 && currentRoom==1 && !objectives[10]){
objectives[10]=true;
removeItem(inventory,inventoryCount,'%');
score+=100;
}

//im securing cat if player uses the cat carrier in an allowed room
else if(itemSymbol=='@' && currentFloor==3 && currentRoom==3 && !objectives[12]){
objectives[12]=true;
removeItem(inventory,inventoryCount,'@');
score+=100;
}

//im preparing train if player uses train key in train room
else if(itemSymbol=='^' && currentFloor==5 && currentRoom==2 && !objectives[13]){
objectives[13]=true;
score+=100;
}

else return false;

applyResourceDelta(energy,time,-1,-1);
return true;
}


// im checking cabinet code in this function
bool handleUnlockCabinetCommand(const char* code,int currentFloor,int currentRoom,bool objectives[NUM_OBJECTIVES],int& score,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int& entityCount)
{
//im making sure player is in cabinet room and cabinet isnt already unlocked
if(currentFloor!=5 || currentRoom!=1 || objectives[4]) return false;

//im checking code and clues using this function
if(!verifyCabinetCode(code,objectives)) return false;

bool found=false;

//im checking if cabinet exists in current room
for(int i=0;i<entityCount;i++){
if(roomEntities[currentFloor-1][currentRoom-1][i]=='^') found=true;
}

if(!found) return false;

objectives[4]=true;
score+=30;

return true;
}


//here im checking if game has ended or no
bool checkEndConditions(int energy,int time,const bool objectives[NUM_OBJECTIVES])
{
//im ending game if i run  out of resources or if mission has been failed or won    
if(energy<=0 || time<=0 || objectives[14] || objectives[15]) return true;
return false;
}


// im running the main game through this function
void runSimulation(int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft,char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){

int choice,f,r;

char item;
string code;

renderAsciiBuilding(playerFloor,playerRoom);

//im using this loop and it will keep running until any of the end condition becoems true
while(!checkEndConditions(energy,time,objectives)){

renderDashboard(playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,emergencyTripsLeft);

 cout<<"1. move"<<endl;
cout<<"2. inspect"<<endl;
cout<<"3. collect item"<<endl;
cout<<"4.  use item"<<endl;
cout<<"5. unlock cabinet"<<endl;
 cout<<"6. objectives"<<endl;
cout<<"7.  save game"<<endl;
 cout<<"8. load game"<<endl;
cout<<"9. map"<<endl;
 cout<<"10.  board train"<<endl;
cout<<"11. exit"<<endl;

cin>>choice;

if(!cin) return;

//i made this variable to check if the action i selected worked or not
bool success=false;

//i am using byref here so that if the count changes it also changes the entity count array
int& count=entityCounts[playerFloor-1][playerRoom-1];

switch(choice){

case 1:
cout<<"enter floor and room"<<endl;
cin>>f>>r;
if(!cin) return;


success=handleMoveCommand(f,r,playerFloor,playerRoom,energy,time,horizontalConn,inventory,inventoryCount,emergencyTripsLeft,objectives[9]);
break;

case 2:
success=handleInspectCommand(playerFloor,playerRoom,energy,time,roomEntities,count,objectives,score);
break;

case 3:
cout<<"enter item symbol"<<endl;
cin>>item;
if(!cin) return;
success=handleCollectCommand(item,playerFloor,playerRoom,energy,time,score,roomEntities,count,inventory,inventoryCount,objectives);
break;

case 4:
cout<<"enter item symbol"<<endl;
cin>>item;
if(!cin) return;
success=handleUseCommand(item,playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives);
break;

case 5:
cout<<"enter code"<<endl;
cin>>code;
if(!cin) return;

//i am using c_str because function needs a cstyle string
success=handleUnlockCabinetCommand(code.c_str(),playerFloor,playerRoom,objectives,score,roomEntities,count);
break;

case 6:
renderObjectivesChecklist(objectives);
continue;

case 7:
//here i am sending current game states to the logger function to save
success=saveGameStateToFile("out/savegame.txt",playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,roomEntities,entityCounts);
break;

case 8:
//i am replacing the current state with the old saved state here
success=loadGameStateFromFile("out/savegame.txt",playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,roomEntities,entityCounts);

if(!success){
cout<<"loading failed"<<endl;
return;
}

//here im seeing if loaded player location is allowed or no
if(!validateStartingRoom(playerFloor,playerRoom,horizontalConn)) return;
break;

case 9:
renderAsciiBuilding(playerFloor,playerRoom);
continue;

case 10:
//im checking if player is at train and all the required objectives are done
if(playerFloor==5 && playerRoom==2 && objectives[13] && objectives[10] && objectives[12] && objectives[7] && objectives[5] && objectives[8] && objectives[0] && objectives[1] && objectives[2] && objectives[3] && objectives[4]){
objectives[14]=true;
score+=100;
success=true;
}
break;

case 11:
return;

default:
cout<<"wrong choice"<<endl;
continue;
}

//im showing the new map after i perform the action
if(success){
cout<<"action successful"<<endl;
renderAsciiBuilding(playerFloor,playerRoom);
}
else cout<<"action failed"<<endl;
}

//im checking final result of game
if(objectives[14]) cout<<"mission won"<<endl;
else{
objectives[15]=true;
cout<<"mission failed"<<endl;
}

//here im sending final game states to make the summary report
writeGameSummaryReport("out/game_summary.txt",score,energy,time,objectives,objectives[14]);
}