#pragma once

#include "RedCommon.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdarg.h>
#include <Windows.h>
#include <ctime>

namespace Red
{
	extern	std::ofstream* GLogStream;

	bool InitLog(const RString& fileName);

	void Log(const RChar *string, ...);

	void CloseLog();

	inline RString IntToString(RFloat num)
	{
		std::stringstream ss;
		ss << num;
		RString re;
		ss >> re;
		return re;
	}

	inline RInt RandomInt(RInt from = 0, RInt to = 10)
	{
		RInt ran = rand() & (to - from + 1) + from;
		return ran;
	}

	inline RInt StringToInt(const RString& str)
	{
		return atoi(str.c_str());
	}

	inline RFloat StringToFloat(const RString& str)
	{
		return (RFloat)atof(str.c_str());
	}

	inline RWString ToRWString(const RString& str)
	{
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), NULL, 0);
		WCHAR* tString = new WCHAR[wcsLen + 1];
		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), tString, wcsLen);

		tString[wcsLen] = '\0';

		return RWString(tString);
	}

	inline RBool EqualFloat(RFloat l, RFloat r)
	{
		return abs(l - r) <= EPSILON_E6;
	}

	inline RString	GetNameFromPath(const RString& path)
	{
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");

		if (beg == RString::npos)
		{
			beg = -1;
		}
		return path.substr(beg + 1, end - beg - 1);
	}

	inline RString Trim(const RString& msg)
	{
		const static RString SPACE_CHAR = "\t\f\v\n\r";
		std::size_t beg = msg.find_first_not_of(SPACE_CHAR);

		if (beg > msg.length())
		{
			return RString();
		}
		RString result = msg.substr(beg);

		std::size_t end = result.find_last_not_of(SPACE_CHAR);
		if (end != RString::npos)
			end++;

		return result.substr(0, end);
	}
}