#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
using namespace std;
const int dx[4]={-1,1,0,0};
const int dy[4]={0,0,-1,1};
const int map_xmin=1,map_xmax=5,map_ymin=1,map_ymax=9;
int game_mp[6][10];
bool endturn_fl,ctrl_fl;
char ch;
int cnt=0;
int nowunit;

struct unit{
	int mode;//1-WALL  2-A SIDE  3-B SIDE  4-MINE
	char sch;
	int unitx,unity;
	int hp;
	int sp;
}un[55];

int shift_direction(char pch){//WSAD转化为实际方向
	if(pch=='W')return 0;
	else if(pch=='S')return 1;
	else if(pch=='A')return 2;
	else if(pch=='D')return 3;
}

void createunit(int px,int py,int pmode,int php){//创建单位
	un[++cnt].mode=pmode;
	un[cnt].hp=php;
	game_mp[px][py]=cnt;
	un[cnt].unitx=px;
	un[cnt].unity=py;
	if(pmode==1)un[cnt].sch='/';
	if(pmode==2)un[cnt].sch='a';
	if(pmode==3)un[cnt].sch='b';
	if(pmode==4)un[cnt].sch = '*';
}

void mapinit(){//初始化地图
	memset(game_mp,0,sizeof(game_mp));
	printf("WELCOME TO THE GAME!:\n"
		"LET'S START!\n");
	createunit(2,2,2,20);//1
	createunit(2,8,3,20);createunit(4,8,3,20);//2-3
	createunit(4,2,2,20);//4
	createunit(2,5,1,20);createunit(4,5,1,20);createunit(3,3,1,20);createunit(3,7,1,20);
	system("pause");
	system("cls");
}

void showmap(){//显示地图
	for(int i=0;i<=5;i++){
		for(int j=0;j<=9;j++){
			if(i==0){
				printf("%d ",j);
				if(j==9)printf("(y)");
				continue;
			}
			else if(j==0){
				printf("%d ",i);
				continue;
			} 
			if(!game_mp[i][j])printf(". ");
			else printf("%c ",un[game_mp[i][j]].sch);
			//if(j!=9)printf(" ");
		}printf("\n");
	}printf("(x)\n");
}

void showfile(int u,int v){//显示unit
	int pcnt;
	pcnt = game_mp[u][v];
	if(!game_mp[u][v]){
		printf("No Unit There!\n");
		return;
	}
	if(un[pcnt].mode==1)printf("UNIT:WALL\n");
	else if(un[pcnt].mode==2)printf("UNIT:A-Side\n");
	else if(un[pcnt].mode==3)printf("UNIT:B-Side\n");
	if(un[pcnt].mode>1){
		printf("SP:%d HP:%d\n",un[pcnt].sp,un[pcnt].hp);
	}
}

void loadturn(){
	//寻找当前回合操作的单位
	nowunit=nowunit%4+1;
	if(un[nowunit].mode==2)printf("A side,this is your turn.\n");
	else printf("B side,this is your turn.\n");
	printf("The unit you control in this turn lies in (%d,%d)\n",un[nowunit].unitx,un[nowunit].unity);
	un[nowunit].sp+=6;//回复Sp
	if(un[nowunit].hp<=0){
		printf("But Unfortunately, the unit has died.\n");
		endturn_fl=true;
	}
	 
	// 
}

bool mov(int dire){
	int px=un[nowunit].unitx;
	int py=un[nowunit].unity;
	int gx=px+ dx[dire];
	int gy=py+ dy[dire];
	if(gx<map_xmin||gx>map_xmax||gy<map_ymin||gy>map_ymax){
		printf("Out of border!\n");
		return false;
	}
	if(game_mp[gx][gy]!=0&&un[game_mp[gx][gy]].mode!=4){
		printf("Collision!\n");
		return false;
	}
	if(un[nowunit].sp<3){
		printf("No Enough SP!\n");
		return false;
	}
	if(un[game_mp[gx][gy]].mode==4){
		system("cls");
		un[nowunit].hp -= 5;
		if(un[nowunit].hp<=0){
			game_mp[px][py] = 0;
			game_mp[gx][gy] = 0;
			endturn_fl = true;
			showmap();
			printf("You touched the mine!\n");
			printf("The Unit has died!\n");
			return false;
		}
		un[nowunit].unitx = gx;
		un[nowunit].unity = gy;
		un[nowunit].sp -= 3;
		game_mp[gx][gy] = nowunit;
		game_mp[px][py] = 0;
		showmap();
		printf("You touched the mine!\n");
		printf("Your remaining HP:%d\n", un[nowunit].hp);
		return false;
	}
	un[nowunit].unitx = gx;
	un[nowunit].unity = gy;
	un[nowunit].sp -= 3;
	game_mp[gx][gy] = nowunit;
	game_mp[px][py] = 0;
	return true;
}

