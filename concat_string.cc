#include <iostream>
#include <sstream>
#include <string>
// 作用：     利用concat_string接口可以随便不分类型的将他们作为字符串叠加在一起
// 	      方便了不同类型序列化的繁杂代码书写过程
// 用法：　   直接在concat_string(arg1, arg2, arg3, ...);这样就直接将args串在一块了
// 代码来源:  http://www.reddit.com/r/cpp/comments/21pxx2/what_is_your_favorite_little_c_snippet_of_code
// 注：       这可是reddit上面最受欢迎的c++代码片段哟

inline void build_string(std::ostream& o) {}

template<class First, class... Rest>
inline void build_string(std::ostream& o, const First& value, const Rest&... rest)
{
	o << value;
	build_string(o, rest...);
}

template<class... T>
std::string concat_string(const T&... value)
{
	std::ostringstream o;
	build_string(o, value...);
	return o.str();
}

int main()
{
	int year = 2014;
	int month = 11;
	int day = 19;
	std::cout << concat_string(year, '-', month, '-', day) << std::endl;
}
