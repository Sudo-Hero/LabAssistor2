#include "mysql_conn.h"
#include "common.h"

sql::Connection* con;
/*
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
*/
sql::Connection* startMysqlConn() {
	sql::Connection* connx;
	FileLogger logger("log.txt");
	string servertemp, porttemp,usernametemp, passwordtemp;
	readConfigMYSQL("mysql.ini", &servertemp, &porttemp, &usernametemp, &passwordtemp);
	const string server = "tcp://"+ servertemp +":" + porttemp;
	const string username = usernametemp;
	const string password = passwordtemp;
	sql::Driver* driver;
	try
	{
		driver = get_driver_instance();
		connx = driver->connect(server, username, password);
		logger.log("MYSQL Connected!!");
		return connx;
	}
	catch (sql::SQLException e)
	{
		logger.log(e.what());
		return nullptr;
	}


}