bool dash(int dire){
	int px=un[nowunit].unitx;//当前地点的坐标
	int py=un[nowunit].unity;
	int gx=px, gy=py;//目标地点的坐标
	if(un[nowunit].sp<6){
		printf("No Enough SP!\n");
		return false;
	}
	while(true){
		if((gx+dx[dire])<map_xmin||(gx+dx[dire])>map_xmax||(gy+dy[dire])<map_ymin||(gy+dy[dire])>map_ymax)break;
		if(un[game_mp[gx+dx[dire]][gy+dy[dire]]].mode==4){
			gx += dx[dire];
			gy += dy[dire];
			break;
		}
		if(game_mp[gx+dx[dire]][gy+dy[dire]]!=0)break;
		gx += dx[dire];
		gy += dy[dire];
	}
	if(un[game_mp[gx][gy]].mode==4){
		system("cls");
		un[nowunit].hp -= 5;
		if(un[nowunit].hp<=0){
			game_mp[px][py] = 0;
			game_mp[gx][gy] = 0;
			endturn_fl = true;
			showmap();
			printf("You touched the mine!\n");
			printf("The Unit has died!\n");
			return false;
		}
		un[nowunit].unitx = gx;
		un[nowunit].unity = gy;
		un[nowunit].sp -= 6;
		game_mp[gx][gy] = nowunit;
		game_mp[px][py] = 0;
		showmap();
		printf("You touched the mine!\n");
		printf("Your remaining HP:%d\n", un[nowunit].hp);
		return false;
	}
	un[nowunit].unitx = gx;
	un[nowunit].unity = gy;
	un[nowunit].sp -= 6;
	game_mp[gx][gy] = nowunit;
	game_mp[px][py] = 0;
	return true;
}

bool attack(int pcnt,int method,int dire){
	int px=un[nowunit].unitx;
	int py=un[nowunit].unity;
	int gx = px + dx[dire], gy = py + dy[dire];
	if(method==1){
		if(un[nowunit].sp<3){
			printf("No Enough SP!\n");
			return false;
		}	
		if(gx<map_xmin||gx>map_xmax||gy<map_ymin||gy>map_ymax||game_mp[gx][gy]==0){
			printf("You attacked,but nothing happened!\n");
			un[nowunit].sp -= 3;
			return true;
		}
		printf("You punched the UNIT in (%d,%d)\n", gx, gy);
		if(un[game_mp[gx][gy]].sch==un[nowunit].sch)
			printf("Ouch!You attacked your own unit!\n");
		un[game_mp[gx][gy]].hp -= 5;
		if(un[game_mp[gx][gy]].hp>0)
			printf("Its remaining HP:%d\n", un[game_mp[gx][gy]].hp);
		else{
			printf("You defeat it!\n");
			game_mp[gx][gy] = 0;
			showmap();
		}
		return true;
	}
	else{
		if(un[nowunit].sp<3){
			printf("No Enough SP!\n");
			return false;
		}
		while(true){
			if(gx<map_xmin||gx>map_xmax||gy<map_ymin||gy>map_ymax){
				printf("You attacked,but nothing happened!\n");
				un[nowunit].sp -= 3;
				return true;
			}
			else if(game_mp[gx][gy]!=0){
				printf("You shooted the UNIT in (%d,%d)\n", gx, gy);
			if(un[game_mp[gx][gy]].sch==un[nowunit].sch)
			printf("Ouch!You attacked your own unit!\n");
				un[game_mp[gx][gy]].hp -= 2;
				if(un[game_mp[gx][gy]].hp>0)
					printf("Its remaining HP:%d\n", un[game_mp[gx][gy]].hp);
				else{
					printf("You defeat it!\n");
					showmap();
					game_mp[gx][gy] = 0;
				}
				return true;
			}
			gx += dx[dire];
			gy += dy[dire];
		}
	}

}

bool setup(int dire,int type){
	int px=un[nowunit].unitx;
	int py=un[nowunit].unity;
	px += dx[dire];
	py += dy[dire];
	if(px<map_xmin||px>map_xmax||py<map_ymin||py>map_ymax){
		printf("Out of border!\n");
		return false;
	}
	if(game_mp[px][py]!=0){
		printf("Collision!\n");
		return false;
	}
	if(un[nowunit].sp<12&&type==1){
		printf("No Enough SP!\n");
		return false;
	}
	if(un[nowunit].sp<6&&type==2){
		printf("No Enough SP!\n");
		return false;
	}
	if(type==1){
		un[nowunit].sp -= 12;
		createunit(px, py, 1, 1);
	}
	if(type==2){
		un[nowunit].sp -= 6;
		createunit(px, py, 4, 1);
	}
	system("cls");
	showmap();
	if(type==1)
		printf("You've set the 1HP barrier.\n");
	if(type==2)
		printf("You've set the mine.\n");
	return true;
}

void unitcontrol(){
	int u,v;
	char tch1;
	cin>>ch;
	if(ch=='F'){
		cin >> u >> v;
		showfile(u,v); //检索信息
	}
	if(ch=='M'){//移动
		cin>>tch1;
		ctrl_fl=mov(shift_direction(tch1));
		if(ctrl_fl){
			system("cls");
			showmap();
			printf("Continue your action!\n");
		}else{
			//
		}
	}
	if(ch=='P'){//emergency stop
		exit(0);
	}
	if(ch=='Q')
		endturn_fl = true; //end the turn;
	if(ch=='A')//attack
	{
		cin >> tch1;
		cin >> u;
		attack(nowunit,u,shift_direction(tch1));
	}
	if(ch=='S')//show map
	{
		system("cls");
		showmap();
	}
	if(ch=='D')//show map
	{
		cin>>tch1;
		ctrl_fl=dash(shift_direction(tch1));
		if(ctrl_fl){
			system("cls");
			showmap();
			printf("Continue your action!\n");
		}else{
			//
		}
	}
	if (ch == 'U')//Utilize
	{
		cin >> tch1;
		cin >> u;
		setup(shift_direction(tch1),u);
	}
}

int main(){
	mapinit();
	while(true){
		showmap();
		loadturn();
		endturn_fl=false;
		
		while(!endturn_fl)unitcontrol();
		printf("This turn has ended.\n");
		system("pause");
		system("cls");
	}
}

