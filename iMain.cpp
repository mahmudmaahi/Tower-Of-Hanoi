#include "iGraphics.h"
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<unistd.h>
#define screenWidth 1200
#define screenHeight 700
#define Pi acos(-1)
using namespace std;

int xd[7]={0},yd[7]={0}; 
int tow[4]={0,142,533,926};
bool tow1[7]={0,1,1,1,1,1,1},tow2[7]={0,0,0,0,0,0,0},tow3[7]={0,0,0,0,0,0,0};
int top[4]={0,1,0,0};
int h=0,m=0;
float s=0;
int music=0;

char Music[2][30]={"MusicOff.bmp","MusicOn.bmp"};
char hd[6][30]={"hd1.bmp","hd2.bmp","hd3.bmp","hd4.bmp","hd5.bmp","hd6.bmp"};
char level[4][10]={"EASY","MEDIUM","HARD","EXTREME"};
char stopWatch[15];
char name[20];
int nameIndex=0,indexNumber=0;
int indexPro[10]={0};
int mode1Names=584;
int gameState=0,subgameState=0;
int diskNumber=3;
int moveCount=0,validmoveCount=0,sourceTower=0,destinationTower=0;
int levelButton=1,modeButton=1,playButton=2,RuleBook=1;
int hdIndex=0,Reset=0;
int autoSolve=0,box=-1;
int solveS[70],solveD[70],S=0,D=0;
int iTxt=0,endGame=0;
int sc,dc;
char txt[10][20]; 

struct Leaderboard{
	char Name[20];
	char time[15];
	char Level[10];
	float seconds;
};
 
struct Leaderboard  L[11],P[10];

void Pinitialize(){
	for(int i=0;i<10;i++){
		P[i].seconds=3600;
	}
}

void Linitialize(){
	for(int i=0;i<11;i++){
		L[i].seconds=3600;
		P[i].seconds=3600;
	}
}

void loadLeaderboard(){
	FILE* file;
	file = fopen("mode1.txt","r");
	if(file==NULL){
		return;
	}
	for(int i=0;i<10;i++){
		fscanf(file,"%s %s %s %f",&L[i].Name,&L[i].time,&L[i].Level,&L[i].seconds);
	}
	fclose(file);
}

void saveLeaderboard(){
	FILE* file=fopen("mode1.txt","w");
	if(file==NULL){
		return;
	}
	for(int i=0;i<10 && strlen(L[i].Name)>0;i++){
		fprintf(file,"%s %s %s %.2f\n",L[i].Name,L[i].time,L[i].Level,L[i].seconds);
	}
	fclose(file);
}

void sortLeaderboard(){
	for(int i=0;i<10;i++){
		for(int j=i+1;j<11;j++){
			if(L[i].seconds>L[j].seconds){
				struct Leaderboard temp=L[i];
				L[i]=L[j];
				L[j]=temp;
			}
		}
	}
}

void addEntry(char* playerName,char* playerTime,char* playerLevel){
	strncpy(L[10].Name,playerName,sizeof(L[10].Name));
	strncpy(L[10].time,playerTime,sizeof(L[10].time));
	strncpy(L[10].Level,playerLevel,sizeof(L[10].Level));
	L[10].seconds=s;
	sortLeaderboard();
}

