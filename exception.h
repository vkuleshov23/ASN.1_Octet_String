#ifndef _EXCEPTION_
#define _EXCEPTION_
#include <cstring>
#include <exception>
class Exception : public std::exception{
private:
	char message[256];
public:
	Exception(const char* msg) : message{0}{
		strcpy(this->message, msg);
	}
	const char* getMessage() const {return this->message;}
	~Exception(){}
};
#endif