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
#include <lclib-cxx/Config.hpp>
namespace db{
	class LIBLCAPI Connection{
	private:
		Connection(const Connection&)=delete;
		Connection& operator=(const Connection&)=delete;
	public:
		Connection()=default;
		virtual ~Connection()=default;
		virtual void open(std::string_view)=0;
		virtual void beginTransaction()=0;
		virtual void commit()=0;
		virtual void rollback()=0;
		virtual std::unique_ptr<Statement> newStatement()=0;
		virtual std::unique_ptr<PreparedStatement> newPreparedStatement(std::string_view)=0;
		bool operator==(const Connection&)const;
		bool operator!=(const Connection&)const;
	};

	class LIBLCAPI Statement{
	private:
		Statement(const Statement&)=delete;
		Statement& operator=(const Statement&)=delete;
	public:
		Statement()=default;
		Statement(Statement&&)=default;
		Statement& operator=(Statement&&)=default;
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
		bool operator==(const Statement&)const;
		bool operator!=(const Statement&)const;
	};

	class LIBLCAPI PreparedStatement:public Statement{
	public:
		Rowset& executeQuery(std::string_view);
		int executeUpdate(std::string_view);
	};


	class LIBLCAPI Rowset{
	protected:
		~Rowset()=default;
		virtual bool isEnd()=0;
		virtual bool first()=0;
		virtual bool last()=0;
		virtual bool next()=0;
		virtual bool previous()=0;
		virtual int row()=0;
	public:
		Rowset()=default;
		typedef Rowset& iterator;
		typedef Rowset value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef std::ptrdiff_t difference_type;
		virtual std::string_view getCollumName(int)=0;
		virtual int getCollumNumber(std::string_view)=0;
		virtual Connection& getConnection()=0;
		virtual Statement& getStatement()=0;
		virtual int getRowCount()=0;
		Rowset& begin();
		virtual Rowset& end()=0;
		Rowset& operator*();
		Rowset& operator++();
		Rowset& operator--();
		bool operator==(Rowset& o);
		bool operator!=(Rowset& o);
		virtual int getInteger(int)=0;
		virtual int getInteger(std::string_view);
		virtual std::string_view getString(int)=0;
		virtual std::string_view getString(std::string_view);
		virtual float getFloat(int)=0;
		virtual float getFloat(std::string_view);
		virtual int64_t getLong(int)=0;
		virtual int64_t getLong(std::string_view);
		virtual double getDouble(int)=0;
		virtual double getDouble(std::string_view);
	};

	class LIBLCAPI EndRowset final:public Rowset{ // @suppress("Class has a virtual method and non-virtual destructor")
	private:
		Rowset* owner;
	protected:
		bool isEnd();
		bool first();
		bool last();
		bool next();
		bool previous();
		int row();
	public:
		EndRowset();
		EndRowset(Rowset&);
		std::string_view getCollumName(int);
		int getCollumNumber(std::string_view);
		int getInteger(int);
		int64_t getLong(int);
		std::string_view getString(int);
		float getFloat(int);
		double getDouble(int);
		int getRowCount();
		Connection& getConnection();
		Statement& getStatement();
		Rowset& end();
	};
	class LIBLCAPI DirectStatement:public Statement{
	public:
		void setString(int,std::string_view);
		void setInt(int,int);
		void setFloat(int,float);
		void setLong(int,int64_t);
		void setDouble(int,double);
		Rowset& executePreparedQuery();
		int executePreparedUpdate();
	};

	class LIBLCAPI ConnectionProvider{
	private:
		ConnectionProvider(const ConnectionProvider&)=delete;
		ConnectionProvider& operator=(const ConnectionProvider&)=delete;
	protected:
		ConnectionProvider()=default;
		~ConnectionProvider()=default;
	public:
		virtual bool supports(std::string_view)const=0;
		virtual std::unique_ptr<Connection> open(std::string_view)const=0;
		bool operator==(const ConnectionProvider&)const;
		bool operator!=(const ConnectionProvider&)const;
	};
	LIBLCAPI void registerProvider(ConnectionProvider&);
	LIBLCAPI void unregisterProvider(ConnectionProvider&);
	LIBLCAPI const std::vector<ConnectionProvider*>& getProviderList();
	LIBLCAPI std::unique_ptr<Connection> open(std::string_view);
}


#endif /* __INCLUDE_LCLIB_CXX_DATABASE_DATABASE_HPP__2018_10_10_16_02_53 */