void displayLeaderboard(){
	for(int i=0;i<10;i++){
		if(strlen(L[i].Name)>0){
			char nAme[20],tIme[15],lEvel[10];
			sprintf(nAme,"%d. %s",i+1,L[i].Name);
			sprintf(tIme,"%s",L[i].time);
			sprintf(lEvel,"%s",L[i].Level);
			iSetColor(128,41,4);
			iText(330,448-(i*42),nAme,GLUT_BITMAP_TIMES_ROMAN_24);
			iText(585,448-(i*42),tIme,GLUT_BITMAP_TIMES_ROMAN_24);
			iText(755,448-(i*42),lEvel,GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
}

void clearLeaderboard() {
    FILE* file=fopen("mode1.txt","w");
    if(file==NULL){
    	return;
    }
    fclose(file);
	for(int i=0;i<11;i++){
		L[i].Name[0]='\0';
		L[i].time[0]='\0';
	}
	Linitialize();
}

void sortPro(){
	for(int i=0;i<9;i++){
		for(int j=i+1;j<10;j++){
			if(P[i].seconds>P[j].seconds){
				struct Leaderboard temp=P[i];
				P[i]=P[j];
				P[j]=temp;
			}
		}
	}
}

void reset(){
	xd[7]={0},yd[7]={0}; 
	h=0,m=0,s=0;
	moveCount=0,validmoveCount=0,sourceTower=0,destinationTower=0;
	memset(tow1,1,sizeof(tow1));
	memset(tow2,0,sizeof(tow2));
	memset(tow3,0,sizeof(tow3));
	memset(top,0,sizeof(top));
	top[1]=1;
	iTxt=0;
	Reset=0,endGame=0;
	autoSolve=0;
	subgameState=0;
	mode1Names=584;
	indexNumber=0;
	name[0]='\0';
	stopWatch[0]='\0';
}

void showDisks(int n){
	int x=pow(2,n)-1;
	char move[10];
	sprintf(move,"%d",x);
	iSetColor(128,41,4);
	iText(910,17,move,GLUT_BITMAP_TIMES_ROMAN_24);
	//tower1
	int k1=0,k2=0,k3=0;
	for(int i=n;i>=1;i--){
		if(tow1[i]==1){
			iShowBMP2(xd[i]+tow[1]-(i-1)*10,yd[i]+44+30*k1,hd[hdIndex+i-1],0);
			k1++;
		}
	} 
	//tower2
	for(int i=n;i>=1;i--){
		if(tow2[i]==1){
			iShowBMP2(xd[i]+tow[2]-(i-1)*10,yd[i]+44+30*k2,hd[hdIndex+i-1],0);
			k2++;
		}
	}
	//tower3
	for(int i=n;i>=1;i--){
		if(tow3[i]==1){
			iShowBMP2(xd[i]+tow[3]-(i-1)*10,yd[i]+44+30*k3,hd[hdIndex+i-1],0);
			k3++;
		}
	}
}

void moveDisks(int source,int destination){
	bool *srcTower,*destTower; 
	switch(source){
		case 1: srcTower=tow1; break;
		case 2: srcTower=tow2; break;
		case 3: srcTower=tow3; break;
	}
	switch(destination){
		case 1: destTower=tow1; break;
		case 2: destTower=tow2; break;
		case 3: destTower=tow3; break;
	}

	if(top[source]==0){
		iTxt=1;
		sprintf(txt[1],"Tower %d is Empty!",source);
		return;
	}

	int diskSize=top[source];

	if(top[destination]==0 || diskSize<top[destination]){
		srcTower[diskSize]=0;
		destTower[diskSize]=1;
		top[source]=0;
		for(int i=diskSize;i<=diskNumber;i++){
			if(srcTower[i]==1){
				top[source]=i;
				break;
			}
		}
		top[destination]=diskSize;
		if(top[1]==0 && top[2]==0 && top[3]==1){
			endGame=1;
			subgameState=111;
			if(modeButton==3){
				sprintf(P[box].time,"%02d:%05.2f",m,s);
				sprintf(P[box].Level,"%s",level[levelButton-1]);
				P[box].seconds=s;
				box++;
				Reset=1;
			}
		}
		validmoveCount++;
		iTxt=0;
	}else{
		iTxt=2;
		return;
	}
}

void TOH(int n,int a,int b,int c){
	if(n==0) return;
	TOH(n-1,a,c,b);
	solveS[S]=a,solveD[S]=c,S++;
	TOH(n-1,b,a,c);
	if(n==diskNumber) autoSolve=0;
}

void Solve(){
	if(D==S) return;
	moveDisks(solveS[D],solveD[D]);
	D++;
}

void button(char key){
	int towerInput=key-'0';
	if(towerInput<1 || towerInput>3){
		iTxt=3;
		moveCount--;
    }else{
		if(moveCount%2==0){
		sourceTower=towerInput;
		}else{
			destinationTower=towerInput;
			moveDisks(sourceTower,destinationTower);
			showDisks(diskNumber);
		}
	}
	moveCount++;
}

void updateStopWatch(){
	if(endGame==1){
		moveCount=0;
	}
	if(moveCount){
		s+=(0.1);
		if(s>=60.0){
            m+=s/60;
            s=(int)s%60;
        }
	}
}

void iStopWatch(){
	sprintf(stopWatch,"%02d:%05.2f",m,s);
	iSetColor(128,41,4);
	iText(636,628,stopWatch,GLUT_BITMAP_TIMES_ROMAN_24);
}

void iDraw() {
	iClear();
	if(gameState==0) iShowBMP(0,0,"Loading Screen.bmp");
	if(gameState==1) iShowBMP(0,0,"Home Screen.bmp");
	if(gameState==11){
		iShowBMP(0,0,"Game Screen.bmp");
		char vmc[10];
		sprintf(vmc,"%d",validmoveCount);
		iSetColor(128,41,4);
		iText(636,660,vmc,GLUT_BITMAP_TIMES_ROMAN_24);
		iStopWatch();
		if(modeButton==3){
			if(strlen(P[box].Name)>0){
				char proName[30]="It's ";
				strcat(proName,P[box].Name);
				strcat(proName,"'s Turn!");
				iSetColor(128,41,4);
				iText(500,585,proName,GLUT_BITMAP_TIMES_ROMAN_24);
			}else if(strlen(P[box].Name)==0){
				sortPro();
				iShowBMP(0,0,"LeaderboardPro.bmp");
				for(int i=0;i<10;i++){
					iSetColor(128,41,4);
					iText(330,448-(i*42),P[i].Name,GLUT_BITMAP_TIMES_ROMAN_24);
					iText(585,448-(i*42),P[i].time ,GLUT_BITMAP_TIMES_ROMAN_24);
					iText(755,448-(i*42),P[i].Level,GLUT_BITMAP_TIMES_ROMAN_24);
				}
				subgameState=312;
			}
		}
		if(modeButton==1||modeButton==3){
			if(iTxt==1){
				iSetColor(128,41,4);
				iText(118,15,txt[1],GLUT_BITMAP_TIMES_ROMAN_24);
			}else if(iTxt==2){
				iSetColor(128,41,4);
				iText(118,15,"Cannot place larger disk on smaller disk!",GLUT_BITMAP_TIMES_ROMAN_24);
			}else if(iTxt==3){
				iSetColor(128,41,4);
				iText(118,15,"Invalid Tower Entry!",GLUT_BITMAP_TIMES_ROMAN_24);
			}else if(endGame==1){
				iSetColor(128,41,4);
				iText(118,15,"Congratulations!",GLUT_BITMAP_TIMES_ROMAN_24);
				if(validmoveCount==pow(2,diskNumber)-1) iText(286,15,"You solved it with minimum moves!",GLUT_BITMAP_TIMES_ROMAN_24);
			}
		}
		if(modeButton==2){
			if(autoSolve==1){
				TOH(diskNumber,1,2,3);
				Solve();
			}
		}
		
		if(Reset==1) reset();
		if(subgameState!=312) showDisks(diskNumber);

		if(subgameState==111 && modeButton==1){
			iShowBMP(327,204,"Name Input Mode 1.bmp");
			sprintf(stopWatch,"%02d:%05.2f",m,s);
			iSetColor(128,41,4);
			iText(540,460,stopWatch,GLUT_BITMAP_TIMES_ROMAN_24);
			iText(mode1Names,323,name,GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
	if(subgameState==311){
		iShowBMP(0,0,"LeaderboardPro.bmp");
		for(int i=0;i<10;i++)
		iText(330,448-(i*42),P[i].Name,GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if(gameState==121){
		iShowBMP(0,0,"Game Mode Selection.bmp");
		reset();
		iSetColor(128,41,4);
		if(modeButton==1) iFilledCircle(426,435,15,1000);
		else if(modeButton==2) iFilledCircle(426,359,15,1000);
		else if(modeButton==3) iFilledCircle(426,282,15,1000);
	}
	if(gameState==122){
		iShowBMP(0,0,"Game Mode.bmp");
		iSetColor(128,41,4);
		if(levelButton==1) iFilledCircle(485,435,15,1000);
		else if(levelButton==2) iFilledCircle(485,378,15,1000);
		else if(levelButton==3) iFilledCircle(486,322,15,1000);
		else if(levelButton==4) iFilledCircle(486,265,15,1000);
	}
	if(gameState==13) iShowBMP(0,0,"Credits.bmp");
	if(gameState==14){
		iShowBMP(0,0,Music[music]);
		iSetColor(130,0,0);
		if(playButton==1) iRectangle(411,380,152,55),iRectangle(412,381,150,53);
		else if(playButton==2) iRectangle(646,380,162,55),iRectangle(647,381,160,53);
	}
	if(gameState==15){
		iShowBMP(0,0,"Leaderboard.bmp");
		if(iTxt==9){
			clearLeaderboard();
			iTxt=0;
		}
		displayLeaderboard();
	}
	if(gameState==16){
		if(RuleBook==1) iShowBMP(0,0,"Rulebook1.bmp");
		if(RuleBook==2) iShowBMP(0,0,"Rulebook2.bmp");
	}
}

void iMouseMove(int mx, int my) {

}

void iMouse(int button, int state, int mx, int my) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if(gameState==0){
			if(mx>=953 && mx<=1156 && my>=21 && my<=74) gameState=1;
		}
		if(gameState==1){
			if(mx>=22 && mx<=66 && my>=640 && my<=676) gameState=0;//Home
			else if(mx>=254 && mx<=368 && my>=26 && my<=143){
				if(modeButton==3) subgameState=311;
				else gameState=11;//Play
			}
			else if(mx>=397 && mx<=512 && my>=26 && my<=143) gameState=121;//GameMode
			else if(mx>=541 && mx<=653 && my>=26 && my<=143) gameState=13;//Credits
			else if(mx>=685 && mx<=796 && my>=26 && my<=143) gameState=14;//Settings
			else if(mx>=829 && mx<=943 && my>=26 && my<=143) exit(0);//Exit
			else if(mx>=31 && mx<=59 && my>=566 && my<=613) gameState=15;//Leaderboard
			else if(mx>=22 && mx<=64 && my>=506 && my<=548) gameState=16;//Rules
		}
		if(gameState==11){
			if(mx>=28 && mx<=76 && my>=635 && my<=679) gameState=1;
			if(mx>=30 && mx<=73 && my>=580 && my<=622) Reset=1;
			if(modeButton==2){
				if(mx>=1019 && mx<=1171 && my>=633 && my<=675){
					autoSolve=1;
				}
			}
			if(subgameState==111){
				if((mx>=392 && mx<=544 && my>=234 && my<=286)||iTxt==10){//save
					addEntry(name,stopWatch,level[levelButton-1]);
					saveLeaderboard();
					Reset=1;
				}
				else if(mx>=625 && mx<=776 && my>=234 && my<=286){//exit
					Reset=1;
				}
			}
		}
		if(subgameState==311){
				if(mx>=315 && mx<=351 && my>=539 && my<=573){
					gameState=11;
					subgameState=0;
					box=0;
				}
				if(mx>=316 && mx<=529 && my>=436 && my<=475) box=0;
				if(mx>=316 && mx<=529 && my>=397 && my<=436) box=1;
				if(mx>=316 && mx<=529 && my>=352 && my<=397) box=2;
				if(mx>=316 && mx<=529 && my>=313 && my<=352) box=3;
				if(mx>=316 && mx<=529 && my>=269 && my<=313) box=4;
				if(mx>=316 && mx<=529 && my>=221 && my<=269) box=5;
				if(mx>=316 && mx<=529 && my>=175 && my<=221) box=6;
				if(mx>=316 && mx<=529 && my>=137 && my<=175) box=7;
				if(mx>=316 && mx<=529 && my>=96 && my<=137) box=8;
				if(mx>=316 && mx<=529 && my>=56 && my<=96) box=9;
		}
		if(subgameState==312){
			if(mx>=315 && mx<=351 && my>=539 && my<=573) gameState=1;
		}
		if(gameState==121){
			if((mx>=410 && mx<=440 && my>=422 && my<=450)||(mx>=470 && mx<=797 && my>=410 && my<=461)) modeButton=1;
			if((mx>=410 && mx<=440 && my>=344 && my<=373)||(mx>=470 && mx<=797 && my>=336 && my<=386)) modeButton=2;
			if((mx>=410 && mx<=440 && my>=269 && my<=296)||(mx>=470 && mx<=797 && my>=261 && my<=316)){
				modeButton=3;
				Pinitialize();
			}
			if(mx>=369 && mx<=402 && my>=470 && my<=500) gameState=1;
			if(mx>=798 && mx<=834 && my>=470 && my<=500) gameState=122;
		}
		if(gameState==122){
			if((mx>=470 && mx<=500 && my>=422 && my<=445)||(mx>=533 && mx<=612 && my>=429 && my<=445)) levelButton=1,diskNumber=3;
			if((mx>=470 && mx<=500 && my>=365 && my<=391)||(mx>=531 && mx<=643 && my>=373 && my<=387)) levelButton=2,diskNumber=4;
			if((mx>=470 && mx<=500 && my>=308 && my<=338)||(mx>=532 && mx<=613 && my>=316 && my<=332)) levelButton=3,diskNumber=5;
			if((mx>=470 && mx<=500 && my>=252 && my<=280)||(mx>=531 && mx<=677 && my>=258 && my<=273)) levelButton=4,diskNumber=6;
			if(mx>=367 && mx<=404 && my>=463 && my<=493) gameState=121;
		}
		if(gameState==13){
			if(mx>=319 && mx<=353 && my>=491 && my<=522) gameState=1;
		}
		if(gameState==14){
			if(mx>=574 && mx<=634 && my>=254 && my<=311){
				if(music==1){
					music=0;
					PlaySound(0,0,0);
				}else{
					music=1;
					PlaySound("Traditional_1.wav",NULL,SND_LOOP|SND_ASYNC);
				}
			}
			if(mx>=416 && mx<=552 && my>=392 && my<=429) playButton=1;
			if(mx>=651 && mx<=798 && my>=392 && my<=429) playButton=2;
			if(mx>=369 && mx<=405 && my>=467 && my<=497) gameState=1;
		}
		if(gameState==15){
			if(mx>=315 && mx<=351 && my>=539 && my<=573) gameState=1;
			if(mx>=835 && mx<=879 && my>=535 && my<=576) iTxt=9;
		}
		if(gameState==16){
			if(RuleBook==1){
				if(mx>=1068 && mx<=1103 && my>=106 && my<=141) RuleBook=2;
				if(mx>=82 && mx<=115 && my>=515 && my<=542) gameState=1;
			}
			if(RuleBook==2 && mx>=95 && mx<=130 && my>=117 && my<=149) RuleBook=1;
		}
		
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		printf("%d %d\n",mx,my);
	}
}

void iKeyboard(unsigned char key) {
	if(gameState==0 && key==13) gameState=1;
	if(subgameState==111){
		if(key!='\b'){
			name[indexNumber]=key;
			indexNumber++;
			mode1Names-=7;
			name[indexNumber]='\0';
		}else{
			if(indexNumber<=0) indexNumber=0;
			else{
				indexNumber--;
				mode1Names+=7;
			}
			name[indexNumber]='\0';
		}
	}else if(subgameState==311){
		if(key!='\b'){
			P[box].Name[indexPro[box]]=key;
			indexPro[box]++;
			P[box].Name[indexPro[box]]='\0';
		}else{
			if(indexPro[box]<=0) indexPro[box]=0;
			else{
				indexPro[box]--;
			}
			P[box].Name[indexPro[box]]='\0';
		}
	}
	else if(gameState==11 && subgameState!=111 && subgameState!=311){
		if(modeButton==1 || modeButton==3){
			if(key>='A' && key<='Z'){
				key+=32;
			}
			if(playButton==2 && (key=='a'||key=='s'||key=='d')){
				if(key=='a') key='1';
				else if(key=='s') key='2';
				else if(key=='d') key='3';
				button(key);
			}
			else if(playButton==1 && (key=='1'||key=='2'||key=='3')) button(key);
			if(key==13) iTxt=10;
		}
	}
}

void iSpecialKeyboard(unsigned char key) {
	if(key==GLUT_KEY_HOME){
		music=1;
		PlaySound("Traditional_1.wav",NULL,SND_LOOP|SND_ASYNC);
	}else if(key==GLUT_KEY_END){
		music=0;
		PlaySound(0,0,0);
	}
}

int main() {
	iSetTimer(100,updateStopWatch);
	iSetTimer(500,Solve);

	Linitialize();
	loadLeaderboard();
	if(music==1)
		PlaySoundW(L"Traditional_1.wav",NULL,SND_LOOP|SND_ASYNC);
	iInitialize(screenWidth, screenHeight, "Tower of Hanoi");
	return 0;
}
