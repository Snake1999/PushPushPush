#define VERSION "1.3"
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <stack>
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x)
#define rdint(i) scanf("%d",&i)
#define oo 2147483647
#define M 100
using namespace std;
int n,m;
struct pos {
	int x,y;
};
bool wall[M][M];
int pdir=0;//0up 1down 2left 3right
int step=0;
pos player;
vector<pos> box,hole;
struct rec{
	vector<pos> box,hole;
	pos player;
	int pdir,step;
};
stack<rec> records;
void record() {
	rec r;
	r.box.clear();
	r.hole.clear();
	r.box.assign(box.begin(),box.end());
	r.hole.assign(hole.begin(),hole.end());
	r.player=player;
	r.pdir=pdir;
	r.step=step;
	records.push(r);
}
void rollback() {
	if (records.empty()) return;
	rec r=records.top();
	player=r.player;
	box.clear();
	hole.clear();
	box.assign(r.box.begin(),r.box.end());
	hole.assign(r.hole.begin(),r.hole.end());
	pdir=r.pdir;
	step=r.step;
	records.pop();
}
bool existbox(int x,int y) {
	for(vector<pos>::iterator it=box.begin(); it!=box.end(); ++it) {
		pos p=*it;
		if (p.x==x&&p.y==y) return true;
	}
	return false;
}
pos& boxat(int x,int y) {
	for(vector<pos>::iterator it=box.begin(); it!=box.end(); ++it) {
		pos p=*it;
		if (p.x==x&&p.y==y) return *it;
	}
}
bool existhole(int x,int y) {
	for(vector<pos>::iterator it=hole.begin(); it!=hole.end(); ++it) {
		pos p=*it;
		if (p.x==x&&p.y==y) return true;
	}
	return false;
}
void go(int x, int y) { 
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
void color(int n_color) { 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n_color);
}
void dump() {
	int i,j;

	FOR(i,0,n+1) {
		if (i==0||i==n+1)
			FOR(j,0,m+1) color(3),cout<<"■",color(15);
		else
			FOR(j,0,m+1) {
			if (j==0||j==m+1) color(3),cout<<"■",color(15);
			else if (wall[i][j]) color(3),cout<<"■",color(15);
			else if (player.x==i&&player.y==j) {
				color(15);
				switch (pdir) {
					case 0:
						cout<<"↑";
						break;
					case 1:
						cout<<"↓";
						break;
					case 2:
						cout<<"←";
						break;
					case 3:
						cout<<"→";
						break;
				}
				color(15);
			} else if (existbox(i,j)) {
				if (!existhole(i,j))color(14);
				else color(12);
				cout<<"▓",color(15);
			}
			else if (existhole(i,j)) color(3),cout<<"※",color(15);
			else cout<<"  ",color(15);
		}
		cout<<endl;
	}
}
void clean() {
	system("cls");
}
void in() {
	int i,j;
	cin>>n>>m;
	FOR(i,1,n) FOR(j,1,m) {
		char c;
		cin>>c;
		pos p;
		if (c=='X') player.x=i,player.y=j;
		if (c=='#') wall[i][j]=true;
		if (c=='@') p.x=i,p.y=j,hole.push_back(p);
		if (c=='*') p.x=i,p.y=j,box.push_back(p);
	}
}
bool canW(pos p) {
	if (p.x<=1) return false;
	if (wall[p.x-1][p.y]) return false;
	if (existbox(p.x-1,p.y)) {
		if (wall[p.x-2][p.y]||p.x-2<=0) return false;
		if (existbox(p.x-2,p.y)) return false;
		return true;
	}
	return true;
}
bool canS(pos p) {
	if (p.x>=n) return false;
	if (wall[p.x+1][p.y]) return false;
	if (existbox(p.x+1,p.y)) {
		if (wall[p.x+2][p.y]||p.x+1>=n) return false;
		if (existbox(p.x+2,p.y)) return false;
		return true;
	}
	return true;
}
bool canA(pos p) {
	if (p.y<=1) return false;
	if (wall[p.x][p.y-1]) return false;
	if (existbox(p.x,p.y-1)) {
		if (wall[p.x][p.y-2]||p.y-2<=0) return false;
		if (existbox(p.x,p.y-2)) return false;
		return true;
	}
	return true;
}
bool canD(pos p) {
	if (p.y>=m) return false;
	if (wall[p.x][p.y+1]) return false;
	if (existbox(p.x,p.y+1)) {
		if (wall[p.x][p.y+2]||p.y+1>=m) return false;
		if (existbox(p.x,p.y+2)) return false;
		return true;
	}
	return true;
}
bool win() {
	for(vector<pos>::iterator it=box.begin(); it!=box.end(); ++it) {
		pos p=*it;
		if (!existhole(p.x,p.y)) return false;
	}
	return true;
}
int main() {
	char title[105]="PushPushPush推箱子 v"; 
	strcat(title,VERSION);
	strcat(title," by 林木兰");
	SetConsoleTitle(title);
	in();
	int err=-1;
	bool win1=false;
	while(true) {
		char o;
		clean();
//		for(vector<pos>::iterator it=box.begin(); it!=box.end(); ++it) {
//			pos p=*it;
//			cout<<"x "<<p.x<<" y "<<p.y<<endl;
//		}
//		cout<<"oper: "<<o<<endl;
//		cout<<"rollback size: "<<records.size();
//		if (records.size()>0) cout<<" ("<<records.top().step<<")";
//		cout<<endl;
//		cout<<"step: #"<<step<<endl;
		dump();
		switch(err) { //message
			case -1:
				color(154);
				cout<<"欢迎来到PushPushPush推箱子 v"<<VERSION<<" by 林木兰"<<endl
				<<"使用WASD来移动,Z键可以回滚操作."<<endl;
				color(15);
				break;
			case 1:
				color(207);
				cout<<"请使用WASD来移动!"<<endl;
				color(15);
				break;
			case 2:
				color(207);
				cout<<"对不起,你无法推动这个方块!"<<endl;
				color(15);
				break;
			case 3:
				color(154);
				cout<<"恭喜你,你赢了!"<<endl;
				color(15);
				break;
			case 4:
				color(154);
				cout<<"回滚成功."<<endl;
				color(15);
		}
		
		o=getch();
		if ((o=='a'||o=='s'||o=='w'||o=='d')&&err!=2) record(),++step;
		err=0;
		switch(o) {
			case 'z':
				rollback();
				err=4;
				break;
			case 'w':
				if (canW(player)) {
					if (existbox(player.x-1,player.y)) {
						boxat(player.x-1,player.y).x--;
					}
					player.x--;
				} else err=2;
				pdir=0;
				break;
			case 's':
				if (canS(player)) {
					if (existbox(player.x+1,player.y)) {
						boxat(player.x+1,player.y).x++;
					}
					player.x++;
				} else err=2;
				pdir=1;
				break;
			case 'a':
				if (canA(player)) {
					if (existbox(player.x,player.y-1)) {
						boxat(player.x,player.y-1).y--;
					}
					player.y--;
				} else err=2;
				pdir=2;
				break;
			case 'd':
				if (canD(player)) {
					if (existbox(player.x,player.y+1)) {
						boxat(player.x,player.y+1).y++;
					}
					player.y++;
				} else err=2;
				pdir=3;
				break;
			default:
				err=1;
		}
		if (win()) err=3,win1=true;
	}
}
