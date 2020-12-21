#include <Core/Field.h>

using namespace DB;

int main()
{
	Field number = 10;
	Field str = "10";
	
	std::cout << number.getTypeName() << ": " << toString(number) << std::endl;
	std::cout << str.getTypeName() << ": " << toString(str) << std::endl;
	return 0;
}
