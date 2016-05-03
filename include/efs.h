/*
 * efs.h
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


#ifndef EFS_H
#define EFS_H

#include <fsutil/scanner.h>
#include <list>

#include "descriptor.h"

class EFS: public DirectoryScanner
{
public:

		EFS();
		
		EFS(string bootfile);
		
		virtual void scanCallback(File& file);
		
		void compileImage(string output);
			
private:
		
		static const uint64_t EFS_MAGIC_1 = 0x2053494854204948;
		static const uint64_t EFS_MAGIC_2 = 0x21544F4F42205349;

		uint64_t generateDescriptors();
		
		void writeEFSHead(ofstream& fout, uint64_t imgSize);
		
		void writeEFSFileHead(ofstream& fout, Descriptor& desc);
		
		void writeFile(ofstream& fout, File& file);
		
		uint64_t getIndexByPath(string path);

		list<File> files;
		
		list<Descriptor> desc;
		
		string bootfile;
		
};

#endif
