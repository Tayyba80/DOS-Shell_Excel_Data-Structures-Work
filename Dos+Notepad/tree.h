#pragma once
#include<iostream>
#include<iterator>
#include<list>
#include <sstream>
#include<string>
#include<queue>
#include<fstream>
#include <chrono>
#include <thread>
#include<limits>
#include "folder.h"

#define elif else if
using namespace std;

class tree {
public:
	folder* currentFolder;
	folder* root;
	priority_queue<file*, vector<file*>, greater<file*>> filePriorityQueue;
	tree()
	{
		root = currentFolder = new folder("V");
	}
	void printPaths(string prompt = "\\>")
	{
		if (currentFolder->folderName == "V")
		{
			cout << " V:" << prompt;
		}
		else
		{
			cout  << currentFolder->path.substr(2, currentFolder->path.length()) << "\\" << currentFolder->folderName << prompt;
		}
	}
	void printPathsAfterLoad(string prompt = "\\>")
	{
		if (currentFolder->folderName == "V")
		{
			cout << " V:" << prompt;
		}
		else
		{
			cout<< "V" << currentFolder->path.substr(2, currentFolder->path.length()) << "\\" << currentFolder->folderName << prompt;
		}
	}
	void addDirectory(folder* newDir)
	{
		currentFolder->folders.push_back(newDir);
	}
	void addFile(file* newFile)
	{
		currentFolder->files.push_back(newFile);
		filePriorityQueue.push(newFile);
	}
	void displayFileQueueWithTime()
	{
		auto currentTime = chrono::system_clock::now();
		while (!filePriorityQueue.empty())
		{
			if (_kbhit() && _getch() == 'c')
			{
				break;
			}
			file* currentFile = filePriorityQueue.top();
			auto timeLeft = currentFile->timeToDisplay -chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - currentTime).count();

			if (timeLeft > 0)
			{
				cout << currentFile->fileName << currentFile->extension<< " - Time Left: " << timeLeft << " seconds" << endl;
			}
			else
			{
				cout << currentFile->fileName << currentFile->extension<< " - Time Expired" << endl;
				filePriorityQueue.pop();
			}
			Sleep(1000);
		}
	}

	bool folderExist(string name)
	{
		list<folder*>::iterator it;
		for (it = currentFolder->folders.begin(); it != currentFolder->folders.end(); ++it)
		{
			folder* folderPointer = *(it);
			if (folderPointer->folderName == name)
			{
				return true;
			}
		}
		return false;
	}
	folder* findFolder(string name)
	{
		list<folder*>::iterator it;
		for (it = currentFolder->folders.begin(); it != currentFolder->folders.end(); ++it)
		{
			folder* currentSubFolder = *it;
			if (currentSubFolder->folderName == name)
			{
				return currentSubFolder;
			}
		}
		return nullptr;
	}
	file* FindFile(string name)
	{
		list<file*>::iterator it;
		for (it = currentFolder->files.begin(); it != currentFolder->files.end(); ++it)
		{
			file* currentFile = *it;
			if (currentFile->fileName+currentFile->extension == name)
			{
				return currentFile;
			}
		}
		return nullptr;
	}
	void DisplayFile()
	{
		list<file*>::iterator it;
		for (it = currentFolder->files.begin(); it != currentFolder->files.end(); ++it)
		{

			file* filePointer = *it;
			cout << "             ";
			printPaths();
			cout << filePointer->fileName << endl;
		}
	}
	void DisplayDirectories()
	{
		folder* forParent = currentFolder;
		int i = numberOfParents();
		while (forParent != root)
		{
			if (forParent->parent != nullptr)
			{
				forParent = forParent->parent;
				cout << "             <DIR>        ";
				for (int j = 0; j < i; j++)
				{
					cout << ".";
				}
				cout << endl;
				i--;
			}

		}
		list<file*>::iterator it;
		for (it = currentFolder->files.begin(); it != currentFolder->files.end(); ++it)
		{
			file* filePointer = *it;
			cout << "                          ";
			cout << filePointer->fileName<<filePointer->extension << endl;
		}
		list<folder*>::iterator iter;
		for (iter = currentFolder->folders.begin(); iter != currentFolder->folders.end(); ++iter)
		{
			folder* folderPointer = *iter;
			cout << "             <DIR>        ";
			cout << folderPointer->folderName << endl;
		}
	}
	int numberOfParents()
	{
		folder* forParent = currentFolder;
		int i = 0;
		while (forParent != root)
		{
			if (forParent->parent != nullptr)
			{
				forParent = forParent->parent;
				i++;
			}
		}
		return i;
	}
	void deleteFile(string name)
	{
		list<file*>::iterator iter;
		for (iter = currentFolder->files.begin(); iter != currentFolder->files.end(); ++iter)
		{
			file* filePointer = *iter;

			if (filePointer->fileName == name)
			{
				currentFolder->files.remove(filePointer);
				break;
			}
		}
	}
	void deleteDir(string toRem)
	{
		list<folder*>::iterator iter;
		for (iter = currentFolder->folders.begin(); iter != currentFolder->folders.end(); ++iter)
		{
			folder* folderPointer = *iter;

			if (folderPointer->folderName == toRem)
			{
				currentFolder->folders.remove(folderPointer);
				break;
			}
		}
	}
	void emptyDir()
	{
		currentFolder->folders.resize(0);
		currentFolder->files.resize(0);
	}
	void printDirectoryStructure(folder* current, int level = 0) 
	{
		for (int i = 0; i < level; ++i) {
			cout << "  "; 
		}
		cout << "|-- " << current->folderName << endl;
		for (const auto& file : current->files) {
			for (int i = 0; i <= level; ++i) {
				cout << "  "; 
			}
			cout << "|   " << file->fileName << endl;
		}
		for (const auto& subfolder : current->folders) {
			printDirectoryStructure(subfolder, level + 1);
		}
	}
	void saveTreeToFile(const folder* current, ofstream& outFile, int level = 0)
	{
		if (!current) return;

		outFile << "D " << current->folderName  << current->path <<endl;
		for (const auto& file : current->files) 
		{
			outFile << "F " << file->fileName << endl;
		}
		for (const auto& subfolder : current->folders) {
			saveTreeToFile(subfolder, outFile, level + 1);
		}
		//outFile << "E" << endl; 
	}
	void saveTreeToFile() 
	{
		ofstream outFile("tree.txt");
		if (outFile.is_open()) 
		{
			saveTreeToFile(root, outFile);
			outFile.close();
		}
		else
		{
			cout<< "Error opening file: " << "tree.txt" << endl;
		}
	}
	folder* loadTreeFromFile(folder* current,ifstream& inFile, folder*& currentFolderC)
	{
		if (!current) return nullptr;

		string s;
		int count = 0;
		folder* rootCheck = nullptr;
		while (getline(inFile, s))
		{
			stringstream ss = stringstream(s);
			string token;
			getline(ss, token, ' ');
			if (token == "D") {
				getline(ss, token, ' ');
				folder* subfolder = new folder;
				subfolder->folderName = token;
				count++;
				if (count == 1)
				{
					rootCheck = subfolder;
				}

				getline(ss, token, ' ');

				if (token == "") 
				{
					root = subfolder;
					currentFolder = root;
				}
				else {
					stringstream ssss = stringstream(token);
					folder* currentFolllder = root;
					string folder;
					while (getline(ssss, folder, '\\'))
					{
						if (folder == "")continue;
						// V naam ka folder ni bnna chahiye
						if (folder == "V")continue;
						for (auto i : currentFolllder->folders) {
							if (i->folderName == folder)
							{
								currentFolllder = i;
								break;
							}
						}
					}
					subfolder->parent = currentFolllder;
					subfolder->path = token;
					currentFolllder->folders.insert(currentFolllder->folders.end(), subfolder);
					currentFolderC = subfolder;
				}
			}
			else if (token == "F")
			{
				if (currentFolderC)
				{
					getline(ss, token, ' ');
					file* newfile = new file(token, ".txt");
					currentFolderC->files.insert(currentFolderC->files.end(), newfile);
					//currentFolderC->files.push_back( newfile);
				}
			}
			
		}
		return rootCheck;
	}
	folder* loadTreeFromFile() 
	{
		ifstream inFile("tree.txt");
		folder* rootCheck = nullptr;
		if (inFile.is_open()) {
			rootCheck=loadTreeFromFile(root,inFile,root);
			inFile.close();
			
		}
		else {
			cout << "Error opening file: " << "tree.txt" << endl;
			
		}
		return rootCheck;
	}
	

};

