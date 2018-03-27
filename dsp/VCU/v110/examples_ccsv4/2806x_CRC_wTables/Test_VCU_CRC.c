/******************************************************************************
*******************************************************************************
* 
* FILE:Test_VCU_CRC.c
* 
* DESCRIPTION:Demonstrate the use of the CRC functions available in the VCU lib
* 
*******************************************************************************
*  $TI Release: C28x VCU Library Version v1.10 $
*  $Release Date: September 26, 2011 $
*******************************************************************************
*  This software is licensed for use with Texas Instruments C28x
*  family DSCs.  This license was provided to you prior to installing
*  the software.  You may review this license by consulting a copy of
*  the agreement in the doc directory of this library.
* ------------------------------------------------------------------------
*          Copyright (C) 2010-2011 Texas Instruments, Incorporated.
*                          All Rights Reserved.
******************************************************************************/
#include "crc.h"
#include "crc_tbl.h"
//###########################################################################
//
/*! \page CRC_PAGE (Cyclic Redundancy Check)
*
* The CRC check(data verification code) is basically a polynomial long division
* where the dividend is your data stream, the divisor is the generator polynomial
* and the remainder of the division is the CRC code. It is considered redundant
* as it doesnt add any information to the actual message
* 
* If the length of the CRC is n and the length of the generator polynomial is m,
* then n<m for all n and m and a CRC check can failtfully detect errors of less than
* n bits but can detect errors of greater than n bits (1-2^(-n))*100% of the time
* 
* \section Library Implementation
* There are 4 polynomials available for use with TI's VCU library. Each polynomial is
* defined by a hex number. They are,
* CRC8_PRIME 		- 0x7
* CRC16_802_154		- 0x1021
* CRC16_ALT			- 0x8005
* CRC32_PRIME		- 0x04c11db7
* The library routines permit you to do 8,16 and 32 bit CRC calculations. The CRC functions
* are split into six source files crc8.c, crc8_vcu.asm, crc16.c, cr168_vcu.asm, crc32.c and
* crc32_vcu.asm. 
* 
* \subsection Source Files
* The crcX.c files (X=8,16,32) provide you with the option of generating a table of CRC values
* or using a predefined set for CRC calculations. The purpose of these tables is purely for 
* diagnostic verification. 
* \note There is a getCRCX() that will calculate the CRC of the bitstream but note that it doesnt
* make use of the VCU functions and will, therefore, take a longer time to execute
* 
* The crcX_vcu.asm files on the other hand provide hand optimized VCU code to check the CRC of 
* a data stream. This example shows use of both sets of functions(C and ASM)
* 
* \subsection CRC generation by the linker file
* A new feature of <em>codegen v6.0.0.B1</em> and above is the linker's ability to 
* generate CRC values for initialized sections. The documentation for the library 
* as well as the codegen provide examples on how to use this feature.
* 
* \note You will need to include the header file, crc_tbl.h that comes with the codegen 
*  
*/
//###########################################################################

/*! \defgroup CRC_MACROS_FUNCTIONS (CRC Macro definitions and Function Protoypes)
*  @{
*/
#define CRC_TEST_PACKET_LEN		128 //!< Test Packet Length in number of words
//! Test Packet Length in number of BYTES
#define CRC_TEST_PACKET_BYTES   (CRC_TEST_PACKET_LEN << 1) 

extern CRC_TABLE CRCTable; //!< Generated by the linker as _CRCTable
extern CRC_TABLE CRCTestVector; //!< Generated by the linker as _CRCTable

void Done(void); //!< Stops the emulator

#pragma DATA_SECTION(crc_linker_test_vector,"CRC_CMD_TEST_VECTOR");

