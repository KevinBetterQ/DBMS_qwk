#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "conio.h"
#include "windows.h"
#include "iostream"
#include "power.h"

#include <sstream>



using namespace std;


#define MAX_PROLEN 10	//��������ַ�����
#define MAX_PRONUM 8	//������Ը���
#define MAX_RECNUM 30	//����¼����
#define MAX_TNAME 15	//�����
#define MAX_TNUM 15		//�������
#define MAX_VIEWNUM 15		//�������
#define MAX_USERLEN 10	//�û�������
#define MAX_PWDLEN 18	//������󳤶�
#define MAX_USERNUM 10	//�û���������

//***************************
typedef struct
{
	int re_num;		//��¼�ĸ���
	int pro_num;	//���Ը���
	int key_no;		//�涨�ؼ����ǵڼ�������,ʹ�õ����±��0��ʼ
	//int pro_len[MAX_PRONUM];	//���Գ���
	char key[MAX_PROLEN];		//����ؼ���,�ؼ���Ҳ��һ������
	char tname[MAX_TNAME];		//����
	char proname[MAX_PRONUM][MAX_PROLEN];	//���Ե�����
	char protype[MAX_PRONUM];	//���Ե�����
	char record[MAX_RECNUM][MAX_PRONUM][MAX_PROLEN];		//��ά��,����������Ǽ�¼��ֵ
}table;

typedef struct
{
	char proname[MAX_PRONUM][MAX_PROLEN];	//�ɼ�������
	int prono[MAX_PRONUM];		//�ɼ����Ե��±�
	char tname[MAX_TNAME];		//��Ӧ�ı���
	char vname[MAX_TNAME];		//��ͼ��
	int pro_num;
}view;		//��ͼ������ͼ��Ӧһ����һ������Զ�Ӧ�����ͼ

typedef struct
{
	char iname[MAX_TNAME];		//һ��������Ӧһ����
	int key_num;
	char key[MAX_RECNUM][MAX_PROLEN];	//
}index;
typedef struct
{
	char username[MAX_USERLEN];
	char pwd[MAX_PWDLEN];
	int revoke;		//0�ǹ���Ա��1����ͨ�û�
	int r_select;
	int r_delete;
	int r_update;
	int r_insert;
	char r_table[MAX_TNUM][MAX_TNUM];
}user;

table NULLTABLE;	//�ձ���ʼ����ʱ����NULLTABLE.key_no=-1
table t[MAX_TNUM];
view vi[MAX_VIEWNUM];
index ind[MAX_TNUM];

user u[MAX_USERNUM];
FILE *alfp;

int t_num=0;
int i_num=0;
int v_num=0;
int u_num=0;

int CURRENTREVOKE;
user currentuser;
int speedfactor=10;

//��ʼ����
void initialize()
{
	int i,j,k,p;
	t_num=0;
	NULLTABLE.key_no=-1;	//�ձ�Ķ������key_no��-1�ı�Ҳ��һ�ֱ�ʶ
	for(i=0;i<MAX_TNUM;i++)
	{
		for(j=0;j<MAX_PRONUM;j++)
			t[i].key[j]='\0';
		t[i].key_no=0;
		t[i].pro_num=0;
		t[i].re_num=0;
		for(j=0;j<MAX_PRONUM;j++)
			t[i].protype[j]='\0';
		for(j=0;j<MAX_PRONUM;j++)
			for(k=0;k<MAX_PROLEN;k++)
				t[i].proname[j][k]='\0';
		for(j=0;j<MAX_TNAME;j++)
			t[i].tname[j];
		for(j=0;j<MAX_RECNUM;j++)
			for(k=0;k<MAX_PRONUM;k++)
				for(p=0;p<MAX_PROLEN;p++)
					t[i].record[j][k][p]='\0';
	}
}

//�û����½
void login()
{
	u_num=4;
	strcpy(u[0].username,"0");
	strcpy(u[0].pwd,"0");
	u[0].revoke=0;
	strcpy(u[1].username,"a");
	strcpy(u[1].pwd,"a");
	u[1].revoke=0;
	u[1].r_delete=0;
	u[1].r_insert=0;
	u[1].r_select=0;
	u[1].r_update=0;
	strcpy(u[2].username,"qwk");
	strcpy(u[2].pwd,"qwk");
	u[2].revoke=1;
	u[2].r_delete=1;
	u[2].r_insert=1;
	u[2].r_select=1;
	u[2].r_update=1;
	strcpy(u[3].username,"1");
	strcpy(u[3].pwd,"1");
	u[3].revoke=1;
	int t=1;
ag:	clrscr();
	PrintStringXY(24,7, "��������������������������������");
	PrintStringXY(24,8, "��                            ��");
	PrintStringXY(24,9, "�ǩ�����������������������������");
	PrintStringXY(24,10,"��  �û���:                   ��");
	PrintStringXY(24,11,"�ǩ�����������������������������");
	PrintStringXY(24,12,"��  ����  :                   ��");
	PrintStringXY(24,13,"��������������������������������");
	PrintColorStringXY(36,8,0,7,0,15,"�û���½");

	int i;
	char username[MAX_USERLEN];
	char pwd[MAX_PWDLEN];
	gotoxy(37,10);
	gets(username);
	//***************************
	for(i=0;i<u_num;i++)
	{//acc
		if(strcmp(username,u[i].username)==0)
			break;
		if((strcmp(username,u[i].username)!=0)&&(i==u_num-1))
		{
			if(t==3)
			{
				PrintStringXY(3,24,"��������ﵽ����,ϵͳǿ���˳�.\a��������˳�.");
				getch();
				exit(-1);
			}
			PrintStringXY(3,24,"û�и��û���,��˶Ժ�������.\a�����������.");
			getch();
			t++;
			goto ag;
		}
	}
	//***************************
	gotoxy(37,12);
	strcpy(pwd,InputStarString());
//***********************************************************
	for(i=0;i<u_num;i++)
	{
		if(strcmp(username,u[i].username)==0)
		{
			if(strcmp(pwd,u[i].pwd)==0)
			{
				printf("\n\n\n��½�ɹ��������������...");
				CURRENTREVOKE=u[i].revoke;
				currentuser=u[i];
				getch();
				clrscr();
				return;
			}
		}
	}
	if(t==3)
	{
		PrintStringXY(3,24,"��������ﵽ����,ϵͳǿ���˳�.\a��������˳�.");
		getch();
		exit(-1);
	}
	PrintStringXY(3,24,"\a���������У�Ժ���ʹ�ã������������...");
	getch();
	t++;
	goto ag;
//***********************************************************
}


//---------���ļ����------------------------------------------------

//�ж��Ƿ�������1������
bool dbislock()
{
	int databaselock;
	FILE *fp;
	if((fp=fopen("dblock.db","rb"))==NULL)
	{
		printf("���������ļ�,�������ʹ��.\a");
		fclose(fp);
		exit(-1);
	}
	fread(&databaselock,sizeof(int),1,fp);
	fclose(fp);
	if(databaselock==1)
		return true;
	else
		return false;
}
void dblock()
{
	int databaselock=1;
	FILE *fp;
	if((fp=fopen("dblock.db","wb"))==NULL)
	{
		printf("���������ļ�,�������ʹ��\n");
		fclose(fp);
		exit(-1);
	}
	fwrite(&databaselock,sizeof(int),1,fp);
	fclose(fp);
}
void dbunlock()
{
	int databaselock=0;
	FILE *fp;
	if((fp=fopen("dblock.db","wb"))==NULL)
	{
		printf("���������ļ�,�������ʹ��.\a");
		fclose(fp);
		exit(-1);
	}
	fwrite(&databaselock,sizeof(int),1,fp);
	fclose(fp);
}
void StronglySetLock0()//����Ա��ǿ�ƽ���
{
	int databaselock=0;	//ǿ���趨����״̬Ϊ��
	FILE *fp;
	if((fp=fopen("dblock.db","wb"))==NULL)
		fp=fopen("dblock.db","wb+");
	fwrite(&databaselock,sizeof(int),1,fp);
	fclose(fp);
	PrintStringXY(1,24,"\a�Ѿ�������Ϊ��,���������...");
	getch();
}

//---------һЩ�ж����------------------------------------------------
bool IsRecEqu(table *r,int i,table *s,int j)
{
	int p;
	if(r->pro_num!=s->pro_num)		//���Ը�����ƥ��
		return false;
	for(p=0;p<s->pro_num;p++)		//�������ֲ�ƥ��
		if(strcmp(s->proname[p],r->proname[p])!=0)
			return false;
	for(p=0;p<r->pro_num;p++)
		if(strcmp(r->record[i][p],s->record[j][p])!=0)
			return false;
	return true;
}
bool IsTableSameType(table *t1,table *t2)
{
	int p;
	if(t1->pro_num!=t2->pro_num)
	{
		gotoxy(1,20);printf("ErrorNo1:unequal number of properities.");
		return false;
	}
	for(p=0;p<t1->pro_num;p++)
		if(strcmp(t1->proname[p],t2->proname[p])!=0)
		{
			gotoxy(1,20);printf("ErrorNo2:different proname[%d]",p);
			return false;
		}
	return true;
}



