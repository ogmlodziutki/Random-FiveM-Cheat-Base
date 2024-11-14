#pragma once


class Auth
{
public:
 	std::string __domain = E("http://localhost/flyside/");
	bool __getserver(std::string hwid); 
};

inline Auth* g_auth = new Auth; 
