#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<Winsock2.h>


//#define LOG_ENABLED

#ifdef LOG_ENABLED
#include <sys/time.h>
	FILE *log_file;
	timeval timestamp;
#endif	

/* application data */
#define SERV_IP "192.168.208.110"
#define MY_IP "192.168.208.146"
const char application_name[]="PIONEER2AT_1";
const int dev_number = 2;
const char* dev_types[dev_number] =  {"2D-MOTION","GPS-DATA"}; // !!! bydlocode. Make normal structure.
const char* dev_names[dev_number] =  {"PIONEER","PIONEER_INTERNALGPS"};
#define COORDINATOR_PORT 2352
#define MY_PORT 6667

#define PI 3.14159265359

/* program state for controlling*/
enum program_state{
	NORMAL,
	QUIT,
	DROPPED};


struct param{
	//CPioneer2 pioneer2;
	program_state state;

	/* velocities: target values and current values*/
	
};


const int output_message_number = 2;
enum type_of_output_message {IAM_PROGRAM = 0,
	//giveme my-position,
	DEVICESOF,
	//GIVEME,
	//PLEASE_START
	};

enum type_of_input_message {
	WHOIS_PROGRAM,
//	TAKE_DEVICES,
//	TAKE_DATA,
//	TAKE_RESULT,
	PROTOCOL_MESSAGE
	};

char *output_message_text[output_message_number];
	/*!!! add all types; change for non-fixed message generation */
const char* get_output_message(type_of_output_message tp/*, const char* extras*/)
{
	return output_message_text[tp];
}

//DWORD WINAPI network_listener(LPVOID param)
void network_listener(char* msg_tst)
{
	/* data for message receiving */
	#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	#endif

	struct sockaddr_in servaddr,cliaddr;
	int len;
	char mesg[1000]; // !!! OCHE BYDLOCODE

	/* data for message sending */
	int s = INVALID_SOCKET;
	int sockfd=INVALID_SOCKET,n,sock_sender;
	if((s = socket (AF_INET, SOCK_DGRAM, 0))==INVALID_SOCKET)
		return; 
	/* make udp listener socket*/
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==INVALID_SOCKET)
		return;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(MY_IP);
	servaddr.sin_port=htons(MY_PORT);
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	len = sizeof(cliaddr);
	
	char my_messgn[64]="giveme my-position 3";
	
	
	printf(my_messgn);
		

   

	/* make udp sender socket and send first message */
	sock_sender=socket(AF_INET,SOCK_DGRAM,0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port=htons(COORDINATOR_PORT);
	cliaddr.sin_addr.s_addr=inet_addr(SERV_IP);
	const char * iam_mess = my_messgn;
	n = sendto(sock_sender,iam_mess,strlen(iam_mess),0,(struct sockaddr *)&cliaddr,sizeof(struct sockaddr));
	for (;;)
	{
		for(int i=0;i<1000;i++)
			mesg[i]=0;
		n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
		/* convert recieved message to lowercase*/
		for(int i=0;i<len;++i)
			if(isupper(mesg[i]))
				mesg[i]=tolower(mesg[i]);
			printf (mesg);
		mesg[len]='\0';
		//std::cerr<<"Recieved "<<n<<"bytes: "<<mesg<<std::endl;
		/* WHOIS */
		const char *whois_templ="whois program";
		if(strncmp(mesg,whois_templ,strlen(whois_templ))==0){
			cliaddr.sin_port=htons(COORDINATOR_PORT);
			const char * devof_mess = get_output_message(DEVICESOF);
			n = sendto(sock_sender,devof_mess,strlen(devof_mess),0,(struct sockaddr *)&cliaddr,sizeof(struct sockaddr)); /* !!! check n*/
			continue;}

		/* !!! bydlocode; need normal parser*/
		
	
	}
}

/*
int main(){

	
	network_listener();
	

	
	/* thread for network listening */
	/*HANDLE m_listener = *///CreateThread(NULL,0,&network_listener,(void*)&p,0,NULL);
	/* command interpreter */
	
//}