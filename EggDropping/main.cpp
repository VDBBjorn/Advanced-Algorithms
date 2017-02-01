# include <stdio.h>
# include <limits.h>
#include <iostream>

// A utility function to get maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

/* Function to get minimum number of trails needed in worst
case with n eggs and k floors */
int eggDrop(int eggs, int floors)
{
	// If there are no floors, then no trials needed. OR if there is
	// one floor, one trial needed.
	if (floors == 1 || floors == 0)
		return floors;

	// We need all floors trials for one egg and all floors
	if (eggs == 1)
		return floors;

	int min = INT_MAX;
	int x, res;

	// Consider all droppings from 1st floor to kth floor and
	// return the minimum of these values plus 1.
	for (x = 1; x <= floors; x++)
	{
		res = max(eggDrop(eggs - 1, x - 1), eggDrop(eggs, floors - x));
		if (res < min)
			min = res;
	}

	return min + 1;
}

int main()
{
	int n = 2, k = 10;
	printf("\nMinimum number of trials in worst case with %d eggs and "
		"%d floors is %d \n", n, k, eggDrop(n, k));
	std::cin.get();
	return 0;
}