#pragma once
#include<iostream>
#include "file.h"
using namespace std;
class folder {
public:
	list<folder*> folders;
	list<file*> files;
	string folderName;
	string path;
	string owner;
	folder* parent;
public:
	folder(string name = " ", string pth = " ", folder* prnt = nullptr, list<folder*> fldr = list<folder*>(), list<file*> fil = list<file*>())
	{
		folderName = name;
		path = pth;
		folders = fldr;
		files = fil;
		parent = prnt;
	}
	folder* findFolder(string name, folder* cf)
	{
		list<folder*>::iterator it;
		for (it = cf->folders.begin(); it != cf->folders.end(); ++it)
		{
			folder* tofind = *it;
			if (tofind->folderName == name)
			{
				return tofind;
			}
		}
		return nullptr;
	}
	file* findFile(string name, folder* cf)
	{
		list<file*>::iterator it;
		for (it = cf->files.begin(); it != cf->files.end(); ++it)
		{
			file* tofind = *it;
			if (tofind->fileName == name)
			{
				return tofind;
			}
		}
		return nullptr;
	}
	void copyFile(file* newFile, folder* cf)
	{
		cf->files.push_back(newFile);
	}

};



