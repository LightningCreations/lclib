/*
 * ThreadSafeIO.hpp
 *
 *  Created on: Oct 5, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_THREADSAFEIO_HPP__2018_10_05_09_02_57
#define __INCLUDE_LCLIB_CXX_THREADSAFEIO_HPP__2018_10_05_09_02_57

#include <lclib-cxx/IOWrapper.hpp>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <type_traits>
#include <functional>
#include <chrono>

using namespace std::chrono;

/**
 * Tag Type for opening a TSOutputStream in asynchronous mode.
 */
struct async_t{
public:
	constexpr explicit async_t()=default;
};
const async_t async{};

/**
 * Exception type thrown to indicate that a transaction is in progress.
 */
struct in_progress{
public:
	explicit constexpr in_progress()=default;
};


/**
 * A Threadsafe Wrapper on an OutputStream.
 * All operations are locked, and all interactions are atomic.
 * TSOutputStream, unlike FilterOutputStreams are intended to take full ownership
 * of the underlying OutputStream.
 * The behavior is undefined if the OutputStream is interacted with,
 * except through this object.
 * The ThreadSafety is guarenteed using transactions.
 * A transaction is either asynchronous, in which case it executes in parallel with the requesting thread,
 *  or is synchronous, in which case it executes in the requesting thread.
 * All methods defined by TSOutputStream are transactions, transaction observers,
 *  or atomic operations.
 * At most 1 transaction may be in progress for a given thread.
 *  Starting a new transaction while another is in progress will block the new transaction.
 *  In addition, starting a new transaction while a transaction observer is in progress,
 *   will block until the observer completes.
 * Any number of transaction observers may be executing at once,
 *  however, none may be executing while a transaction is in progress.
 *  regardless of in progress transactions or observers.
 *  However, calling one or more atomic operations in parallel may have unpredictable results.
 */
