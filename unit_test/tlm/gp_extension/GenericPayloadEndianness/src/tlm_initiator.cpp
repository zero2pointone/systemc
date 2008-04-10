/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

*****************************************************************************/
#include "tlm_initiator.h"

//
// Read data to address (data in host endianness)
//
void tlm_initiator::write(sc_dt::uint64 address,
                          unsigned char *data,
				          unsigned int nr_bytes,
                          unsigned char* byte_enable,
                          unsigned int byte_enable_length)
{
    sc_core::sc_time t;

	// Single WRITE transaction 
    
    std::cout << name() << " : Single WRITE transaction : ADDRESS = 0x"
              << std::hex << address << std::dec << " : "; 
    
    m_gp.set_command(tlm::TLM_WRITE_COMMAND);
    m_gp.set_address(address);
    m_gp.set_data_ptr(data);
    m_gp.set_data_length(nr_bytes);
    m_gp.set_byte_enable_ptr(byte_enable);
    m_gp.set_byte_enable_length(byte_enable_length);
    
    t = sc_core::SC_ZERO_TIME;
    
    socket->b_transport(m_gp, t);
    wait(t);
    
    if(m_gp.is_response_ok())
    {
        std::cout << " OK " << std::endl;
    }
    else
    {
        std::cout << m_gp.get_response_string() << std::endl;
    }
}

//
// Read data from address (returns data in host endianness)
//
void tlm_initiator::read(sc_dt::uint64 address,
                         unsigned char *data,
				         unsigned int nr_bytes,
                         unsigned char* byte_enable,
                         unsigned int byte_enable_length)
{
    sc_core::sc_time t;

	// Single READ transaction 
    std::cout << name() << " : Single READ transaction : ADDRESS = 0x"
              << std::hex << address << std::dec << " : ";  
    
    m_gp.set_command(tlm::TLM_READ_COMMAND);
    m_gp.set_address(address);
    m_gp.set_data_ptr(data);
    m_gp.set_data_length(nr_bytes);
    m_gp.set_byte_enable_ptr(byte_enable);
    m_gp.set_byte_enable_length(byte_enable_length);
    
    t = sc_core::SC_ZERO_TIME;
    
    socket->b_transport(m_gp, t);
    wait(t);
    
    if(m_gp.is_response_ok())
    {
        std::cout << " OK " << std::endl;
    }
    else
    {
        std::cout << m_gp.get_response_string() << std::endl;
    }
}


void tlm_initiator::compare_arrays(unsigned char* wrdata, unsigned char* rddata, int nbytes) 
{
    // check if everything went ok
    bool error_flag = false;
    
    for(int i=0;i<nbytes;i++)
    {
        if(wrdata[i]!= rddata[i])
            error_flag =true;
    }
    
    if(error_flag)
        std::cout << name() << " : read and write " << nbytes*8
                  << "-bit operation : DON'T MATCH \n";
    else
        std::cout << name() << " : read and write " << nbytes*8
                  << "-bit operation : MATCH  \n";
    
    std::cout << name() << " : WRITE ARRAY = 0x" << std::hex;
    for(int i=0; i<nbytes; i++)
    {
        std::cout.width(2);
        std::cout.fill('0');
        std::cout << (int)wrdata[i];
    }
    std::cout << std::endl;
    std::cout << name() << " : READ  ARRAY = 0x" << std::hex; 
    for(int i=0;i<nbytes;i++)
    {
        std::cout.width(2);
        std::cout.fill('0');
        std::cout << (int)rddata[i];
    }
    std::cout << std::endl;
    
    std::cout << std::dec;
}
