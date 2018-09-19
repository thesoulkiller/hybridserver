#include "DBHandler.hpp"

DBHandler *DBHandler::s_instance = NULL;

DBHandler *DBHandler::instance()
{
  if(!s_instance)
    s_instance = new DBHandler();
  return s_instance;
}

void DBHandler::init(std::string opts)
{
  c = boost::shared_ptr<pqxx::connection>(new pqxx::connection(opts));
  //w = boost::shared_ptr<pqxx::work>(new pqxx::work( *c ));

  std::cout << "DBNAME: " << std::string(c.get()->dbname()) << std::endl;
}

pqxx::result DBHandler::query(std::string sql)
{
  w = boost::shared_ptr<pqxx::work>(new pqxx::work( *c ));
  pqxx::result r = w.get()->exec(sql);
  w.get()->commit();

  return r;
}


bool DBHandler::putStatus(Status &status)
{
  pqxx::result r = query(status.insertSql());

  return true;
}
