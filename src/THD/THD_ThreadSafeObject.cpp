/*
 * THD_ThreadSafeObject.cpp
 *
 *  Created on: Mar 23, 2020
 *      Author: green
 */

#include "THD_ThreadSafeObject.hpp"

using namespace std;

namespace THD
{

	// Specialization - compile specific types for cmnlib
	template class CThreadSafeObject<int> ;
	template class CThreadSafeObject<string> ;
	template class CThreadSafeObject<cv::Mat> ;

	template<typename T>
	CThreadSafeObject<T>::CReader::CReader(CThreadSafeObject<T> *p_parent)
	{
		m_parent = p_parent;
		// Take a read token
		m_parent->lockReadAccess();
		ptr = m_parent->getPtr();
	}

	template<typename T>
	CThreadSafeObject<T>::CReader::~CReader()
	{
		// Release a read token
		m_parent->unlockReadAccess();
	}

	template<typename T>
	CThreadSafeObject<T>::CWriter::CWriter(CThreadSafeObject<T> *p_parent)
	{
		m_parent = p_parent;
		// Take a write token
		m_parent->lockWriteAccess();
		ptr = m_parent->getPtr();
	}

	template<typename T>
	CThreadSafeObject<T>::CWriter::~CWriter()
	{
		// Release a write token
		m_parent->unlockWriteAccess();
	}

	template<typename T>
	CThreadSafeObject<T>::CThreadSafeObject()
	{
		m_readCounter = 0;

	}

	template<typename T>
	CThreadSafeObject<T>::CThreadSafeObject(T &p_sharedObject)
	{
		m_sharedObject = p_sharedObject;
		m_readCounter = 0;
	}

	template<typename T>
	CThreadSafeObject<T>::~CThreadSafeObject()
	{
	}

	template<typename T>
	T* CThreadSafeObject<T>::getPtr()
	{
		return &m_sharedObject;
	}

	template<typename T>
	void CThreadSafeObject<T>::lockReadAccess()
	{
		// Take access mutex
		unique_lock<mutex> lckAcc(m_accessMutex);
		// Take counter mutex
		unique_lock<mutex> lckCnt(m_counterMutex);
		// If a write token has been taken (m_readCounter < 0), wait until it is released
		// Counter mutex is released during wait
		// Keeping access mutex avoid new incoming access and give priority
		while (m_readCounter < 0)
			m_counterCV.wait(lckCnt);
		++m_readCounter;
	}

	template<typename T>
	bool CThreadSafeObject<T>::tryLockReadAccess()
	{
		unique_lock<mutex> lckAcc(m_accessMutex, try_to_lock);
		if (!lckAcc.owns_lock()) return false;
		unique_lock<mutex> lckCnt(m_counterMutex, try_to_lock);
		if (!lckCnt.owns_lock()) return false;
		if (m_readCounter < 0) return false;
		// Take a read token only if available
		++m_readCounter;
		return true;
	}

	template<typename T>
	void CThreadSafeObject<T>::unlockReadAccess()
	{
		// Don't take access mutex on unlock. We already have a token/the access
		unique_lock<mutex> lckCnt(m_counterMutex);
		--m_readCounter;
		m_counterCV.notify_all();
	}

	template<typename T>
	void CThreadSafeObject<T>::lockWriteAccess()
	{
		// Take access mutex
		unique_lock<mutex> lckAcc(m_accessMutex);
		// Take counter mutex
		unique_lock<mutex> lckCnt(m_counterMutex);
		// If read token has been taken (m_readCounter > 0), wait until they are released
		// Counter mutex is released during wait
		// Keeping access mutex avoid new incoming access and give priority
		while (m_readCounter != 0)
			m_counterCV.wait(lckCnt);
		m_readCounter = -1;
	}

	template<typename T>
	bool CThreadSafeObject<T>::tryLockWriteAccess()
	{
		unique_lock<mutex> lckAcc(m_accessMutex, try_to_lock);
		if (!lckAcc.owns_lock()) return false;
		unique_lock<mutex> lckCnt(m_counterMutex, try_to_lock);
		if (!lckCnt.owns_lock()) return false;
		if (m_readCounter != 0) return false;
		// Take a write token only if available
		m_readCounter = -1;
		return true;
	}

	template<typename T>
	void CThreadSafeObject<T>::unlockWriteAccess()
	{
		// Don't take access mutex on unlock. We already have a token/the access
		unique_lock<mutex> lckCnt(m_counterMutex);
		m_readCounter = 0;
		m_counterCV.notify_all();
	}

} /* namespace THD */
