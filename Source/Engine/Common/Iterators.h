#pragma once

/*
///////////////////////////////////////////////////
 arrays and other data structures iterators
///////////////////////////////////////////////////
*/


// gets static array length
#define GET_ARRAY_LENGTH (arr) ((sizeof(arr)) / (sizeof(arr[0])))

// while(1)
#define LOOP while(1)

// foor loop for simple arrays
#define FOREACH_ARRAY(n) for(unsigned int i = 0; i < n; i++)

// for loop for std::map iterator
#define FOREACH_STD_MAP(stdMap, key, item) \
	for (std::map<key, item>::iterator iter = stdMap.begin(); iter != stdMap.end(); iter++)

// for loop for std::unordered map iterator
#define FOREACH_STD_UNORDERED_MAP(stdUMap, key, item) \
    for (std::unordered_map<key, item>::iterator iter = stdUMap.begin(); iter != stdUMap.end(); iter++)