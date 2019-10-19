/**
* @file   : threadpool.h
* @brief  : �̳߳��࣬��Ҫ������̵߳Ĺ�������
* @version: 1.0
* @author : ��³��
* @create : 2017��04��
* @date   : 2017��6��3��
* @note   : ���� Apache ����֤ 2.0 �棨���¼�ơ�����֤������Ȩ;
*           �������ر����ɣ�����������ʹ������ļ���
* @remarks: �����Ի�ø����ɵĸ�����
*           http://www.apache.org/licenses/LICENSE-2.0
*           �������÷�����Ҫ��������ͬ�⣬�������ɷַ�������
*           Ҫ����ԭ�����ַ���û���κ���ʽ�ģ���ȷ�������ĵ������
*           �μ����ձ����ɿ�������Ȩ�޼����Ƶ��ض����Ե�����֤��
*
*   ����Ի�øô�������°汾��
*
*        http://git.oschina.net/Mr_ChenLuYong/oeasypool
*
*   ��Դ�����������˶��ڴ�����Ĺ�ͬά����
*
*
*   ��������Щ���뻹�в�����ĵط�����ͨ�����µ����½���ѧϰ��
*
*        ���͵�ַ��http://blog.csdn.net/csnd_ayo
*
*        ���µ�ַ��http://blog.csdn.net/csnd_ayo/article/details/72457190
*
* 	 �ڴ����ύBug����ӭIssues��
*
*/


#ifndef __OETHREADPOOL_H__
#define __OETHREADPOOL_H__




#include <atomic>
#include <memory>
#include <mutex>

#include "HLTaskQueue.h"

// 1. �̳߳صĳ�ʼ��  (init)
//  while:
//    1. ����������
//    2. ���������̳߳ص���  addTask
//    3. �̳߳��첽����
// 2. �����̳߳���Դ  (release)

namespace HL
{
	class HLTask;

	/// ���������
	class HLThreadPool
	{
	public:
		/// �̳߳����ò���
		typedef struct tagThreadPoolConfig {
			int     nMaxThreadsNum;		    /// ����߳�����
			int     nMinThreadsNum;		    /// ��С�߳�����
			double  dbTaskAddThreadRate;    /// �� ����߳������ (�����������߳�������ʲô������ʱ��ż�)
			double  dbTaskSubThreadRate;    /// �� ��С�߳������ (�����������߳�������ʲô������ʱ��ż�)
		} ThreadPoolConfig;

	private:
		/// �������
		std::shared_ptr<HLTaskQueue<HLTask> >   taskQueue_;

		/// �̳߳����ã������С�߳�����Ϊ1�����ʾ��Ҫһ����פ�Ĵ����̣߳�
		ThreadPoolConfig                        threadPoolConfig_;
		/// �̳߳��Ƿ�Ҫ�����
		std::atomic<bool>                       atcWorking_;
		/// ��ǰ�̸߳���
		std::atomic<int>                        atcCurTotalThrNum_;

	public:

		HLThreadPool(void);
		~HLThreadPool(void);

		/**
		* @brief ���̳߳���Դ���ó�ʼ��
		* @param ��config ��ʼ����������Ϣ
		* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
		*/
		int init(const ThreadPoolConfig& config);
		/**
		* @brief ���ͷ���Դ���ͷ��̳߳ء��ͷ�������У�
		* @return��true ִ�гɹ�  false ִ��ʧ��
		*/
		bool release(void);

		/**
		* @brief ����������
		* @param ��taskptr ������
		* @param ��priority �Ƿ����޴��� true�����ȴ���
		* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
		*/
		int addTask(std::shared_ptr<HLTask> taskptr, bool priority = false);

		/**
		* @brief ��ɾ�����񣨴Ӿ�������ɾ���������������û�У���ִ�ж�����û�У��еĻ�����ȡ��״̬λ��
		* @param ��nID ������
		* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
		*/
		int deleteTask(size_t nID);

		/**
		* @brief ��ɾ����������
		* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
		*/
		int deleteAllTasks(void);
		/**
		* @brief ���ж������Ƿ�ִ�����
		* @param ��nID ������
		* @return��ִ����ϣ�ִ���귵��null�����򷵻�����ָ��
		*/
		std::shared_ptr<HLTask> isTaskProcessed(size_t nId);

	private:
		/**
		* @brief ����ȡ��ǰ�߳������
		* @return���߳������
		*/
		double getThreadTaskRate(void);
		/**
		* @brief ����ǰ�߳��Ƿ���Ҫ����
		* @return��true:���Խ��� false:�����Խ���
		* @note  ���ѿ��ǵ���С�߳�����
		*/
		bool shouldEnd(void);
		/**
		* @brief ������ָ�������Ĵ����߳�
		* @param ��nThreadsNum ���ӵ��߳�����
		* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
		*/
		int addProThreads(int nThreadsNum);
		/**
		* @brief ���ͷ��̳߳�
		* @return��true ִ�гɹ�  false ִ��ʧ��
		*/
		bool releaseThreadPool(void);
		/**
		* @brief ���������̺߳���
		*/
		void taskProcThread(void);

	};
}

// extern HLThreadPool SystemThreadPool;

#endif // __OETHREADPOOL_H__