
#ifndef __MPI_SERVER_H__
#define __MPI_SERVER_H__

#include <mpi.h>
#include <string>
#include <thread>
using namespace std;

class MPIServer{
public:
	MPIServer();
	void startServer();
	void stopServer();
	void accept();
	void handleRequest(MPI_Comm *client);
private:
	string   mServerPortFileName;
	string   mClusterName;
	int      mClusterSize;
	int      mMyId;
	char     mPortName[MPI_MAX_PORT_NAME];
	thread   serverThread;
	MPI_Comm mCommClient;
};

#endif //__MPI_SERVER_H__