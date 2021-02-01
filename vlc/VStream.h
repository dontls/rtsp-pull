#pragma once
#include "videostream_abstract.h"
#include "common.h"
#include <string.h>

struct TaskMat {
	size_t read_pos = 0;
	size_t write_pos = 0;
	cv::Mat frame[MAX_TASK_QUEUE_CNT];
};

struct FrameQueue {
	int Length;
	BYTE TaskSet[MAX_TASK_NUM];
	struct TaskMat TaskQueue[MAX_TASK_NUM];

	int Init(int width, int height)
	{
		Length = width * height * 3;
		for(int i = 0; i < MAX_TASK_NUM; i++) 
		{	
			for(int j = 0; j < MAX_TASK_QUEUE_CNT; j++) 
			{
				if(TaskQueue[i].frame[j].empty()) 
				{
					TaskQueue[i].frame[j].create(height, width, CV_8UC3);
					gdbprintf("frame buffer task %d , buffer %d malloc success.\n", i, j);
				}
			}
		}
	}

	void Push(BYTE* src)
	{
		for(int i = 0; i < MAX_TASK_NUM; i++)
		{	
			BYTE task = TaskSet[i];
			if(task == '1') 
			{
				int curr_write_pos = TaskQueue[i].write_pos;
				int next_pos = curr_write_pos + 1;
				if(next_pos >= MAX_TASK_QUEUE_CNT) 
				{
					next_pos = 0;
				}
				if(next_pos == TaskQueue[i].read_pos)
				{
					//gdbprintf(":---> frame queue is full\n");
					return;
				}
				gdbprintf("push task； %2d, w: %2d, r : %2d, %d\n", i, curr_write_pos, TaskQueue[i].read_pos,  MAX_TASK_QUEUE_CNT);
				memcpy(TaskQueue[i].frame[curr_write_pos].data, src, Length);
				TaskQueue[i].write_pos = next_pos;
			}
		}
	}

	cv::Mat Pop(int task_id) 
	{
		cv::Mat oframe;
		if(TaskQueue[task_id].read_pos == TaskQueue[task_id].write_pos)
		{
			//gdbprintf(":---> frame queue is empty\n");
			return oframe;
		}
		int curr_read_pos = TaskQueue[task_id].read_pos;
		gdbprintf("pop  task； %2d, w: %2d, r : %2d, %d\n", task_id, TaskQueue[task_id].write_pos, curr_read_pos,  MAX_TASK_QUEUE_CNT);
		oframe = TaskQueue[task_id].frame[curr_read_pos];
		int next_pos = curr_read_pos + 1;
		TaskQueue[task_id].read_pos  = next_pos >= MAX_TASK_QUEUE_CNT ? 0 : next_pos; 
		return oframe;
	}
};

class VVideoStream : public VAbstractVideoStream
{
public:
	VVideoStream();
	~VVideoStream();
	int GetStreamUri(const char *ip, const char* username, const char* password, int channel);
	int Open();
	int ReOpen();
	int Close();
	int State();

	void SetTask(BYTE* task_id);
    struct VideoStream m_pVideoStream;
	
	cv::Mat GetFrameData(int task_id);
private:
	void* lockCallback(void **planes);
	void  unlockCallback(void *picture, void *const *planes);
	void  displayCallback(void *picture);
	
private:
	
	VMediaPlayer *m_pMediaPlayer;
	int m_nFrameLength;
	BYTE* m_uFrameData;
	struct FrameQueue m_pFrameQueue; 
};

