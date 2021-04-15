#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static int revbuilder(int *a)
{
	int i = 64;
	for(; i > 0; i--)
	{
		a[i] = i;
	}
	return 1;
}

static int forwardbuilder(int *a)// reference pg.147
{
	int i = 0;
	
	for(; i < 64; i++)
	{
		a[i] = i;
	}
	return 1;
}

static int swap(int i, int j, int *a)
{
int c = a[i];
a[i] = a[j];
a[j] = c;
return 1;
}

static int find_smallest(int *a, size_t length, int j)
{
	int small;
	small = a[j];
	int n;
	n = j;
	for(; j < length; j++)
	{
		if(small > a[j])
		{
			small = a[j];
			n = j;
		}
	}
	return n;
}

static int selection_sort(int *a, size_t length)//pg.180 of text
{
	int i;
	for(i = 0; i < length; i++)
	{
		swap(find_smallest(a, length, i), i);
	}
	/*
	int i;//for current array pt
	int j;// iterator through current array.
	int smallnow;
	int smalltobe;
	smallnow = a[0];
	smalltobe = a[0];
	
	
	
	for(i = 0; i < length; i++)
	{
		for(j = i + 1; j < length; j++)
		{
			if(smallnow > a[j] && smalltobe > a[j])
			{
				smalltobe = a[j];
			}
		}
		
	}
	*/
	return 1;
	
}

static int iterbin_search(int *a, int l, int r, int m)
{
	while (l <= r) 
	{
		int mid = l + (r-l)/2;
		
		if(a[mid] == m)
			return mid;
		if(a[mid] < m)
			l = mid + 1;
		else
			r = mid - 1;
	}
	return -1;
}

static int recbin_search(int *a, int l, int r, int m)
{
	if (r >= l) 
	{
		int mid = l + (r - l)/2;
		
		if(a[mid] == m)
			return mid;
		
		if(a[mid] > m)
			return recbin_search(a, l, mid - 1, m);
			
		return recbin_search(a, mid + 1, r, m);
	}
	return -1;
}

int main(int argc, char *argv[])
{
	int a[64];
	revbuilder(a);
	selection_sort(a, sizeof(64));
	int f;
	int g;
	int x0;
	f = iterbin_search(a, 0, 63, 23);
	//print to show.
	g = recbin_search(a, 0, 63, 23);
	//print to show.
	x0 = f - g;
	//DUMP
	return 0;
}
