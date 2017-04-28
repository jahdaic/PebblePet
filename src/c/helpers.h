#pragma once

unsigned int get_latest(unsigned int stack[])
{	
	for(int i = ARRAY_LENGTH(stack) - 1 ; i >= 0 ; i--)
	{
		if(stack[i])
		{
			return stack[i];
		}
	}	
	
	return 0;
}
	
void push (unsigned int stack[], unsigned int item)
{   
	int stack_max = sizeof(stack);
	
	for(int i = 0 ; i <= stack_max ; i++)
	{
		stack[i] = stack[i + 1];
	}
	
	stack[stack_max] = item;
}