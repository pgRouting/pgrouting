
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

	bool getToken(std::string& strToken, size_t iTokenIndex)
	{
		auto lTokenCount = vecTokens.size();
		if(iTokenIndex < lTokenCount)
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
