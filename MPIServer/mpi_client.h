
#ifndef __MPI_CLIENT_H__
#define __MPI_CLIENT_H__

#include <mpi.h>
#include <string>
using namespace std;

class MPIClient{
public:
	MPIClient();
	void startClient();
	void stopClient();
	void connect();
private:
	string   mServerPortFileName;
	string   mClusterName;
	int      mClusterSize;
	int      mMyId;
	char     mPortName[MPI_MAX_PORT_NAME];
	MPI_Comm mCommServer;
};

#endif //__MPI_CLIENT_H__