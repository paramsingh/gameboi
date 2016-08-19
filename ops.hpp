struct operation
{
	string name;
	int size;
	int cycles;
	operation(string, int , int , bool(*)(cpu*));
	bool (*func)(cpu*);
};
