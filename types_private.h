#ifndef _TYPES_PRIVATE_H_
#define _TYPES_PRIVATE_H_

#include <string>
#include <cinttypes>

typedef struct
{
	std::string name;
	uint64_t size;
}FileInfo;

#endif