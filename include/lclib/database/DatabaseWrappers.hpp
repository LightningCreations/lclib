/*
 * DatabaseWrappers.hpp
 *
 *  Created on: Oct 11, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_DATABASE_DATABASEWRAPPERS_HPP__2018_10_11_14_39_11
#define __INCLUDE_LCLIB_CXX_DATABASE_DATABASEWRAPPERS_HPP__2018_10_11_14_39_11

#include <lclib/database/Database.hpp>
#include <string>

namespace db{
	class DBConnection;
	class LIBLCAPI SQLStatement:public db::DirectStatement{
	private:
		friend class DBConnection;
		std::unique_ptr<db::Statement> underlying;
		SQLStatement(std::unique_ptr<db::Statement>);
	public:
		Rowset& executeQuery(std::string_view);
		int executeUpdate(std::string_view);
		db::Connection& getConnection();
	};
	class LIBLCAPI SQLPreparedStatement:public db::PreparedStatement{
	private:
		friend class DBConnection;
		std::unique_ptr<db::PreparedStatement> underlying;
		SQLPreparedStatement(std::unique_ptr<db::PreparedStatement>);
	public:
		void setString(int,std::string_view);
		void setInt(int,int);
		void setLong(int,int64_t);
		void setFloat(int,float);
		void setDouble(int,double);
		Rowset& executePreparedQuery();
		int executePreparedUpdate();
		db::Connection& getConnection();
	};
	class LIBLCAPI DBConnection{
	private:
		std::unique_ptr<db::Connection> underlying;
	public:
		DBConnection(std::string_view);
		void beginTransaction();
		void commit();
		void rollback();
		SQLStatement createStatement();
		SQLPreparedStatement prepareStatement(std::string_view);
	};
	/**
	 * Immobile RAII Wrapper that manages a transaction,
	 * committing it if the destructor is called at end of scope,
	 * or rolling it  back if an exception was thrown.
	 */
	class LIBLCAPI Transaction{
	private:
		DBConnection* conn;
		Transaction(const Transaction&)=delete;
		Transaction& operator=(const Transaction&)=delete;
	public:
		Transaction(DBConnection&);
		~Transaction();
	};

}




#endif /* __INCLUDE_LCLIB_CXX_DATABASE_DATABASEWRAPPERS_HPP__2018_10_11_14_39_11 */
