/**
* @author : 陈鲁勇
* @date   : 2017年04月
* @version: 1.0
* @note   : 根据 Apache 许可证 2.0 版（以下简称“许可证”）授权;
*           除非遵守本许可，否则您不能使用这个文件。
* @remarks: 您可以获得该许可的副本：
*           http://www.apache.org/licenses/LICENSE-2.0
*           除非适用法律需要或者书面同意，按本许可分发的软件
*           要按“原样”分发，没有任何形式的，明确或隐含的担保条款。
*           参见按照本许可控制许可权限及限制的特定语言的许可证。
*
*   你可以获得该代码的最新版本：
*
*        http://git.oschina.net/Mr_ChenLuYong/oeasypool
*
*   开源社区的所有人都期待与你的共同维护。
*
*
*   如果你对这些代码还有不理解的地方可以通过最新的文章进行学习：
*
*        博客地址：http://blog.csdn.net/csnd_ayo
*
*        文章地址：http://blog.csdn.net/csnd_ayo/article/details/72457190
*
* 	 期待你提交Bug，欢迎Issues。
*
*/


#include "HLThreadPool.h"
#include <iostream>
#include <thread>

#include "HLTask.h"
#include "HLTaskQueue.h"

namespace HL
{
	//HLThreadPool SystemThreadPool;

	HLThreadPool::HLThreadPool(void)
		:taskQueue_(new HLTaskQueue<HLTask>()), atcCurTotalThrNum_(0), atcWorking_(true) {
	}

	HLThreadPool::~HLThreadPool(void) {
		// @note: 曾经因析构自动调用 release 触发了错误
		release();
	}

	// 初始化资源
	int HLThreadPool::init(const ThreadPoolConfig& threadPoolConfig) {
		// 错误的设置
		if (threadPoolConfig.dbTaskAddThreadRate < threadPoolConfig.dbTaskSubThreadRate)
			return 87;

		threadPoolConfig_.nMaxThreadsNum = threadPoolConfig.nMaxThreadsNum;
		threadPoolConfig_.nMinThreadsNum = threadPoolConfig.nMinThreadsNum;
		threadPoolConfig_.dbTaskAddThreadRate = threadPoolConfig.dbTaskAddThreadRate;
		threadPoolConfig_.dbTaskSubThreadRate = threadPoolConfig.dbTaskSubThreadRate;

		int ret = 0;
		// 创建线程池
		if (threadPoolConfig_.nMinThreadsNum > 0)
			ret = addProThreads(threadPoolConfig_.nMinThreadsNum);

		return ret;
	}

	// 添加任务
	int HLThreadPool::addTask(std::shared_ptr<HLTask> taskptr, bool priority) {
		const double& rate = getThreadTaskRate();
		int ret = 0;
		if (priority)
		{
			if (rate > 1000)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));

			taskQueue_->put_front(taskptr);
		}
		else
		{
			// 检测任务数量
			if (rate > 100)
			{
				taskptr->onCanceled();
				return 298;
			}

			// 将任务推入队列
			taskQueue_->put_back(taskptr);
		}

		// 检查是否要扩展线程
		if (atcCurTotalThrNum_ < threadPoolConfig_.nMaxThreadsNum
			&& rate > threadPoolConfig_.dbTaskAddThreadRate)
			ret = addProThreads(1);

		return ret;
	}

	// 删除任务（从就绪队列删除，如果就绪队列没有，则看执行队列有没有，有的话置下取消状态位）
	int HLThreadPool::deleteTask(size_t nID)
	{
		return taskQueue_->deleteTask(nID);
	}

	// 删除所有任务
	int HLThreadPool::deleteAllTasks(void)
	{
		return taskQueue_->deleteAllTasks();
	}

	std::shared_ptr<HLTask> HLThreadPool::isTaskProcessed(size_t nId)
	{
		return taskQueue_->isTaskProcessed(nId);
	}

	// 释放资源（释放线程池、释放任务队列）
	bool HLThreadPool::release(void)
	{
		// 1、停止线程池。2、清楚就绪队列。3、等待执行队列为0
		releaseThreadPool();
		taskQueue_->release();

		int i = 0;
		while (atcCurTotalThrNum_ != 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			// 异常等待
			if (i++ == 10)
				exit(23);
		}

		atcCurTotalThrNum_ = 0;
		return true;
	}

	// 获取当前线程任务比
	double HLThreadPool::getThreadTaskRate(void)
	{
		// @note :无所谓线程安全
		//std::unique_lock<std::mutex> lock(mutex_)

		if (atcCurTotalThrNum_ != 0)
			return taskQueue_->size() * 1.0 / atcCurTotalThrNum_;

		return 0;
	}

	// 当前线程是否需要结束
	bool HLThreadPool::shouldEnd(void)
	{
		bool bFlag = false;
		double dbThreadTaskRate = getThreadTaskRate();

		// 检查线程与任务比率
		if (!atcWorking_ || atcCurTotalThrNum_ > threadPoolConfig_.nMinThreadsNum
			&& dbThreadTaskRate < threadPoolConfig_.dbTaskSubThreadRate)
			bFlag = true;

		return bFlag;
	}

	// 添加指定数量的处理线程
	int HLThreadPool::addProThreads(int nThreadsNum)
	{
		try
		{
			for (; nThreadsNum > 0; --nThreadsNum)
				std::thread(&HLThreadPool::taskProcThread, this).detach();
		}
		catch (...)
		{
			return 155;
		}

		return 0;
	}

	// 释放线程池
	bool HLThreadPool::releaseThreadPool(void)
	{
		threadPoolConfig_.nMinThreadsNum = 0;
		threadPoolConfig_.dbTaskSubThreadRate = 0;
		atcWorking_ = false;
		return true;
	}

	// 任务处理线程函数
	void HLThreadPool::taskProcThread(void)
	{
		int nTaskProcRet = 0;
		// 线程增加
		atcCurTotalThrNum_.fetch_add(1);
		std::chrono::milliseconds mills_sleep(500);

		std::shared_ptr<HLTask> pTask;
		while (atcWorking_)
		{
			// 获取任务
			pTask = taskQueue_->get();
			if (pTask == nullptr)
			{
				if (shouldEnd())
					break;

				// 进入睡眠池
				taskQueue_->wait(mills_sleep);
				continue;
			}

			// 检测任务取消状态
			if (pTask->isCancelRequired())
				pTask->onCanceled();
			else
				// 处理任务
				pTask->onCompleted(pTask->doWork());

			// 从运行任务队列中移除任务
			taskQueue_->onTaskFinished(pTask->getID());

			// 判断线程是否需要结束
			if (shouldEnd())
				break;
		}

		// 线程个数减一
		atcCurTotalThrNum_.fetch_sub(1);
	}
}