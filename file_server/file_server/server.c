#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>

void receive_data(FILE *fp){
	char buff[1022],ch;
	int count;
	while (1){
		//ch = -1;
		count = fread(&ch, sizeof(char), 1, fp);
		if (count == 1){
			if (ch == 'c'){
				Sleep(100);
				fseek(fp, 1, SEEK_CUR);
				fread(&buff, sizeof(char), 1022, fp);
				printf("\nmsg from client:%s\n", buff);
			}
			else if (ch == 's')
				fseek(fp, 1023, SEEK_CUR);
			else
				fseek(fp, -1, SEEK_CUR);
		}
	}
}

void send_data(FILE *fp){
	char buff[1022], ch;
	while (1){
		memset(&buff, 0, 1022 * sizeof(char));
		fflush(stdin);
		printf("Enter the message to send to client\n");
		while (!_kbhit()){}
		scanf("%[^\n]s", &buff);
		system("cls");
		fseek(fp, 0, SEEK_END);
		fwrite("s:", sizeof(char), 2, fp);
		fwrite(buff, sizeof(char), 1022, fp);
		fflush(fp);
		Sleep(100);
		printf("reply to client:: %s\n\n", buff);
	}
}

int main(){
	FILE *fp1,*fp2=NULL;
	char *status1=NULL, *status2=NULL;
	pthread_t sender, receiver;
	int temp;
	fp1 = fopen("socket.bin", "rb+");
	if (fp1 == NULL){
		fp1 = fopen("socket.bin", "w");
		fclose(fp1);
		fp1 = fopen("socket.bin", "rb+");
	}
	fp2 = fopen("socket.bin", "rb+");
	pthread_create(&sender, NULL, send_data, (void *)fp1);
	fseek(fp2, 0, SEEK_END);
	pthread_create(&receiver, NULL, receive_data, (void *)fp2);
	pthread_join(sender, status1);
	pthread_join(receiver, status2);
	getchar();
	return 0;
}