//-------��Ĳ���---------------------------------------------------
void tablecreate()
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���
		dblock();	//dblock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	clrscr();
	char mn[MAX_TNAME];
	char ck[MAX_PROLEN];
	table tnew;
	int nnum;
	int i,j,k;
	gotoxy(37,7);setcolor(0,14);
	printf("�������");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("�����½��������:");
	gets(mn);
	//������������������
	strcpy(tnew.tname,mn);	//�±����
	gotoxy(27,10); printf("�����½�������Ը���:");
	cin>>nnum;
	//��Ӵ�����
	if(nnum>=MAX_TNUM)
	{
		gotoxy(1,20);printf("�Ѿ��ﵽ���������,�޷�����\a!");
		goto eotc1;
	}
	tnew.pro_num=nnum;
	//��ʼѭ��������������;
	//ע������������
	gotoxy(1,15);
	i=0;
	while(i<tnew.pro_num)
	{
		printf("\n�������%d����������:",i+1);
		cin>>mn;
		//��������!!!!!
		strcpy(tnew.proname[i],mn);
		printf("\n�������%d����������n=num  c=char:",i+1);
		//������,�����ϵ�����Ҫ�ų���!
		cin>>tnew.protype[i];
		i++;
	}

	//��������,������Ψһ��ʾһ����¼������,����Ҫ������.������λ����tnew.key,�����������е��±�.
	printf("\n��������һ�����Ǳز����ٵ�.\n");
	printf("����������������,����������Զ�ѡ���һ������:");
	cin>>ck;
	for(i=0;i<tnew.pro_num;i++)
	{
		if(strcmp(ck,tnew.proname[i])==0)
		{
			tnew.key_no=i;
			strcpy(tnew.key,ck);
			goto nex1;
		}
	}
	printf("\n�������,�����Զ�ѡ��Ϊ��һ������:  \a%s",tnew.proname[0]);
	strcpy(tnew.key,tnew.proname[0]);
	tnew.key_no=0;
nex1:;
	tnew.re_num=0;		//��¼������Ϊ��
	for(i=0;i<MAX_RECNUM;i++)
		for(j=0;j<MAX_PRONUM;j++)
			for(k=0;k<MAX_PROLEN;k++)
				tnew.record[i][j][k]='\0';	//��ʼ����¼��
	t[t_num++]=tnew;
	printf("\n����ɹ�,�س�������һ���˵�.");
	getch();
eotc1:;
	dbunlock();		//����
	fprintf(alfp,"\n[%d��%d��%d�� %d:%d:%d]ִ�����²���:\n",GetCurrentYear(),GetCurrentMonth(),GetCurrentDay(),
									GetCurrentHour(),GetCurrentMinute(),GetCurrentSecond() );
	fprintf(alfp,"�û�������ɹ�,���������:����:%s,�����Ը���:%d,����:%s.",tnew.tname,
								tnew.pro_num,tnew.key);fputc('\n',alfp);
}
void tablecreate2();

void tabledrop()
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	clrscr();
	char mn[MAX_TNAME];
	int i,j;
	table tdel;
	gotoxy(37,7);setcolor(0,14);
	printf("��ɾ������");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("����ɾ���ı���:");
	cin>>mn;
	//��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫɾ���ı��λ��.
			tdel=t[i];
			for(j=i;j<t_num-1;j++)
				t[j]=t[j+1];
			t_num--;
			goto eodrp;
		}
	gotoxy(1,20);printf("δ�ҵ�Ҫɾ���ı�,��ȷ������.\a");
	getchar();
	dbunlock();
	return;
eodrp:;
	gotoxy(1,20);
	printf("��%s���ɹ���ɾ��!�س�������һ���˵�.",mn);
	getch();
	dbunlock();
		fprintf(alfp,"\n[%d��%d��%d�� %d:%d:%d]ִ�����²���:\n",GetCurrentYear(),GetCurrentMonth(),GetCurrentDay(),
									GetCurrentHour(),GetCurrentMinute(),GetCurrentSecond() );
	fprintf(alfp,"�û��ɹ�ɾ��һ����,���������:����:%s,�����Ը���:%d,����:%s.",tdel.tname,
								tdel.pro_num,tdel.key);fputc('\n',alfp);
}

void tableprint(table *t1)//�����
{
	int m,n;
	printf("\n-----------------------------------------\n");
	for(m=0;m<t1->pro_num;m++)
		printf("%s\t",t1->proname[m]);
	printf("\n-----------------------------------------\n");
	for(m=0;m<t1->re_num;m++)
	{
		for(n=0;n<t1->pro_num;n++)
			printf("%s\t",t1->record[m][n]);
		printf("\n\n");
	}
}

void outputts()		//������Ϣ��ʾ
{
	clrscr();
	int i,j;
	for(i=0;i<t_num;i++)
	{
		printf("\n����:%s   ���Ը���:%d    ����:%s",t[i].tname,t[i].pro_num,t[i].key);
		printf("\n----------------------------------------------------------------\n");
		for(j=0;j<t[i].pro_num;j++)
		{
			printf("%s(%c)\t",t[i].proname[j],t[i].protype[j]);
		}
		printf("\n\n");
	}
	getchar();
}



//---------��¼�Ĳ���-----------------------------------------------
void recordinsert()//����
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	clrscr();
	char mn[MAX_TNAME];
	char mq[MAX_TNAME];
	int i,j;
	gotoxy(37,7);setcolor(0,14);
	printf("��¼�������");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("�������ı���:");
	gets(mn);

	//��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫɾ���ı��λ��.
			goto eodrp;
		}
	gotoxy(1,20);printf("δ�ҵ�Ҫɾ���ı�,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
eodrp:;//�ҵ�������λ��,�������ǲ����¼����,��t[i]�ǲ鵽�ı�.
	//����ж�,�Ƿ���Լ�����Ӽ�¼�����ﵽ��ֵ.������
	strcpy(mq,mn);		//������������õ�mq����
	if(t[i].re_num>=MAX_RECNUM)
	{
		gotoxy(1,20);printf("�ñ��Ѿ��ﵽ��¼�����ֵ!�޷�����.\a");
		getch();
		if(dbislock())
		dbunlock();
		return;
	}
	gotoxy(1,20);
	for(j=0;j<t[i].pro_num;j++)
	{
		printf("\n����������%s��ֵ:",t[i].proname[j]);
		gets(mn);
		//��Ӵ�����,���������������ظ��ȵ�.
		strcpy(t[i].record[t[i].re_num][j],mn);		//������ֵ������Ӧ�ļ�¼����λ��.
				//re_num++
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	t[i].re_num++;

	printf("\n��%s�Ѿ����ɹ��ز�����һ���¼�¼!�س�������һ���˵�.",mq);
	getch();
	if(dbislock())
		dbunlock();
}
void recorddel()//ɾ��
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	clrscr();
	char mn[MAX_TNAME];
	char mq[MAX_TNAME];
	char zj[MAX_PROLEN];
	int i,j,k;
	gotoxy(37,7);setcolor(0,14);
	printf("��¼ɾ������");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("����ɾ����¼�ı���:");
	gets(mn);

	//��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫɾ���ı��λ��.
			goto eodrd;
		}
	gotoxy(1,20);printf("δ�ҵ�Ҫɾ����¼�ı�,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
eodrd:;
	//�ҵ�������λ��,�������ǲ����¼����,��t[i]�ǲ鵽�ı�.
	//����ж�,�Ƿ���Լ�����Ӽ�¼�����ﵽ��ֵ.������
	strcpy(mq,mn);		//������������õ�mq����
	if(t[i].re_num<=0)
	{
		gotoxy(1,20);printf("�ñ��޼�¼!�޷�ɾ��.\a");
		getch();
		if(dbislock())
			dbunlock();
		return;
	}
	gotoxy(27,10); printf("����Ҫɾ����¼������ֵ:");
	gets(zj);	//����������ֵ
	//��Ӵ�����,�ж��Ƿ��дʼ�¼
	for(j=0;j<t[i].re_num;j++)
	{
		if( strcmp( t[i].record[j][t[i].key_no] , zj ) ==0)
			goto findkey1;
	}
	gotoxy(1,20);printf("δ�ҵ��ü�¼,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
findkey1:;
	//�ҵ��ü�¼,��ʼ����ɾ������.j����Ҫɾ���ļ�¼���±�
	for(;j<t[i].re_num-1;j++)
	{
		for(k=0;k<t[i].pro_num;k++)
			strcpy(t[i].record[j][k],t[i].record[j+1][k]);
	}
	t[i].re_num--;
	printf("\n��%s�Ѿ����ɹ���ɾ����һ����¼!�س�������һ���˵�.",mq);
	getch();
	if(dbislock())
		dbunlock();
}
void outputrec()//��ʾ
{
	clrscr();
	int i,j,k;
	for(i=0;i<t_num;i++)
	{
		printf("\n����:%s   ���Ը���:%d    ����:%s",t[i].tname,t[i].pro_num,t[i].key);
		printf("\n----------------------------------------------------------------\n");
		for(j=0;j<t[i].pro_num;j++)
		{
			printf("%s\t",t[i].proname[j]);
		}
		printf("\n----------------------------------------------------------------\n");
		for(k=0;k<t[i].re_num;k++)
		{
			for(j=0;j<t[i].pro_num;j++)
				printf("%s\t",t[i].record[k][j]);
			printf("\n");
		}
		printf("\n\n���س��������һ�ű�...\n");
		getchar();
	}
	printf("\n��������������س�������һ���˵�.");
	getchar();
}
void recordupdate()////����
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	clrscr();
	char mn[MAX_TNAME];
	char mq[MAX_TNAME];
	char zj[MAX_PROLEN];
	int i,j,k;
	gotoxy(37,7);setcolor(0,14);
	printf("��¼���Ĳ���");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("�����¼���ڵı���:");
	gets(mn);

	//��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫɾ���ı��λ��.
			goto eodrp;
		}
	gotoxy(1,20);printf("δ�ҵ�Ҫɾ���ı�,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
eodrp:;//�ҵ�������λ��,�������ǲ����¼����,��t[i]�ǲ鵽�ı�.
	//����ж�,�Ƿ���Լ�����Ӽ�¼�����ﵽ��ֵ.������
	strcpy(mq,mn);		//������������õ�mq����
	if(t[i].re_num<=0)
	{
		gotoxy(1,20);printf("�ñ��¼ֵΪ0!�޷�����.\a");
		getch();
		if(dbislock())
			dbunlock();
		return;
	}
	//�����￪ʼ��Ѱ������ֵ
	gotoxy(27,10); printf("�����¼����ֵ:");
	gets(zj);	//����������ֵ
	//��Ӵ�����,�ж��Ƿ��дʼ�¼
	printf("   key_no=%d   ",t[i].key_no);
	for(j=0;j<t[i].re_num;j++)
	{
		if( strcmp( t[i].record[j][t[i].key_no] , zj ) ==0)
			goto findkey1;
	}
	gotoxy(1,20);printf("δ�ҵ��ü�¼,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
	//�ҵ��ü�¼,��ʼ����ɾ������.j���Ǽ�¼���±�,i�Ǳ���±�
findkey1:;
	gotoxy(1,20);
	for(k=0;k<t[i].pro_num;k++)
	{
		printf("\n����������%s��ֵ:",t[i].proname[k]);
		gets(mn);
		//��Ӵ�����,���������������ظ��ȵ�.
		strcpy(t[i].record[j][k],mn);		//������ֵ������Ӧ�ļ�¼����λ��.
				//re_num++
	}

	printf("\n��%s�ɹ��޸���һ����¼!�س�������һ���˵�.",mq);
	getch();
	if(dbislock())
		dbunlock();
}



//---------�����Ĳ���-----------------------------------------------
void indexcreate()
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	clrscr();
	char mn[MAX_TNAME];	//Ϊ�˴������������ı���
	int i,j;
	index inew;
	setcolor(0,14);
	gotoxy(37,7);setcolor(0,14);
	printf("��������");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("������������:");
	gets(mn);
	//��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫɾ���ı��λ��.
			goto eodrp;
		}
	gotoxy(1,20);printf("δ�ҵ��ñ�,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
eodrp:;//�ҵ�������λ��,�������ǽ�����������,��t[i]�ǲ鵽�ı�.
	//����ж�,�Ƿ���Լ�����Ӽ�¼�����ﵽ��ֵ.������
	strcpy(inew.iname,mn);
	inew.key_num=t[i].key_no;
	for(j=0;j<t[i].re_num;j++)
		strcpy(inew.key[j],t[i].record[j][t[i].key_no]);
	gotoxy(1,20);printf("�ڱ�%s�ϳɹ�����������,�س�������һ���˵�.",mn);
	ind[i_num++]=inew;
	getch();
	if(dbislock())
		dbunlock();
}
void indexdrop()
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	if(dbislock())
		dbunlock();
}
void indexshow()			//��ʾ�������ݣ����������������ʾ����ľ�������!!
{
	clrscr();
	char mn[MAX_TNAME];
	int i,j,k;
	gotoxy(37,7);setcolor(0,14);
	printf("��ʾ����");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("������������:");
	gets(mn);
	for(i=0;i<i_num;i++)
		if(strcmp(mn,ind[i].iname)==0)
			goto aas;
	printf("\a\nû���ҵ�������!");
	getchar();
	return;
aas:;
	for(j=0;j<t_num;j++)
		if(strcmp(mn,t[j].tname)==0)
			break;
			cout<<j<<"+";
	gotoxy(1,20);
	printf("\n\n��������:%s   ����������Ϊ:%s\n",ind[i].iname,t[j].key);
	printf("\n-------------------------------------");
	for(k=0;k<=t[j].re_num;k++)
        for(int ji=0;ji<t[j].pro_num;ji++)
		printf("\t%s",t[j].record[k][ji]);
	printf("\n\n��ʾ�����������ļ�¼����\n");
	getchar();

}


