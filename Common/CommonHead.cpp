#include <stdio.h>
#ifdef __linux
#include <dlfcn.h>
#endif

#ifdef __linux
//UTF8
#pragma executi qon_character_set("utf-8")

#include <windows.h>
#else
#include <cmath>
#include <cfloat>
#endif

#include <time.h>
//C++ STL
#include <vector>
#include <string>
#include <map>
//sort 
#include <algorithm>
#include <math.h>

#include <fstream>
#include <iostream>



#define LODGD(format,...)
{
	Logger::GetLogger();
	Logger::Log(DEBUG,__FILE__,__FUNCTION__,__LINE__,format,##__VA_ARGS__);
}


Logger * Logger::GetLogger()
{
	if(0==_instance.get())
	{
		string logFileName = FILENAME_LOG_DEFAULT;

		_instance.reset(new Logger(DEFAULT_LOGDATADIR,logFileName,DEBUG,LOG_FILECOUNT));
	}

	return _instance.get();
}



