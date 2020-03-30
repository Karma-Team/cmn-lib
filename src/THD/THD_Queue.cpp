/*
 * THD_Queue.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: green
 */

#include <THD/THD_Queue.hpp>

namespace THD
{

	template<typename T>
	T CQueue<T>::pop()
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		while (m_queue.empty())
		{
			m_cond.wait(mlock);
		}
		auto item = m_queue.front();
		m_queue.pop();
		return item;
	}

	template<typename T>
	void CQueue<T>::pop(T &item)
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		while (m_queue.empty())
		{
			m_cond.wait(mlock);
		}
		item = m_queue.front();
		m_queue.pop();
	}

	template<typename T>
	void CQueue<T>::push(const T &item)
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		m_queue.push(item);
		mlock.unlock();
		m_cond.notify_one();
	}

	template<typename T>
	void CQueue<T>::push(T &&item)
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		m_queue.push(std::move(item));
		mlock.unlock();
		m_cond.notify_one();
	}

} /* namespace THD */
