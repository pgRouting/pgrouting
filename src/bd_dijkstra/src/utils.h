/******************************************************************************
* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
*

******************************************************************************
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies of this Software or works derived from this Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.

*****************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
#include <vector>
#include <queue>
#include <iostream>




typedef std::vector<long> LongVector;
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
		size_t iStartPos = 0;
		while(true)	
		{
			size_t iPos = strInput.find(chDelim[0],iStartPos);
			if(iPos != std::string::npos)
			{
				std::string strToken;
				strToken = strInput.substr(iStartPos,iPos - iStartPos);
				vecTokens.push_back(strToken);
				iStartPos = iPos + 1;
			}
			else if(iPos == std::string::npos && strInput.length() > 0 )
			{
				std::string strToken;
				strToken = strInput.substr(iStartPos, strInput.length() - iStartPos);
				if(strToken.length() > 0)
					vecTokens.push_back(strToken);
				break;
			}		
			else 
				break;

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
