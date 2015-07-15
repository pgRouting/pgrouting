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