//---------��ͼ�Ĳ���-----------------------------------------------
bool IsInView(view *v1,char *proname)
{
	int i;
	for(i=0;i<v1->pro_num;i++)
		if(strcmp(v1->proname[i],proname)==0)
			return true;
	return false;
}

void viewcreate()
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	clrscr();
	char mn[MAX_TNAME];	//Ϊ�˴������������ı���
	char zj[MAX_PROLEN];
	int pnum;
	int i,j,k;
	view vnew;
	gotoxy(37,7);setcolor(0,14);
	printf("������ͼ");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("������ͼ��:");
	gets(vnew.vname);	//������ͼ��
	//��Ӵ�������������ͼ�ȵ�
	gotoxy(27,10); printf("������ͼ��Ӧ�ı���:");
	gets(mn);
	for(i=0;i<t_num;i++)
		if(strcmp(t[i].tname,mn)==0)
			goto sse;
	gotoxy(1,20);printf("û���ҵ��ñ�,��ȷ������\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
sse:;
	//�ҵ� �ñ�i���Ǳ���±�
	strcpy(vnew.tname,mn);
	gotoxy(1,20);
	printf("��������ͼ��ʾ�����Ը���:");
	cin>>pnum;
	vnew.pro_num=pnum;
	//��������������� ����˵�ˡ�
	k=0;
	while(k<pnum)
	{
		printf("\n�������%��������:",k+1);
		gets(zj);
		strcpy(vnew.proname[k],zj);
		for(j=0;j<t[i].pro_num;j++)
			if(strcmp(t[i].proname[j],zj)==0)
				goto asqq;
		printf("\a\nû���ҵ�������.");
		getch();
		if(dbislock())
			dbunlock();
		return;
asqq:;
		vnew.prono[k]=j;
		k++;
	}
	vi[v_num++]=vnew;
	printf("�ɹ���������ͼ%s",vnew.vname);
	getch();
	if(dbislock())
			dbunlock();
}
void viewdrop()
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	clrscr();
	char mn[MAX_TNAME];	//Ϊ�˴������������ı���
	int i;
	gotoxy(37,7);setcolor(0,14);
	printf("������ͼ");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("������ͼ��:");
	gets(mn);	//������ͼ��
	//��Ӵ�������������ͼ�ȵ�
	if(v_num<=0)
	{
		gotoxy(1,20);
		printf("��ͼ��Ϊ0���޷�ɾ����ͼ\a");
	}
	for(i=0;i<v_num;i++)
	{
		if(strcmp(mn,vi[i].vname)==0)
			goto ae;
	}
	printf("\nû���ҵ�����ͼ%s\a",mn);
	getch();
	if(dbislock())
		dbunlock();
	return;
ae:;for(;i<v_num-1;i++)
		vi[i]=vi[i+1];
	v_num--;
	printf("\nɾ����ͼ�ɹ�! ���س�������һ���˵���");
	getch();
	if(dbislock())
		dbunlock();
}
void viewtable()
{
	clrscr();
	char mn[MAX_TNAME];
	int i,j,k,p;
	gotoxy(37,7);setcolor(0,14);
	printf("��ͼ�鿴��");setcolor(0,15);
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("������ͼ��:");
	gets(mn);	//������ͼ��
	//�ж���ͼ�Ƿ����
	for(i=0;i<v_num;i++)
		if(strcmp(vi[i].vname,mn)==0)
			goto val1;
	gotoxy(1,20);printf("�����ڵ���ͼ��!\a");
	getch();
	return;
val1:;//���ָ���ͼ�����Ϊi
	for(j=0;j<t_num;j++)
		if(strcmp(vi[i].tname,t[j].tname)==0)
			break;
	//jΪ�������±�

    gotoxy(1,20);
	printf("\n����:%s   ���Ը���:%d    ����:%s",t[j].tname,t[j].pro_num,t[j].key);
		printf("\n----------------------------------------------------------------\n");
		for(k=0;k<t[j].pro_num;k++)
		{
			if(IsInView(&vi[i],t[j].proname[k]))
				printf("%s\t",t[j].proname[k]);
		}
		printf("\n----------------------------------------------------------------\n");
		for(k=0;k<t[j].re_num;k++)
		{
			for(p=0;p<t[j].pro_num;p++)
				if(IsInView(&vi[i],t[j].proname[p]))
					printf("%s\t",t[j].record[k][p]);
			printf("\n");
		}
	getch();
}


