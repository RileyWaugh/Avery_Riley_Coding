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
	return (n & (1 << p));
}

void setBit(uint& n, uint p, bool set)
{
	n = (n & (~(1 << p))) | (set << p);
}

void printVecUi(vector<uint> const& a)
{

	for (int i = 0; i < a.size(); i++)
		cout << a.at(i) << ' ';
	cout << endl;
	return;
}


void printVecI(vector<int> const& a)
{
	for (int i = 0; i < a.size(); i++)
		cout << a.at(i) << ' ';
	cout << endl;
	return;
}


bool check_key(const restriction_map& m, int key)
{
	// Key is not present
	if (m.find(key) == m.end())
	{
		return false;
	}
	return true;
}


bool checkList(const vector<uint>& tl)
{
	uint li = tl.size() - 1;
	if (li <= 1)
	{
		return true;
	}
	uint mgap = (li >> 1);
	uint gap = 1;
	bool bad = false;
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


void combinePlaceRestrictions(uint& lpr, const uint& spr)
{
	lpr = lpr | spr;
}

restriction_map copyRelevantRestrictions(const restriction_map& large_rest, uint min_place)
{
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
	restriction_map new_rest = copyRelevantRestrictions(large_rest, min_place);
	// this copying is inefficient -- hopefully there is a way aroun this
	uint place;

	for (auto& it : small_rest)
	{
		// Do stuff
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
	return (pr + 1) == (1 << sn);
}



int getMaxPossibleLength(const restriction_map& rest, uint sn)
{
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


int main_opt_1(const uint sn)
{
	uint symbolNum = sn;
	//cout << symbolNum;
	vector<uint> curList = { 0 };
	//done = 
	vector<uint> maxList = { 0 };
	uint maxListLength = 1;

	stack<uint> maxNumStack;
	maxNumStack.push(0);

	stack<uint> nextNumStack;
	nextNumStack.push(0);

	stack<restriction_map> restrictions;
	restrictions.push(restriction_map());


	/*vector<Node*> previousNumbers = { (new Node(0, NULL)) };
	for (uint i = 1; i < symbolNum; i++)
	{
		previousNumbers.push_back(NULL);
	}*/
	//Node* toDelete = NULL;

	vector<int> numberJumpBacks = { -1 };
	vector<int>	lastJumpBacks = { 0 };
	for (uint i = 1; i < symbolNum; i++)
	{
		lastJumpBacks.push_back(-1);
	}

	stack<uint> emptyStack;

	const uint table_size = (int(pow(3, symbolNum)) + 15) * 2; //more or less arbitrary guess 
	vector<vector<stack<uint>>> restrictions_table;
	for (uint i = 0; i < symbolNum; i++)
	{
		restrictions_table.push_back({});
		for (uint j = 0; j < table_size; j++)
		{
			restrictions_table[i].push_back(emptyStack);
		}
	}
	restrictions_table[0][0].push(0);

	vector<uint> block_table;
	for (uint i = 0; i < table_size; i++)
	{
		block_table.push_back(0);
	}



	uint ncap;
	uint csize = 1;
	uint undnum = symbolNum + 2;
	uint topnum;
	uint lastnum;

	int mps;



	while (csize > 0)
	{
		/*if (csize < 18)
		{
			printVecUi(curList);
		}*/

		ncap = min(maxNumStack.top() + 1, symbolNum - 1);

		/*curLen = len(curList)
		for next_symbol in range(0, ncap) :
		print("nns", nextNumStack)
		print(restrictions, getMaxPossibleLength(restrictions[-1]), maxListLength)
		printVecUi(curList);
		printVecI(numberJumpBacks);
		printVecI(lastJumpBacks);*/


		if (nextNumStack.top() == undnum)
		{
			popRestrictionsTable(curList, restrictions_table, numberJumpBacks, block_table, table_size);

			lastnum = curList[csize - 1];
			lastJumpBacks[lastnum] = numberJumpBacks[csize - 1];
			numberJumpBacks.pop_back();

			curList.pop_back();
			csize--;
			nextNumStack.pop();
			maxNumStack.pop();
			//restrictions.pop();


		}
		else
		{
			mps = getMaxPossibleLength_2(block_table, csize - 1, symbolNum);
			if ((getBit(block_table[csize], nextNumStack.top()) == true) || (1 <= mps && mps <= maxListLength))
			{
				nextNumStack.top()++;
				if (nextNumStack.top() > ncap)
				{
					nextNumStack.top() = undnum;
				}
			}
			else
			{
				curList.push_back(nextNumStack.top());
				csize++;

				nextNumStack.top()++;
				if (nextNumStack.top() > ncap)
				{
					nextNumStack.top() = undnum;
				}
				nextNumStack.push(0);
				//print("test", curList, nextNumStack, "\n")

				lastnum = curList[csize - 1];
				numberJumpBacks.push_back(lastJumpBacks[lastnum]);
				lastJumpBacks[lastnum] = csize - 1;


				maxNumStack.push(max(maxNumStack.top(), curList[csize - 1]));
				//restrictions.push(combineRestrictions(restrictions.top(), getNewRestrictions(curList, symbolNum, numberJumpBacks), csize));
				addToRestrictionsTable(curList, restrictions_table, numberJumpBacks, block_table, table_size);




				//#print(curList, "\n", restrictions, "\n", getNewRestrictions(curList, symbolNum), "\n")


				//printVecUi(curList);
				//cout << endl;
				if (csize > maxListLength)
				{
					maxListLength = csize;
					maxList = curList;
					//cout << "new max:" << endl;
					printVecUi(maxList);
				}

			}

		}





	}

	printVecUi(maxList);
	std::cout << maxListLength << endl;
	std::cout << "done!" << endl;
	return 0;
}


int main_opt_2()
{
	const uint cap = 4;
	for (int i = 2; i <= cap; i++)
	{
		cout << endl << i << " symbols-----\n" << endl;
		main_opt_1(i);
	}
	return 0;
}
