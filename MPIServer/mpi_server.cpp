#include <stdio.h>
#include <string.h>
#include "mpi_server.h"

#define DOMP_MAX_CLIENT_NAME  512
MPIServer::MPIServer()
{
	 mServerPortFileName = "mpiportname.txt";
	 mClusterName        = "qnlmdb";
	 MPI_Init(NULL, NULL);
	 MPI_Comm_rank(MPI_COMM_WORLD, &mMyId);
}
void MPIServer::startServer() 
{
	if(0 != mMyId)
		return;
		
    MPI_Open_port(MPI_INFO_NULL, mPortName);
    char name[DOMP_MAX_CLIENT_NAME];
    snprintf(name, DOMP_MAX_CLIENT_NAME, "%s-%d", mClusterName.c_str(), mMyId);
    //MPI_Publish_name(name, MPI_INFO_NULL, mPortName);
    printf("Server %s for node %d available at %s\n", name, mMyId, mPortName);

	FILE* port_file = NULL;
	port_file = fopen(mServerPortFileName.c_str(), "wt");
    if (port_file)
    {
        // open port
		printf("%s ln %d\n", __FUNCTION__,__LINE__);
        fprintf(port_file, "%s", mPortName); 
        fclose(port_file);
    }
	printf("%s ln %d\n", __FUNCTION__,__LINE__);
    // First start your own server thread
	#if 0
	accept();
	#else
    serverThread = std::thread(&MPIServer::accept, this);
    printf("%s ln %d\n", __FUNCTION__,__LINE__);
    // Wait for all nodes to start their server first
    MPI_Barrier(MPI_COMM_WORLD);
	while(1);
    printf("%s ln %d\n", __FUNCTION__,__LINE__);
    // Now create connection to all other threads
    /*char c_port_name[MPI_MAX_PORT_NAME];
    for (int i = 0; i < clusterSize; i++) {
      if (i != rank) {
        snprintf(name, DOMP_MAX_CLIENT_NAME, "%s-%d", clusterName, i);
        MPI_Lookup_name(name, MPI_INFO_NULL, c_port_name);
        log("Node %d connecting to client %s at port %s\n", rank, name, c_port_name);
        MPI_Comm_connect(c_port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &nodeConnections[i]);
      }
    }*/
	#endif
}
void MPIServer::stopServer()
{
	 MPI_Close_port(mPortName);
	 MPI_Finalize();
}
// Here is the accept method
void MPIServer::accept() 
{
    while (true) {
      //MPI_Comm client;// = new MPI_Comm();
      MPI_Comm_accept(mPortName, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &mCommClient);
      //log("Node %d received a request\b", rank);
      // Handle in a new thread
      // TODO: Consider using a threadpool instead of spawning thread every time
	  printf("%s ln %d client=%d\n", __FUNCTION__,__LINE__,mCommClient);
      std::thread(&MPIServer::handleRequest, this, &mCommClient);
	  //handleRequest(&mCommClient);
    }
 }
void MPIServer::handleRequest(MPI_Comm *client)
{
	int passed_num = 100;
	MPI_Status status;
	
	MPI_Send(&passed_num, 1, MPI_INT, 0, 0, *client);
	
	printf("%s ln %d client=%d\n", __FUNCTION__,__LINE__,*client);
	MPI_Request  request;
	passed_num = 0;
	//MPI_Recv(&passed_num, 1, MPI_INT,MPI_ANY_SOURCE, MPI_ANY_TAG, *client, &status);
	//printf("after receiving passed_num %d\n", passed_num);
	int again = 1; 
        while (again) { 
            MPI_Recv( &passed_num, 1, MPI_INT,  
                      MPI_ANY_SOURCE, MPI_ANY_TAG, *client, &status ); 
            switch (status.MPI_TAG) { 
                case 0: MPI_Comm_free( client ); 
                        MPI_Close_port(mPortName); 
                        MPI_Finalize(); 
                        return ; 
                case 1: MPI_Comm_disconnect( client ); 
                        again = 0; 
                        break; 
                case 2: /* do something */ 
				    printf("%s ln %d client %d\n", __FUNCTION__,__LINE__,client);
					break;
                default: 
                        /* Unexpected message type */ 
                        MPI_Abort( MPI_COMM_WORLD, 1 ); 
                } 
            } 
 
	/*while(1){
		//MPI_Irecv(&passed_num, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, *client, &request);
		MPI_Irecv(&passed_num, 1, MPI_INT, 0, 0, *client, &request);
		//printf("%s ln %d\n", __FUNCTION__,__LINE__);
		printf("after receiving passed_num %d\n", passed_num); 
   }*/
}
 
#if 1
int main()
{
	MPIServer server;
	server.startServer();
	return 0;
}

#else

#define MAX_DATA 10
int main( int argc, char **argv ) 
{ 
    MPI_Comm client; 
    MPI_Status status; 
    char port_name[MPI_MAX_PORT_NAME]; 
    double buf[MAX_DATA]; 
    int    size, again; 
 
    MPI_Init( &argc, &argv ); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    if (size != 1) printf("Server too big"); 
    MPI_Open_port(MPI_INFO_NULL, port_name); 
    printf("server available at %s\n",port_name); 
	
	FILE* port_file = NULL;
	port_file = fopen("mpiportname.txt", "wt");
    if (port_file)
    {
        // open port
		printf("%s ln %d\n", __FUNCTION__,__LINE__);
        fprintf(port_file, "%s", port_name); 
        fclose(port_file);
    }
	
    while (1) { 
        MPI_Comm_accept( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD,  
                         &client ); 
        again = 1; 
        while (again) { 
            MPI_Recv( buf, MAX_DATA, MPI_DOUBLE,  
                      MPI_ANY_SOURCE, MPI_ANY_TAG, client, &status ); 
            switch (status.MPI_TAG) { 
                case 0: MPI_Comm_free( &client ); 
                        MPI_Close_port(port_name); 
                        MPI_Finalize(); 
                        return 0; 
                case 1: MPI_Comm_disconnect( &client ); 
                        again = 0; 
                        break; 
                case 2: /* do something */ 
						printf("%s ln %d client %d\n", __FUNCTION__,__LINE__,client);
						break;
                default: 
                        /* Unexpected message type */ 
                        MPI_Abort( MPI_COMM_WORLD, 1 ); 
                } 
            } 
        } 
} 
#endif