//---------������㣨����SQL��ѯ��-----------------------------------------------
table yununion(table *t1,table *t2)
{
	table tnew;	//�����
	int k,p,m,q;
		if(!IsTableSameType(t1,t2))
	{
		printf("\n�����������������ͬһ�����͵�!\a");
		getchar();
		return NULLTABLE;
	}
//����������T1��ֵ�������
	tnew.key_no=t1->key_no;
	tnew.pro_num=t1->pro_num;
	tnew.re_num=t1->re_num;
	strcpy(tnew.key,t1->key);
	strcpy(tnew.tname,"T");
	for(k=0;k<t1->pro_num;k++)
	{
		strcpy(tnew.proname[k],t1->proname[k]);
		tnew.protype[k]=t1->protype[k];
	}
	for(k=0;k<tnew.re_num;k++)
		for(p=0;p<tnew.pro_num;p++)
			strcpy(tnew.record[k][p],t1->record[k][p]);	//��T���ǽ����

	for(p=0;p<t2->re_num;p++)
		for(q=0;q<t1->re_num;q++)
		{
			if(IsRecEqu(t1,q,t2,p))	//��¼��ȡ�
				break;
			if((q==t1->re_num-1)&&(!IsRecEqu(t1,q,t2,p)))
			{
				for(m=0;m<t1->pro_num;m++)
					strcpy(tnew.record[tnew.re_num][m],t2->record[p][m]);
				tnew.re_num++;
			}
		}
	return tnew;
}
table yunjiao(table *t1,table *t2)
{
	table tnew;
	int m,q,p,k;
	//������룬�����������ͬ���Ĵ�����
	if(!IsTableSameType(t1,t2))
	{
		gotoxy(1,20);
		printf("�����������������ͬһ�����͵�!\a");
		getchar();
		return NULLTABLE;
	}
	tnew.key_no=t1->key_no;
	tnew.pro_num=t1->pro_num;	//ʹ�����T�ı����ͺ�R,S��ͬ
	tnew.re_num=0;		//�ѿռ����������T
	strcpy(tnew.key,t1->key);
	strcpy(tnew.tname,"T");
	for(k=0;k<t1->pro_num;k++)
	{
		strcpy(tnew.proname[k],t1->proname[k]);
		tnew.protype[k]=t1->protype[k];
	}
//��������ʼ��

	for(p=0;p<t2->re_num;p++)
		for(q=0;q<t1->re_num;q++)
		{
			if(IsRecEqu(t1,q,t2,p))	//��¼��ȡ�
			{
				for(m=0;m<t1->pro_num;m++)
					strcpy(tnew.record[tnew.re_num][m],t2->record[p][m]);
				tnew.re_num++;
				break;
			}
		}
	return tnew;
}
table yundiff(table *t1,table *t2)
{
	table tnew;
	int k,m,n,p,q;
		//������룬�����������ͬ���Ĵ�����
	if(!IsTableSameType(t1,t2))
	{
		gotoxy(1,20);
		printf("�����������������ͬһ�����͵�!\a");
		getchar();
		return NULLTABLE;
	}

	tnew.key_no=t1->key_no;
	tnew.pro_num=t1->pro_num;
	tnew.re_num=t1->re_num;
	strcpy(tnew.key,t1->key);
	strcpy(tnew.tname,"T");
	for(k=0;k<t1->pro_num;k++)
	{
		strcpy(tnew.proname[k],t1->proname[k]);
		tnew.protype[k]=t1->protype[k];
	}
	for(k=0;k<tnew.re_num;k++)
		for(p=0;p<tnew.pro_num;p++)
			strcpy(tnew.record[k][p],t1->record[k][p]);	//��T���ǽ����,����������ݾ���R
//��ʼ������ˣ�����
	for(p=0;p<t2->re_num;p++)
		for(q=0;q<tnew.re_num;q++)
		{
			if(IsRecEqu(&tnew,q,t2,p))	//��¼��ȡ�
			{
				for(m=q;m<tnew.re_num;m++)
					for(n=0;n<tnew.pro_num;n++)
						strcpy(tnew.record[m][n],tnew.record[m+1][n]);
				tnew.re_num--;
				break;
			}
		}
	return tnew;
}
table yunshadow(table *t1,char pro[MAX_PRONUM][MAX_PROLEN],int k)
{
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	table *t1			�Ǳ��ָ��
	char pro[][]		��ͶӰ���Ե����ƣ���ά����һά��������ţ��ڶ�ά��������
	int k				��ͶӰ�������Եĸ���
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	table tnew;
	int m,n,p;
	for(m=0;m<=k;m++)
	{
		strcpy(tnew.proname[m],pro[m]);
		//����tnew.protype�Ͳ������ˣ���Ϊʵ���Բ�ǿ�����Ҳ����ȼ۸��ӡ�
	}
	tnew.pro_num=k;
	tnew.key_no=0;
	tnew.re_num=t1->re_num;
	strcpy(tnew.tname,"T");
	for(m=0;m<t1->re_num;m++)		//��¼�ı仯
	{
		for(n=0;n<t1->pro_num;n++)	//���Եı仯
		{
			for(p=0;p<tnew.pro_num;p++)
				if(strcmp(t1->proname[n],tnew.proname[p])==0)
				{
					strcpy(tnew.record[m][p],t1->record[m][n]);
				}	//�ҵ���Ӧ����뵽�±���ȥ
		}
	}
	return tnew;
}
table yunsel0(table *t1,char *sx,int ysf,int shu,char *str)	//���������������ﴫ
{
	table tnew;
	int m,n,k;
	//�ȳ�ʼ��tnew��ʹ��tnew����t1��ͬ����
	tnew.pro_num=t1->pro_num;
	tnew.key_no=t1->key_no;
	tnew.re_num=0;
	strcpy(tnew.tname,"T");
	for(m=0;m<t1->pro_num;m++)
		strcpy(tnew.proname[m],t1->proname[m]);
	//���ж�sx������±��Ƕ��١�
	for(k=0;k<tnew.pro_num;k++)
		if(strcmp(tnew.proname[k],sx)==0)
			break;
	//k����sx���±꣬�������ŷ���!!!
	for(m=0;m<t1->re_num;m++)
	{
		//1.��  2.��  3.��  4.��  5.��  6.��
		if(ysf==1)		//1.��
		{
			if(strcmp(t1->record[m][k],str)==0)
			{
				for(n=0;n<t1->pro_num;n++)
					strcpy(tnew.record[tnew.re_num][n],t1->record[m][n]);
				tnew.re_num++;
			}
		}
		else if(ysf==2)	//2.��
		{
			if(strcmp(t1->record[m][k],str)!=0)
			{
				for(n=0;n<t1->pro_num;n++)
					strcpy(tnew.record[tnew.re_num][n],t1->record[m][n]);
				tnew.re_num++;
			}
		}
		else if(ysf==3)	//3.��
		{
			if(strtonum(t1->record[m][k])<=shu)
			{
				for(n=0;n<t1->pro_num;n++)
					strcpy(tnew.record[tnew.re_num][n],t1->record[m][n]);
				tnew.re_num++;
			}
		}
		else if(ysf==4)	//4.��
		{
			if(strtonum(t1->record[m][k])>=shu)
			{
				for(n=0;n<t1->pro_num;n++)
					strcpy(tnew.record[tnew.re_num][n],t1->record[m][n]);
				tnew.re_num++;
			}
		}
		else if(ysf==5)	//5.��
		{
			if(strtonum(t1->record[m][k])<shu)
			{
				for(n=0;n<t1->pro_num;n++)
					strcpy(tnew.record[tnew.re_num][n],t1->record[m][n]);
				tnew.re_num++;
			}
		}
		else	//ysf==6//6.��
		{
			if(strtonum(t1->record[m][k])>shu)
			{
				for(n=0;n<t1->pro_num;n++)
					strcpy(tnew.record[tnew.re_num][n],t1->record[m][n]);
				tnew.re_num++;
			}
		}
	}
	return tnew;
}


//---------SQL�Ĳ���-----------------------------------------------
//��ѯʱ��and�ж�
table seland(table *t,char *pro1,char o1,char *shu1,char *pro2,char o2,char *shu2)
{
	int shu,ysf;	//�����ͱ���������������
	table tnew1,tnew2;		//�����
	table *tnull=NULL;

	//��Ӵ������ڵ��ñ�����֮ǰ
	//�ҵ�������λ��,��t[i]�ǲ鵽�ı�.    �����١��ܡ��ݡ�������
	switch(o1)
	{
		case '>':ysf=6;break;
		case '<':ysf=5;break;
		case '=':ysf=1;break;
		case '!':ysf=2;break;
	}
	if(ysf>2)
		shu=strtonum(shu1);	//3.��  4.��  5.��  6.������������ֵ�Ƚ�
	tnew1=yunsel0(t,pro1,ysf,shu,shu1);
//****************************************************************
//��Ӵ�����
	switch(o2)
	{
		case '>':ysf=6;break;
		case '<':ysf=5;break;
		case '=':ysf=1;break;
		case '!':ysf=2;break;
	}
	if(ysf>2)
		shu=strtonum(shu2);	//3.��  4.��  5.��  6.������������ֵ�Ƚ�

	tnew2=yunsel0(t,pro2,ysf,shu,shu2);
//***************************************************************
	tnew1=yunjiao(&tnew1,&tnew2);
	return tnew1;
}
//��ѯʱ��or�ж�
table selor(table *t,char *pro1,char o1,char *shu1,char *pro2,char o2,char *shu2)
{
	int shu,ysf;	//�����ͱ���������������
	table tnew1,tnew2;		//�����
	table *tnull=NULL;

	//��Ӵ������ڵ��ñ�����֮ǰ
	//�ҵ�������λ��,��t[i]�ǲ鵽�ı�.    �����١��ܡ��ݡ�������
	switch(o1)
	{
		case '>':ysf=6;break;
		case '<':ysf=5;break;
		case '=':ysf=1;break;
		case '!':ysf=2;break;
	}
	if(ysf>2)
		shu=strtonum(shu1);	//3.��  4.��  5.��  6.������������ֵ�Ƚ�
	tnew1=yunsel0(t,pro1,ysf,shu,shu1);
//****************************************************************
//��Ӵ�����
	switch(o2)
	{
		case '>':ysf=6;break;
		case '<':ysf=5;break;
		case '=':ysf=1;break;
		case '!':ysf=2;break;
	}
	if(ysf>2)
		shu=strtonum(shu2);	//3.��  4.��  5.��  6.������������ֵ�Ƚ�

	tnew2=yunsel0(t,pro2,ysf,shu,shu2);
//***************************************************************
	tnew1=yununion(&tnew1,&tnew2);
	return tnew1;
}

