// StringUtil Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "StringUtils.h"

int main()
{
    std::string testStr = "StringUtil";
    std::string testStr2 = "sTringuTil";
    std::string testStr3 = "Not StringUtil";

    std::cout << nc::ToLower(testStr) << std::endl;
    std::cout << nc::ToUpper(testStr3) << std::endl;
    std::cout << "are " + testStr + " and " + testStr2 + " equal? " << nc::IsEqualIgnoreCase(testStr, testStr2) << std::endl;
    std::cout << "are " + testStr + " and " + testStr3 + " equal? " << nc::IsEqualIgnoreCase(testStr, testStr3) << std::endl;
    std::cout << nc::MakeUnique(testStr) << std::endl;
    std::string testStr4 = nc::MakeUnique(testStr);
    std::string testStr5 = nc::MakeUnique(testStr);
    std::cout << "are " + testStr4 + " and " + testStr5 + " equal? " << nc::IsEqualIgnoreCase(testStr4, testStr5) << std::endl;
    std::cout << nc::MakeUnique(testStr2) << std::endl;

    std::cout << nc::MakeUnique(testStr3) << std::endl;



}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