class LIBLCAPI TSOutputStream:OutputStream{
private:
	OutputStream* owned;
	mutable std::mutex lock;
	std::atomic<bool> isBulkTransaction;
	std::atomic<std::thread::id> bulkTransactionOwner;
	std::atomic<bool> isInDestruction;
	std::atomic<bool> err;
	std::atomic<bool> async;
	std::atomic<bool> transactionThrew;
	std::atomic<std::size_t> asyncWriteSz;
	std::condition_variable waitForCompletion;
	std::condition_variable waitForBulkCompletion;
	template<typename F,typename... Args,typename=std::enable_if_t<std::is_invocable_v<F,TSOutputStream*,Args...>>>
			std::size_t doTransaction(F&& f,Args&&... args){
			std::unique_lock<std::mutex> sync(lock);
			if(isBulkTransaction&&bulkTransactionOwner.load()!=&std::this_thread::get_id())
				waitForBulkCompletion.wait(sync);
			if(async){
				std::thread t([this,f,args...](){
					std::unique_lock sync(lock);
					try{
						std::invoke(f,this,std::forward(args)...);
						err = owned->checkError();
						transactionThrew = false;
						waitForCompletion.notify_all();
					}catch(...){
						err = true;
						asyncWriteSz = EOF;
						transactionThrew = true;
						waitForCompletion.notify_all();
						return;
					}
				});
				t.detach();
				throw in_progress{};
			}else{
				try{
					std::invoke(f,*this);
					err = owned->checkError();
					transactionThrew = false;
					waitForCompletion.notify_all();
				}catch(...){
					err = true;
					asyncWriteSz = EOF;
					transactionThrew = true;
					waitForCompletion.notify_all();
					throw;
				}
				return asyncWriteSz;
			}
		}
	template<typename F,typename... Args,typename=std::enable_if_t<std::is_invocable_v<F,TSOutputStream*,Args...>>>
		auto doTransactionObserve(F&& f,Args&&... args)const
		->std::invoke_result_t<F,TSOutputStream*,Args...>{
			std::shared_lock<std::mutex> sync(lock);
			return std::invoke(f,this,std::forward(args)...);
		}
	template<typename F,typename... Args,typename=std::enable_if_t<std::is_invocable_v<F,TSOutputStream*,Args...>>>
		void doAtomic(F&& f,Args&&... args){
		std::unique_lock<std::mutex> sync(lock);
		if(isBulkTransaction&&bulkTransactionOwner.load()!=std::this_thread::get_id())
			waitForBulkCompletion.wait(sync);
		std::invoke(f,this,std::forward(args)...);
	}
	TSOutputStream& operator=(const TSOutputStream&)=delete;
	TSOutputStream(const TSOutputStream&)=delete;
public:
	/**
	 * Constructs a new TSOutputStream in synchronous write mode, owning out.
	 */
	TSOutputStream(OutputStream& out);
	/**
	 * Constructs a new TSOutputStream in asynchronous write mode, owning out.
	 */
	TSOutputStream(OutputStream& out,async_t);
	/**
	 * Destroys this Stream. (This is a transaction).
	 * Any transactions that are started during this destructor
	 */
	~TSOutputStream();
	/**
	 * Sets the write mode to synchronous. (Atomic Operation)
	 * Subsequent write calls will be made in the calling thread,
	 * and will always return a value to the caller.
	 * It is unspecified if transactions started concurrently to this method call
	 *  will use the previous mode of operation or the new.
	 *  However, the behavior is well defined in either case.
	 * In addition, if this call is made while a setAsynchronous method call is in progress,
	 *  it is unspecified the mode that the stream is placed in.
	 */
	void setSynchronous();
	/**
	 * Sets the write mode to asynchronous. (Atomic Operation)
	 * Subsequent write calls will be made in a seperate thread and write
	 * will return inProgress;
	 * It is unspecified if transactions started concurrently to this method call
	 *  will use the previous mode of operation or the new.
	 *  However, the behavior is well defined in either case.
	 * In addition, if this call is made while a setSynchronous method call is in progress,
	 *  it is unspecified the mode that the stream is placed in.
	 */
	void setAsynchronous();
	/**
	 * Writes sz bytes from ptr to the stream.
	 * If the stream is being destroyed, returns EOF.
	 * A new Transaction is started, blocking until all other complete.
	 * Then if the stream is in synchronous mode:
	 * 	The write method of the underlying stream is called with the arguements,
	 * 	 and the write size tracker is atomically updated to the result, then that same result is returned.
	 * 	If after the write method completes, the underlying stream signals an error condition,
	 * 	 then the error flag is set atomically.
	 * 	If an exception is thrown in synchronous mode, the error flag is set atomically, and the exception is rethrown
	 * 	 no changes are made to the write size tracker
	 * If the stream is in asynchronous mode, the following is done in a separate thread:
	 *  A new transaction is started, blocking until all others complete (including the synchronous write).
	 *  The write method of the underlying stream is called,
	 *   and the write size tracker is atomically updated to the result.
	 *  If after the write method completes, the underlying stream signals an error condition,
	 *   the error flag is atomically set.
	 *  If an exception is thrown, the error flag is atomically set, the size tracker is atomically set to EOF,
	 *   then the transaction completes.
	 *  In the calling thread:
	 *   The asynchronous transaction is dispatched in a separate thread.
	 *   The write transaction completes,
	 *   and inProgress is returned to indicate that the write is in progress.
	 */
	std::size_t write(const void* ptr,std::size_t sz);
	/**
	 * Starts a new transaction to write a single byte to the stream.
	 * The behavior in both modes of operation is specified by write,
	 *  except that the operation completing, atomically sets the write tracker to 1.
	 */
	void write(uint8_t);
	/**
	 * Returns the result of the previous transaction.
	 *  If the stream is being closed, returns EOF.
	 *  If the transaction was a flush operation that completed, returns 0.
	 *  If the transaction was a single byte write operation that completed, returns 1.
	 *  If the previous transaction was asynchronous, and it completed abruptly, returns EOF.
	 */
	std::size_t getTransactionResult()const noexcept(true);
	/**
	 * Atomically queries the error flag. This is a transaction observer.
	 * \Ensures: Given underlying is the owned stream checkError()==underlying->checkError().
	 *   However, the reserve is not true,
	 *   as underlying->checkError() may complete while a transaction is in progress
	 */
	bool checkError()const noexcept(true);
	/**
	 * Atomically clears the error flag and calls the clearError() method of the underlying stream.
	 * This is a transaction observer.
	 */
	void clearError()noexcept(true);
	/**
	 * Starts a new transaction to flush the stream.
	 * The behavior in both modes of operation is specified by write,
	 *  except that the size tracker is set to zero when the transaction completes.
	 */
	void flush();

	/**
	 * Waits for all transactions to complete.
	 * This method is a transaction observer.
	 * returns true if the transaction completed abruptly, that is, an exception was thrown.
	 */
	bool waitFor()const;

	/**
	 * Starts a bulk transaction, IE a transaction which consists of multiple transactions.
	 * Atomic Operation
	 */
	void startBulk();
	/**
	 * Ends a bulk transaction.
	 * This is technically an atomic operation,
	 *  but will block until all transactions complete.
	 *  Will have no effect if the Stream is not in bulk mode.
	 */
	void endBulk();
};

struct BulkOpWrapper{
private:
	TSOutputStream* target;
	BulkOpWrapper(const BulkOpWrapper&)=delete;
	BulkOpWrapper& operator=(const BulkOpWrapper&)=delete;
public:
	BulkOpWrapper(TSOutputStream&);
	~BulkOpWrapper();
};



#endif /* __INCLUDE_LCLIB_CXX_THREADSAFEIO_HPP__2018_10_05_09_02_57 */
