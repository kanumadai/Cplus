#define LOG_LEV_DEBUG 1
#define LOG_LEV_ERROR 2
#define LOG_LEV_WARN 3
#define LOG_LEV_INFO 4
#define LOG_LEV_FATAL 5

#define LOG_LEV 2

#define LOG_DEBUG(inf) logger.writeLog(LOG_LEV_DEBUG, __FILE__, __LINE__, __FUNCTION__, inf)
#define LOG_ERROR(inf) logger.writeLog(LOG_LEV_ERROR, __FILE__, __LINE__, __FUNCTION__, inf)
#define LOG_WARN(inf) logger.writeLog(LOG_LEV_WARN, __FILE__, __LINE__, __FUNCTION__, inf)
#define LOG_INFO(inf) logger.writeLog(LOG_LEV_INFO, __FILE__, __LINE__, __FUNCTION__, inf)
#define LOG_RATAL(inf) logger.writeLog(LOG_LEV_FATAL, __FILE__, __LINE__, __FUNCTION__, inf)

//#define LOG(level,inf) writeLog(level, __FILE__, __LINE__, __FUNCTION__, inf)

#ifndef _LOG4C_HPP_
#define  _LOG4C_HPP_

#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

using namespace std;

class Log4c
{
    std::string _logFileName = "";

public:
    Log4c() {
    }
    Log4c(std::string logFileName) {
        _logFileName = logFileName;
        getLogFileName();
    }
    ~Log4c() {
    }
    const char LogTip[6][8] = { "","Debug","Error","Warn","Info","Fatal" };

    void setLogger(std::string logFileName) {
        _logFileName = logFileName;
        getLogFileName();
    }

    void getLogFileName()
    {
        time_t t = time(0);
        char tmp[32] = { NULL };
        tm tmptm;
        localtime_s(&tmptm, &t);
        //file name app date
        strftime(tmp, sizeof(tmp), "%Y-%m-%d", &tmptm);
        strcat_s(tmp, ".log");
        _logFileName = _logFileName + tmp;
    }



    void writeLog(int logLevel,
        const std::string& filename,
        const int line,
        const std::string& function,
        const char* pLogFormat)
    {
        if (logLevel < LOG_LEV) {
            return;
        }

        std::ofstream    OsWrite;
        OsWrite.open(_logFileName, std::ofstream::app);
        char logTxt[2048];
        memset(logTxt, 0, sizeof(logTxt));

        time_t t = time(0);
        char date[32] = { NULL };
        tm tmptm;
        localtime_s(&tmptm, &t);
        //file name app date
        strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &tmptm);

        sprintf_s(logTxt, "%s : [%s] (%s) (%s) (%d): %s \n", date, LogTip[logLevel],filename.c_str(),function.c_str(), line,pLogFormat);
          
        OsWrite << logTxt << std::endl;
        OsWrite.close();
    }
private:

};


#endif // !_LOG4C_HPP_



