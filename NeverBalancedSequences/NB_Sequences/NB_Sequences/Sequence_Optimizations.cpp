#include "Sequence_Optimizations.h"

/*
Node::Node()
{
	next = NULL;
}

Node::Node(int p, Node* n)
{
	place = p;
	next = n;
}

Node* removeHead(Node* n)
{
	if (n == NULL)
	{
		return NULL;
	}
	Node* temp = n;
	n = n->next;
	delete temp;

	return n;
}


void push(struct Node** head_ref, int new_data)
{
	Node* new_node = new Node;
	new_node->place = new_data;
	new_node->next = (*head_ref);
	(*head_ref) = new_node;
	return;
}
*/

bool getBit(uint n, uint p)
{
	// returns the nth bit of a (binary) integer
	return (n & (1 << p));
}

void setBit(uint& n, uint p, bool set)
{
	// sets the nth bit of an integer to the desired value
	n = (n & (~(1 << p))) | (set << p);
}

void printVecUi(vector<uint> const& a)
{
	// prints a vector of unsigned integers in a sensible way
	for (int i = 0; i < a.size(); i++)
	{
		cout << a.at(i) << ' ';

	}
	cout << endl;
	return;
}


void printVecI(vector<int> const& a)
{
	// prints a vector of integers in a sensible way
	for (int i = 0; i < a.size(); i++)
	{
		cout << a.at(i) << ' ';

	}
	cout << endl;
	return;
}


bool check_key(const restriction_map& m, int key)
{
	// checks if a key is present in a dicionary
	if (m.find(key) == m.end())
	{
		return false;
	}
	return true;
}


bool checkList(const vector<uint>& tl)
{
	//checks if the list has a 3-in-a-row with the last symbol
	uint li = tl.size() - 1;
	if (li <= 1)
	{
		return true;
	}
	uint mgap = (li >> 1);

	uint gap = 1;
	bool bad = false;
	// we just check all possible gap sizes
	for (uint gap = 1; gap <= mgap && bad == false; gap++)
	{
		if (tl[li] == tl[li - gap] && tl[li - gap] == tl[li - (gap << 1)])
		{
			bad = true;
		}
	}
	return !bad;
}


void addRestriction(restriction_map& rest, const uint ind, const uint symb, const uint sn)
{
	//adds a restirction to a restriction_map -- that is, it says that in some spot, some symbol, say a 2, can't go there
	if (check_key(rest, ind) == true)
	{
		setBit(rest[ind], symb, true);
	}

	else
	{
		rest[ind] = (1 << symb);
	}

	return;
}



restriction_map getNewRestrictions(const vector<uint>& tl, uint sn, const vector<int>& jumps)
{
	//Gets all restrictions the last symbol of the sequence creates
	uint li = tl.size() - 1;
	int ci = jumps[li];
	restriction_map rests;
	/*while (ci >= 0)
	{
		addRestriction(rests, li + (li-ci), tl[li], sn);
		ci = jumps[ci];
	}*/
	for (uint i = 0; i < li; i++)
	{
		if (tl[i] == tl[li])
		{
			addRestriction(rests, li + (li - i), tl[li], sn);
		}
	}
	return rests;
}


void combinePlaceRestrictions(uint& lpr, uint spr)
{
	// combines two single-place restictions -- same as bitwise or-'ing
	lpr = lpr | spr;
}

restriction_map copyRelevantRestrictions(const restriction_map& large_rest, uint min_place)
{
	// copies a restriction map, but only the restrictions for future values -- this saves on time/memory
	restriction_map new_rest;
	for (auto& it : large_rest)
	{
		if (it.first >= min_place)
		{
			new_rest[it.first] = it.second;
		}
	}
	return new_rest;
}

restriction_map combineRestrictions(const restriction_map& large_rest, restriction_map& small_rest, uint min_place)
{
	//Combines new restircions with old restirctions to get a new restirction map -- copying the old restircitons is really time-intensive
	restriction_map new_rest = copyRelevantRestrictions(large_rest, min_place);  // this copying is inefficient -- There is a way around this!
	uint place;

	for (auto& it : small_rest)
	{

		place = it.first;
		if (check_key(new_rest, place))
		{
			combinePlaceRestrictions(new_rest[place], small_rest[place]);
		}

		else
		{
			new_rest[place] = small_rest[place];
		}
	}

	return new_rest;

}


bool isFullyRestricted(uint pr, uint sn)
{
	// tests wheter a spot is fully restricted -- just test if it is 1 less than a power of 2!
	return (pr + 1) == (1 << sn);
}



