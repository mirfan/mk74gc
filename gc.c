#include <stdio.h>
#define HEAP_SIZE  32
//512
#define ROOTS_N 10

#define NULL_DATA 0
#define FWD_PTR 1
#define CONST_INT_DATA 2

int heap[2*HEAP_SIZE] = {0};
int from_hp = 0, to_hp = HEAP_SIZE;
//scavenging

int roots[ROOTS_N];
int roots_i=0;

void trigger_gc();
void copy_objs();
int evacuate(int i);
void traverse_roots();
void scaveneging();

int add_data(int number);
void add_root(int index);

void print_heap();
void print_roots();

void test_case1();


//------------------------------------------------------------------------------------------
// Garbage collector
//------------------------------------------------------------------------------------------


int evacuate(int i){
	if(heap[i]== FWD_PTR)	//check whether already evacuated
		return heap[i+1];
	else {
		//evacuate
		int fwd_ptr_value = to_hp;

		heap[to_hp++] = heap[i];
		heap[i++] = FWD_PTR;
		heap[to_hp++] = heap[i];
		heap[i++] = fwd_ptr_value;

		return fwd_ptr_value;
	}
};

void trigger_gc()
{
	traverse_roots();
	scaveneging();
};

void traverse_roots()
{
	int i;
	for(i=0; i<roots_i; i++)
		roots[i] = evacuate(roots[i]);
};

void scaveneging()
{
	// int i;
	// for(i=0; i<from_hp; i++)
};

void copy_objs()
{
	int i=0;
	while(i<from_hp){
		evacuate(i);
		i+=2;
	}
};


//------------------------------------------------------------------------------------------
// Initializing data
//------------------------------------------------------------------------------------------


int add_data(int number)
{
	int index = from_hp;
	heap[from_hp++] = CONST_INT_DATA;
	heap[from_hp++] = number;
	return index;
}

void add_root(int index){
	roots[roots_i]=index;
	roots_i++;
}


//------------------------------------------------------------------------------------------
// Main function
//------------------------------------------------------------------------------------------


int main(void)
{
	test_case1();
	printf("Before:\nfrom_space:\n");
	print_heap(0, from_hp);
	printf("roots:\n");
	print_roots();

	trigger_gc();

	printf("\n\nAfter:\n");
	printf("from_space:\n");
	print_heap(0, from_hp);
	printf("to_space:\n");
	print_heap(HEAP_SIZE, to_hp);
	printf("roots:\n");
	print_roots();
	return 1;
}


//------------------------------------------------------------------------------------------
// Debugging functions
//------------------------------------------------------------------------------------------


void print_heap(int i, int hn)
{
	while(i<hn){
		switch(heap[i]){
			case FWD_PTR:
				printf("Forward ptr: %d\n", heap[i+1]);
				break;
			case CONST_INT_DATA:
				printf("INTEGER: %d\n", heap[i+1]);
				break;
		}
		i+=2;
	}
}

void print_roots()
{
	int i;
	for(i=0; i<roots_i; i++)
		printf("Root -> %d\n", roots[i]);
}


//------------------------------------------------------------------------------------------
// Test cases
//------------------------------------------------------------------------------------------


void test_case1()
{
	add_root(add_data(10));
	add_root(add_data(30));
	add_root(add_data(20));
}