void relationcalculate(char *large)
{
    if(currentuser.r_select==0){
        cout<<"�Բ�����û�д�Ȩ��"<<endl;
        return;
    }
    table t1;
    //����select sno,sage from stu where sno<0003 and sage!=22;\n\n\n
	char tname[MAX_TNAME];
	char pro[MAX_PRONUM][MAX_PROLEN];	//��ҪͶӰ���������Ƽ���
	char pro1[MAX_PROLEN];
	char pro2[MAX_PROLEN];
	char shu1[8],shu2[8];
	char o1,o2;
	int i,j,k=0;	//�����ͱ���������������

	//*******************************������from
	//�Ժ������������������from where��
	int p;
	for(p=0;p<(int)strlen(large);p++)
	{
		if(strcmp(substr(large,p,4),"from")==0)
			break;
		if(strcmp(substr(large,p,4),"from")!=0 && p>(int)strlen(large)-4)
		{
			printf("��ʽ����,û�з���from�ؼ���\n");
			getch();
			return;
		}
	}



	//******************************
	for(j=0;j<MAX_PRONUM;j++)
		strcpy(pro[j]," ");
	i=7;		//��select ���һ����' '��ʼ
	j=0;
	//select sno,sage from stu where sno=0001 and sage=22;
	//select sno,sage from stu where sno!=0003 and sage!=22;
	//select * from stu
//************************���￪ʼ����************************************
	int secret=0;
	if(large[i]=='*')
	{
		secret=1;
		i+=7;
	}
	else
	{
		while(large[i]!=' ')
		{
			while(large[i]!=',' && large[i]!=' ')
				pro[k][j++]=large[i++];	//��pro1��ֵ
			pro[k][j]='\0';
			j=0;
			if(large[i]==',')
				i++;
			k++;
		}
		i+=6;
	}
	//Ŀǰi��λ����tname��0�±���
	while((large[i]!=' '))
		tname[j++]=large[i++];	//��tname��ֵ
	tname[j]='\0';
	if(secret==1)
	{
		int p,q;
		for(p=0;p<t_num;p++)
		{
			if(strcmp(tname,t[p].tname)==0)
				break;
		}
		t1=t[p];
        goto qwkl;
		for(q=0;q<t[p].pro_num;q++)
			{strcpy(pro[q],t[p].proname[q]);}
	}
	j=0;
	i+=7;		//Ŀǰi��λ����pro1��0�±���
	while(large[i]!='>' && large[i]!='=' && large[i]!='<'&& large[i]!='!')
		pro1[j++]=large[i++];	//��pro1��ֵ
	pro1[j]='\0';
	j=0;
	o1=large[i++];	//Ŀǰi��o1��,��o1��ֵ
	if(large[i]=='=')
		i++;
	while(large[i]!=' ')
		shu1[j++]=large[i++];	//��shu1��ֵ
	shu1[j]='\0';
	j=0;			//i��' '��
	i++;
	char andor[4];
	while(large[i]!=' ')
		andor[j++]=large[i++];
	andor[j]='\0';
	j=0;
	i++;
	while(large[i]!='>' && large[i]!='=' && large[i]!='<' && large[i]!='!')
		pro2[j++]=large[i++];	//��pro2��ֵ
	pro2[j]='\0';
	j=0;
	//i��o2��
	o2=large[i++];	//Ŀǰi��o2��,��o2��ֵ
	if(large[i]=='=')
		i++;
	while(large[i]!=';')
		shu2[j++]=large[i++];	//��shu2��ֵ
	shu2[j]='\0';
	j=0;			//i��';'��
//************************************************************
	//�����Ͱ�һ�����large�ַ�����ֿ���.

	for(i=0;i<t_num;i++)
		if(strcmp(tname,t[i].tname)==0)
		{
			//i��ֵ���Ǳ��λ��.
			goto eodrp;
		}
	printf("δ�ҵ���,��ȷ������.\n");
	getchar();
	return;
eodrp:;//�ҵ�������λ��,��t[i]�ǲ鵽�ı�.

	if(strcmp(andor,"and")==0)
		t1=seland(&t[i],pro1,o1,shu1,pro2,o2,shu2);
	else if(strcmp(andor,"or")==0)
		t1=selor(&t[i],pro1,o1,shu1,pro2,o2,shu2);
	else
		printf("\n\n error!!!\a\a");
	t1=yunshadow(&t1,pro,k);
qwkl:;
	tableprint(&t1);
}


//---------��SQL�����ж�-----------------------------------------------
        //������create table students
        //      (Sno char primary key,Sname char unique,Sage int)
