/*PGR

Copyright (c) 2013 Khondoker Md. Razequl Islam
ziboncsedu@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
#include <vector>
#include <queue>
#include <iostream>



typedef std::vector<int> IntVector;
typedef std::vector<double> DoubleVector;
typedef std::vector<std::string> StringVector;


class StringOperation
{

public:

	static std::string TRIMWHITESPACE(std::string strInput)
	{
		std::string strOut = strInput;
		std::string strWht = " \f\n\r\t\v";	

		strOut.erase(0,strOut.find_first_not_of(strWht));
		strOut.erase(strOut.find_last_not_of(strWht) + 1);
		return strOut;
	};
};

class StringTokenizer
{
public:
	StringTokenizer()
	{
		reset();
	}

	void reset()
	{
		vecTokens.clear();
		strInput = "";
	}

	bool parse(std::string strInput, std::string chDelim)
	{
		char *ptr = strtok((char *)strInput.c_str(), chDelim.c_str());
		while(ptr != NULL)
		{
			std::string str = ptr;
			vecTokens.push_back(str);
			ptr = strtok(NULL, chDelim.c_str());
		}
		return true;
	}

	size_t getTokenCount() 
	{
		return vecTokens.size();
	}

	bool getToken(std::string& strToken, long iTokenIndex)
	{
		long lTokenCount = vecTokens.size();
		if(iTokenIndex < 0 || iTokenIndex < lTokenCount)
		{
			strToken = vecTokens[iTokenIndex];
			return true;
		}
		return false;
	}

	bool getTokens(StringVector& vecTokensRef)
	{
		vecTokensRef = vecTokens;
		return true;
	}

private:
	std::string strInput;
	StringVector vecTokens;

};

#endif
