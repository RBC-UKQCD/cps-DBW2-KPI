#include<config.h>
#include<stdlib.h>
CPS_START_NAMESPACE
/*----------------------------------------------------------*/
/*!\file
  \brief  Declaration and definition of the MPIRequestManager class

  $Id: mpi_requests.h,v 1.7 2008/02/08 18:35:05 chulwoo Exp $
*/
/*----------------------------------------------------------*/
/* The MPI comms request handle manager: mpi_requests.h

  A.N.Jackson: ajackson@epcc.ed.ac.uk                      
  -----------------------------------------------------------
  CVS keywords
 
  $Author: chulwoo $
  $Date: 2008/02/08 18:35:05 $
  $Header: /space/cvs/cps/cps++/include/comms/Attic/mpi_requests.h,v 1.7 2008/02/08 18:35:05 chulwoo Exp $
  $Id: mpi_requests.h,v 1.7 2008/02/08 18:35:05 chulwoo Exp $
  $Name: v5_0_16_hantao_io_test_v7 $
  $Locker:  $
  $RCSfile: mpi_requests.h,v $
  $Revision: 1.7 $
  $Source: /space/cvs/cps/cps++/include/comms/Attic/mpi_requests.h,v $
  $State: Exp $  */
/*----------------------------------------------------------*/

CPS_END_NAMESPACE
CPS_START_NAMESPACE


#ifndef INCLUDED_MPI_REQ_MAN
#define INCLUDED_MPI_REQ_MAN

CPS_END_NAMESPACE
#include <comms/sysfunc_cps.h>
CPS_START_NAMESPACE

//! Maximum number of concurrent MPI requests.
#define MPI_REQ_BASE_SIZE 100 

/*!
  \defgroup mpicomms Code specific to the MPI interface
  \ingroup comms
*/

//! This handles the MPI requests.
/*!
  The MPI requests are used as identifying handles in non-blocking
  communications; see MPI literature for more more details, <e>e.g.</e>
  http://www-unix.mcs.anl.gov/mpi/mpi-standard/mpi-report-1.1/node44.htm#Node44
  
  Currently, it is very dumb and uses a fixed size (#MPI_REQ_BASE_SIZE)
  array to store the handles.  If this is exceeded, it just crashes out.

  \todo This should  
- Use a linked list.  
- Store the direction as well as the handle, in case we wish to
  implement the form of SCUTransComplete that only waits for the
  transfers in a specified direction to cease.
  
  \ingroup comms mpicomms
*/

class MPIRequestManager {
 private:
    MPI_Request *mpi_req;
    int num_req, max_req;

 public:

    //! Default constructor:
    /*!
      Initialises the list of requests.
      \post All MPI requests are initially set to MPI_REQUEST_NULL.
    */
    MPIRequestManager() {
	max_req = MPI_REQ_BASE_SIZE;
	mpi_req = new MPI_Request[max_req];
	num_req = 0;
	for( int i=0; i < max_req; i++ ) mpi_req[i] = MPI_REQUEST_NULL;
    }
    //! Default destructor.
    /*! Deletes the storage associated with the request array. */
    ~MPIRequestManager() {
	delete[] mpi_req;
    }

    //! Store a request handle.
    /*!
      \param req The request handle.
      \post The request is stored.
    */
    void AddRequest( MPI_Request req ) {
	mpi_req[num_req] = req;
	num_req++;
	if( num_req >= max_req ) {
	    // This should do something more intelligent than this:
	    printf("ERROR: Maximum request-handle limit has been exceeded!\n");
	    exit(EXIT_FAILURE);
	}
    }

    //! Get the list of requests.
    /*!
      \return A pointer to the first request.
    */
    MPI_Request* ReqArray() {
	return mpi_req;
    }

    //! Get the number of requests currently stored.
    /*!
      \return The number of stored handles.
    */
    int NumReq() {
	return num_req;
    }

    //! Empty the list.
    void Clear() {
	for( int i=0; i < max_req; i++ ) mpi_req[i] = MPI_REQUEST_NULL;
	num_req = 0;
    }

};

#endif

CPS_END_NAMESPACE