void sqlcreate(char *sql){
    if(strcmp(substr(sql,7,5),"table")==0)
    {

        char mn[MAX_TNAME];
        char ck[MAX_PROLEN];
        table tnew;

        int i,j,k;
        int nnum=0;
        int ii,jj=0;
        for(ii=0; sql[ii]!='\0'; ii++)
        {
            if((sql[ii]=='(')||(sql[ii]==',')||(sql[ii]==' ')) {sql[ii]=' ';jj++;}
        }
        nnum=(jj-3+1)/2;
        istringstream ss(sql);
        char tmp[10];
        ss>>tmp;ss>>tmp;ss>>mn;
          if(CURRENTREVOKE==0)
        {
            printf("\a�Բ���,����Ȩ�޲������������.�����������...");
            getch();
            return;
        }
        if(!dbislock())		//û�������Ǿͼ���
            dblock();	//dblock();
        else
        {
            clrscr();
            printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
            getch();
            return;
        }

        //������������������
        strcpy(tnew.tname,mn);	//�±����


        //��Ӵ�����
        if(nnum>=MAX_TNUM)
        {
            printf("�Ѿ��ﵽ���������,�޷�����\a!");
            goto eotc1;
        }
        tnew.pro_num=nnum;
        //��ʼѭ��������������;
        //ע������������

        i=0;
        char mmm[MAX_TNAME];
        while(i<tnew.pro_num)
        {
            ss>>mmm;
            //��������!!!!!
            strcpy(tnew.proname[i],mmm);
            //������,�����ϵ�����Ҫ�ų���!
            ss>>tnew.protype;
            i++;
        }

        //��������,������Ψһ��ʾһ����¼������,����Ҫ������.������λ����tnew.key,�����������е��±�.
        printf("����������������,����������Զ�ѡ���һ������:");
        gets(ck);
        for(i=0; i<tnew.pro_num; i++)
        {
            if(strcmp(ck,tnew.proname[i])==0)
            {
                tnew.key_no=i;
                strcpy(tnew.key,ck);
                goto nex1;
            }
        }
        printf("\n�������,�����Զ�ѡ��Ϊ��һ������:\a%s",tnew.proname[0]);
        strcpy(tnew.key,tnew.proname[0]);
        tnew.key_no=0;
nex1:
        ;
        tnew.re_num=0;		//��¼������Ϊ��
        for(i=0; i<MAX_RECNUM; i++)
            for(j=0; j<MAX_PRONUM; j++)
                for(k=0; k<MAX_PROLEN; k++)
                    tnew.record[i][j][k]='\0';	//��ʼ����¼��
        t[t_num++]=tnew;
        printf("����ɹ�\n");
        getch();
eotc1:
        ;
        dbunlock();		//����
        fprintf(alfp,"\n[%d��%d��%d�� %d:%d:%d]ִ�����²���:\n",GetCurrentYear(),GetCurrentMonth(),GetCurrentDay(),
                GetCurrentHour(),GetCurrentMinute(),GetCurrentSecond() );
        fprintf(alfp,"�û�������ɹ�,���������:����:%s,�����Ը���:%d,����:%s.",tnew.tname,
                tnew.pro_num,tnew.key);
        fputc('\n',alfp);
    }
    if(strcmp(substr(sql,7,5),"index")==0){
        char mn[MAX_TNAME];	//
	    int i,j;
        index inew;

        int nnum=0;
        int ii,jj=0;
        for(ii=0; sql[ii]!='\0'; ii++)
        {
            if((sql[ii]=='(')||(sql[ii]==',')||(sql[ii]==' ')) {sql[ii]=' ';jj++;}
        }
        nnum=(jj-5)/2;
        istringstream ss(sql);
        char tmp[10];
        char mm2[10];
        ss>>tmp;ss>>tmp;ss>>mn;ss>>tmp;ss>>mm2;

        if(CURRENTREVOKE==0)
        {
            printf("\a�Բ���,����Ȩ�޲������������.�����������...");
            getch();
            return;
        }
        if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            dblock();	//dblock();     dbunlock();
        else
        {
            printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
            printf("\n�����������...");
            getch();
            return;
        }


        //��Ӵ�����
        for(i=0; i<t_num; i++)
            if(strcmp(mm2,t[i].tname)==0)
            {
                //i��ֵ����Ҫ��λ��.
                goto eodrp;
            }
        printf("δ�ҵ��ñ�,��ȷ������.\a");
        getch();
        if(dbislock())
            dbunlock();
        return;
eodrp:;//�ҵ�������λ��,�������ǽ�����������,��t[i]�ǲ鵽�ı�.
        //����ж�,�Ƿ���Լ�����Ӽ�¼�����ﵽ��ֵ.������
        strcpy(inew.iname,mn);
        inew.key_num=t[i].key_no;
        for(j=0; j<t[i].re_num; j++)
            strcpy(inew.key[j],t[i].record[j][t[i].key_no]);
        printf("�ڱ�%s�ϳɹ�����������%s\n",mm2,mn);
        ind[i_num++]=inew;
        getch();
        if(dbislock())
            dbunlock();

    }
    if(strcmp(substr(sql,7,4),"view")==0){

        char mn[MAX_TNAME];	//Ϊ�˴������������ı���
	    char zj[MAX_PROLEN];
        int pnum;
        int i,j,k;
        view vnew;

        int ii,jj=0;
        for(ii=0; sql[ii]!='\0'; ii++)
        {
            if((sql[ii]=='(')||(sql[ii]==',')||(sql[ii]==' ')||(sql[ii]==';')) {sql[ii]=' ';jj++;}
            if(sql[ii]=='*'||(sql[ii]==';')) sql[ii]==' ';
        }

        istringstream ss(sql);
        char tmp[10];
        char mm2[10];
        ss>>tmp;ss>>tmp;ss>>vnew.vname;ss>>tmp;ss>>tmp;ss>>mm2;ss>>tmp;ss>>mn;
        cout<<mn<<"+";

        if(CURRENTREVOKE==0)
	{
		printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");

		getch();
		return;
	}



	for(i=0;i<t_num;i++)
		if(strcmp(t[i].tname,mn)==0)
			goto sse;
	printf("û���ҵ��ñ�,��ȷ������\n");
	getch();
	if(dbislock())
		dbunlock();
	return;
sse:;
	//�ҵ� �ñ�i���Ǳ���±�

	pnum=t[i].pro_num;
	vnew.pro_num=pnum;
	strcpy(vnew.tname,t[i].tname);
	//��������������� ����˵�ˡ�
	k=0;
	while(k<pnum)
	{
		strcpy(vnew.proname[k],t[i].proname[k]);
		vnew.prono[k]=k;
		k++;
	}
	vi[v_num++]=vnew;
	printf("�ɹ���������ͼ%s\n",vnew.vname);
	getch();
	if(dbislock())
			dbunlock();
    }
}
void sqlinsert(char *sql){
    if(currentuser.r_insert==0){
        cout<<"�Բ�����û�д�Ȩ��"<<endl;
        return;
    }
    char mn[MAX_TNAME];
	char mq[MAX_TNAME];
	int i,j;
	int nnum;
    int ii,jj=0;
        for(ii=0; sql[ii]!='\0'; ii++)
        {
            if((sql[ii]=='(')||(sql[ii]==',')||(sql[ii]==' ')||(sql[ii]==')')) {sql[ii]=' ';jj++;}
        }
        nnum=(jj-3)/2;
        istringstream ss(sql);
        char tmp[10];
        ss>>tmp;ss>>tmp;ss>>mn;ss>>tmp;

    if(CURRENTREVOKE==0)
	{
		printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}

	//��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫ����ı��λ��.
			goto eodrp;
		}
	gotoxy(1,20);printf("δ�ҵ�Ҫ����ı�,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
eodrp:;//�ҵ�������λ��,�������ǲ����¼����,��t[i]�ǲ鵽�ı�.
	//����ж�,�Ƿ���Լ�����Ӽ�¼�����ﵽ��ֵ.������
	strcpy(mq,mn);		//������������õ�mq����
	if(t[i].re_num>=MAX_RECNUM)
	{
		printf("�ñ��Ѿ��ﵽ��¼�����ֵ!�޷�����.\a");
		getch();
		if(dbislock())
		dbunlock();
		return;
	}
	char mnn[10];
	for(j=0;j<t[i].pro_num;j++)
	{
		t[i].proname[j];
		ss>>mnn;
		//��Ӵ�����,���������������ظ��ȵ�.
		strcpy(t[i].record[t[i].re_num][j],mnn);		//������ֵ������Ӧ�ļ�¼����λ��.
				//re_num++
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	t[i].re_num++;

	printf("��%s�Ѿ����ɹ��ز�����һ���¼�¼.\n",mq);

	fprintf(alfp,"\n[%d��%d��%d�� %d:%d:%d]ִ�����²���:\n",GetCurrentYear(),GetCurrentMonth(),GetCurrentDay(),
                GetCurrentHour(),GetCurrentMinute(),GetCurrentSecond() );
        fprintf(alfp,"�û��������ݳɹ�,������:��%s,�����Ը���:%d,����:%s.",t[i].tname,
                t[i].pro_num,t[i].key);
        fputc('\n',alfp);
	getch();
	if(dbislock())
		dbunlock();
}
void sqldelete(char *sql){
    if(currentuser.r_delete==0){
        cout<<"�Բ�����û�д�Ȩ��"<<endl;
        return;
    }

    int nnum;
    int ii,jj=0;

    if(CURRENTREVOKE==0)
	{
		printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	char mn[MAX_TNAME];
	char mq[MAX_TNAME];
	char zj[MAX_PROLEN];
	int i,j,k;

        for(ii=0; sql[ii]!='\0'; ii++)
        {
            if((sql[ii]=='=')||(sql[ii]=='(')||(sql[ii]==',')||(sql[ii]==' ')||(sql[ii]==')')) {sql[ii]=' ';jj++;}
        }
        nnum=(jj-3)/2;
        istringstream ss(sql);
        char tmp[10];
        char sno[10];char snober[10];
        ss>>tmp;ss>>tmp;ss>>mn;ss>>tmp;ss>>sno;ss>>snober;


     //��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫɾ���ı��λ��.
			goto eodrd;
		}
	gotoxy(1,20);printf("δ�ҵ�Ҫɾ����¼�ı�,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
eodrd:;
	//�ҵ�������λ��,�������ǲ����¼����,��t[i]�ǲ鵽�ı�.
	//����ж�,�Ƿ���Լ�����Ӽ�¼�����ﵽ��ֵ.������
	strcpy(mq,mn);		//������������õ�mq����
	if(t[i].re_num<=0)
	{
		printf("�ñ��޼�¼!�޷�ɾ��.\a");
		getch();
		if(dbislock())
			dbunlock();
		return;
	}

	//��Ӵ�����,�ж��Ƿ��дʼ�¼
	int jjj;
	for(j=0;j<t[i].re_num;j++)
	{
		for(jjj=0;jjj<t[i].pro_num;jjj++)
		if(( strcmp( t[i].record[j][jjj],snober)==0))
			goto findkey1;
	}
	gotoxy(1,20);printf("δ�ҵ����ʼ�¼\n");

	fprintf(alfp,"\n[%d��%d��%d�� %d:%d:%d]ִ�����²���:\n",GetCurrentYear(),GetCurrentMonth(),GetCurrentDay(),
                GetCurrentHour(),GetCurrentMinute(),GetCurrentSecond() );
        fprintf(alfp,"�û�ɾ�����ݳɹ�,������:��%s,�����Ը���:%d,����:%s.",t[i].tname,
                t[i].pro_num,t[i].key);
        fputc('\n',alfp);

	getch();
	if(dbislock())
		dbunlock();
	return;
findkey1:;
	//�ҵ��ü�¼,��ʼ����ɾ������.j����Ҫɾ���ļ�¼���±�
	for(;j<t[i].re_num-1;j++)
	{
		for(k=0;k<t[i].pro_num;k++)
			strcpy(t[i].record[j][k],t[i].record[j+1][k]);
	}
	t[i].re_num--;
	printf("\n��%s�Ѿ����ɹ���ɾ����һ����¼\n",mq);
	getch();
	if(dbislock())
		dbunlock();


}
void sqldrop(char *sql){
    if(strcmp(substr(sql,5,5),"table")==0){

        char mn[MAX_TNAME];
        int i,j;
        table tdel;
        istringstream ss(sql);
        char tmp[10];
        ss>>tmp;
        ss>>tmp;
        ss>>mn;
        if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}



	//��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫɾ���ı��λ��.
			tdel=t[i];
			for(j=i;j<t_num-1;j++)
				t[j]=t[j+1];
			t_num--;
			goto eodrp;
		}
	gotoxy(1,20);printf("δ�ҵ�Ҫɾ���ı�\n");
	getchar();
	dbunlock();
	return;
eodrp:;
	printf("��%s���ɹ���ɾ��\n",mn);
	getch();
	dbunlock();
		fprintf(alfp,"\n[%d��%d��%d�� %d:%d:%d]ִ�����²���:\n",GetCurrentYear(),GetCurrentMonth(),GetCurrentDay(),
									GetCurrentHour(),GetCurrentMinute(),GetCurrentSecond() );
	fprintf(alfp,"�û��ɹ�ɾ��һ����,���������:����:%s,�����Ը���:%d,����:%s.",tdel.tname,
								tdel.pro_num,tdel.key);fputc('\n',alfp);
    }
    if(strcmp(substr(sql,5,5),"index")==0){

        char mn[MAX_TNAME];	//Ϊ�˴������������ı���
        int i;
        istringstream ss(sql);
        char tmp[10];
        ss>>tmp;
        ss>>tmp;
        ss>>mn;
        if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}



	//��Ӵ�������������ͼ�ȵ�
	if(i_num<=0)
	{

		printf("������Ϊ0���޷�ɾ������\a");
	}
	for(i=0;i<i_num;i++)
	{
		if(strcmp(mn,ind[i].iname)==0)
			goto be;
	}
	printf("\nû���ҵ�������%s\n",mn);
	getch();
	if(dbislock())
		dbunlock();
	return;
be:;
    for(;i<i_num-1;i++)
		ind[i]=ind[i+1];
	i_num--;
	printf("\nɾ�������ɹ�\n");
	getch();
	if(dbislock())
		dbunlock();
    }
    if(strcmp(substr(sql,5,4),"view")==0){
        char mn[MAX_TNAME];	//Ϊ�˴������������ı���
        int i;
        istringstream ss(sql);
        char tmp[10];
        ss>>tmp;
        ss>>tmp;
        ss>>mn;
        if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}



	//��Ӵ�������������ͼ�ȵ�
	if(v_num<=0)
	{

		printf("��ͼ��Ϊ0���޷�ɾ����ͼ\a");
	}
	for(i=0;i<v_num;i++)
	{
		if(strcmp(mn,vi[i].vname)==0)
			goto ae;
	}
	printf("\nû���ҵ�����ͼ%s\a",mn);
	getch();
	if(dbislock())
		dbunlock();
	return;
