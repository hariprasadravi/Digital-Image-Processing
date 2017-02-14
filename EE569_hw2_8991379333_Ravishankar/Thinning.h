// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <vector>
using namespace std;


vector<vector<int> > Thinning_stage1 = {
	{0,1,0,0,1,1,0,0,0},
	{0,1,0,1,1,0,0,0,0},
	{0,0,0,1,1,0,0,1,0},
	{0,0,0,0,1,1,0,1,0},
	{0,0,1,0,1,1,0,0,1},
	{1,1,1,0,1,0,0,0,0},
	{1,0,0,1,1,0,1,0,0},
	{0,0,0,0,1,0,1,1,1},
	{1,1,0,0,1,1,0,0,0},
	{0,1,0,0,1,1,0,0,1},
	{0,1,1,1,1,0,0,0,0},
	{0,0,1,0,1,1,0,1,0},
	{0,1,1,0,1,1,0,0,0},
	{1,1,0,1,1,0,0,0,0},
	{0,0,0,1,1,0,1,1,0},
	{0,0,0,0,1,1,0,1,1},
	{1,1,0,0,1,1,0,0,1},
	{0,1,1,1,1,0,1,0,0},
	{1,1,1,0,1,1,0,0,0},
	{0,1,1,0,1,1,0,0,1},
	{1,1,1,1,1,0,0,0,0},
	{1,1,0,1,1,0,1,0,0},
	{1,0,0,1,1,0,1,1,0},
	{0,0,0,1,1,0,1,1,1},
	{0,0,0,0,1,1,1,1,1},
	{0,0,1,0,1,1,0,1,1},
	{1,1,1,0,1,1,0,0,1},
	{1,1,1,1,1,0,1,0,0},
	{1,0,0,1,1,0,1,1,1},
	{0,0,1,0,1,1,1,1,1},
	{0,1,1,0,1,1,0,1,1},
	{1,1,1,1,1,1,0,0,0},
	{1,1,0,1,1,0,1,1,0},
	{0,0,0,1,1,1,1,1,1},
	{1,1,1,0,1,1,0,1,1},
	{0,1,1,0,1,1,1,1,1},
	{1,1,1,1,1,1,1,0,0},
	{1,1,1,1,1,1,0,0,1},
	{1,1,1,1,1,0,1,1,0},
	{1,1,0,1,1,0,1,1,1},
	{1,0,0,1,1,1,1,1,1},
	{0,0,1,1,1,1,1,1,1},
	{1,1,1,0,1,1,1,1,1},
	{1,1,1,1,1,1,1,0,1},
	{1,1,1,1,1,0,1,1,1},
	{1,0,1,1,1,1,1,1,1},
};

