#include "Exception.h"
#include <exception>
#include <sstream>

void THROW_ERROR_BASE(int line, const char* file, const char* msg){
		
	std::ostringstream oss;
	oss << "[File] " << file << '\n'
		<< "[Line] " << line << '\n'
		<< "[description] " << msg;
	
	throw std::runtime_error(oss.str());

}
