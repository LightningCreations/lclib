/*
 * ConnectionSpi.hpp
 *
 *  Created on: Oct 10, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_DATABASE_DATABASE_HPP__2018_10_10_16_02_53
#define __INCLUDE_LCLIB_CXX_DATABASE_DATABASE_HPP__2018_10_10_16_02_53
#include <string>
#include <memory>
#include <vector>
#include <lclib-cxx/database/Forwards.hpp>
namespace db{
	class Connection{
	private:
		Connection(const Connection&)=delete;
		Connection& operator=(const Connection&)=delete;
	public:
		virtual ~Connection()=default;
		virtual void open(std::string_view)=0;
		virtual void commit()=0;
		virtual void rollback()=0;
		virtual std::unique_ptr<Statement> newStatement()=0;
		virtual std::unique_ptr<PreparedStatement> newPreparedStatement()=0;
	};

	class Statement{
	private:
		Statement(const Statement&)=delete;
		Statement& operator=(const Statement&)=delete;
	public:
		virtual ~Statement()=default;
		virtual void setString(int,std::string_view)=0;
		virtual void setInt(int,int)=0;
		virtual void setFloat(int,float)=0;
		virtual void setLong(int,int64_t)=0;
		virtual void setDouble(int,double)=0;
		virtual Rowset& executeQuery(std::string_view)=0;
		virtual int executeUpdate(std::string_view)=0;
		virtual Rowset& executePreparedQuery()=0;
		virtual int executePreparedUpdate()=0;
		virtual Connection& getConnection()=0;
	};

	class PreparedStatement:public Statement{
	public:
		Rowset& executeQuery(std::string_view);
		int executePrepared();
	};

	class Row{
	protected:
		~Row();
	public:
		virtual Rowset& getRowset()=0;
		virtual int getInteger(int)=0;
		virtual int getInteger(std::string_view);
		virtual std::string getString(int)=0;
		virtual std::string getString(std::string_view);
		virtual float getFloat(int)=0;
		virtual float getFloat(std::string_view);
		virtual int64_t getLong(int)=0;
		virtual int64_t getLong(std::string_view);
		virtual double getDouble(int)=0;
		virtual double getDouble(std::string_view);
	};

	class Rowset{
	protected:
		~Rowset();
	public:
		virtual std::string getRowName(int)=0;
		virtual int getRowNumber(std::string_view)=0;
		virtual Row* begin()=0;
		virtual Row* end()=0;
	};

	class ConnectionProvider{
	private:
		ConnectionProvider(const ConnectionProvider&)=delete;
		ConnectionProvider& operator=(const ConnectionProvider&)=delete;
	protected:
		ConnectionProvider()=default;
	public:
		virtual ~ConnectionProvider()=default;
		virtual bool checkProtocol(std::string_view)const=0;
		virtual std::unique_ptr<Connection> tryOpen(std::string_view)const=0;
	};
	void registerProvider(std::unique_ptr<ConnectionProvider>&&);
	const std::vector<std::unique_ptr<ConnectionProvider>>& getProviderList();
	std::unique_ptr<Connection> open(std::string_view);
}


#endif /* __INCLUDE_LCLIB_CXX_DATABASE_DATABASE_HPP__2018_10_10_16_02_53 */