int getMaxPossibleLength(const restriction_map& rest, uint sn)
{
	//finds the maximum possible length of the sequence in the future, based on the nearest fully restricted spot
	// this is based on restriction maps
	int minBlocked = -2;
	for (auto& it : rest)
	{
		if (isFullyRestricted(it.second, sn))
		{
			if (minBlocked == -2 || it.first < minBlocked)
			{
				minBlocked = it.first;
			}
		}

	}
	return minBlocked + 1;

}

int getMaxPossibleLength_2(vector<uint>& block_table, uint li, uint sn)
{
	//finds the maximum possible length of the sequence in the future, based on the nearest fully restricted spot
	// this is based on a table of restrictions (more efficient)
	int minBlocked = -1;
	const uint full_bit_map = (1 << sn) - 1;
	for (uint i = li + 1; i <= 2 * li && minBlocked < 0; i++)
	{
		if (block_table[i] == full_bit_map)
		{
			minBlocked = i;
		}
	}
	return minBlocked;

}

void addToRestrictionsTable(const vector<uint>& clist, vector<vector<stack<uint>>>& table,
	const vector<int>& jump_backs, vector<uint>& block_table,
	const uint table_size)
{
	//adds a (place) restriction to a table of restrictions
	uint li = clist.size() - 1;
	uint lastNumber = clist[li];
	uint nspot;
	int ci = jump_backs[li];
	while (ci >= 0)
	{
		nspot = (li)+(li - ci);
		if (table[lastNumber][nspot].empty())
		{
			setBit(block_table[nspot], lastNumber, true);
		}

		table[lastNumber][nspot].push(li);
		ci = jump_backs[ci];
	}


}

void popRestrictionsTable(const vector<uint>& clist, vector<vector<stack<uint>>>& table,
	const vector<int>& jump_backs, vector<uint>& block_table,
	const uint table_size)
{
	// removes a restriction from the table of restrictions (pops off from the stack of restrictions in that spot)
	uint li = clist.size() - 1;
	uint lastNumber = clist[li];
	uint nspot;
	int ci = jump_backs[li];
	while (ci >= 0)
	{
		nspot = (li)+(li - ci);
		table[lastNumber][nspot].pop();
		if (table[lastNumber][nspot].empty())
		{
			setBit(block_table[nspot], lastNumber, false);
		}

		ci = jump_backs[ci];
	}

}