vector<vector<int> > Thinning_stage2 = {
	{0,0,1,0,1,0,0,0,0},
	{1,0,0,0,1,0,0,0,0},
	{0,0,0,0,1,0,0,1,0},
	{0,0,0,0,1,1,0,0,0},
	{0,0,1,0,1,1,0,0,0},
	{0,1,1,0,1,0,0,0,0},
	{1,1,0,0,1,0,0,0,0},
	{1,0,0,1,1,0,0,0,0},
	{0,0,0,1,1,0,1,0,0},
	{0,0,0,0,1,0,1,1,0},
	{0,0,0,0,1,0,0,1,1},
	{0,0,0,0,1,1,0,0,1},
	{0,1,1,1,1,0,0,0,0},
	{1,1,0,0,1,1,0,0,0},
	{0,1,0,0,1,1,0,0,1},
	{0,0,1,0,1,1,0,1,0},
	{0,1,1,0,1,0,1,0,0},
	{0,0,1,0,1,1,1,0,0},
	{0,1,1,0,1,1,1,0,0},
	{1,1,0,0,1,0,0,0,1},
	{1,0,0,1,1,0,0,0,1},
	{1,1,0,1,1,0,0,0,1},
	{0,0,1,1,1,0,1,0,0},
	{0,0,1,0,1,0,1,1,0},
	{0,0,1,1,1,0,1,1,0},
	{1,0,0,0,1,1,0,0,1},
	{1,0,0,0,1,0,0,1,1},
	{1,0,0,0,1,1,0,1,1},
	{1,1,0,1,1,0,0,0,0},
	{1,1,0,1,1,0,0,0,1},
	{1,1,0,1,1,0,0,1,0},
	{1,1,0,1,1,0,0,1,1},
	{1,1,0,1,1,0,1,0,0},
	{1,1,0,1,1,0,1,0,1},
	{1,1,0,1,1,0,1,1,0},
	{1,1,0,1,1,0,1,1,1},
	{1,1,0,1,1,1,0,0,0},
	{1,1,0,1,1,1,0,0,1},
	{1,1,0,1,1,1,0,1,0},
	{1,1,0,1,1,1,0,1,1},
	{1,1,0,1,1,1,1,0,0},
	{1,1,0,1,1,1,1,0,1},
	{1,1,0,1,1,1,1,1,0},
	{1,1,0,1,1,1,1,1,1},
	{1,1,1,1,1,0,0,0,0},
	{1,1,1,1,1,0,0,0,1},
	{1,1,1,1,1,0,0,1,0},
	{1,1,1,1,1,0,0,1,1},
	{1,1,1,1,1,0,1,0,0},
	{1,1,1,1,1,0,1,0,1},
	{1,1,1,1,1,0,1,1,0},
	{1,1,1,1,1,0,1,1,1},
	{1,1,1,1,1,1,0,0,0},
	{1,1,1,1,1,1,0,0,1},
	{1,1,1,1,1,1,0,1,0},
	{1,1,1,1,1,1,0,1,1},
	{1,1,1,1,1,1,1,0,0},
	{1,1,1,1,1,1,1,0,1},
	{1,1,1,1,1,1,1,1,0},
	{1,1,1,1,1,1,1,1,1},
	{0,1,0,1,1,1,0,0,0},
	{0,1,0,1,1,1,1,0,0},
	{1,1,0,1,1,1,0,0,0},
	{1,1,0,1,1,1,1,0,0},
	{0,1,0,1,1,1,0,0,0},
	{0,1,0,1,1,1,0,0,1},
	{0,1,1,1,1,1,0,0,0},
	{0,1,1,1,1,1,0,0,1},
	{0,0,0,1,1,1,0,1,0},
	{0,0,0,1,1,1,0,1,1},
	{0,0,1,1,1,1,0,1,0},
	{0,0,1,1,1,1,0,1,1},
	{0,0,0,1,1,1,0,1,0},
	{0,0,0,1,1,1,1,1,0},
	{1,0,0,1,1,1,0,1,0},
	{1,0,0,1,1,1,1,1,0},
	{0,1,0,1,1,0,0,1,0},
	{0,1,1,1,1,0,0,1,0},
	{1,1,0,1,1,0,0,1,0},
	{1,1,1,1,1,0,0,1,0},
	{0,1,0,1,1,0,0,1,0},
	{0,1,0,1,1,0,0,1,1},
	{0,1,0,1,1,0,1,1,0},
	{0,1,0,1,1,0,1,1,1},
	{0,1,0,0,1,1,0,1,0},
	{0,1,0,0,1,1,0,1,1},
	{0,1,0,0,1,1,1,1,0},
	{0,1,0,0,1,1,1,1,1},
	{0,1,0,0,1,1,0,1,0},
	{0,1,1,0,1,1,0,1,0},
	{1,1,0,0,1,1,0,1,0},
	{1,1,1,0,1,1,0,1,0},
	{1,0,1,0,1,0,0,0,1},
	{1,0,1,0,1,1,0,0,1},
	{1,1,1,0,1,0,0,0,1},
	{1,1,1,0,1,1,0,0,1},
	{1,0,1,1,1,0,0,0,1},
	{1,0,1,1,1,1,0,0,1},
	{1,1,1,1,1,0,0,0,1},
	{1,1,1,1,1,1,0,0,1},
	{1,0,1,0,1,0,0,1,0},
	{1,0,1,0,1,1,0,1,0},
	{1,1,1,0,1,0,0,1,0},
	{1,1,1,0,1,1,0,1,0},
	{1,0,1,1,1,0,0,1,0},
	{1,0,1,1,1,1,0,1,0},
	{1,1,1,1,1,0,0,1,0},
	{1,1,1,1,1,1,0,1,0},
	{1,0,1,0,1,0,0,1,1},
	{1,0,1,0,1,1,0,1,1},
	{1,1,1,0,1,0,0,1,1},
	{1,1,1,0,1,1,0,1,1},
	{1,0,1,1,1,0,0,1,1},
	{1,0,1,1,1,1,0,1,1},
	{1,1,1,1,1,0,0,1,1},
	{1,1,1,1,1,1,0,1,1},
	{1,0,1,0,1,0,1,0,0},
	{1,0,1,0,1,1,1,0,0},
	{1,1,1,0,1,0,1,0,0},
	{1,1,1,0,1,1,1,0,0},
	{1,0,1,1,1,0,1,0,0},
	{1,0,1,1,1,1,1,0,0},
	{1,1,1,1,1,0,1,0,0},
	{1,1,1,1,1,1,1,0,0},
	{1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,1,1,0,1},
	{1,1,1,0,1,0,1,0,1},
	{1,1,1,0,1,1,1,0,1},
	{1,0,1,1,1,0,1,0,1},
	{1,0,1,1,1,1,1,0,1},
	{1,1,1,1,1,0,1,0,1},
	{1,1,1,1,1,1,1,0,1},
	{1,0,1,0,1,0,1,1,0},
	{1,0,1,0,1,1,1,1,0},
	{1,1,1,0,1,0,1,1,0},
	{1,1,1,0,1,1,1,1,0},
	{1,0,1,1,1,0,1,1,0},
	{1,0,1,1,1,1,1,1,0},
	{1,1,1,1,1,0,1,1,0},
	{1,1,1,1,1,1,1,1,0},
	{1,0,1,0,1,0,1,1,1},
	{1,0,1,0,1,1,1,1,1},
	{1,1,1,0,1,0,1,1,1},
	{1,1,1,0,1,1,1,1,1},
	{1,0,1,1,1,0,1,1,1},
	{1,0,1,1,1,1,1,1,1},
	{1,1,1,1,1,0,1,1,1},
	{1,1,1,1,1,1,1,1,1},
	{1,0,1,0,1,0,1,0,0},
	{1,0,1,0,1,0,1,0,0},
	{1,1,1,0,1,0,1,1,0},
	{1,1,1,0,1,0,1,1,0},
	{1,0,1,1,1,0,1,0,0},
	{1,0,1,1,1,0,1,0,0},
	{1,1,1,1,1,0,1,1,0},
	{1,1,1,1,1,0,1,1,0},
	{1,0,0,0,1,1,1,0,0},
	{1,0,0,0,1,1,1,0,0},
	{1,1,0,0,1,1,1,1,0},
	{1,1,0,0,1,1,1,1,0},
	{1,0,0,1,1,1,1,0,0},
	{1,0,0,1,1,1,1,0,0},
	{1,1,0,1,1,1,1,1,0},
	{1,1,0,1,1,1,1,1,0},
	{1,0,1,0,1,1,1,0,0},
	{1,0,1,0,1,1,1,0,0},
	{1,1,1,0,1,1,1,1,0},
	{1,1,1,0,1,1,1,1,0},
	{1,0,1,1,1,1,1,0,0},
	{1,0,1,1,1,1,1,0,0},
	{1,1,1,1,1,1,1,1,0},
	{1,1,1,1,1,1,1,1,0},
	{1,0,0,0,1,0,1,0,1},
	{1,0,0,0,1,0,1,0,1},
	{1,1,0,0,1,0,1,1,1},
	{1,1,0,0,1,0,1,1,1},
	{1,0,0,1,1,0,1,0,1},
	{1,0,0,1,1,0,1,0,1},
	{1,1,0,1,1,0,1,1,1},
	{1,1,0,1,1,0,1,1,1},
	{1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,0,1,0,1},
	{1,1,1,0,1,0,1,1,1},
	{1,1,1,0,1,0,1,1,1},
	{1,0,1,1,1,0,1,0,1},
	{1,0,1,1,1,0,1,0,1},
	{1,1,1,1,1,0,1,1,1},
	{1,1,1,1,1,0,1,1,1},
	{1,0,0,0,1,1,1,0,1},
	{1,0,0,0,1,1,1,0,1},
	{1,1,0,0,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,1},
	{1,0,0,1,1,1,1,0,1},
	{1,0,0,1,1,1,1,0,1},
	{1,1,0,1,1,1,1,1,1},
	{1,1,0,1,1,1,1,1,1},
	{1,0,1,0,1,1,1,0,1},
	{1,0,1,0,1,1,1,0,1},
	{1,1,1,0,1,1,1,1,1},
	{1,1,1,0,1,1,1,1,1},
	{1,0,1,1,1,1,1,0,1},
	{1,0,1,1,1,1,1,0,1},
	{1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1},
	{1,0,0,0,1,0,1,0,1},
	{1,0,0,0,1,1,1,0,1},
	{1,0,0,0,1,0,1,1,1},
	{1,0,0,0,1,1,1,1,1},
	{1,0,0,1,1,0,1,0,1},
	{1,0,0,1,1,1,1,0,1},
	{1,0,0,1,1,0,1,1,1},
	{1,0,0,1,1,1,1,1,1},
	{0,1,0,0,1,0,1,0,1},
	{0,1,0,0,1,1,1,0,1},
	{0,1,0,0,1,0,1,1,1},
	{0,1,0,0,1,1,1,1,1},
	{0,1,0,1,1,0,1,0,1},
	{0,1,0,1,1,1,1,0,1},
	{0,1,0,1,1,0,1,1,1},
	{0,1,0,1,1,1,1,1,1},
	{1,1,0,0,1,0,1,0,1},
	{1,1,0,0,1,1,1,0,1},
	{1,1,0,0,1,0,1,1,1},
	{1,1,0,0,1,1,1,1,1},
	{1,1,0,1,1,0,1,0,1},
	{1,1,0,1,1,1,1,0,1},
	{1,1,0,1,1,0,1,1,1},
	{1,1,0,1,1,1,1,1,1},
	{0,0,1,0,1,0,1,0,1},
	{0,0,1,0,1,1,1,0,1},
	{0,0,1,0,1,0,1,1,1},
	{0,0,1,0,1,1,1,1,1},
	{0,0,1,1,1,0,1,0,1},
	{0,0,1,1,1,1,1,0,1},
	{0,0,1,1,1,0,1,1,1},
	{0,0,1,1,1,1,1,1,1},
	{1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,1,1,0,1},
	{1,0,1,0,1,0,1,1,1},
	{1,0,1,0,1,1,1,1,1},
	{1,0,1,1,1,0,1,0,1},
	{1,0,1,1,1,1,1,0,1},
	{1,0,1,1,1,0,1,1,1},
	{1,0,1,1,1,1,1,1,1},
	{0,1,1,0,1,0,1,0,1},
	{0,1,1,0,1,1,1,0,1},
	{0,1,1,0,1,0,1,1,1},
	{0,1,1,0,1,1,1,1,1},
	{0,1,1,1,1,0,1,0,1},
	{0,1,1,1,1,1,1,0,1},
	{0,1,1,1,1,0,1,1,1},
	{0,1,1,1,1,1,1,1,1},
	{1,1,1,0,1,0,1,0,1},
	{1,1,1,0,1,1,1,0,1},
	{1,1,1,0,1,0,1,1,1},
	{1,1,1,0,1,1,1,1,1},
	{1,1,1,1,1,0,1,0,1},
	{1,1,1,1,1,1,1,0,1},
	{1,1,1,1,1,0,1,1,1},
	{1,1,1,1,1,1,1,1,1},
	{0,0,1,0,1,0,1,0,1},
	{0,0,1,0,1,1,1,0,1},
	{0,1,1,0,1,0,1,1,1},
	{0,1,1,0,1,1,1,1,1},
	{0,0,1,0,1,0,1,0,1},
	{0,0,1,0,1,1,1,0,1},
	{0,1,1,0,1,0,1,1,1},
	{0,1,1,0,1,1,1,1,1},
	{0,0,1,1,1,0,0,0,1},
	{0,0,1,1,1,1,0,0,1},
	{0,1,1,1,1,0,0,1,1},
	{0,1,1,1,1,1,0,1,1},
	{0,0,1,1,1,0,0,0,1},
	{0,0,1,1,1,1,0,0,1},
	{0,1,1,1,1,0,0,1,1},
	{0,1,1,1,1,1,0,1,1},
	{0,0,1,1,1,0,1,0,1},
	{0,0,1,1,1,1,1,0,1},
	{0,1,1,1,1,0,1,1,1},
	{0,1,1,1,1,1,1,1,1},
	{0,0,1,1,1,0,1,0,1},
	{0,0,1,1,1,1,1,0,1},
	{0,1,1,1,1,0,1,1,1},
	{0,1,1,1,1,1,1,1,1},
	{1,0,1,0,1,0,0,0,1},
	{1,0,1,0,1,1,0,0,1},
	{1,1,1,0,1,0,0,1,1},
	{1,1,1,0,1,1,0,1,1},
	{1,0,1,0,1,0,0,0,1},
	{1,0,1,0,1,1,0,0,1},
	{1,1,1,0,1,0,0,1,1},
	{1,1,1,0,1,1,0,1,1},
	{1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,1,1,0,1},
	{1,1,1,0,1,0,1,1,1},
	{1,1,1,0,1,1,1,1,1},
	{1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,1,1,0,1},
	{1,1,1,0,1,0,1,1,1},
	{1,1,1,0,1,1,1,1,1},
	{1,0,1,1,1,0,0,0,1},
	{1,0,1,1,1,1,0,0,1},
	{1,1,1,1,1,0,0,1,1},
	{1,1,1,1,1,1,0,1,1},
	{1,0,1,1,1,0,0,0,1},
	{1,0,1,1,1,1,0,0,1},
	{1,1,1,1,1,0,0,1,1},
	{1,1,1,1,1,1,0,1,1},
	{1,0,1,1,1,0,1,0,1},
	{1,0,1,1,1,1,1,0,1},
	{1,1,1,1,1,0,1,1,1},
	{1,1,1,1,1,1,1,1,1},
	{1,0,1,1,1,0,1,0,1},
	{1,0,1,1,1,1,1,0,1},
	{1,1,1,1,1,0,1,1,1},
	{1,1,1,1,1,1,1,1,1},
	{0,1,0,0,1,1,1,0,0},
	{0,1,0,0,1,1,1,0,1},
	{1,1,0,0,1,1,1,0,0},
	{1,1,0,0,1,1,1,0,1},
	{0,1,0,1,1,0,0,0,1},
	{0,1,0,1,1,0,1,0,1},
	{0,1,1,1,1,0,0,0,1},
	{0,1,1,1,1,0,1,0,1},
	{0,0,1,1,1,0,0,1,0},
	{0,0,1,1,1,0,0,1,1},
	{1,0,1,1,1,0,0,1,0},
	{1,0,1,1,1,0,0,1,1},
	{1,0,0,0,1,1,0,1,0},
	{1,0,0,0,1,1,1,1,0},
	{1,0,1,0,1,1,0,1,0},
	{1,0,1,0,1,1,1,1,0},
};