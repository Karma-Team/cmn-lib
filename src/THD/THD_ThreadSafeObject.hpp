/*
 * THD_ThreadSafeObject.h
 *
 *  Created on: Mar 23, 2020
 *      Author: green
 *  Main example:
#include <string>
#include <thread>
#include <mutex>
using namespace std;

mutex coutMtx; // cout mutex

void testReader(string ident, THD::CThreadSafeObject<string> * object)
{
	for(;;)
	{
		usleep(10);
		// Take in this loop step scope a reader
		THD::CThreadSafeObject<string>::CReader reader(object);
		usleep(10);
		unique_lock<mutex> lckAcc(coutMtx);
		// Print the shared string
		cout << ident << *reader.ptr << endl;
	}
}

void testWriter(string newMesg, THD::CThreadSafeObject<string> * object)
{
	// Take in the function scoope a writer
	THD::CThreadSafeObject<string>::CWriter writer(object);
	*writer.ptr += newMesg;
	unique_lock<mutex> lckAcc(coutMtx);
	cout << "Update: " << *writer.ptr << endl;
}

int main() {
	int i;
	string test = "ABCDEF";

	unique_lock<mutex> lckAcc(coutMtx);
	cout << "Test safeThread object" << endl;
	lckAcc.unlock();

	// Create shared object
	THD::CThreadSafeObject<string> obj(test);

	// Create Readers
	for (i = 0; i < 10; ++i)
		thread(testReader, to_string(i), &obj).detach();
	sleep(1);

	// Create a writer
	lckAcc.lock();
	cout << "Create a writer" << endl;
	lckAcc.unlock();
	thread writer (testWriter, "miaou", &obj);
	writer.join();

	// Create more reader
	for (; i < 20; ++i)
		thread(testReader,  to_string(i), &obj).detach();
	sleep(1);

	lckAcc.lock();
	cout << "end test safeThread object" << endl;
	lckAcc.unlock();

	return 0;
}
 *
 */

#ifndef INC_THD_THREADSAFEOBJECT_H_
#define INC_THD_THREADSAFEOBJECT_H_

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <mutex>
#include <condition_variable>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

namespace THD
{
	/**
	 @class CThreadSafeObject
	 @brief Shared object with thread safe accessor
	 Provide two subclasses: CReader and CWriter, for read and write operation.
	 Simultaneous read are possible, but only one write at a time.
	 The CReader and CWriter must be used in a limited scope.
	 **/
	template<typename T>
	class CThreadSafeObject
	{
		public:

			/**
			 * @class CReader
			 * @brief Give access to a pointer to the shared object.
			 * Request a read access on creation, and release it on destruction
			 */
			class CReader
			{
				public:
					/**
					 * @brief constructor - take a read token
					 */
					CReader(CThreadSafeObject<T> *p_parent);
					/**
					 * @brief destructor - release a read token
					 */
					~CReader();
					/**
					 * @brief read pointer to the object
					 */
					const T *ptr;
				private:
					CThreadSafeObject<T> *m_parent; // pointer to the parent class
			};

			/**
			 * @class CWriter
			 * @brief Give access to a pointer to the shared object.
			 * Request a write access on creation, and release it on destruction
			 */
			class CWriter
			{
				public:
					/**
					 * @brief constructor - take a write token
					 */
					CWriter(CThreadSafeObject<T> *p_parent);
					/**
					 * @brief destructor - release a write token
					 */
					~CWriter();
					/**
					 * @brief write pointer to the object
					 */
					T *ptr;
				private:
					CThreadSafeObject<T> *m_parent; // pointer to the parent class
			};

			/**
			 @brief constructor
			 */
			CThreadSafeObject();

			/**
			 * @brief constructor
			 * @param reference of the shared object
			 */
			CThreadSafeObject(T &p_sharedObject);

			/**
			 * @brief destructor
			 */
			~CThreadSafeObject();

			/**
			 * @brief get pointer to the object - should not be used.
			 */
			T* getPtr();

			/**
			 * @brief get a slot for a read access
			 */
			void lockReadAccess();

			/**
			 *  @brief try to get a slot for a read access
			 * @return true if lock successful, false otherwise
			 */
			bool tryLockReadAccess();

			/**
			 * @brief free a read slot
			 */
			void unlockReadAccess();

			/**
			 * @brief get a slot for a write access
			 */
			void lockWriteAccess();

			/**
			 * @brief try to get a slot for a write access
			 * @return true if lock successful, false otherwise
			 */
			bool tryLockWriteAccess();

			/**
			 * @brief free a write slot
			 */
			void unlockWriteAccess();

		private:
			T m_sharedObject;						// Shared object
			std::mutex m_accessMutex;				// Lock access to the mutex to limit new access to the object
			std::mutex m_counterMutex;				// Lock access to the counter
			std::condition_variable m_counterCV;	// Notify for updates on the counter
			int32_t m_readCounter;					// Count number of reader, or writer (-1)
	};

} /* namespace THD */

#endif /* INC_THD_THREADSAFEOBJECT_H_ */
