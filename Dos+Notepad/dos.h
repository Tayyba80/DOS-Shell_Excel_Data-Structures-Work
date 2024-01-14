#pragma once
#include<iostream>
#include<Windows.h>
#include "notepad.h"
#include<exception>
#include<string>
#include "tree.h"

class DOS
{
private:
	string Lower(string s)
	{
		for (int i = 0; i < s.length(); i++)
		{
			s[i] = tolower(s[i]);
		}
		return s;
	}

	bool prompt = false;

public:
	tree t;
	notepad n;
	string name = "Tayyba";
	bool isLoad= false;

	void print()
	{
		cout << "\t\t\t\t" << " |Tayyba Haider| |2022-CS-198| |Section D|" << endl << endl;
		t.printPaths();
		if(isLoad)
			t.printPathsAfterLoad();
	}
	void run()
	{
		print();
		
		bool exit = false;
		while (!exit)
		{
			exit = input();
		}
	}
	bool input()
	{
		string command;
		getline(cin, command);
		string mainCommand = command.substr(0, command.find(" "));
		mainCommand = Lower(mainCommand);

		if (mainCommand == "mkdir")
		{
			t.addDirectory(new folder(command.substr(mainCommand.length() + 1, command.length()), t.currentFolder->path + "\\" + t.currentFolder->folderName, t.currentFolder));
		}
		else if (mainCommand == "create")
		{ 
			string name = command.substr(mainCommand.length() + 1, command.find(".") - mainCommand.length() - 1);
			string ext = command.substr(name.length() + mainCommand.length() + 1, command.find(" ")-2);
			string pri = command.substr(ext.length() + name.length() + mainCommand.length()+1 , command.find(" ")-3);
			string time = command.substr(pri.length() + ext.length() + name.length() + mainCommand.length() + 1, command.length());

			if (ext != "" && pri !="" && time!="")
			{
				int times = stoi(time);
				int prio = stoi(pri);
				t.addFile(new file(name, ext, prio, times));
				string fullname = t.currentFolder->folderName + "-" + name + ext;
				n.creatFileForDos(fullname);
			}
			else
			{
				cout << "Invalid Input choice"<<endl;
				cout << "[File name] [extension] [priority] [Time for Display]" << endl;
				
			}
		}
		else if (mainCommand == "pprint")
		{
			string name = command.substr(mainCommand.length() + 1, command.find(".") - mainCommand.length() - 1);
			string ext = command.substr(name.length() + mainCommand.length() + 1, command.find(" ") - 2);
			string pri = command.substr(ext.length() + name.length() + mainCommand.length() + 1, command.find(" ") - 3);
			string time = command.substr(pri.length() + ext.length() + name.length() + mainCommand.length() + 1, command.length());

			if (name != " " && ext != "" && pri != "" && time != "")
			{
				int times = stoi(time);
				int prio = stoi(pri);
				t.addFile(new file(name, ext, prio, times));
				string fullname = t.currentFolder->folderName + "-" + name + ext;
				n.creatFileForDos(fullname);
				t.displayFileQueueWithTime();
			}
			else
			{
				cout << "Invalid Input choice" << endl;
				cout << "[File name] [extension] [priority] [Time for Display]" << endl;

			}
		}
		else if (mainCommand == "pqueue")
		{
			t.displayFileQueueWithTime();
		}
		else if (mainCommand == "attrib")
		{
			t.DisplayFile();
		}
		else if (mainCommand == "edit")
		{
			string name = command.substr(mainCommand.length() + 1, command.length());
			string fullname = t.currentFolder->folderName+"-"+ name;
			n.openFileForDos(fullname);
		}
		else if (mainCommand == "dir")
		{
			t.DisplayDirectories();
		}
		else if (mainCommand == "rmdir")
		{
			string toRem = t.currentFolder->folderName;
			t.currentFolder = t.currentFolder->parent;
			t.deleteDir(toRem);
		}
		else if (mainCommand == "cd")
		{
			if (t.folderExist(command.substr(mainCommand.length() + 1, command.length())))
			{
				t.currentFolder = t.findFolder(command.substr(mainCommand.length() + 1, command.length()));
			}
			else
				cout << "No such folder exist" << endl;
		}
		else if (mainCommand == "cd.")
		{
		}
		else if (mainCommand == "cd..")
		{
			if (t.currentFolder->parent != nullptr)
			{
				t.currentFolder = t.currentFolder->parent;
			}
		}
		else if (mainCommand == "cd\\")
		{
			if (t.currentFolder->parent != nullptr)
			{
				t.currentFolder = t.root;
			}
		}
		else if (mainCommand == "del")
		{
			t.deleteFile(command.substr(mainCommand.length() + 1, command.length()));
		}
		else if (mainCommand == "find")
		{
			file* filestatus = t.FindFile(command.substr(mainCommand.length() + 1, command.length()));
			try
			{
				if (filestatus != nullptr)
					cout <<"            "<< filestatus->fileName + filestatus->extension << endl;;
			}
			catch (const invalid_argument& e)
			{
				cout << "--------";
			}
		}
		else if (mainCommand == "findf")
		{
			string name = command.substr(mainCommand.length() + 1, command.find(".") - mainCommand.length() - 1);
			string ext = command.substr(name.length() + mainCommand.length() + 1, command.find(" "));
			string word= command.substr(name.length()+ ext.length() + mainCommand.length() + 1, command.length());
			string fullname = t.currentFolder->folderName + "-" + name + ext;
			string fname = fullname.substr(0, fullname.length() - 1);
			n.findString(fname, word);
		}
		else if (mainCommand == "findstr")
		{
			list<file*>::iterator it;
			for (it =t.currentFolder->files.begin(); it !=t. currentFolder->files.end(); ++it)
			{
				file* currentFile = *it;
				string filename= currentFile->fileName + currentFile->extension;
				string word = command.substr(mainCommand.length() + 1 , command.length());
				string fullname = t.currentFolder->folderName+"-" + filename;
				n.findString(fullname, word);
			}
		}
		else if (mainCommand == "copy")        //V\f1\x.txt\ V\f1\f2\*/                                             
		{  
			command = command.substr(mainCommand.length() + 3, command.length());
			string sourcePath = command.substr(0, command.find(" "));
			string destPath = command.substr(sourcePath.length() + 3, command.length());
			file* toCopy;

			folder* cf = t.root;
			string toFind;

			while (sourcePath.length() > 0)
			{
				toFind = sourcePath.substr(0, sourcePath.find("\\"));
				sourcePath = sourcePath.substr(toFind.length() + 1, sourcePath.length());
				if (sourcePath.length() != 0)
				{
					cf = t.currentFolder->findFolder(toFind, cf);
				}
				if (cf == nullptr)
				{
					cout << "Path not found" << endl;
					return true;
				}
			}
			toCopy = t.FindFile(toFind);

			cf = t.root;
			while (destPath.length() > 0)
			{
				toFind = destPath.substr(0, destPath.find("\\"));
				destPath = destPath.substr(toFind.length() + 1, destPath.length());
				cf = t.currentFolder->findFolder(toFind, cf);
				if (cf == nullptr)
				{
					cout << "Path not found";
					return true;
				}
			}
			t.currentFolder->copyFile(toCopy, cf);
			cout << "             1 file(s) copied" << endl;
		}
		else if (mainCommand == "mov")
		{
			command = command.substr(mainCommand.length() + 3, command.length());
			string sourcePath = command.substr(0, command.find(" "));
			string destPath = command.substr(sourcePath.length() + 3, command.length());
			file* toMov;

			folder* cf = t.root;
			string toFind;

			while (sourcePath.length() > 0)
			{
				toFind = sourcePath.substr(0, sourcePath.find("\\"));
				sourcePath = sourcePath.substr(toFind.length() + 1, sourcePath.length());
				if (sourcePath.length() != 0)
				{
					cf = t.currentFolder->findFolder(toFind, cf);
				}
				if (cf == nullptr)
				{
					cout << "Path not found" << endl;
					return true;
				}
			}
			toMov = t.FindFile(toFind);
			string name = toMov->fileName;
			t.deleteFile(name);

			cf = t.root;
			while (destPath.length() > 0)
			{
				toFind = destPath.substr(0, destPath.find("\\"));
				destPath = destPath.substr(toFind.length() + 1, destPath.length());
				cf = t.currentFolder->findFolder(toFind, cf);
				if (cf == nullptr)
				{
					cout << "Path not found";
					return true;
				}
			}
			t.currentFolder->copyFile(toMov, cf);
			cout << "             1 file(s) moved" << endl;
		}
		
		else if (mainCommand == "format")
		{
			t.emptyDir();
		}
		else if (mainCommand == "rename")
		{
			command = command.substr(mainCommand.length() + 1, command.length());
			string currName = command.substr(0, command.find(" "));
			string newName = command.substr(currName.length() + 1, command.length());
			file* requiredFile = t.FindFile(currName);
			requiredFile->fileName = newName;
		}
		else if (mainCommand == "pwd")
		{
			cout << "               ";
			if (isLoad)
				t.printPathsAfterLoad();
			else
				t.printPaths();
			cout << endl;
		}
		else if (mainCommand == "tree")
		{
			t.printDirectoryStructure(t.root);
		}
		else if (mainCommand == "loadtree")
		{
			folder* rootCheck=t.loadTreeFromFile();
			isLoad = true;
			t.root = rootCheck;
		}
		else if (mainCommand == "ver")
		{
			cout << "Microsoft Windows[Version 10.0.19045.3693]" << endl;
		}
		else if (mainCommand == "exit")
		{
			return true;
		}
		else if (mainCommand == "prompt")
		{
			prompt = !prompt;
		}
		else if (mainCommand == "help")
		{
			cout << "ATTRIB Displays file(name provided as input) attributes." << endl;
			cout <<"CD Displays the name of or changes the current directory.See details below." << endl;
			cout << "CD.Prints working directory(i.e.your current directory in your tree structure)" << endl;
			cout << "CD..Change directory to previous directory" << endl;
			cout << "CD\ Changes directory to root directory(i.e.V:\ & gt;)" << endl;
			cout << "CONVERT Asks two types and converts extension of all files of one type to another type." << endl;
			cout << "MOVE Moves one file(whose name is provided as input) from one directory to another directory." << endl;
			cout << "                     [Source path][Destination Path]" << endl;
			cout << "COPY Copies one file in the current directory to another location(directory).See details below." << endl;
			cout << "                     [Source path][Destination Path]" << endl;
			cout << "MKDIR Creates a virtual directory." << endl;
			cout << "                     [Folder name]" << endl;
			cout << "CREATE Creates a file with the name provided and allows the user to enter contents of the file." << endl;
			cout << "                     [File name] [Extension] [Priority] [Time to print]" << endl;
			cout << "DEL Delete a file whose name is provided in input." << endl;
			cout << "DIR Displays a list of files and subdirectories in a directory." << endl;
			cout << "EDIT Opens a file(loads into a linked list) and allows the user to edit and save the contents ofthe file.See bullet 5 below" << endl;
			cout << "EXIT Quits the program" << endl;
			cout << "FIND Searches for a file in your current virtual directory whose name is provided as input." << endl;
			cout << "FINDF Searches for a text string in the currently open file or the file whose name is provided as input." << endl;
			cout << "FINDSTR Searches for strings in all files of your current virtual directory, prints names of files with the string" << endl;
			cout << "FORMAT Formats the current virtual directory i.e.empties the current directory and all subdirectories." << endl;
			cout << "HELP Provides Help information for all these commands." << endl;
			cout << "LOADTREE Load a given tree in your tree structure.Given tree is in a file named tree.txt" << endl;
			cout << "PPRINT Adds a text file to the priority based print queue, and displays the current priority queue." << endl;
			cout << "                       [File name] [Extension] [Priority] [Time to print]" << endl;
			cout << "PROMPT Changes the Windows command prompt(for example from V : \& gt; to V$)." << endl;
			//cout << "PRINT Adds a text file to the print queue, and displays the current queue." << endl;
			cout << "PQUEUE Shows current state of the priority based print queue, with time left for each element" << endl;
			cout << "PWD Prints working directory" << endl;
			//cout << "QUEUE Shows current state of the print queue, with time left for each element" << endl;
			cout << "RENAME Renames a file whose current and new name is provided as input." << endl;
			cout << "RMDIR Removes a directory along with its contents.Consider setting the respective sibling links" << endl;
			cout << "SAVE Saves the currently open file to disk." << endl;
		}
		else if (mainCommand == "cls")
		{
			system("cls");
			print();
			
		}
		else if (mainCommand == "save")
		{
			t.saveTreeToFile();
		}
		if (mainCommand != "cls")
		{
			if (prompt)
			{
				if (isLoad)
					t.printPathsAfterLoad("$");
				else
					t.printPaths();
			}
			else
			if (isLoad)
				t.printPathsAfterLoad();
			else
				t.printPaths();
		}
		return false;
	}
};

