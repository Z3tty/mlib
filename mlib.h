#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#pragma once

/*
 * STATE HANDLING
 * -> An enum for different errors, states and returns
 */
enum _ML_STATES { 	
					LL_OUT_OF_BOUNDS 	= 8000,
					LL_SEARCH_FAIL		= 8010
				};

/*
 * CUSTOM DATA TYPES
 * -> Typedef wrappers
 * -> 3D vector definiton
 * -> Linked List node definition
 */
typedef unsigned byte_t;
typedef struct vec3 {
	float x; float y; float z;
} vec3_t;
typedef char* string_t;
typedef unsigned int uint_t;
typedef struct node {
	struct node *next;
	vec3_t data;
} node_t;

/*
 * VEC3 SUPPORT FUNCTIONS
 * -> Vector operations
 * -> Printing
 */
// Generic vector math
extern vec3_t vec3_add(vec3_t v, vec3_t u) 		{ 	return (vec3_t){.x = v.x +u.x, .y = v.y +u.y, .z = v.z +u.z}; 	}
extern vec3_t vec3_sub(vec3_t v, vec3_t u) 		{ 	return (vec3_t){.x = v.x -u.x, .y = v.y -u.y, .z = v.z -u.z}; 	}
extern vec3_t vec3_flip(vec3_t v) 				{ 	return (vec3_t){.x = v.x *-1,  .y = v.y *-1,  .z = v.z *-1}; 	}
extern float vec3_dot(vec3_t v, vec3_t u) 		{	return (float)((v.x *u.x) + (v.y *u.y) + (v.z *u.z));			}
extern vec3_t vec3_const(int k, vec3_t v)		{	return (vec3_t){.x = v.x *k, .y = v.y *k, .z = v.z *k};			}
extern float  vec3_len(vec3_t v)				{	return (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));			}
extern void   vec3_print(vec3_t v)				{   printf("[%f, %f, %f]\n", v.x, v.y, v.z);						}
extern uint_t vec3_compare(vec3_t v, vec3_t u)	{	if(v.x == u.x && v.y == u.y && v.z == u.z) return 1; return 0;	}

/*
 * LINKED LIST OF VEC3 NODES
 * -> End-Node of the list is a NULL object
 * -> Single-linked
 * -> Stores a 3D vector struct, to server as a point thats easy
 *    to manipulate
 */

extern node_t	*create	(vec3_t data)
{
	// Create a node in a linked list
	// vec3_t data => Data to be stored in the node
	node_t *head = (node_t*)(malloc(sizeof(node_t*))); 
	head->next = NULL;
	head->data = data;
	return head;
}
extern node_t 	*prepend(node_t *head, vec3_t data)
{
	// Add a node to the front of a linked list
	// node_t * head => The head of a linked list
	// vec3_t data => Data to be stored at the new head
	node_t * node = create(data);
	node->next = head;
	return node;
}
extern node_t 	*append	(node_t *head, vec3_t data)
{
	// Add a node the the end of a linked list
	// node_t * head => The head of a linked list
	// vec3_t data => Data to be stored at the end of the list
	node_t * cursor = head;
	while(cursor != NULL && cursor->next != NULL)
		cursor = cursor->next;
	cursor->next = create(data);
	return head;
}
extern node_t 	*placeat(node_t *head, vec3_t data, size_t pos)
{
	// Add a node at a certain index
	// node_t * head => The head of a linked list
	// vec3_t data => Data to be stored at the index
	// size_t pos => Index to place at
	// prints an LL_OUT_OF_BOUNDS on error, and returns the original listhead
	node_t * cursor = head;
	uint_t c = 0;
	node_t * newnode = create(data);
	while(cursor != NULL && cursor->next != NULL) {
		if(c++ == pos) { newnode->next = cursor->next; cursor->next = newnode; return head; }
		cursor = cursor->next;
	} printf("Error in extern node_t *placeat(node_t*, vec3_t, size_t) -> ERRNO:\t%d\n", LL_OUT_OF_BOUNDS); return head;
}
extern uint_t	search	(node_t *head, vec3_t data)
{
	// Check if data exists in the list
	// node_t * head => The head of a linked list
	// vec3_t data => Data to search for
	// Returns position if found, LL_SEARCH_FAIL if not
	node_t * cursor = head;
	uint_t c = 0;
	while(cursor != NULL && cursor->next != NULL) {
		if(vec3_compare(cursor->data, data)) return c;
		cursor = cursor->next; c++;
	} return LL_SEARCH_FAIL;
}
extern void   	traverse(node_t *head)
{
	// Traverse the linked list, printing out the data at each node
	// node_t * head => The head of a linked list
	node_t * cursor = head;
	while(cursor != NULL && cursor->next != NULL) {
		vec3_print(cursor->data);
		cursor = cursor->next;
	}
}
extern void 	lldelete	(node_t *head)
{
	// Delete a linked list once its use is finished
	// node_t * head => The head of a linked list
	node_t * cursor = head;
	node_t * next;
	while(cursor != NULL && cursor->next != NULL) {
		next = cursor->next;
		free(cursor); cursor = next;
	} head = NULL;
}

/*
 * UTILITY
 * -> Generic length (Element count) of arrays
 * NOTICE: Array length checker is NULL terminated. Meaning 0 will be seen as end-of-array
 * Due to the way pointers work, you should self-terminate arrays with a 0/0.0f/0l value
 * Uses generic to switch to the correct function as needed
 */
size_t length_str(string_t s) 	{ return strlen(s); 						}
size_t length_int(int i[]) 		{ uint_t c = 0; while(*i++) c++; return c; 	}
size_t length_flt(float f[])	{ uint_t c = 0; while(*f++) c++; return c; 	}
size_t length_sht(short s[])	{ uint_t c = 0; while(*s++) c++; return c;	}
size_t length_lng(long l[])		{ uint_t c = 0; while(*l++) c++; return c;	}
size_t length_dbl(double d[])	{ uint_t c = 0; while(*d++) c++; return c;	}
#define length(x) _Generic((x),												\
								string_t:	length_str,						\
								int*:		length_int,						\
								float*:		length_flt,						\
								short*:		length_sht,						\
								long*:		length_lng,						\
								double*:	length_dbl 						\
								)(x)

// EOF