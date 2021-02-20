#include <stdio.h>    
#include <string>    
#include <windows.h>    
#include <sql.h>   
#include <sqlext.h>    
#include <sqltypes.h>   
#include <iostream>


using namespace std;


#define MAXBUFLEN 255
SQLHENV henv = SQL_NULL_HENV;
SQLHDBC hdbc1 = SQL_NULL_HDBC;
SQLHSTMT hstmt1 = SQL_NULL_HSTMT;

/*   cpp文件功能说明：
1.数据库操作中的添加，修改，删除，主要体现在SQL语句上a
2.采用直接执行方式和参数预编译执行方式两种
*/

int main()
{



	RETCODE retcode;

	UCHAR   szDSN[15] = "mySql"; //数据源名称
	UCHAR userID[6] = "root";//数据库用户ID
	UCHAR passWORD[29] = "lee";//用户密码
	
	//1.连接数据源  
	//1.环境句柄   
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	//2.连接句柄    
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc1);
	retcode = SQLConnect(hdbc1, (SQLWCHAR*)szDSN, SQL_NTS, (SQLWCHAR*)userID, SQL_NTS, (SQLWCHAR*)passWORD, SQL_NTS);
	//判断连接是否成功   
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		printf("连接失败!\n");
	}
	else
	{
		/*
		1.分配一个语句句柄(statement handle)
		2.创建SQL语句
		3.执行语句
		4.销毁语句
		*/
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
		char sql[100] = "select * from students";
	
		//SQLBindCol
			retcode = SQLExecDirect(hstmt1, (SQLWCHAR*)sql, strlen(sql));

			if (retcode < 0)
			{
				cout << "没有执行语句" << endl;
			}
			retcode = SQLFetch(hstmt1);

			while (retcode == SQL_ROW_SUCCESS || retcode == SQL_ROW_SUCCESS_WITH_INFO)
			{
				if (retcode == SQL_ROW_SUCCESS || retcode == SQL_ROW_SUCCESS_WITH_INFO)
				{
					//printf("%s\t%s\t%s\t%s\t%s\n", sno, sname, ssex, sage, sdept);
					retcode = SQLFetch(hstmt1);
				}
			}


		printf("操作成功!");
		//释放语句句柄 
		retcode = SQLCloseCursor(hstmt1);
		retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
	}
	//3.断开数据库连接
	/*
	1. 断开数据库连接
	2.释放连接句柄.
	3.释放环境句柄(如果不再需要在这个环境中作更多连接)
	*/
	SQLDisconnect(hdbc1);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
	return(0);
}



 class Students {

 public:
	  



};