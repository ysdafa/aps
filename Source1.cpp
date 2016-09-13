#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define CLIP0(a) (((a) >= 0)?(a):(0))
#define MIN(a,b) (((a)<(b))?(a):(b))
typedef struct TASK
{
	int start;
	int process;
}TASK;

typedef struct STATE
{
	int index;//当前状态的任务编号
	int minUse;//最小cpu使用情况
	int cpu[5];//1-5号cpu在下一个任务来的时候，需要等多少秒
}STATE;

int visit[11][11][11][11][11] = { 0 };//第i个任务是否有当前这种状态，如果有,值为i，否则小于i
int minUse[11][11][11][11][11] = { 0 };//最小cpu使用情况
#define QUEUE_MAX 200000
STATE queue[QUEUE_MAX] = { 0 };

void qsort(int a[], int begin, int end)
{
	if (begin >= end)return;
	int l = begin, r = end, k = a[l];
	while (l < r)
	{
		while ((l < r) && k <= a[r])r--;
		a[l] = a[r];
		while ((l < r) && k >= a[l])l++;
		a[r] = a[l];
	}
	a[l] = k;
	qsort(a, begin, l - 1);
	qsort(a, l + 1, end);
}

int test(int MAX, TASK task[])
{
	//数据生成结束
	//将第一个任务放入第一个cpu并加入队列
	int front = 0;
	int rear = 0;
	queue[rear].index = 1;
	queue[rear].minUse = 1;
	queue[rear].cpu[0] = task[1].start + task[1].process - task[2].start;
	queue[rear].cpu[0] = CLIP0(queue[rear].cpu[0]);
	visit[queue[rear].cpu[0]][0][0][0][0] = 1;
	rear++;
	while (front != rear)
	{
		STATE last = queue[front++];//通过队列中的状态
		if (front == QUEUE_MAX)front = 0;//队列循环使用，但每个任务的状态不可能超过20万个
		if (last.index >= MAX)break;//等于最后一个任务即结束
		int minVale = 20;
		for (int i = 0; i < last.minUse; i++)//如果上个任务有一个cpu消耗为0，则没有必要开一个新的cpu
		{
			minVale = MIN(minVale, last.cpu[i]);
		}

		int N = 0;
		if (minVale == 0)N = last.minUse;
		else N = last.minUse + 1;//当前任务可放在上个任务使用的cpu+1的位置上,但不能超过5个
		if (N > 5)N = 5;
		for (int i = 0; i < N; i++)//把下一个任务放在第i个位置上，更新需要执行的时间
		{
			STATE cur = last;
			cur.index += 1;
			cur.cpu[i] += task[cur.index].process;//计算把当前任务放在第i个cpu时，需要执行的时间
			if (cur.cpu[i] > 10)continue;
			if (i + 1 <= last.minUse)cur.minUse = last.minUse;//如果当前任务使用的cpu编号小于上个任务的cpu最小使用个数，则最小使用个数不变
			else cur.minUse = i + 1;//否则cpu最小使用个数则为当前的编号（+1）
			for (int j = 0; j < N; j++)//计算当前的状态到下个任务的时间 范围为0-10
			{
				cur.cpu[j] = task[cur.index].start + cur.cpu[j] - task[cur.index + 1].start;
				cur.cpu[j] = CLIP0(cur.cpu[j]);
			}

			qsort(cur.cpu, 0, cur.minUse - 1);//排序以将不同的状态改为相同的状态
			if (visit[cur.cpu[0]][cur.cpu[1]][cur.cpu[2]][cur.cpu[3]][cur.cpu[4]] == cur.index)//如果当前这种状态已经被访问过
			{
				if (minUse[cur.cpu[0]][cur.cpu[1]][cur.cpu[2]][cur.cpu[3]][cur.cpu[4]] > cur.minUse)
				{//如果当前cpu使用数比该状态小，则更新该状态中的cpu使用情况
					minUse[cur.cpu[0]][cur.cpu[1]][cur.cpu[2]][cur.cpu[3]][cur.cpu[4]] = cur.minUse;
					int tail = rear;
					while (--tail)
					{
						if (tail < 0)tail = QUEUE_MAX - 1;
						if (queue[tail].index != cur.index)break;
						else if ((queue[tail].cpu[0] == cur.cpu[0])
							&& (queue[tail].cpu[1] == cur.cpu[1])
							&& (queue[tail].cpu[2] == cur.cpu[2])
							&& (queue[tail].cpu[3] == cur.cpu[3])
							&& (queue[tail].cpu[4] == cur.cpu[4]))
						{
							queue[tail].minUse = cur.minUse;
							break;
						}
					}
				}
				continue;
			}
			visit[cur.cpu[0]][cur.cpu[1]][cur.cpu[2]][cur.cpu[3]][cur.cpu[4]] = cur.index;//更新visit和cpu使用情况的状态
			minUse[cur.cpu[0]][cur.cpu[1]][cur.cpu[2]][cur.cpu[3]][cur.cpu[4]] = cur.minUse;
			if (rear == QUEUE_MAX)rear = 0;
			queue[rear++] = cur;
		}
	}

	if (visit[0][0][0][0][0] == MAX)return minUse[0][0][0][0][0];//如果1000个任务走完了，那么到第1001个状态开始时，等待时间都为0，0，0，0，0。否则就是没走完
	else return -1;
}
#define TEST_4
void main(void)
{

	TASK task[1002] = { 0 };
	int MAX = 0;

#ifdef TEST_0
	//数据生成
	task[1].start = 1;
	task[1].process = rand() % 9 + 1;
	MAX = 1000;
	for (int i = 2; i <= MAX; i++)
	{
		task[i].start = task[i - 1].start + rand() % 2 + 1;
		task[i].process = rand() % 9 + 1;
	}
#elif defined TEST_1
	task[1].start = 1;
	task[1].process = 6;
	task[2].start = 2;
	task[2].process = 7;
	task[3].start = 8;
	task[3].process = 9;
	task[4].start = 9;
	task[4].process = 10;
	task[5].start = 17;
	task[5].process = 10;
	MAX = 5;
#elif defined TEST_2
	task[1].start = 1;
	task[1].process = 8;
	task[2].start = 2;
	task[2].process = 8;
	task[3].start = 7;
	task[3].process = 3;
	task[4].start = 9;
	task[4].process = 10;
	MAX = 4;

#elif defined TEST_3
	freopen("input.txt", "r", stdin);
	MAX = 1000;
	for (int i = 1; i <= MAX; i++)
	{
		scanf("%d %d\n", &task[i].start, &task[i].process);
	}
#elif defined TEST_4
	for (int i = 1; i <= 40; i++)
	{
		task[i].start = 1;
		task[i].process = 1;
	}
	int a = 10;
	for (int j = 0; j < 19; j++)
	{
		for (int i = 41+j*50; i < 41 + (j+1) * 50; i++)
		{
			task[i].start =a * (j + 1);
			task[i].process = 1;
		}
	}
	MAX = 990;

#endif

	task[MAX + 1].start = 10000000;
	task[MAX + 1].process = 1;
	time_t c_start, c_end;
	c_start = clock();
	printf("%d\n", test(MAX, task));
	c_end = clock();
	printf("%f ms\n", difftime(c_end, c_start));
}

