#include<config.h>
CPS_START_NAMESPACE
//--------------------------------------------------------------------
//  CVS keywords
//
//  $Author: chulwoo $
//  $Date: 2008/09/22 15:24:18 $
//  $Header: /space/cvs/cps/cps++/src/comms/qcdoc/cbuf/cram.C,v 1.6 2008/09/22 15:24:18 chulwoo Exp $
//  $Id: cram.C,v 1.6 2008/09/22 15:24:18 chulwoo Exp $
//  $Name: v5_0_16_hantao_io_test_v7 $
//  $Locker:  $
//  $Revision: 1.6 $
//  $Source: /space/cvs/cps/cps++/src/comms/qcdoc/cbuf/cram.C,v $
//  $State: Exp $
//
//--------------------------------------------------------------------
CPS_END_NAMESPACE
#include<comms/nga_reg.h>
CPS_START_NAMESPACE
//Allocate space for the SCRATCH CRAM 
//Workstation version only
#ifndef _TARTAN
//long CRAM_SCRATCH_INTS[CRAM_SCRATCH_SIZE] ;
//unsigned long CRAM_SCRATCH_ADDR = (unsigned long)CRAM_SCRATCH_INTS ;
#endif
CPS_END_NAMESPACE
