/*
 * THD_Queue.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: green
 *      From https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
 */

#ifndef INC_THD_QUEUE_HPP_
#define INC_THD_QUEUE_HPP_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace THD
{

	template<typename T>
	class CQueue
	{
		public:

			T pop();

			void pop(T &item);

			void push(const T &item);

			void push(T &&item);

		private:
			std::queue<T> m_queue;
			std::mutex m_mutex;
			std::condition_variable m_cond;
	};

} /* namespace THD */

#endif /* INC_THD_QUEUE_HPP_ */
