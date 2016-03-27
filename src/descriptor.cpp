/*
 * descriptor.cpp
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


#include "descriptor.h"

#include <cstring>


Descriptor::Descriptor()
{
	
}

Descriptor::Descriptor(
	uint64_t next, 
	uint64_t parent, 
	uint64_t size, 
	uint32_t frags, 
	uint32_t mark, 
	uint64_t location, 
	string name)
{
	this->next = next;
	this->parent = parent;
	this->size = size;
	this->frags = frags;
	this->mark = mark;
	this->location = location;
	this->name = name;
}

void Descriptor::write(void* loc)
{
	memcpy(loc, &next, 8);
	memcpy(loc+8, &parent, 8);
	memcpy(loc+16, &size, 8);
	memcpy(loc+24, &frags, 4);
	memcpy(loc+28, &mark, 4);
	memcpy(loc+32, &location, 8);
	memset(loc+40, 0, 216);
	strcpy((char*)(loc + 256), name.c_str());
}
