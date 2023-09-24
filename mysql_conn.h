#ifndef _MYSQL_CONN_
#define _MYSQL_CONN_

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include "Logger.h"


using namespace std;


//for demonstration only. never save your password in the code!
sql::Connection*  startMysqlConn();
#endif // _MYSQL_CONN_
