#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

int main()
{
	std::fstream fs;
	fs.open("/home/f_x_p/test_code/c++/testfile");	
	if(!fs.is_open()){
		std::cerr << "file open failed!" << std::endl;	
		return -1;
	}
	int i = 0;
	while(!fs.eof())
	{
		std::string str;		
		if(!std::getline(fs, str)){
			std::cerr << "getline failed!" << std::endl;
			if((fs.rdstate() & std::fstream::eofbit) != 0)
			{
					continue;
			}else{
				fs.clear();	
			}
		}
		std::cout << str << std::endl;
		if(i == 0){
			fs.setstate(std::fstream::badbit);
			++i;
		}
	}

}
