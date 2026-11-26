#include "util.h"
#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>
using namespace std;

// im keeping value between min and max
int clampInt(int val,int minval,int maxval){
 if(val<minval) return minval;
 if(val>maxval) return maxval;
 return val;
}

// im removing spaces from sides
void trimWhitespace(char* str){
    int i=0;
    while(isspace(str[i])) i++;
    memmove(str,str+i,strlen(str+i)+1);

    int n=strlen(str);
    //im using loop n finding a nonwhitespace characcter 
    while(n>0 && isspace(str[n-1])) n--;
    str[n]='\0';
}

// im checking if strings are equal
bool stringEquals(const char* a,const char* b){
    return strcmp(a,b)==0;
}

void printHeader(const char* title){
    cout <<title;
    cout << endl;
    cout <<"----------";
    cout << endl;
}


void clearScreen(){
    system("cls");
}
