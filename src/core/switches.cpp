
#include "switches.h"
#include "physics.h"

// im checking if lift 1 can travel between floors 1 n 2
bool canUseLift1(int fromFloor, int toFloor)
{
    
 if (fromFloor == 2 && toFloor == 1)return true;

    
    
if (fromFloor == 1 && toFloor == 2) return true;

return false;
}

// im checking if lift 2 can travel between floors 2 4 5
bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft)
{
    
//im making sure the floor im moving from is 2 or 4 or 5
 if (fromFloor != 2 && fromFloor != 4 && fromFloor != 5) return false;


//validation on reaching floor
if (toFloor != 2 && toFloor != 4 && toFloor != 5) return false;



//the lift isnt used if the two floors are same
if (fromFloor == toFloor) return false;

if (lift2Operational) return true;


if (emergencyTripsLeft>0 && emergencyTripsLeft<=3) return true;
        
 return false;
 
 
}
        
        
        
// the master key is needed on f3r2 and f3r1
bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount)
{
 return hasItem(inventory, inventoryCount, ENT_MASTER_KEY);
}




// im installing fuse in f3r4 and changing stats
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int &inventoryCount, bool objectives[NUM_OBJECTIVES], int &score)
{
    //im making sure fuse is used once max
     if (objectives[OBJ_FUSE_INSTALLED]) return false;
     
     
      if (!hasItem(inventory, inventoryCount, ENT_FUSE)) return false;


//im removing it once used
if (!removeItem(inventory, inventoryCount, ENT_FUSE)) return false;


objectives[OBJ_FUSE_COLLECTED]=true;

objectives[OBJ_FUSE_INSTALLED]=true;


//i will activate lift if player has liftpass 
objectives[OBJ_LIFT2_ACTIVE] = objectives[OBJ_LIFT_PASS];



applyScoreDelta(score, SCORE_INSTALL_FUSE);
return true;
     
     
}





// im checkingg if code is 427
bool verifyCabinetCode(const char *enteredCode, const bool objectives[NUM_OBJECTIVES])
{
 if (enteredCode == 0) return false;
 
  if (!objectives[OBJ_CLUE_4] || !objectives[OBJ_CLUE_2] || !objectives[OBJ_CLUE_7]) return false;
 
 //im checking each character 
  if (enteredCode[0] != '4' || enteredCode[1] != '2' ||
        enteredCode[2] != '7' || enteredCode[3] != '\0') return false;
    
    
    return true;
        
}