ae:;
    for(;i<v_num-1;i++)
		vi[i]=vi[i+1];
	v_num--;
	printf("\nɾ����ͼ�ɹ�\n");
	getch();
	if(dbislock())
		dbunlock();
    }

}
void sqlupdate(char *sql){
    if(currentuser.r_update==0){
        cout<<"�Բ�����û�д�Ȩ��"<<endl;
        return;
    }
	char mn[MAX_TNAME];
	char mq[MAX_TNAME];
	char zj[MAX_PROLEN];
	int i,j,k;
    int nnum;
    int ii,jj=0;
        for(ii=0; sql[ii]!='\0'; ii++)
        {
            if((sql[ii]==';')||(sql[ii]=='=')||(sql[ii]=='(')||(sql[ii]==',')||(sql[ii]==' ')||(sql[ii]==')')) {sql[ii]=' ';jj++;}
        }
        nnum=(jj-3)/2;
        istringstream ss(sql);
        char tmp[10];
        char sno[10];
        char snober[10];
        char sageber[10];
        char sage[10];
        ss>>tmp;ss>>mn;ss>>tmp;
        ss>>sage;ss>>sageber;ss>>tmp;ss>>sno;ss>>snober;


    if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}


	//��Ӵ�����
	for(i=0;i<t_num;i++)
		if(strcmp(mn,t[i].tname)==0)
		{
			//i��ֵ����Ҫ�ı��λ��.
			goto eodrp;
		}
	printf("δ�ҵ�Ҫ���ĵı�,��ȷ������.\n");
	getch();
	if(dbislock())
		dbunlock();
	return;
eodrp:;//�ҵ�������λ��,�������ǲ����¼����,��t[i]�ǲ鵽�ı�.
	//����ж�,�Ƿ���Լ�����Ӽ�¼�����ﵽ��ֵ.������
	strcpy(mq,mn);		//������������õ�mq����
	if(t[i].re_num<=0)
	{
		printf("�ñ��¼ֵΪ0!�޷�����.\n");
		getch();
		if(dbislock())
			dbunlock();
		return;
	}
	int jjj;
	//��Ӵ�����,�ж��Ƿ��дʼ�¼
	for(j=0;j<t[i].re_num;j++)
	{
		for(jjj=0;jjj<t[i].pro_num;jjj++)
		if( strcmp( t[i].record[j][jjj] , snober ) ==0)
			goto findkey1;
	}
	printf("δ�ҵ��ü�¼,��ȷ������.\a");
	getch();
	if(dbislock())
		dbunlock();
	return;
	//�ҵ��ü�¼,��ʼ����ɾ������.j���Ǽ�¼���±�,i�Ǳ���±�
