#include<config.h>
CPS_START_NAMESPACE
//--------------------------------------------------------------------
//  CVS keywords
//
//  $Author: chulwoo $
//  $Date: 2007/01/11 22:48:12 $
//  $Header: /space/cvs/cps/cps++/src/comms/bgl/cbuf/cram.C,v 1.3 2007/01/11 22:48:12 chulwoo Exp $
//  $Id: cram.C,v 1.3 2007/01/11 22:48:12 chulwoo Exp $
//  $Name: v5_0_16_hantao_io_test_v7 $
//  $Locker:  $
//  $Revision: 1.3 $
//  $Source: /space/cvs/cps/cps++/src/comms/bgl/cbuf/cram.C,v $
//  $State: Exp $
//
//--------------------------------------------------------------------
CPS_END_NAMESPACE
#include<comms/nga_reg.h>
CPS_START_NAMESPACE
//Allocate space for the SCRATCH CRAM 
//Workstation version only
#ifndef _TARTAN
int CRAM_SCRATCH_INTS[CRAM_SCRATCH_SIZE] ;
unsigned long CRAM_SCRATCH_ADDR = (unsigned long)CRAM_SCRATCH_INTS ;
#endif
CPS_END_NAMESPACE