//! Test Vector for the linker CRC calculator
static const uint16 crc_linker_test_vector[CRC_TEST_PACKET_LEN]=
{
	0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 
	0x1011, 0x1213, 0x1415, 0x1617, 0x1819, 0x1A1B, 0x1C1D, 0x1E1F, 
	0x2021, 0x2223, 0x2425, 0x2627, 0x2829, 0x2A2B, 0x2C2D, 0x2E2F, 
	0x3031, 0x3233, 0x3435, 0x3637, 0x3839, 0x3A3B, 0x3C3D, 0x3E3F, 
	0x4041, 0x4243, 0x4445, 0x4647, 0x4849, 0x4A4B, 0x4C4D, 0x4E4F, 
	0x5051, 0x5253, 0x5455, 0x5657, 0x5859, 0x5A5B, 0x5C5D, 0x5E5F, 
	0x6061, 0x6263, 0x6465, 0x6667, 0x6869, 0x6A6B, 0x6C6D, 0x6E6F, 
	0x7071, 0x7273, 0x7475, 0x7677, 0x7879, 0x7A7B, 0x7C7D, 0x7E7F, 
	0x8081, 0x8283, 0x8485, 0x8687, 0x8889, 0x8A8B, 0x8C8D, 0x8E8F, 
	0x9091, 0x9293, 0x9495, 0x9697, 0x9899, 0x9A9B, 0x9C9D, 0x9E9F, 
	0xA0A1, 0xA2A3, 0xA4A5, 0xA6A7, 0xA8A9, 0xAAAB, 0xACAD, 0xAEAF, 
	0xB0B1, 0xB2B3, 0xB4B5, 0xB6B7, 0xB8B9, 0xBABB, 0xBCBD, 0xBEBF, 
	0xC0C1, 0xC2C3, 0xC4C5, 0xC6C7, 0xC8C9, 0xCACB, 0xCCCD, 0xCECF, 
	0xD0D1, 0xD2D3, 0xD4D5, 0xD6D7, 0xD8D9, 0xDADB, 0xDCDD, 0xDEDF, 
	0xE0E1, 0xE2E3, 0xE4E5, 0xE6E7, 0xE8E9, 0xEAEB, 0xECED, 0xEEEF, 
	0xF0F1, 0xF2F3, 0xF4F5, 0xF6F7, 0xF8F9, 0xFAFB, 0xFCFD, 0xFEFF
};

//###########################################################################

