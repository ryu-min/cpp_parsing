#include "statement.h"

#include <iostream>

void Statement::debugPrint() const {
	std::cout << type.m_name << " " << name << " ";
	for (const auto & statement : statements) {
		statement.debugPrint();
	}
}
