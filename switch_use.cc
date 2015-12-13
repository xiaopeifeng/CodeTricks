#include <iostream>
/// a better use of switch which can decrease the number
/// of 'if' occurence
///

bool is_tspecial(int c)
{
	switch(c)
	{
	case ' ': case '{' : case '}' : case '^' : case '|':
		return true;
	default:
		return false;
	}
}

int main()
{
	char c = ' ';
	if(is_tspecial(c)){
		 std::cout << "yes" << std::endl;
	}else{ 
		std::cout << "no" << std::endl;
	}
}
