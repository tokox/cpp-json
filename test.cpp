#include <iostream>
#include "json.hpp"

int main()
{
	tokox::json::object obj;
	std::cin >> obj;
	std::cout << obj << std::endl;
}
