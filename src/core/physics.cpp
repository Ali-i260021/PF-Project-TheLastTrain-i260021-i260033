
#include "physics.h"
#include "util.h"

// im updating energy and time and keeping them within limits
void applyResourceDelta(int &energy, int &time, int deltaEnergy, int deltaTime)
{
    time+=deltaTime;
    energy += deltaEnergy;
    
    if(energy>100)energy=100;
    if(energy<0)energy=0;
    
    if(time>100)time=100;
    if(time<0) time=0;
    
}

// im adding or subtracting the player's score
void applyScoreDelta(int &score, int deltascore)
{
    score+=deltascore;
    if(score<0) score=0;
    
    if(score>520)score=520;
}

// im checking if an item is in the inventory
bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol)
{
    //im doing validation on the count
   if (inventoryCount<0 ||inventoryCount>MAX_INVENTORY_CAPACITY) return false;
   
   for(int i =0;i<inventoryCount;i++){
        if (inventory[i] == itemSymbol) return true;
   }
   
  return false;
}

// im adding an item to the inventory
bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int &inventoryCount, char itemSymbol)
{
    //im doing the same validation here on the count to see if i can add item or no
     if (inventoryCount<0 || inventoryCount>=MAX_INVENTORY_CAPACITY) return false;
     
     
     inventory[inventoryCount]=itemSymbol;
     inventoryCount+=1;
     return true;
 
     
}

// im removing an item from the inventory
bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int &inventoryCount, char itemSymbol)
{
    //im doing validation again on count
     if (inventoryCount <= 0 || inventoryCount > MAX_INVENTORY_CAPACITY) return false;
     
     
     //im finding item and removing it then im shiffting the rest of teh stuff to the left 
     for(int i =0;i<inventoryCount;i++){
       if(inventory[i]==itemSymbol){
           for(int j=i;j<inventoryCount-1;j++){
            inventory[j]=inventory[j+1];   
           }
           inventoryCount -=1;
           inventory[inventoryCount]=' ';
           return true;
       }   
     }
     
    
    return false; 

}

//im checking which floors can the lift 2 stop at
bool checkvalidfloor(int floor)
{
 int validchecking = floor==2 || floor==4 || floor==5;
 return validchecking;
}

// im checking if two rooms have any direct connection
bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char &outConnType)
{
    // im assuming theres no connection at start
outConnType = '#';


//im checking if any coordinate is outside building
if(fromF <1 || fromF > NUM_FLOORS || toF <1 || toF > NUM_FLOORS || fromR < 1 || fromR > ROOMS_PER_FLOOR || toR < 1 || toR > ROOMS_PER_FLOOR) return false;


//im seeing if any rooms are neighbours and on same floor
if(toF==fromF && (fromR == toR +1 || fromR == toR -1)){
    int index;
    
    
    //im trying to find index of position of the connection that connects the 2 rooms
    if(fromR < toR) index = fromR-1;
    else index = toR - 1;
    
    //im seeing which connection is available at that place
    char connection = horizontalConn[fromF-1][index];
    
    
//im rejecting walls but allowing everything else    
    if(connection == 'D' || connection == 'W' || connection == 'L'){
        outConnType=connection;
        return true;
    }
    
    return false;
}

//im checking the 3 connections on floors that use room1
if (fromR == 1 && toR == 1) {
 if ((fromF == 1 && toF == 2) || (fromF == 2 && toF == 1) ||(fromF == 2 && toF == 3) || (fromF == 3 && toF == 2) ||
            (fromF == 4 && toF == 5) || (fromF == 5 && toF == 4)){
                
            outConnType = 'S';
            return true;
            
        }
    }


 //im likning the  lift 1 with floor 1 and 2 using room 2 and 3

    if (((fromF == 1 && toF == 2) || (fromF == 2 && toF == 1)) &&
        (fromR == 2 || fromR == 3) &&
        (toR == 2 || toR == 3))
    {
        outConnType = '1';
        return true;
    }
    
    

 //linking room 4 on the floors 542 using lift 2
   
    if (fromF != toF && fromR == 4 && toR == 4 &&
        checkvalidfloor(fromF) && checkvalidfloor(toF)){
        outConnType = '2';
        return true;
    }

    return false;
    
}

// im checking if the player can use a passage
bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational)
{


// im doing validation on the rooms and floors value
 if (targetFloor < 1 || targetFloor > NUM_FLOORS ||
        targetRoom < 1 || targetRoom > ROOMS_PER_FLOOR)
        return false;


//im doing it on inventory amount here
 if (inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
        return false;


//im checking the allowed types of connection 
 if (connType == 'D' || connType == 'W' ||
        connType == 'S' || connType == '1')
        return true;


  // the door is locked so i need to check about master key
    if (connType == 'L')
        return hasItem(inventory, inventoryCount, '<');



//im seeing if the type is lift 2
 if (connType == '2')
    {
        // i need repairred lift and lift pass for lift 2 so im checking if that is available
        if (lift2Operational && hasItem(inventory, inventoryCount, '('))
            return true;

        // im checkign if the lift2 can be used in emergency case
        if (emergencyTripsLeft > 0 && hasItem(inventory, inventoryCount, '$'))
            return true;
    }

return false;
}

