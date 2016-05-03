/*
 * efs.cpp
 * 
 * Copyright 2016 Nathan Giddings <nathan@localhost>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "efs.h"

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

EFS::EFS()
	: DirectoryScanner()
{
	
}

EFS::EFS(string bootfile)
	: DirectoryScanner()
{
	this->bootfile = bootfile;
}

void EFS::scanCallback(File& file)
{
	if(file.getFilename() != ".." 
		&& file.getFilename() != "." 
		&& (file.getType() == REGULAR || file.getType() == DIRECTORY))
	{
		files.push_back(file);
		if(file.getType() == DIRECTORY)
		{
			scan(file.getFilename());
		}
	}
}

void EFS::compileImage(string output)
{
	uint64_t filesizeSum = generateDescriptors();
	ofstream fout(output, ofstream::binary);
	writeEFSHead(fout, 1 + files.size() + filesizeSum);
	for(Descriptor d : desc)
	{
		writeEFSFileHead(fout, d);
	}
	for(File f : files)
	{
		writeFile(fout, f);
	}
	fout.close();
}

uint64_t EFS::generateDescriptors()
{
	uint64_t filesizeSum = 0;
	uint64_t index = 0;
	for(File f : files)
	{
		
		uint64_t filesize = f.getType() == REGULAR ? (f.getSize() + (512 - (f.getSize() % 512))) / 512 : 0;
		uint64_t parent = getIndexByPath(f.getPath());
		uint64_t next = index + 1 < files.size() ? index + 2 : 1;
		uint32_t frags = 1;
		uint32_t mark = f.getFilename() == bootfile ? 0x544F4F42 : 0;
		cout << f.getFilename() << " : " << bootfile << endl;
		uint64_t location = f.getType() == REGULAR ? 1 + files.size() + filesizeSum : 0;
		desc.push_back(Descriptor(next, parent, filesize, frags, mark, location, f.getFilename()));
		index++;
		filesizeSum += filesize + (filesize > 0 ? 1 : 0);
	}
	return filesizeSum;
}

void EFS::writeEFSHead(ofstream& fout, uint64_t imgSize)
{
	char* sector = new char[512];
	uint64_t descListLoc = 1;
	memset(sector, 0, 512);
	memcpy(sector, &EFS_MAGIC_1, 8);
	memcpy(sector+8, &EFS_MAGIC_2, 8);
	memcpy(sector+16, &imgSize, 8);
	memcpy(sector+24, &descListLoc, 8);
	
	fout.write(sector, 512);
	delete[] sector;
}

void EFS::writeEFSFileHead(ofstream& fout, Descriptor& desc)
{
	char* sector = new char[512];
	memset(sector, 0 ,512);
	desc.write(sector);
	fout.write(sector, 512);
	delete[] sector;
}

void EFS::writeFile(ofstream& fout, File& file)
{
	if(file.getType() == REGULAR)
	{
		size_t fsize = file.getSize() + (512 - (file.getSize() % 512));
		char* chunkSector = new char[512];
		char* fileData = new char[fsize];
		memset(chunkSector, 0, 512);
		memset(fileData, 0, fsize);
		ifstream fin(file.getPath() + "/" + file.getFilename(), ifstream::binary);
		fin.read(fileData, file.getSize());
		fout.write(chunkSector, 512);
		fout.write(fileData, fsize);
		fin.close();
		delete[] chunkSector;
		delete[] fileData;
	}
}

uint64_t EFS::getIndexByPath(string path)
{
	uint64_t index = 0;
	for(File f : files)
	{
		if(f.getPath() + "/" + f.getFilename() == path)
		{
			return index + 2;
		}
	}
	return 0;
}
