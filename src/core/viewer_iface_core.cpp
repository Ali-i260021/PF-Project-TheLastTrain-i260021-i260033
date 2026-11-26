#include <iostream>
#include "viewer_iface.h"
#include "util.h"
#include "level.h"
using namespace std;
// Terminal UI and dashboard rendering using primitive parameters

// im showing state of the player using this function
void renderDashboard(int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount, int emergencyTripsLeft){

  for(int i=0;i<=6;i++) cout<<"-";

    cout<<"The last train";

    for(int i=0;i<=6;i++) cout<<"-";

    cout<<endl;
    cout<<"Floor: "<<playerFloor;
    cout<<endl;
    cout<<"Room: "<<playerRoom;

    cout<<endl;
    cout<<"Time: "<<time<<" /100";
    cout<<endl;
    cout<<"Energy: "<<energy<<"/100";
    cout<<endl;
    cout<<"score : "<<score;
    cout<<endl;

    
    if(inventoryCount>0)
    {
        cout<<"inventory: ";

        // im showing all inventory items in this loop
        for(int i=0;i<inventoryCount;i++) cout<<inventory[i]<< "  ";
    }
    else
        cout<<"inventory: "<<"empty";

  cout<<endl;

  cout<<"Emergency lift tripps : "<<emergencyTripsLeft;

   cout<<endl;
}

// im showing all items and room details 
void renderRoomDetails(int floor,int room,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCount)
{

    for(int i=0;i<=6;i++) cout<<"-";

    cout<<"Room details";

    for(int i=0;i<=6;i++) cout <<"-";
    cout<<endl;

   cout<<"floor :"<<floor;
   cout<<endl;

   cout<<"room: "<<room;
   cout<<endl;

   cout<<"items in the room: "<<entityCount;

   cout<<endl;

   cout<<endl;

   
    if(entityCount!=0)
    {
        for(int i=0;i<entityCount;i++)
        {
            //im using getentitynamem to get itemname through symbol
            // i removed 1 because i need index position but floor starts from 1
            cout<<i+1<<"  "<<getEntityName(roomEntities[floor-1][room-1][i]);
            cout<<endl;
        }
    }
    else
        cout<<"no items here";

    cout<<endl;
    for(int i=0;i<=17;i++) cout<<"-";
}

// im showing completed objectives
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES])
{
    for(int i=0;i<=6;i++) cout<<"-";

    cout<<"Mission objjectives";

    for(int i=0;i<=6;i++) cout <<"-";

    cout<<endl;

   
    if(objectives[0]==0)
        cout<<"master key collected :"<<"no";
    else
        cout<<"master key collected :"<<"yes";

    cout<<endl;

    
    if(objectives[1]&&objectives[2]&&objectives[3])
        cout<<"clue digits found: yes";
    else
        cout<<"clue digits found: no";
    cout<<endl;

   
    if(objectives[4])
        cout<<"cabinet unlocked: yes";
    else
        cout<<"cabinet unlocked: no";

    cout<<endl;

  
    if(objectives[5])
        cout<<"train key takenm: yes";
    else
        cout<<"train key taken: no";
    cout<<endl;

    
    if(objectives[10])
        cout<<"passenger rescued: yes";
    else
        cout<<"passenger rescued: no";
    cout<<endl;

    
    if(objectives[12])
        cout<<"cat secured: yes";
    else
        cout<<"cat secured: no";

    cout<<endl;

    
    if(objectives[7])
        cout<<"fuse installed: yes";
    else
        cout<<"fuse installed: no";

    cout<<endl;

  
    if(objectives[8])
        cout<<"lift pass collected: yes";
    else
        cout<<"lift pass collected: no";

    cout<<endl;

 
    if(objectives[13])
        cout<<"train prepared: yes";
    else
        cout<<"train prepared: no";

    cout<<endl;

  
    if(objectives[14])
        cout<<"mission won: yes";
    else
        cout<<"mission won: no";

    cout<<endl;

    cout<<endl;
    for(int i=0;i<=6;i++)
        cout<<"-";

    cout<<endl;
}

// im displaying map and showing playr location on map
void renderAsciiBuilding(int playerfloor,int playerroom)
{
    cout<<"The Last Train building map";
    cout<<endl;

    
    for(int i=0;i<=5-1;i++)
    {
        cout<<"Floor "<<5-i<<":  ";

        
        for(int j=0;j<4;j++)
        {
            cout<<"[R"<<j+1<<"]";

            // im showing the player 
            if(playerfloor== 5-i && playerroom== j+1) cout<<"[P] ";
        }
        cout<<endl;
    }
}