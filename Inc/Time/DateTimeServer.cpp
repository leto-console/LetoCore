#include "DateTimeServer.hpp"

#include "DateTime.hpp"

bool DateTimeServer::Do()
{
	DateTime::Loop();
	return true;
}
