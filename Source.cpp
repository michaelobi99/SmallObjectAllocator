#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <memory>
#include "Allocator.h"

struct student {
	const char* matricNumber;
	const char* name;
};

//test code
int main() {
	std::vector<std::unique_ptr<student>, MyAllocator<std::unique_ptr<student>, 1024>> students;
	for (int i = 0; i < 10; ++i)
		students.push_back(std::make_unique<student>("400", "mike"));
	for (int i = 0; i < 10; ++i)
		std::cout << students[i]->name << " " << students[i]->matricNumber << "\n";
}