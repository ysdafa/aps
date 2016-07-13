// dp_state2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <memory.h>
using namespace std;

int n, m;
long long dp[31][8];

bool checkState(int state) {
	int flag = 0;
	int tmp = 0;
	int checkNum = 0;
	tmp = state;
	while(tmp>0) {
		checkNum = tmp&3;
		if( checkNum ==0 || checkNum ==3) {
			tmp = tmp >>2;
			continue;
		} else if (checkNum == 1) {
			flag = 1;
			break;
		} else if (checkNum == 2) {
			tmp = tmp >> 1;
			continue;
		}
	}

	if(flag) {
		return false;
	}
	return true;
}

void init()
{
	memset(dp, 0, sizeof(dp));
}


int main()
{
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
#endif
	cin>>n;
	m=3;
	
	while(n!=-1) {
		init();
		if(n==0) {
			cout << 1 << endl;
		} else if (n*m%2) {
			cout << 0 << endl;
		} else {
			int i,j,k;
			int totalState=(1<<m)-1;

			//init the first row 0
			for( j=0; j<=totalState; j++ ) {
				if(checkState(j)) 
					dp[0][j]=1;
			}
			
			// caculate second row
			for(i=1;i<n;i++) {
				for(j=0;j<=totalState;j++) {
					//if((i*j)%2) continue;
					for(k=0;k<=totalState;k++) {
						if( dp[i-1][k] && (j|k)==totalState && checkState(j&k) )
							dp[i][j] = dp[i][j] + dp[i-1][k];
					}
				}
			}
			cout << dp[n-1][totalState] << endl;
		}
		cin>>n;
	}
	return 0;
}