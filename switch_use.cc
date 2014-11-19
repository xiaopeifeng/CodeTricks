#include <iostream>
// 使用这种switch case的方法，在某些简单的场景下可以减少if语句的使用，极大的增加了程序的观赏性和可读性
// 这里只是给出一种思路，来减少if语句的使用

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
	if(is_tspecial(c)) std::cout << "yes" << std::endl;
	else std::cout << "no" << std::endl;
}
