# Thread Safe Output #

Included by `<lclib/ThreadSafeIO.hpp>` <br/>
Part of the io library

## Synopsis ##

```cpp
struct async_t;
const async_t async;
struct in_progress;
struct BulkOpWrapper;
class TSOutputStream:public OutputStream;
```

## Overview ##

Provides a wrapper for output streams which allow for asynchronous, and thread safe output. There is no equivalent wrapper for InputStreams due to possible issues with asynchronous reading. <br/>


## Classes ##

### struct async_t ###
Tag type for opening TSOutputStreams initially in asynchronous mode. 
This mode may be changed by the objects setSynchronous/setAsynchronous modes.

### struct in_progress ###
Exception type thrown by transaction methods to notify callers that the transaction was dispatched asynchronously and will complete at some point, before any others are dispatched.<br/>

### struct BulkOpWrapper ###
Immobile RAII Struct which starts a bulk transaction in a given TSOutputStream& at construction, and stops the transaction at destruction.<br/>

#### Constructors, Assignment Operators, and Destructor ####

```cpp
BulkOpWrapper(TSOutputStream& out); //(1)
~BulkOpWrapper(); //(2)
BulkOpWrapper(const BulkOpWrapper&)=delete; //(3)
BulkOpWrapper& operator=(const BulkOpWrapper&)=delete; //(4)
```

(1): Starts a bulk transaction in out. The Stream is put into exclusive/bulk transaction mode<br/>
(2): Ends the bulk transaction in out. The Stream is released from exclusive/bulk transation mode<br/>
(3),(4): BulkOpWrapper cannot be copied or moved.<br/>

### class TSOutputStream ###

Thread-safe Wrapper over OutputStreams. 
The wrapped stream and this object have an ownership relationship, that is, the Wrapper "owns", rather then "wraps" the target stream. 
The behavior is undefined if the stream is interacted with, except through this class. <br/>
TSOutputStream methods are divided into 3 categories:
<dl>
<dt>Transactions</dt>
<dd>Interacts with the owned stream in a way that has externally observable behavior. 
At most one transaction may be in progress at a time. If a transaction is started while another is in progress, the new transaction will wait for the transaction to complete. 
A special subset of transactions are transaction dispatch methods. These will wait for any current transactions to complete, then dispatch the actual transaction in the current operation mode of the stream.</dd>
<dt>Transaction Observers</dt>
<dd>Views some variable that may be changed by transactions. Any number of Transaction observers may execute concurrently, but none may execute while a Transaction is in progress, nor may a transaction start while one is executing</dd>
<dt>Atomic Operations</dt>
<dd>Operations which atomically modify the internal state of the Wrapper stream. Atomic Operations can be considered transactions, except they always occur synchronously, have no result, nor throw any exceptions</dd>
</dl>

TSOutputStreams have 2 modes of operations which control how transactions are executed, being synchronous and asynchronous. <br/>
Synchronous Transactions execute in line with the current thread, as though they were executed directly by the dispatch method. <br/>
Asynchronous Transactions execute at some point after the dispatch method completes, but before any subsequent transaction starts. The dispatch method will complete abruptly by throwing an object of in_progress. <br/>
Transactions started in synchronous mode will have the same directly observable behavior as the target method in the owned stream. 
They will throw whatever that method throws, return (if applicable) the value returned by that method, and have the same side-effects as that method. 
The result of the transaction is also atomically stored in the result field. If  the transaction results in an exception, the abrupt termination flag will be set atomically before the exception is rethrown. <br/>
Transactions started in asynchronous mode will execute in parallel to the dispatching thread. 
The dispatch method will throw an object of in_progress after initiating the transaction. 
The result of the transaction will stored atomically as the transaction result field, and may be inspected by getTransactionResult(). 
If an exception is thrown, the  abrupt termination flag will be set atomically, the transaction result is atomically set to EOF, and the transaction will complete.<br/><br/>
In addition, transactions may be disjoint, in which case, dispatch methods will order them in an unspecified manner, or bulk. 
All transactions that are part of the same bulk transaction will execute in the order they are given. 
Only the thread which starts a bulk transaction may end the bulk transaction, or start any new transactions. 
Ending a bulk transaction from a thread that does not own it will have no effect. 
Transactions that are started disjoint during a bulk transaction (IE. started from a separate thread), will be scheduled in an unspecified order after the bulk transaction completes. <br/>
Bulk transactions started from the owning thread will join the same bulk transaction. Bulk transactions started from any other thread during a bulk transaction will be scheduled after the transaction completes. <br/>
Bulk transactions are transactions suitable for writing multiple bytes from individual calls to write(uint8_t), IE. writes requested by MB Write methods of DataOutputStream. <br/>
It is the responsibility of the caller to ensure that any bulk transaction started is completed. This may be achieved using the BulkOpWrapper class.<br/><br/>

#### Constructors, Assignment Operators, and Destructors ####

```cpp
TSOutputStream(OutputStream& out); //(1)
TSOutputStream(OutputStream& out,async_t); //(2)
~TSOutputStream(); //(3)
TSOutputStream(const TSOutputStream&)=delete; //(4)
TSOutputStream& operator=(const TSOutputStream&)=delete; //(5)
```

(1): Opens a new TSOutputStream owning out in synchronous mode<br/>
(2): Opens a new TSOutputStream owning out in asynchronous mode<br/>
(3): Closes the TSOutputStream when the current bulk transaction (if any) completes<br/>
(4),(5): TSOutputStream is neither copyable, nor movable.<br/>

#### Transaction Dispatch Methods ####

```cpp
std::size_t write(const void* ptr,std::size_t sz); //(1)
void write(uint8_t); //(2)
void flush(); //(3)
```

Dispatches a transaction which calls the appropriate method in the underlying stream.<br/> 
(1): The result is the number of bytes written (IE return value of OutputStream::write)<br/>
(2): The result is 1 when the transaction completes (not returned)<br/>
(3): The result is 0 when the transaction completes (not returned)<br/>

<h5>Exceptions</h5>
In synchronous mode, throws anything that the respective method of the underlying stream throws.<br/>
In Asynchronous mode, unconditionally throws in_progress.

#### Mode Changing Atomic Operations ####

```cpp
void setSynchronous(); //(1)
void setAsynchronous(); //(2)
```
Atomically changes the operation mode. The resultant mode of operation applies to all subsequent transactions. <br/>
(1): Subsequent transactions occur in synchronous mode. <br/>
(2): Subsequent transactions occur in asynchronous mode.<br/>

#### Stream Observers ####

```cpp
std::size_t getTransactionResult()const; //(1)
bool checkError()const noexcept; //(2)
bool waitFor()const; //(3)
```

(1): Atomically obtains the result of the previous transaction and returns it. If no transaction has occured, returns EOF.<br/>
(2): Checks if an error is on the stream.<br/>
(3): Waits for the active transaction to complete. Returns true if and only if the transaction completed without throwing an exception.<br/>

#### Clear Error ####

```cpp
void clearError()noexcept;
```

Atomic operation which calls clearError() on the underlying stream. <br/>

#### Exclusive/Bulk Operations ####

```cpp
void startBulk(); //(1)
void endBulk(); //(2)
```

(1): Atomically starts a bulk transaction. (Atomic Operation)<br/>
(2): Atomically ends the active bulk transaction.<br/>



