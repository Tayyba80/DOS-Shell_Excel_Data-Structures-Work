#pragma once
#include<iostream>
#include <vector>
using namespace std;

class file
{
public:
	string fileName;
	string extension;
	int priority;
	int timeToDisplay;
	file(string Fn, string fextention, int pri, int time)
	{
		fileName = Fn;
		extension = fextention;
		priority = pri;
		timeToDisplay = time;
	}
	file(string Fn, string fextention)
	{
		fileName = Fn;
		extension = fextention;
	}
	bool operator>(const file& other) const
	{
		return priority > other.priority;
	}

};

