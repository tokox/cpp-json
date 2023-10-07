#include <iostream>
#include "json.hpp"

int main()
{
	json::object obj;
	std::cin >> obj;
	std::cout << obj << std::endl;
}