int main_opt_1(const uint sn) // takes number of symbols as an argument
{
	//driver function

	uint symbolNum = sn; // number of symbols
	//cout << symbolNum;
	vector<uint> curList = { 0 }; // the current sequence of symbols
	//done = 
	vector<uint> maxList = { 0 }; // the longest sequence found
	uint maxListLength = 1;		// the length of the longest sequence found

	stack<uint> maxNumStack;	// sequence of the max symbol hit so far
	maxNumStack.push(0);

	stack<uint> nextNumStack;	// sequence of what number to try next (at each spot i) in the current sequence
	nextNumStack.push(0);

	/*
	*restriction_map stuff -- now defunct
	*
	stack<restriction_map> restrictions;
	restrictions.push(restriction_map());
	*/


	/*
	Linked list stuff -- optional, but not really any better than what is there currently
	vector<Node*> previousNumbers = { (new Node(0, NULL)) };
	for (uint i = 1; i < symbolNum; i++)
	{
		previousNumbers.push_back(NULL);
	}*/
	//Node* toDelete = NULL;

	vector<int> numberJumpBacks = { -1 }; // jumpbacks -- efficient way of remembering all previous same symbols: at each index, provides the index for the most recent same symbol
	vector<int>	lastJumpBacks = { 0 }; // rembmers indices of the most recent symbols of each type
	for (uint i = 1; i < symbolNum; i++)
	{
		lastJumpBacks.push_back(-1); // -1 means that it is the first symbol of its kind
	}

	stack<uint> emptyStack; // literally an empty stack

	const uint table_size = (int(pow(3, symbolNum)) + 15) * 2; // size of the restrciton table (size based on a more or less arbitrary guess) 
	vector<vector<stack<uint>>> restrictions_table; // one subtable for each symbol, each subtable contains an array of stacks which keep track of which spot restricted the given number in which spot
	for (uint i = 0; i < symbolNum; i++)
	{
		restrictions_table.push_back({});
		for (uint j = 0; j < table_size; j++)
		{
			restrictions_table[i].push_back(emptyStack); // initalizing with empy stacks
		}
	}
	restrictions_table[0][0].push(0);

	vector<uint> block_table; // table of how each spot is blocked
	for (uint i = 0; i < table_size; i++)
	{
		block_table.push_back(0);
	}


	//integers that show up later
	uint ncap;
	uint csize = 1; // current size of the sequence
	uint undnum = symbolNum + 2; // a special symbol
	uint topnum;
	uint lastnum;

	int mps;



	while (csize > 0)
	{
		/*if (csize < 18)
		{
			printVecUi(curList);
		}*/

		ncap = min(maxNumStack.top() + 1, symbolNum - 1); // this is the maximum symbol that could occur next -- no jumping!

		/*curLen = len(curList)
		for next_symbol in range(0, ncap) :
		print("nns", nextNumStack)
		print(restrictions, getMaxPossibleLength(restrictions[-1]), maxListLength)
		printVecUi(curList);
		printVecI(numberJumpBacks);
		printVecI(lastJumpBacks);*/


		if (nextNumStack.top() == undnum) // if the next num to add is the special symbol, this means we are out of options and have to pop
		{
			popRestrictionsTable(curList, restrictions_table, numberJumpBacks, block_table, table_size); // pop off from the table of restrictions

			lastnum = curList[csize - 1]; // this is the number we must pop off
			lastJumpBacks[lastnum] = numberJumpBacks[csize - 1]; // fix the lastjumpbacks
			numberJumpBacks.pop_back();	// fix the jumpbacks

			curList.pop_back(); // pop off the current symbol
			csize--; // size decreases by 1
			nextNumStack.pop(); // get rid of the current next number
			maxNumStack.pop(); // get rid of the current maxNum
			//restrictions.pop();


		}
		else // otherwise we are (probably) adding a symbol
		{
			mps = getMaxPossibleLength_2(block_table, csize - 1, symbolNum); // maxlength the sequence could concievably be, according to the restrictions
			if ((getBit(block_table[csize], nextNumStack.top()) == true) || (1 <= mps && mps <= maxListLength)) // if the number we are trying to add is blocked or if the max 
																												   //length we could achieve is smaller than the current max length
			{
				nextNumStack.top()++; // we can't do the current number, so go on to the next
				if (nextNumStack.top() > ncap) // but if the next number is too high, we should insted pop off next time around
				{
					nextNumStack.top() = undnum; //... so we set it to the special "error" symbol
				}
			}
			else // otherwise we are actually adding a symbol (finally)
			{
				curList.push_back(nextNumStack.top()); // add the symbol
				csize++; // track the size

				nextNumStack.top()++; // next number in this spot goes up
				if (nextNumStack.top() > ncap)
				{
					nextNumStack.top() = undnum; // if too high, track that we will pop it off later using the special symbol
				}
				nextNumStack.push(0); // the next symbol should be a 0 (it could always be)
				//print("test", curList, nextNumStack, "\n")

				lastnum = curList[csize - 1]; // temp variable: just the current number
				numberJumpBacks.push_back(lastJumpBacks[lastnum]); // record the jumpback for the current number
				lastJumpBacks[lastnum] = csize - 1; // record that this was the most recent time the currrent symbol was hit


				maxNumStack.push(max(maxNumStack.top(), curList[csize - 1])); // track the maxnums

				//restrictions.push(combineRestrictions(restrictions.top(), getNewRestrictions(curList, symbolNum, numberJumpBacks), csize));

				addToRestrictionsTable(curList, restrictions_table, numberJumpBacks, block_table, table_size); // add restrictions (this is fairly complicated!)




				//#print(curList, "\n", restrictions, "\n", getNewRestrictions(curList, symbolNum), "\n")


				//printVecUi(curList);
				//cout << endl;


				// and now we keep track of the longest sequnce in the standard way
				if (csize > maxListLength)
				{
					maxListLength = csize;
					maxList = curList;
					//cout << "new max:" << endl;
					printVecUi(maxList); // we print the new max each time
				}

			}

		}





	}

	// print the true max and indicate we are done!
	printVecUi(maxList);
	std::cout << maxListLength << endl;
	std::cout << "done!" << endl;
	return 0;
}


int main_opt_2() // calls the main driver for symbol numbers 2-n (n is hardcoded)
{
	const uint cap = 4; // hardcoded -- up to 4 symbols
	for (int i = 2; i <= cap; i++)
	{
		cout << endl << i << " symbols-----\n" << endl;
		main_opt_1(i);
	}
	return 0;
}
