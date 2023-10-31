#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>


namespace nc {

	static unsigned int signifier = 0;

	std::string ToUpper(std::string in)
	{
		for(int i = 0; i < in.size(); i++)
		{
			in[i] = toupper(in[i]);
		}
	return in;
	}

	std::string ToLower(std::string in)
	{
		for (int i = 0; i < in.size(); i++)
		{
			in[i] = tolower(in[i]);
		}

	return in;
	}

	bool IsEqualIgnoreCase(std::string a, std::string b)
	{
		return (ToLower(a) == ToLower(b));
	}

	std::string MakeUnique(std::string in)
	{
		signifier++;
	return in + std::to_string(signifier);


	}

}