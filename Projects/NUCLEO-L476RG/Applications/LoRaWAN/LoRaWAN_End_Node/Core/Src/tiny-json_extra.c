#include "tiny-json.h"
#include <string.h>
#include "settings_json.h"

struct node{
	//char prop[70];
	const json_t* prop;
	int32_t level;
};

struct node props[Num_Field];
int32_t props_pointer = 0;
const json_t* stack[Num_Field];			///????
int32_t stack_pointer = 0;

void createStack()
{
	stack_pointer = 0;
}

int32_t stackIsEmpty()
{
	if( stack_pointer == 0 ) return 1;
	else return 0;
}

int32_t to_list(const json_t* node, int32_t level)
{
	if(props_pointer <= Num_Field)
	{
		props[props_pointer].prop = node;
		props[props_pointer].level = level;
		props_pointer++;
		return 0;
	}
	else return -1;
	
}

int32_t push(const json_t* node)
{
	if(stack_pointer <= Num_Field)
	{
		stack[stack_pointer] = node;
		stack_pointer++;
		return 0;
	}
	else return -1;
}

const json_t* pop(void)
{
	const json_t* retvalue;
	if(stack_pointer > 0)
	{
		retvalue = stack[stack_pointer];
		stack_pointer--;
		return retvalue;
	}
	else return NULL; // or JSON_NULL
}
///* Search a property by its name in a JSON object. */
//json_t const* json_getProperty( json_t const* obj, char const* property ) {
//    json_t const* sibling;
//    for( sibling = obj->u.c.child; sibling; sibling = sibling->sibling )
//        if ( sibling->name && !strcmp( sibling->name, property ) )
//            return sibling;
//    return 0;
//}

/* Search a property by its name in a JSON object. */
struct node* json_allProperties( json_t const* obj, char const* property ) {
    json_t const* sibling = obj->u.c.child;
	 int32_t level = 0;
	createStack();
	while(sibling && stackIsEmpty())
	{
		to_list(sibling, level);
		if(sibling->u.c.child) {
			push(sibling);
			sibling = sibling -> u.c.child;
			level++;
		}
		else if(sibling->sibling) {
			sibling = sibling->sibling;
		}
		else if(!stackIsEmpty()) {
			while(sibling -> sibling && !stackIsEmpty())
			sibling = pop();
			if( level>0 ) level--;
		}
			if(sibling == NULL) break;
	}
	return props;
}
