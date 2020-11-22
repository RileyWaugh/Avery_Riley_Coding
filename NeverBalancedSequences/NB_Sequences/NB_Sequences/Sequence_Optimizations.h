#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_map>

using namespace std;
typedef unsigned int uint;
typedef unordered_map<uint, uint> restriction_map;

/*
Linked list stuff. I was going to use a vector of linked list for the jumpbacks but I decided a vector was simpler, and I think it has less overhead
even though pop_back() is not in all cases O(1) like it is with linked lists.

class Node {
public:
    Node();
    Node(int p, Node* n);

    int place;
    Node* next;
};
Node* removeHead(Node* n);

void push(Node** head_ref, int new_data);
*/


//bitmap functions
//------------------------------------------
bool getBit(uint n, uint p); // gets the pth bit of a bitstring

void setBit(uint& n, uint p, bool set); // sets the pth bit of a bitstring to <set>
//------------------------------------------

//printing
//------------------------------------------
void printVecUi(vector<uint> const& a); // prints the elements of a vector of unsigned integers 

void printVecI(vector<int> const& a);// prints the elements of a vector of signed integers -- this really ought to just be overloaded or templated. 
//------------------------------------------


//std::unordered_set methods
//------------------------------------------
bool check_key(const restriction_map& m, int key); // checks if a key is present in the dictionary
//------------------------------------------


bool checkList(const vector<uint>& tl);

//restriction dictionary functions
//------------------------------------------
void addRestriction(restriction_map& rest, const uint ind, const uint symb, const uint sn);
// adds a restriction to a particular index (say in spot 10 there is no 2)

restriction_map getNewRestrictions(const vector<uint>& tl, uint sn, const vector<int>& jumps);
// gets all the new restrictions adding the newest number in the sequence produces

restriction_map copyRelevantRestrictions(const restriction_map& large_rest, uint min_place);
// copies a restriction_map, but only future restrictions. This is supposed to ease the time/space wasted when copying restrictions of previous indices.

void combinePlaceRestrictions(uint& lpr, uint spr);
// combines the restrictions for a single index (if I can't have a 1 at spot 10 and I can't have a 2 at spot 10, I can't have a 1 or a 2 at spot 10)


restriction_map combineRestrictions(const restriction_map& large_rest, restriction_map& small_rest, uint min_place);
// combines two restriction_maps (in all places)

bool isFullyRestricted(uint pr, uint sn);
// tests if a restriction for a place is fully restricted (all 1's)


int getMaxPossibleLength(const restriction_map& rest, uint sn);
// gives the maximum possible length the current sequence could ever be based on the restrictions in future indices. Returns -1 if if
// there is no index that is fully blocked 

//------------------------------------------

int main_opt_1(const uint sn);
//finds the maximum sequence with n symbols thta never has 3 of the same symbols evenly spaced.

int main_opt_2();
//hardcoded to call main_opt_1 for sn=2,3,4