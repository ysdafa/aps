/*
1，首先理解最基本的3学者3野人过河问题，再进行拓展。本题使用状态转移的思想来判断每一次
过河的情况，所有状态可用左岸的学者数，野人数以及船的位置进行描述，状态转移方程需要
根据输入的船的限制人数M进行更新，如限制2人那么转移：01 02 11 10 20.
2，由于本题要找出所有合理解中过河次数最少的一种，可用循环队列实现广度遍历。
3，由于1 =< N <= 10,1 <= M <=5，所以可直接用一个数组记录下所有可能的解，直接输出即可。

*/


#include <stdio.h>
#include <memory.h>

#define SIZE  256

typedef struct
{
	int ml, cl, boat, level;
}conNode;	//condition nodes;	ml, cl means scholars and cannibals on the left, boat == 1 means left or right, level to record times crossing the river

int N, M, Answer;	//N the number of the cannibals  and also scholars	,	M	the limit of the boat
conNode conQueue[SIZE];	//N*N*2		condition queue
int flag[11][11][2];		//1 visited, 0 not

int front, rear;

void initQueue()
{
	front=0;
	rear=0;
}
int fullQueue()
{
	if(front==(rear+1)%SIZE)return 1;
	else
		return 0;
}

int emptyQueue()
{
	if(front==rear)return 1;
	else 
		return 0;
}
void inQueue(conNode node)
{
	if(fullQueue())return;
	conQueue[rear]=node;
	rear=(rear+1)%SIZE;
}

conNode deQueue()
{
	if(!emptyQueue())
	{
		conNode n = conQueue[front];
		front = (front+1)%SIZE;
		return n;
	}
}



int validCon(conNode n);

int main(void)
{
	int T, test_case;
	
	setbuf(stdout, NULL);

	scanf("%d", &T);
	for(test_case = 0; test_case < T; test_case++)
	{
		int f;
		conNode n;
		scanf("%d %d", &N, &M);
		
		Answer=-1;
		memset(flag, 0, sizeof(flag));
		initQueue();
		n.ml=N;
		n.cl=N;
		n.boat=1;
		n.level=0;
		inQueue(n);
		f = 1;
		flag[n.ml][n.cl][n.boat]=1;
		while(!emptyQueue())
		{
			conNode tmp=deQueue();
			conNode newNode;
			int s=0,c=1;

			//printf("%d %d %d\n", tmp.ml, tmp.cl, tmp.boat);
			
			for(s=0;s<=M;s++)
			{
				if(s==0)
					c=1;
				else
					c=0;

				while((s==0&&s+c<=M)||(s!=0&&s+c<=M&&c<=s))
				{
					//printf("s %d c %d\n", s, c);
					if(tmp.boat==1)
					{
						newNode.ml=tmp.ml-s;
						newNode.cl=tmp.cl-c;
						newNode.boat=1-tmp.boat;
						newNode.level=tmp.level+1;
					}
					else
					{
						newNode.ml=tmp.ml+s;
						newNode.cl=tmp.cl+c;
						newNode.boat=1-tmp.boat;
						newNode.level=tmp.level+1;
					}
					
					if(newNode.ml==0&&newNode.cl==0&&newNode.boat==0)
					{
						//printf("s %d c %d\n", s, c);
						f=0;
						Answer=newNode.level;
						break;
					}
					
					if(validCon(newNode))
					{
						if(flag[newNode.ml][newNode.cl][newNode.boat]==1)
						{
							c++;
							continue;
						}
						else 
							flag[newNode.ml][newNode.cl][newNode.boat]=1;
						
						//printf("inqueue: %d %d %d\n", newNode.ml, newNode.cl, newNode.boat);
						inQueue(newNode);
					}
					
					c++;
				}
				
				if(f==0)break;
			}
			if(f==0)break;
		}
		if(Answer==-1)
			printf("impossible\n");
		else
			printf("%d\n", Answer);
	}

	return 0;//Your program should return 0 on normal termination.
}

int validCon(conNode n)
{
	int ml=n.ml, cl=n.cl, boat=n.boat;
	
	if(ml!=0&&ml<cl)return 0;
	
	if(N-ml!=0&&N-ml<N-cl)return 0;
	
	if(ml<0||cl<0)return 0;
	
	if(ml>N||cl>N)return 0;
	
	if(ml==N&&cl==N&&boat==0)return 0;
	
	if(ml==0&&cl==0&&boat==1)return 0;
	
	return 1;
}

//
//int result[11][6] = {
//		{-1,-1,-1,-1,-1,-1},
//		{-1,-1, 1, 1, 1, 1},
//		{-1,-1, 5, 3, 1, 1},
//		{-1,-1,11, 5, 3, 3},
//		{-1,-1,-1, 9, 5, 3},
//		{-1,-1,-1,11, 7, 5},
//		{-1,-1,-1,-1, 9, 7},
//		{-1,-1,-1,-1,11, 7},
//		{-1,-1,-1,-1,13, 9},
//		{-1,-1,-1,-1,15,11},
//		{-1,-1,-1,-1,17,13}
//};
//
//int main()
//{
//   int T, test_case, N, M;
//	
//	setbuf(stdout, NULL);
//
//	scanf("%d", &T);
//	for(test_case = 0; test_case < T; test_case++)
//    {
//        scanf( "%d %d", &N, &M );
//
//        if( result[N][M] == -1 )
//            printf("impossible\n");
//        else
//            printf("%d\n", result[N][M]);
//    }
//
//    return 0;
//}
//

