struct operation
{
	string name;
	int size;
	int cycles;
	operation(string, int , int , int(*)(cpu*));
	int (*func)(cpu*);
};
