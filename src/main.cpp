#include <iostream>
#include <vector>
#include <string>

#include "efs.h"

using namespace std;

int main(int argc, char** args)
{
	int flagState = 0;
	string outputFile = "efs.img";
	string sysroot = ".";
	for(int i = 1; i < argc; i++)
	{
		string arg(args[i]);
		switch(flagState)
		{
		case 0:
			if(arg[0] != '-')
			{
				sysroot = arg;
			}
			else if(arg == "-o")
			{
				flagState = 1;
			}
			break;
		case 1:
			if(arg[0] != '-')
			{
				outputFile = arg;
				flagState = 0;
			}
			else
			{
				cout << "Invalid use of flag '-o'" << endl;
				return -1;
			}
		}
	}
	
	EFS filesystem;
	filesystem.scan(sysroot);
	filesystem.compileImage(outputFile);
	return 0;
}
