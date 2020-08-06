/* -*- c++ -*- */
/* 
 * Copyright 2018 Volker Schroer, DL1KSV
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#include "fcdcmd.h"
#include "fcdpp_control_impl.h"

#include <gnuradio/logger.h>


#define FCDPROPLUS_VENDOR_ID    0x04d8
#define FCDPROPLUS_PRODUCT_ID   0xfb31

#define TIMEOUT 5000

//using namespace std;
namespace gr {
  namespace fcdproplus {

    fcdpp_control::sptr
    fcdpp_control::make()
    {
      return gnuradio::get_initial_sptr (new fcdpp_control_impl());
    }

    /*
     * The private constructor
     */
    fcdpp_control_impl::fcdpp_control_impl()
      : gr::block("fcdpp_control",
                    gr::io_signature::make(0, 0, 0),
                    gr::io_signature::make(0, 0, 0))
    {
        /* setup the control part */
        d_control_handle =NULL;
        hid_init();
        d_control_handle = hid_open ( FCDPROPLUS_VENDOR_ID ,FCDPROPLUS_PRODUCT_ID,NULL );
        if(d_control_handle == NULL ) {
            GR_LOG_ERROR(d_logger,"FunCube Dongle  V2.0 not found.");
            throw std::runtime_error("FunCube Dongle  V2.0 not found.");
        }
        else {
            GR_LOG_INFO(d_logger,"FunCube Dongle  V2.0 initialized.");
        }

        /*
         * Check mode, so
         * Send a BL Query Command
         */
        aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
        aucBuf[1] = FCD_HID_CMD_QUERY;
        hid_write(d_control_handle,aucBuf,65);
        hid_read(d_control_handle,aucBuf,65);
        aucBuf[15]=0;
        GR_LOG_INFO(d_logger,boost::format("Dongle: %S ") % &aucBuf[2]);
      /*
       * Initialize message handling
       */
      message_port_register_in(pmt::mp("freq"));
      set_msg_handler(pmt::mp("freq"), boost::bind(&fcdpp_control_impl::set_frequency_msg, this, boost::placeholders::_1));

    }
    fcdpp_control_impl::~fcdpp_control_impl()
    {
        if(d_control_handle !=0)   {
            hid_close(d_control_handle);
        }
        hid_exit();

    }
    void
    fcdpp_control_impl::set_freq(float freq)
    {
        unsigned int nfreq = freq;
        aucBuf[0]=0;
        aucBuf[1]=FCD_HID_CMD_SET_FREQUENCY_HZ;
        aucBuf[2] = (unsigned char)nfreq;
        aucBuf[3] = (unsigned char)(nfreq>>8);
        aucBuf[4] = (unsigned char)(nfreq>>16);
        aucBuf[5] = (unsigned char)(nfreq>>24);
        hid_write(d_control_handle, aucBuf, 65);
        aucBuf[1]=0;
        hid_read(d_control_handle, aucBuf, 65);
        if (aucBuf[0]==FCD_HID_CMD_SET_FREQUENCY_HZ && aucBuf[1]==1) {
            nfreq = 0;
            nfreq = (unsigned int) aucBuf[2];
            nfreq += (unsigned int) (aucBuf[3] << 8);
            nfreq += (unsigned int) (aucBuf[4] << 16);
            nfreq += (unsigned int) (aucBuf[5] << 24);
            GR_LOG_INFO(d_logger,boost::format("Set Frequency to: %1% Hz") % nfreq);

        }
        else {
            GR_LOG_INFO(d_logger,boost::format("Set Frequency to %1% Hz failed") % nfreq );
        }
    }
    void
    fcdpp_control_impl::set_frequency_msg(pmt::pmt_t msg)
    {
      // Accepts either a number that is assumed to be the new
      // frequency or a key:value pair message where the key must be
      // "freq" and the value is the new frequency.
      GR_LOG_DEBUG(d_logger,"Funcube Control frequency message arrived");
      if(pmt::is_number(msg)) {
        set_freq(pmt::to_float(msg));
      }
      else if(pmt::is_pair(msg)) {
        pmt::pmt_t key = pmt::car(msg);
        pmt::pmt_t val = pmt::cdr(msg);
        if(pmt::eq(key, pmt::intern("freq"))) {
          if(pmt::is_number(val)) {
              set_freq(pmt::to_float(val));
          }
        }
        else { 
          GR_LOG_WARN(d_logger, boost::format("Set Frequency Message must have the key = 'freq'; got '%1%'.")% pmt::write_string(key));
        }
      }
      else {
        GR_LOG_WARN(d_logger, "Set Frequency Message must be either a number or a key:value pair where the key is 'freq'.");
      }
    }
    void
    fcdpp_control_impl::set_lna(int gain)
    {
        aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
        aucBuf[1] = FCD_HID_CMD_SET_LNA_GAIN;
        if(gain !=0 ) {
            aucBuf[2] = 1;
        }
        else {
            aucBuf[2]=0;
        }
        hid_write(d_control_handle, aucBuf, 65);
        hid_read(d_control_handle, aucBuf, 65);
        if(aucBuf[0] == FCD_HID_CMD_SET_LNA_GAIN) {
            if (gain != 0) {
                GR_LOG_INFO(d_logger,"LNA gain enabled");
            }
             else {
                GR_LOG_INFO(d_logger,"LNA gain disabled");
            }
        }
        else {
            GR_LOG_ERROR(d_logger,boost::format("Failed to modify LNA gain. Result of transaction: %1%,%2%") % aucBuf[0] %aucBuf[1]);
        }
    }
    void
    fcdpp_control_impl::set_mixer_gain(int gain)
    {
        aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
        aucBuf[1] = FCD_HID_CMD_SET_MIXER_GAIN;
        if(gain !=0 ) {
            aucBuf[2] = 1;
        }
        else {
            aucBuf[2]=0;
        }
        hid_write(d_control_handle, aucBuf, 65);
        hid_read(d_control_handle, aucBuf, 65);
        if(aucBuf[0] == FCD_HID_CMD_SET_MIXER_GAIN) {
            if (gain != 0) {
                GR_LOG_INFO(d_logger,"Mixer gain enabled");
            }
             else {
                GR_LOG_INFO(d_logger,"Mixer gain disabled");
            }
        }
        else {
            GR_LOG_ERROR(d_logger,boost::format("Failed to modify Mixer gain. Result of transaction: %1%,%2%") % aucBuf[0] %aucBuf[1]);
        }
    }

    void
    fcdpp_control_impl::set_if_gain(int gain)
    {
        if( (gain < 0) || gain > 59) {
            GR_LOG_ERROR(d_logger,boost::format("Invalid IF gain value: %1%") % gain);
            return;
        }
        aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
        aucBuf[1] = FCD_HID_CMD_SET_IF_GAIN;
        aucBuf[2] = (unsigned char) gain;
        hid_write(d_control_handle, aucBuf, 65);
        hid_read(d_control_handle, aucBuf, 65);
        if(aucBuf[0] == FCD_HID_CMD_SET_IF_GAIN) {
            GR_LOG_INFO(d_logger,boost::format("IF gain set to: %1%") % gain );
        }
        else {
            GR_LOG_ERROR(d_logger,"Could not set IF gain");
        }

    }

  } /* namespace fcdproplus */
} /* namespace gr */

