#include <stdio.h>
#include <string.h>
#include "mpi_client.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

MPIClient::MPIClient()
{
	 mServerPortFileName = "mpiportname.txt";
	 mClusterName        = "qnlmdb";
	 MPI_Init(NULL, NULL);
	 MPI_Comm_rank(MPI_COMM_WORLD, &mMyId);
	 memset(mPortName,0,MPI_MAX_PORT_NAME);
	 printf("%s ln %d\n", __FUNCTION__,__LINE__);
}
void MPIClient::startClient() 
{
	if(0 != mMyId)
		return;
	
	ifstream fin(mServerPortFileName); 
	string s;  
    //getline(fin,s);
	fin.getline(mPortName,MPI_MAX_PORT_NAME);
		
    printf("%s ln %d mPortName=%s\n", __FUNCTION__,__LINE__,mPortName);
	//MPI_Comm newcomm;
	printf("%s ln %d\n", __FUNCTION__,__LINE__);
	MPI_Comm_connect(mPortName, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &mCommServer); 
	int passed_num = 0;
	//usleep(10000);
	MPI_Status status;
	MPI_Recv(&passed_num, 1, MPI_INT, 0, 0, mCommServer, &status);
	printf("%s ln %d rev %d mCommServer=%d\n", __FUNCTION__,__LINE__,passed_num,mCommServer);
	passed_num = 111;
	
	int tag =0;
    int	dest = 0;
	MPI_Request request;
    //MPI_Send(&passed_num, 1, MPI_INT, 0, 0, mCommServer);
	    int done = 0;
    while (!done) { 
        tag = 2; /* Action to perform */ 
		//printf("%s ln %d server %d\n", __FUNCTION__,__LINE__,mCommServer); 
        MPI_Send(&passed_num, 1, MPI_INT, 0, tag, mCommServer ); 
        /* etc */ 
        } 
	/*MPI_Isend(&passed_num, 1, MPI_INT, dest, tag, newcomm,&request);
	passed_num = 112;
	MPI_Isend(&passed_num, 1, MPI_INT, dest, tag, newcomm,&request);
	passed_num = 113;
	MPI_Isend(&passed_num, 1, MPI_INT, dest, tag, newcomm,&request);
	printf("%s ln %d\n", __FUNCTION__,__LINE__);
	*/
	while(1);
}
void MPIClient::stopClient()
{
	 MPI_Finalize();
}

#if 1
int main()
{
	MPIClient client;
	client.startClient();
	return 0;
}
#else
	
#define MAX_DATA 10
int main( int argc, char **argv ) 
{ 
    MPI_Comm server; 
    double buf[MAX_DATA]; 
    char port_name[MPI_MAX_PORT_NAME]; 
 
    MPI_Init( &argc, &argv ); 
	
	ifstream fin("mpiportname.txt"); 
	string s;  
    //getline(fin,s);
	fin.getline(port_name,MPI_MAX_PORT_NAME);
	
    //strcpy(port_name, argv[1] );/* assume server's name is cmd-line arg */ 
    printf("port_name %s\n",port_name);
    MPI_Comm_connect( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD,  
                      &server ); 
    int done = 0;
	int tag = 0;
	int  n =  1;
    while (!done) { 
        tag = 2; /* Action to perform */ 
		printf("%s ln %d server %d\n", __FUNCTION__,__LINE__,server); 
        MPI_Send( buf, n, MPI_DOUBLE, 0, tag, server ); 
        /* etc */ 
        } 
    MPI_Send( buf, 0, MPI_DOUBLE, 0, 1, server ); 
    MPI_Comm_disconnect( &server ); 
    MPI_Finalize(); 
    return 0; 
} 
#endif