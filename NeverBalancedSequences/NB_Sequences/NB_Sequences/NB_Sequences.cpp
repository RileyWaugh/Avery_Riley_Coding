#pragma once
#include "Sequence_Optimizations.h"
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;
typedef unsigned int uint;


/*void printVecUi(vector<uint> const &a)
{

	for (int i = 0; i < a.size(); i++)
		cout << a.at(i) << ' ';
	cout << endl;
	return;
}

bool checkList(const vector<uint> &tl)
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
}*/


void main1(uint sn)
{
	//Not much optimization here -- just using the "no jumping rule"

	uint symbolNum = sn;

	vector<uint> curList = { 0 };
	//done = 
	vector<uint> maxList = { 0 };
	uint maxListLength = 1;

	stack<uint> maxNumStack;
	maxNumStack.push(0);

	stack<uint> nextNumStack;
	nextNumStack.push(0);

	uint ncap;
	uint csize = 1;
	uint undnum = symbolNum + 2;
	uint topnum;

	while (csize > 0)
	{
		if (nextNumStack.empty())
		{
			std::cout << "empty?" << endl;
		}
		ncap = min(maxNumStack.top() + 1, symbolNum - 1);

		if (nextNumStack.top() == undnum)
		{
			curList.pop_back();
			csize--;
			nextNumStack.pop();
			maxNumStack.pop();
		}

		else
		{
			topnum = nextNumStack.top();
			curList.push_back(topnum);
			csize++;

			nextNumStack.pop();
			nextNumStack.push(topnum + 1);

			if (nextNumStack.top() > ncap)
			{
				nextNumStack.pop();
				nextNumStack.push(undnum);
			}


			if (checkList(curList) == true)
			{
				nextNumStack.push(0);
				maxNumStack.push(max(maxNumStack.top(), curList[csize - 1]));
				if (csize > maxListLength)
				{
					maxListLength = csize;
					maxList = curList;
					printVecUi(maxList);

				}

			}
			else
			{
				curList.pop_back();
				csize--;
			}



		}
	}
	printVecUi(maxList);
	std::cout << maxListLength << endl;
	std::cout << "done!" << endl;
	return;


}





int main()
{
	// driver function
	main_opt_2();
	while (true) {}
	return 0;
}