void main(void)
{

  UINT16 kk;
  //! Test Vector for the CRC check
  UINT16 test_vector[CRC_TEST_PACKET_LEN];
  //! Pointer to the head of message
  UINT16 *puxMsg;
  //! Size of the Message in bytes, Linker CRC_TABLE records
  //  report size of the message in words
  UINT16 uxMsgSizeBytes;
  //! Error Counter
  UINT16 err = 0;
  //! 8 bit CRC results from the C and asm routines
  //  and the linker generated CRC
  volatile UINT16 crc_c, crc_vcu,crc_cmd;
  //! 16 bit CRC results from the C and asm routines
  //  and the linker generated CRC
  volatile UINT16 crc_c16p1, crc_vcu16p1,crc_cmd16p1;
  volatile UINT16 crc_c16p2, crc_vcu16p2,crc_cmd16p2;
  //! 32 bit CRC results from the C and asm routines
  //  and the linker generated CRC
  volatile UINT32 crc_c32, crc_vcu32,crc_cmd32;
  //! Pointer to the linker generated CRC table
  CRC_TABLE *psTable;
  //! A single CRC table record
  CRC_RECORD sRecord;

  psTable = &(CRCTestVector);

// Generating the test vector 
  for(kk=0;kk<CRC_TEST_PACKET_LEN;kk++)
  {
  	test_vector[kk] = (kk<<9) | ((kk<<1)+1);
  }
	
/* Crosschecking the result of the CRC 'C' routine with the assembly routine for a
 * 8 bit datastream
 */  
  crc_c = getCRC8_cpu(INIT_CRC8, test_vector, 0,CRC_TEST_PACKET_BYTES);

  crc_vcu = getCRC8_vcu(INIT_CRC8, test_vector, 0,CRC_TEST_PACKET_BYTES);
  if(crc_c != crc_vcu) 
  {
  	err++;
  }   
 
/* Crosschecking the result of the CRC 'C' routine with the assembly routine for a
 * 16 bit datastream using CRC16_802_15_4
 */
  crc_c16p1 = getCRC16P1_cpu(INIT_CRC16, test_vector, 0,CRC_TEST_PACKET_BYTES);

  crc_vcu16p1 = getCRC16P1_vcu(INIT_CRC16, test_vector, 0,CRC_TEST_PACKET_BYTES);
  if(crc_c16p1 != crc_vcu16p1) 
  {
  	err++;
  }
  
/* Crosschecking the result of the CRC 'C' routine with the assembly routine for a
 * 16 bit datastream using CRC16_ALT
 */
  crc_c16p2 = getCRC16P2_cpu(INIT_CRC16, test_vector, 0,CRC_TEST_PACKET_BYTES);

  crc_vcu16p2 = getCRC16P2_vcu(INIT_CRC16, test_vector, 0,CRC_TEST_PACKET_BYTES);
  if(crc_c16p2 != crc_vcu16p2) 
  {
  	err++;
  }

/* Crosschecking the result of the CRC 'C' routine with the assembly routine for a
 * 32 bit datastream
 */  
  crc_c32 = getCRC32_cpu(INIT_CRC32, test_vector, 0,CRC_TEST_PACKET_BYTES);

  crc_vcu32 = getCRC32_vcu(INIT_CRC32, test_vector, 0,CRC_TEST_PACKET_BYTES);
  if(crc_c32 != crc_vcu32) 
  {
  	err++;
  }   
/* The linker has generated CRC values for each of the crc tables,
 * crc8_table,crc16_table and crc32_table and stored them in the 
 * memory map.
 * Crosschecking the result of the linker generated CRCs with the
 * result from the assembly routine
 */
 
for(kk = 0;kk < psTable->num_recs;kk++){
	sRecord = psTable->recs[kk]; //Get the record
	puxMsg = (UINT16*)sRecord.addr;
	uxMsgSizeBytes = sRecord.size * 2; 
	if(sRecord.crc_alg_ID == CRC8_PRIME){
		crc_c   = getCRC8_cpu(INIT_CRC8, puxMsg, 0,uxMsgSizeBytes);
		crc_cmd = getCRC8_vcu(INIT_CRC8, puxMsg, 0,uxMsgSizeBytes);
		if(crc_cmd != sRecord.crc_value){
			err++;	
		}
	}else if(sRecord.crc_alg_ID == CRC16_802_15_4){
		crc_c16p2 = getCRC16P2_cpu(INIT_CRC16, puxMsg, 0,uxMsgSizeBytes);
		crc_cmd16p2 = getCRC16P2_vcu(INIT_CRC16, puxMsg, 0,uxMsgSizeBytes);
		if(crc_cmd16p2 != sRecord.crc_value){
			err++;
		}	
		
	}else if(sRecord.crc_alg_ID == CRC16_ALT){
		crc_c16p1 = getCRC16P1_cpu(INIT_CRC16, puxMsg, 0,uxMsgSizeBytes);
		crc_cmd16p1 = getCRC16P1_vcu(INIT_CRC16, puxMsg, 0,uxMsgSizeBytes);
		if(crc_cmd16p1 != sRecord.crc_value){
			err++;	
		}
	}else{ //CRC32_PRIME
		crc_c32 = getCRC32_cpu(INIT_CRC32, puxMsg, 0,uxMsgSizeBytes);
		crc_cmd32 = getCRC32_vcu(INIT_CRC32, puxMsg, 0,uxMsgSizeBytes);
		if(crc_cmd32 != sRecord.crc_value){
			err++;	
		}
	}
}
   
  Done();
}
  
/*! End of Main routine
 *
 * \param none
 *
 * call the ESTOP0 routine
 *
 * \return none
 */
void Done(void)
{
    asm("   ESTOP0");
}
//###########################################################################
// Close the Doxygen group.
//! @}
//###########################################################################












