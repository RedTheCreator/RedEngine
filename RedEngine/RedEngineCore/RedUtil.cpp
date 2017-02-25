#include "RedUtil.h"

namespace Red
{
	std::ofstream* GLogStream = NULL;

	bool InitLog(const RString& fileName)
	{
		if (GLogStream == NULL)
		{
			GLogStream = new std::ofstream();
			GLogStream->open(fileName.c_str());
			if (GLogStream->good())
			{
				return true;
			}
			return false;
		}
		return true;
	}

	void Log(const RChar* string, ...)
	{
		if (GLogStream == NULL)
			return;

		RChar buffer[256];
		if (!string || !GLogStream)
			return;

		va_list arglist;
		va_start(arglist, string);
		vsprintf(buffer, string, arglist);
		va_end(arglist);

		RString info(buffer);
		*GLogStream << info << std::endl;
		GLogStream->flush();
	}

	void CloseLog()
	{
		GLogStream->close();
		SafeDelete(GLogStream);
	}
}