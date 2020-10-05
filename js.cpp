#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

//number of mines
#define MINES 10
//field length
#define LENGTH 10
void makeMine(int (*a)[LENGTH]);
void setNum(int (*a)[LENGTH]);
void getXY(int (*a)[LENGTH],int &user_x,int &user_y);
//void status(int (*a)[LENGTH]);
void field(int (*a)[LENGTH],int (*b)[LENGTH],int x,int y);
void fail();
int main(){
	//Array "mines" contains only 0 or -1
	//If (x,y) contains "mine", mine[x-1][y-1] = -1.
	int mine[LENGTH][LENGTH];
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			if(i==0||i==9||j==0||j==9)
				mine[i][j]=-2;
			else
				mine[i][j]=0;
		}
	}
	makeMine(mine);
    setNum(mine);
	for(int i=0;i<LENGTH;i++)
	{
		for(int j=0;j<LENGTH;j++)
        {
            if(mine[j][LENGTH-i-1]==-2)
                cout << "W  ";
            else if (mine[j][LENGTH-i-1]==-1)
                cout << "X  ";
            else
                cout <<mine[j][LENGTH-i-1] << "  ";
        }
	cout<<endl;
    }
    //user[][] initialize
    int user[LENGTH][LENGTH];
    //closed=-3
    for(int i=0;i<LENGTH;i++) {
        for(int j=0;j<LENGTH;j++)
            user[i][j]=-3;
    }

    for(int i=0;i<LENGTH;i++) {
        for(int j=0;j<LENGTH;j++)  {
            if(user[j][LENGTH-i-1]==-3)
                cout << "B  ";
            else
                cout << user[j][LENGTH-i-1] << "  ";
            }
            cout << endl;
        }

    //Game starts
    while(1)
    {
        int user_x, user_y;
        getXY(user,user_x,user_y);
        field(mine,user,user_x,user_y);
	        for(int i=0;i<LENGTH;i++) {
		        for(int j=0;j<LENGTH;j++)  {
                    if(user[j][LENGTH-i-1]==-3)
                        cout << "B  ";
                    else
                        cout << user[j][LENGTH-i-1] << "  ";
                    }
	            cout<<endl;
	            }
        
    }

	return 0;
}


void makeMine(int a[LENGTH][LENGTH])
//지뢰 만들기
//ONLY mines[LENGTH][LENGTH] can be allowed.
{
	
	srand(time(NULL));
	int cnt=0;
	while(cnt<10)
	{
		while(1)
			{
			int temp_x = rand()%8;
			int temp_y = rand()%8;
				if((a[temp_x][temp_y]==0)&&(temp_x)!=0&&(temp_x)!=9&&(temp_y)!=0&&(temp_y)!=9)
				{
					a[temp_x][temp_y]=-1;
					break;
				}
			}
		cnt++;
	}
}


void setNum(int (*a)[LENGTH])
//지뢰주변 숫자 만들기
//ONLY mines[LENGTH][LENGTH] can be ALLOWED.
{
	for(int i=1;i<LENGTH-1;i++)
	{
		for(int j=1;j<LENGTH-1;j++)
		{
			if(a[i][j]==-1)
				continue;
			int cnt=0;
			if(a[i-1][j-1]==-1)
				cnt++;
			if(a[i][j-1]==-1)
				cnt++;
			if(a[i+1][j-1]==-1)
				cnt++;

			if(a[i-1][j]==-1)
				cnt++;
			if(a[i+1][j]==-1)
				cnt++;
			if(a[i-1][j+1]==-1)
				cnt++;
			if(a[i][j+1]==-1)
				cnt++;
			if(a[i+1][j+1]==-1)
				cnt++;
			a[i][j]=cnt;
		}
	}

}


void getXY(int (*a)[LENGTH],int &user_x,int &user_y)
//유저로부터 좌표를 입력받음
//그 좌표가 이미 열려있거나, 경계선에 있는 좌표라면 다시 받음
//ONLY user[LENGTH][LENGTH] can be allowed.
{
    while(1) {
    cout << "Insert x" << endl;
    cin >> user_x;
    cout << "Insert y" << endl;
    cin >> user_y;
    if(user_x==0||user_y==0||user_x==9||user_y==9)
        cout << "Insert 1 to 8 x or y." << endl;
    else if(a[user_x][user_y]!=-3)
        cout << "The spot is opened!" << endl;
    else
        break;        
    }
}


void field(int (*a)[LENGTH],int (*b)[LENGTH],int x,int y)
//유저로부터 입력 받은 좌표에서 얼마나 많은 칸을 열리게 하는지 만드는 함수
//a:mines[][], b:user[][], x=user_x, y=user_y
{
    int path[LENGTH][LENGTH]={0,};
    //footprint
    path[x][y]=1;
    vector<int> path_x;
    vector<int> path_y;
    path_x.clear();
    path_y.clear();
    path_x.push_back(x);
    path_y.push_back(y);
    if(a[x][y]!=0)
        b[x][y]=a[x][y];
    else if(a[x][y]==-1)
        fail();
    else
    {
        int pin=1;
        while(1)
        {
            cout << path_x.size() << endl;
            if((a[x][y+1]==0)&&(path[x][y+1]==0)) {
                pin=1;
                path_x.push_back(x);
                path_y.push_back(y+1);
                path[x][y+1]=1;
                x=x;
                y=y+1;
                cout << "up" << endl;
                continue;
            }
            else if((a[x+1][y]==0)&&(path[x+1][y]==0)) {
                pin=1;
                path_x.push_back(x+1);
                path_y.push_back(y);
                path[x+1][y]=1;
                x=x+1;
                y=y;
                cout << "right" << endl;
                continue;
            }
            else if((a[x][y-1]==0)&&(path[x][y-1]==0)) {
                pin=1;
                path_x.push_back(x);
                path_y.push_back(y-1);
                path[x][y-1]=1;
                x=x;
                y=y-1;
                cout << "down" << endl;
                continue;
            }
            else if((a[x-1][y]==0)&&(path[x-1][y]==0)) {
                pin=1;
                path_x.push_back(x-1);
                path_y.push_back(y);
                path[x-1][y]=1;
                x=x-1;
                y=y;
                cout << "left" << endl;
                continue;
            }
        //back
            else {
            pin=pin+2;
            if((path_x[path_x.size()-1]==path_x[0])&&(path_y[path_y.size()-1]==path_y[0]))
                break;

            path_x.push_back(path_x[path_x.size()-pin]);
            path_y.push_back(path_y[path_y.size()-pin]);
            cout << "back" << endl;
            }

            //if(path_x.size()!=1) {
              //  if((path_x[path_x.size()-1]==x)&&(path_y[path_y.size()-1]==y))
                //    break;
                  //  }
        }
    cout << "path_x.size() : " << path_x.size()<<endl;
    for(int i=0;i<path_x.size();i++)
    {
    cout << i << " : " << "b[path_x[i]][path_y[i]] : " << b[path_x[i]][path_y[i]] << endl;
        b[path_x[i]][path_y[i]]=0;
        }
    }
}


void fail()
{
    cout << "failed!" << endl;
    exit(1);
}
