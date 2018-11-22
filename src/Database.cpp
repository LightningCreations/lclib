/*
 * Database.cpp
 *
 *  Created on: Oct 10, 2018
 *      Author: connor
 */

#include <lclib/database/Database.hpp>
#include <lclib/database/Exceptions.hpp>
#include <shared_mutex>
#include <mutex>

using namespace db;

bool Connection::operator ==(const Connection& o)const{
	return &o==this;
}

bool Connection::operator !=(const Connection& o)const{
	return &o!=this;
}

bool Statement::operator ==(const Statement& s)const{
	return &s==this;
}
bool Statement::operator!=(const Statement& s)const{
	return &s==this;
}

Rowset& PreparedStatement::executeQuery(std::string_view){
	throw SQLException("Inline Query applied on PreparedStatement");
}
int PreparedStatement::executeUpdate(std::string_view){
	throw SQLException("Inline Update applied on PreparedStatement");
}

Rowset& Rowset::begin(){
	first();
	return *this;
}

Rowset& Rowset::operator ++(){
	next();
	return *this;
}
Rowset& Rowset::operator --(){
	previous();
	return *this;
}

bool Rowset::operator ==(Rowset& o){
	return (o.getStatement()==this->getStatement())&&(o.row()==this->row());
}

bool Rowset::operator!=(Rowset& o){
	return !(*this==o);
}

int Rowset::getInteger(std::string_view name){
	return getInteger(this->getCollumNumber(name));
}

float Rowset::getFloat(std::string_view name){
	return getFloat(this->getCollumNumber(name));
}

int64_t Rowset::getLong(std::string_view name){
	return getLong(this->getCollumNumber(name));
}

double Rowset::getDouble(std::string_view name){
	return getDouble(this->getCollumNumber(name));
}

std::string_view Rowset::getString(std::string_view name){
	return getString(getCollumNumber(name));
}


EndRowset::EndRowset(Rowset& r):owner(&r){}

bool EndRowset::isEnd(){
	return true;
}

bool EndRowset::first(){
	return false;
}
bool EndRowset::last(){
	return true;
}

bool EndRowset::next(){
	return false;
}

bool EndRowset::previous(){
	return false;
}

int EndRowset::row(){
	return owner->getRowCount();
}

Rowset& EndRowset::end(){
	return *this;
}

std::string_view EndRowset::getCollumName(int i){
	return owner->getCollumName(i);
}

int EndRowset::getCollumNumber(std::string_view name){
	return owner->getCollumNumber(name);
}

int EndRowset::getRowCount(){
	return owner->getRowCount();
}

int EndRowset::getInteger(int){
	throw SQLException("End of Row");
}
int64_t EndRowset::getLong(int){
	throw SQLException("End of Row");
}
std::string_view EndRowset::getString(int){
	throw SQLException("End of Row");
}
float EndRowset::getFloat(int){
	throw SQLException("End of Row");
}
double EndRowset::getDouble(int){
	throw SQLException("End of Row");
}

Connection& EndRowset::getConnection(){
	return owner->getConnection();
}
Statement& EndRowset::getStatement(){
	return owner->getStatement();
}

void DirectStatement::setInt(int,int){}
void DirectStatement::setFloat(int,float){}
void DirectStatement::setDouble(int,double){}
void DirectStatement::setString(int,std::string_view){}
Rowset& DirectStatement::executePreparedQuery(){
	throw SQLException("No such query");
}
int DirectStatement::executePreparedUpdate(){
	throw SQLException("No such update");
}

bool ConnectionProvider::operator ==(const ConnectionProvider& p)const{
	return this==&p;
}
bool ConnectionProvider::operator!=(const ConnectionProvider& p)const{
	return this!=&p;
}

std::shared_mutex providersLock;

std::vector<ConnectionProvider*> providers;

void registerProvider(ConnectionProvider& provider){
	std::unique_lock sync(providersLock);
	providers.push_back(&provider);
}

void unregisterProvider(ConnectionProvider& provider){
	for(auto& a:providers)
		if(a==&provider){
			a = nullptr;
			return;
		}
}

const std::vector<ConnectionProvider*>& getProviderList(){
	std::shared_lock sync(providersLock);
	return providers;
}

std::unique_ptr<Connection> open(std::string_view uri){
	std::shared_lock sync(providersLock);
	for(auto a:providers)
		if(a&&a->supports(uri))
			return a->open(uri);
	return {};
}
#include <lclib/database/DatabaseWrappers.hpp>
SQLStatement::SQLStatement(std::unique_ptr<Statement> stat):underlying(std::move(stat)){}
Rowset& SQLStatement::executeQuery(std::string_view q){
	return underlying->executeQuery(q);
}

int SQLStatement::executeUpdate(std::string_view q){
	return underlying->executeUpdate(q);
}

SQLPreparedStatement::SQLPreparedStatement(std::unique_ptr<PreparedStatement> stat):underlying(std::move(stat)){}

void SQLPreparedStatement::setInt(int i,int v){
	underlying->setInt(i,v);
}

void SQLPreparedStatement::setString(int i,std::string_view sv){
	underlying->setString(i,sv);
}
void SQLPreparedStatement::setLong(int i,int64_t l){
	underlying->setLong(i,l);
}

void SQLPreparedStatement::setFloat(int i,float f){
	underlying->setFloat(i,f);
}

void SQLPreparedStatement::setDouble(int i,double d){
	underlying->setDouble(i,d);
}

Rowset& SQLPreparedStatement::executePreparedQuery(){
	return underlying->executePreparedQuery();
}

int SQLPreparedStatement::executePreparedUpdate(){
	return underlying->executePreparedUpdate();
}

DBConnection::DBConnection(std::string_view uri):underlying(open(uri)){
	if(!underlying)
		throw SQLException("Failed to open Connection.");
}

void DBConnection::beginTransaction(){
	underlying->beginTransaction();
}

void DBConnection::commit(){
	underlying->commit();
}

void DBConnection::rollback(){
	underlying->rollback();
}

SQLStatement DBConnection::createStatement(){
	return SQLStatement(underlying->newStatement());
}

SQLPreparedStatement DBConnection::prepareStatement(std::string_view q){
	return SQLPreparedStatement(underlying->newPreparedStatement(q));
}



