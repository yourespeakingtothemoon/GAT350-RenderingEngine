#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>


namespace nc {



	 class ncString {

	public:

	

		//Set String to upper case
		std::string static ToUpper(std::string in)
		{
			//loop through string and set each character to upper case
			for (int i = 0; i < in.size(); i++)
			{
				in[i] = toupper(in[i]);
			}
			return in;
		}

		//Set String to lower case
		std::string static ToLower(std::string in)
		{
			//loop through string and set each character to lower case
			for (int i = 0; i < in.size(); i++)
			{
				in[i] = tolower(in[i]);
			}

			return in;
		}

		//check if string is equal to another string
		bool static IsEqualIgnoreCase(std::string a, std::string b)
		{
			//set to lower and check if equal
			return (ToLower(a) == ToLower(b));
		}

		//make string unique by attaching a unsigned int to the end
		std::string static MakeUnique(std::string in)
		{
			static unsigned int signifier =0;

			//increment signifier and return string with signifier
			signifier++;
			return in + std::to_string(signifier);


		}
	};
}