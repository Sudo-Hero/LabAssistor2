#include "mysql_conn.h"


sql::Connection* con;
/*
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
*/
sql::Connection* startMysqlConn() {
	sql::Connection* connx;
	FileLogger logger("log.txt");
	const string server = "tcp://localhost:3306";
	const string username = "root";
	const string password = "1122";
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