findkey1:;

	for(k=0;k<t[i].pro_num;k++)
	{

		if( strcmp( t[i].proname[k] , sage ) ==0)
		strcpy(t[i].record[j][k],sageber);		//������ֵ������Ӧ�ļ�¼����λ��.
	}

	printf("��%s�ɹ��޸���һ����¼\n",mq);
	getch();
	if(dbislock())
		dbunlock();
}
void sqlgrant(char *sql){
    if(CURRENTREVOKE==0)
	{
		printf("\a�Բ���,����Ȩ�޲������������.\n");
		getch();
		return;
	}
    istringstream ss(sql);
    char tmp[10];
    char ri[10];char riuser[10];
    ss>>tmp;ss>>ri;ss>>tmp;ss>>tmp;ss>>tmp;ss>>tmp;ss>>riuser;
    if(strcmp(ri,"select")==0){
        for(int i=0;i<u_num;i++){
            if(strcmp(u[i].username,riuser)==0) {u[i].r_select=1;cout<<"��Ȩ�ɹ�"<<endl;}
        }
    }
    else if(strcmp(ri,"update")==0){
        for(int i=0;i<u_num;i++){
            if(strcmp(u[i].username,riuser)==0) {u[i].r_update=1;cout<<"��Ȩ�ɹ�"<<endl;}
        }
    }
    else if(strcmp(ri,"delete")==0){
        for(int i=0;i<u_num;i++){
            if(strcmp(u[i].username,riuser)==0) {u[i].r_delete=1;cout<<"��Ȩ�ɹ�"<<endl;}
        }
    }
    else if(strcmp(ri,"insert")==0){
        for(int i=0;i<u_num;i++){
            if(strcmp(u[i].username,riuser)==0) {u[i].r_insert=1;cout<<"��Ȩ�ɹ�"<<endl;}
        }
    }
}
void sqlrevoke(char *sql){
    if(CURRENTREVOKE==0)
	{
		printf("\a�Բ���,����Ȩ�޲������������.\n");
		getch();
		return;
	}
    istringstream ss(sql);
    char tmp[10];
    char ri[10];char riuser[10];
    ss>>tmp;ss>>ri;ss>>tmp;ss>>tmp;ss>>tmp;ss>>tmp;ss>>riuser;
    if(strcmp(ri,"select")==0){
        for(int i=0;i<u_num;i++){
            if(strcmp(u[i].username,riuser)==0) {u[i].r_select=0;cout<<"����Ȩ���ɹ�"<<endl;}
        }
    }
    else if(strcmp(ri,"update")==0){
        for(int i=0;i<u_num;i++){
            if(strcmp(u[i].username,riuser)==0) {u[i].r_update=0;cout<<"����Ȩ���ɹ�"<<endl;}
        }
    }
    else if(strcmp(ri,"delete")==0){
        for(int i=0;i<u_num;i++){
            if(strcmp(u[i].username,riuser)==0) {u[i].r_delete=0;cout<<"����Ȩ���ɹ�"<<endl;}
        }
    }
    else if(strcmp(ri,"insert")==0){
        for(int i=0;i<u_num;i++){
            if(strcmp(u[i].username,riuser)==0) {u[i].r_insert=0;cout<<"����Ȩ���ɹ�"<<endl;}
        }
    }
}
//---------������ָ�-----------------------------------------------
void BackUp()
{
	if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	char filename[20];
	clrscr();
	PrintColorStringXY(36,7,0,14,0,15,"���ݵı���");
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("���뱸�����ݵ��ļ���:");
	gotoxy(27,10);
	gets(filename);
	FILE *fp;
	if((fp=fopen(filename,"wb"))==NULL)
		fp=fopen(filename,"wb+");
	//���¹�6���ļ��ṩȫ��������dblock.db,table.db,index.db,view.db,dic.db,log.txt
//*****************��dblock.db�ı���********************************************
	int databaselock;
	if(dbislock())
		databaselock=1;
	else
		databaselock=0;
	fwrite(&databaselock,sizeof(int),1,fp);
	PrintStringXY(1,24,"����dblock.db\a..   ");
	Sleep(300);
//******************��table.db�ı���********************************************
	fwrite(&t_num,sizeof(int),1,fp);
	fwrite(t,sizeof(table),MAX_TNUM,fp);
	PrintStringXY(1,24,"����table.db\a..   ");
	Sleep(300);
//******************��index.db�ı���********************************************
	fwrite(&i_num,sizeof(int),1,fp);
	fwrite(ind,sizeof(index),MAX_TNUM,fp);
	PrintStringXY(1,24,"����index.db\a..   ");
	Sleep(300);
//******************��view.db�ı���**********************************************
	fwrite(&v_num,sizeof(int),1,fp);
	fwrite(vi,sizeof(view),MAX_VIEWNUM,fp);
	PrintStringXY(1,24,"����view.db\a..   ");
	Sleep(300);
//******************��dic.db�ı���***********************************************
	//dic.db�������ֵ䣬����Ҫ���ݣ�ֻ���ڻָ���ʱ���table.db�зֽ��������.
//******************��log.txt�ı���***********************************************
	FILE *fp1;
	char ch;
	if((fp1=fopen("log.txt","rb"))==NULL)
	{
		printf("��������־�ļ�log.txt,���α���δ��ȫ�ɹ�.\a");
		fp1=fopen("log.txt","wb+");
		fclose(fp1);
		fclose(fp);
		if(dbislock())
			dbunlock();
		return;
	}
	ch=fgetc(fp1);
	while(ch!=EOF)
	{
		fputc(ch,fp);
		ch=fgetc(fp1);

	}
	fclose(fp1);
	fclose(fp);
	PrintStringXY(1,24,"����log.txt\a..   ");
	Sleep(300);
	PrintStringXY(1,24,"��ϲ�����������ѳɹ�������");
	printf("%s",filename);
	printf("\n�����������...");
	getch();
	if(dbislock())
		dbunlock();
}
void ReCovery()
{
	//��ĳ��ָ���ı��ݽ��лָ�
		if(CURRENTREVOKE==0)
	{
		gotoxy(1,24);printf("\a�Բ���,����Ȩ�޲������������.�����������...");
		getch();
		return;
	}
	if(!dbislock())		//û�������Ǿͼ���@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		dblock();	//dblock();     dbunlock();
	else
	{
		clrscr();
		printf("���ݿ⴦����״̬����ʹ�����û�Ҳ���ܽ����޸�!\a");
		printf("\n�����������...");
		getch();
		return;
	}
	char filename[20];
	clrscr();
	PrintColorStringXY(36,7,0,14,0,15,"���ݵĻָ�");
	DrawFrame(20,60,5,14);
	gotoxy(27,9); printf("����ָ����ݵ��ļ���:");
	gotoxy(27,10);
	gets(filename);
	FILE *fp;
	FILE *fp2;
	if((fp=fopen(filename,"rb"))==NULL)
	{
		PrintStringXY(1,24,"δ���ִ˱����ļ�!�����������...\a");
		getch();
		if(dbislock())
		dbunlock();
		return;
	}
	//�����ļ��ṩȫ���ȡdblock.db,table.db,index.db,view.db,log.txt
//*****************��dblock.db�Ļָ�********************************************
	int databaselock;
	fread(&databaselock,sizeof(int),1,fp);
	if((fp2=fopen("dblock.db","wb"))==NULL)
		fp2=fopen("dblock.db","wb+");
	fwrite(&databaselock,sizeof(int),1,fp2);
	fclose(fp2);
	PrintStringXY(1,24,"�ָ�dblock.db\a..   ");
	Sleep(300);
//******************��table.db�Ļָ�********************************************
	fread(&t_num,sizeof(int),1,fp);
	fread(t,sizeof(table),MAX_TNUM,fp);
	if((fp2=fopen("table.db","wb"))==NULL)
		fp2=fopen("table.db","wb+");
	fwrite(&t_num,sizeof(int),1,fp2);
	fwrite(t,sizeof(table),MAX_TNUM,fp2);
	fclose(fp2);
	PrintStringXY(1,24,"�ָ�table.db\a..   ");
	Sleep(300);
//******************��index.db�Ļָ�********************************************
	fread(&i_num,sizeof(int),1,fp);
	fread(ind,sizeof(index),MAX_TNUM,fp);
	if((fp2=fopen("index.db","wb"))==NULL)
		fp2=fopen("index.db","wb+");
	fwrite(&i_num,sizeof(int),1,fp2);
	fwrite(ind,sizeof(index),MAX_TNUM,fp2);
	fclose(fp2);
	PrintStringXY(1,24,"�ָ�index.db\a..   ");
	Sleep(300);
//******************��view.db�Ļָ�**********************************************
	fread(&v_num,sizeof(int),1,fp);
	fread(vi,sizeof(view),MAX_VIEWNUM,fp);
	if((fp2=fopen("view.db","wb"))==NULL)
		fp2=fopen("view.db","wb+");
	fwrite(&v_num,sizeof(int),1,fp2);
	fwrite(vi,sizeof(view),MAX_VIEWNUM,fp2);
	fclose(fp2);
	PrintStringXY(1,24,"�ָ�view.db\a..   ");
	Sleep(300);
//******************��dic.db�Ļָ�***********************************************
	//dic.db�������ֵ䣬����Ҫ���ݣ�ֻ���ڻָ���ʱ���table.db�зֽ��������.
	int i,j;
	if((fp2=fopen("dic.db","wb"))==NULL)
		fp2=fopen("dic.db","wb+");
	for(i=0;i<t_num;i++)
	{
		fprintf(fp2,"����:%s   ��������:%d   ����:%s",t[i].tname,t[i].pro_num,t[i].key);
		fprintf(fp2,"\n----------------------------------------------------\n");
		for(j=0;j<t[i].pro_num;j++)
			fprintf(fp2,"%s(%c)    ",t[i].proname[j],t[i].protype[j]);
		fprintf(fp,"\n\n\n");
	}
	fclose(fp2);
	PrintStringXY(1,24,"�ָ�dic.db\a..   ");
	Sleep(300);
//******************��log.txt�ı���***********************************************
	char ch;
	if((fp2=fopen("log.txt","wb"))==NULL)
		fp2=fopen("dic.db","wb+");
	ch=fgetc(fp);
	while(ch!=EOF)
	{
		fputc(ch,fp2);
		ch=fgetc(fp);
	}
	fclose(fp2);
	fclose(fp);
	PrintStringXY(1,24,"�ָ�log.txt\a..   ");
	Sleep(300);
	PrintStringXY(1,24,"��ϲ�����������ѳɹ��ָ���");
	printf("%s",filename);
	printf("\n�����������...");
	getch();
	if(dbislock())
		dbunlock();
}
void filewrite()
{
	FILE *fp;
//*****************��ȡconfig.db����
	if((fp=fopen("config.db","wb"))==NULL)
		fp=fopen("config.db","wb+");
	fwrite(&speedfactor,sizeof(int),1,fp);
	fclose(fp);
//д��table.db�Ĵ���
	if((fp=fopen("table.db","wb"))==NULL)
	{
		printf("������table.db�ļ�,�������ʹ��.\a");
		getchar();
		exit(-1);
	}
	fwrite(&t_num,sizeof(int),1,fp);
	fwrite(t,sizeof(table),MAX_TNUM,fp);
	fclose(fp);
//������д��dic.db�Ĵ���
	int i,j;
	if((fp=fopen("dic.db","wb"))==NULL)
	{
		printf("������dic.db�ļ�,�������ʹ��.\a");
		getchar();
		exit(-1);
	}
	for(i=0;i<t_num;i++)
	{
		fprintf(fp,"����:%s   ��������:%d   ����:%s",t[i].tname,t[i].pro_num,t[i].key);
		fprintf(fp,"\n----------------------------------------------------\n");
		for(j=0;j<t[i].pro_num;j++)
			fprintf(fp,"%s(%c)    ",t[i].proname[j],t[i].protype[j]);
		fprintf(fp,"\n\n\n");
	}
	fclose(fp);
//������д��view.db�Ĵ���
	if((fp=fopen("view.db","wb"))==NULL)
	{
		printf("������view.db�ļ�,�������ʹ��.\a");
		getchar();
		exit(-1);
	}
	fwrite(&v_num,sizeof(int),1,fp);
	fwrite(vi,sizeof(view),MAX_VIEWNUM,fp);
	fclose(fp);
//������д��index.db�Ĵ���
	if((fp=fopen("index.db","wb"))==NULL)
	{
		printf("������index.db�ļ�,�������ʹ��.\a");
		getchar();
		exit(-1);
	}
	fwrite(&i_num,sizeof(int),1,fp);
	fwrite(ind,sizeof(index),MAX_TNUM,fp);
	fclose(fp);
}


int main()
{
    if((alfp=fopen("log.txt","a"))==NULL)
		alfp=fopen("log.txt","w+");


	fprintf(alfp,"*******************************************");fputc('\n',alfp);
	fprintf(alfp,"\n[%d��%d��%d�� %d:%d:%d]ִ�����²���:\n",GetCurrentYear(),GetCurrentMonth(),GetCurrentDay(),
									GetCurrentHour(),GetCurrentMinute(),GetCurrentSecond() );
	fprintf(alfp,"��ʼ��������Ϣ.");fputc('\n',alfp);
	initialize();//��ʼ����

	login();//�û���½
	dbunlock();

	char sql[100];
	clrscr();
    gotoxy(1,8);

	while(1){
        //clrscr();
        cout<<"SQL> ";
        gets(sql);

    if(strcmp(sql,"exit")==0) break;
    else if(strcmp(sql,"su")==0) login();
    else if(strcmp(substr(sql,0,6),"select")==0){
        relationcalculate(sql);
    }
    else if(strcmp(substr(sql,0,6),"create")==0){
        sqlcreate(sql);
	}
	else if(strcmp(substr(sql,0,6),"insert")==0){
        sqlinsert(sql);
	}
	else if(strcmp(substr(sql,0,6),"delete")==0){
        sqldelete(sql);
	}
	else if(strcmp(substr(sql,0,6),"update")==0){
        sqlupdate(sql);
	}
	else if(strcmp(substr(sql,0,4),"drop")==0){
        sqldrop(sql);
	}
	else if(strcmp(substr(sql,0,5),"grant")==0){
        sqlgrant(sql);
	}
	else if(strcmp(substr(sql,0,6),"revoke")==0){
        sqlrevoke(sql);
	}

	else if(strcmp(sql,"help database")==0){
        outputrec();
        indexshow();
        viewtable();
        clrscr();
	}
	else if(strcmp(sql,"help table")==0){
        outputrec();
        clrscr();
	}
	else if(strcmp(sql,"help view")==0){
        viewtable();
        clrscr();
	}
	else if(strcmp(sql,"help index")==0){
        indexshow();
        clrscr();
	}
	else if(strcmp(sql,"����")==0){
        filewrite();
        BackUp();
        clrscr();
	}
	else if(strcmp(sql,"�ָ�")==0){
        ReCovery();
        clrscr();
	}


	else cout<<"����ȷ����..."<<endl;
	}

    filewrite();
	fprintf(alfp,"\n[%d��%d��%d�� %d:%d:%d]ִ�����²���:\n",GetCurrentYear(),GetCurrentMonth(),GetCurrentDay(),
									GetCurrentHour(),GetCurrentMinute(),GetCurrentSecond() );
	fprintf(alfp,"�û������˳�����.");fputc('\n',alfp);
	fclose(alfp);
	return 0;
}
