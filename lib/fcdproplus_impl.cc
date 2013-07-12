/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/blocks/float_to_complex.h>

#include "fcdproplus_impl.h"
#include "fcdcmd.h"

#include <iostream>
#include <fstream>
#include <exception>


#define FCDPROPLUS_VENDOR_ID    0x04d8
#define FCDPROPLUS_PRODUCT_ID   0xfb31

#define TIMEOUT 5000
using namespace std;
namespace gr {
  namespace fcdproplus {

    fcdproplus::sptr
    fcdproplus::make(const std::string device_name,int unit)
    {
      return gnuradio::get_initial_sptr (new fcdproplus_impl(device_name,unit));
    }

    /*
     * The private constructor
     */
    fcdproplus_impl::fcdproplus_impl(const std::string user_device_name,int unit)
      : gr::hier_block2("fcdproplus",
                    gr::io_signature::make(0, 0, 0),
                    gr::io_signature::make(1, 1, sizeof (gr_complex)))
    {
        std::string device_name;
        bool success;
        gr::blocks::float_to_complex::sptr f2c;
        success = false;
        d_freq_req=0;
        d_corr=0;
        d_unit=unit;
        if(!user_device_name.empty())  {
            try {
                 /* Audio source; sample rate fixed at 192kHz */
                 fcd = gr::audio::source::make(192000, user_device_name, true);
                 success=true;
            }
            catch (std::exception ) {
                std::cerr << "Could not open device: " <<user_device_name << std::endl;
                success=false;
            }

        }
        if(success) {
            device_name=user_device_name;
        }
        else {

            device_name.clear();
            std::string line;
            std::ifstream cards( "/proc/asound/cards" );
            if ( cards.is_open() )  {
                while ( cards.good() )  {
                    getline (cards, line);

                    if ( line.find( "USB-Audio - FUNcube Dongle V2.0" ) != std::string::npos )  {
                        int id;
                        std::istringstream( line ) >> id;

                        std::ostringstream hw_id;
                        hw_id << "plughw:" << id<<",0"; // build alsa identifier
                        device_name= hw_id.str();

                    }
                }
                cards.close();
                if(device_name.empty()) {
                    throw std::runtime_error("No FunCube Dongle  V2.0 found.");
                }
            }
            else {
            throw std::runtime_error("Alsa not found.");
            }
            /* Audio source; sample rate fixed at 192kHz */
            fcd = gr::audio::source::make(192000, device_name, true);
        }
        if(success) {
          std::cerr <<"Opened: " << device_name << std::endl;
        }
        else  {
            std::cerr <<"Funcube Dongle Pro+ found as: " << device_name << std::endl;
        }


        /* block to convert stereo audio to a complex stream */
        f2c = gr::blocks::float_to_complex::make(1);

        connect(fcd, 0, f2c, 0);
        connect(fcd, 1, f2c, 1);
        connect(f2c, 0, self(), 0);
        /* setup the control part */
        d_control_handle =NULL;
        hid_init();
        d_control_handle = hid_open ( FCDPROPLUS_VENDOR_ID ,FCDPROPLUS_PRODUCT_ID,NULL );
        if(d_control_handle == NULL ) {
            throw std::runtime_error("FunCube Dongle  V2.0 soundcard found but not controlpart.");
        }
        else {
            std::cerr <<"Dongle sucessfully initialized" << std::endl;
        }

        /*
         * Check mode, so
         * Send a BL Query Command
         */
        aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
        aucBuf[1] = FCD_HID_CMD_QUERY;
        hid_write(d_control_handle,aucBuf,65);
        hid_read(d_control_handle,aucBuf,65);
        std::cerr <<"Result of Action :+++++" << std::endl;
        for(int i=2;i<15;i++)
           std::cerr << aucBuf[i];
        std::cerr << std::endl;

    }

    /*
     * Our virtual destructor.
     */
    fcdproplus_impl::~fcdproplus_impl()
    {
        if(d_control_handle !=0)   {
            hid_close(d_control_handle);
        }
        hid_exit();

    }

    void
    fcdproplus_impl::set_freq(float freq)
    {
        unsigned int nfreq;
        if(d_freq_req == (int) freq)
            return; // Frequency did not change
        d_freq_req=(int) freq;
        if(d_corr == 0) {
            nfreq=((int) freq)*d_unit;
        }
        else {
            nfreq=(int)(((float)d_unit+((float) d_corr)/(1000000./d_unit))*freq);
        }
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
            if(d_unit == 1000) {
                std::cerr <<"Set Frequency to: "<<freq/1000 <<" KHz, corrected to: " <<nfreq/1000 << " Khz"<<std::endl;
            }
            else {
                std::cerr <<"Set Frequency to: "<<freq <<" Hz, corrected to: " <<nfreq << " Hz"<<std::endl;
            }

        }
        else {
            std::cerr <<"Set Frequency failed: " <<freq << " Khz"<<std::endl;
        }
    }

    void
    fcdproplus_impl::set_lna(int gain)
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
                std::cerr <<" Lna gain enabled" << std::endl;
            }
             else {
                std::cerr <<" Lna gain disabled" << std::endl;
            }
        }
        else {
            std::cerr <<"Failed to modify lna gain" <<std::endl;
            std::cerr <<"Result: "<< aucBuf[0]<< " , " << aucBuf[1] <<std::endl;
        }
    }

    void
    fcdproplus_impl::set_mixer_gain(int gain)
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
                std::cerr <<" Mixer gain enabled" << std::endl;
            }
             else {
                std::cerr <<" Mixer gain disabled" << std::endl;
            }
        }
        else {
            std::cerr <<"Failed to modify mixer gain" <<std::endl;
            std::cerr <<"Result: "<< aucBuf[0]<< " , " << aucBuf[1] <<std::endl;
        }
    }

    void
    fcdproplus_impl::set_freq_corr(int ppm)
    {
        float freq;
        if(d_corr == ppm)
            return;
        d_corr=ppm;
        std::cerr <<"Set frequency correction to: "<< ppm <<std::endl;
        //Reset Frequency setting
        freq=d_freq_req;
        d_freq_req=0;
        set_freq(freq);
    }

    void
    fcdproplus_impl::set_if_gain(int gain)
    {
        if( (gain < 0) || gain > 59) {
            std::cerr <<"Invalid If gain value: "<< gain <<std::endl;
            return;
        }
        aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
        aucBuf[1] = FCD_HID_CMD_SET_IF_GAIN;
        aucBuf[2] = (unsigned char) gain;
        hid_write(d_control_handle, aucBuf, 65);
        hid_read(d_control_handle, aucBuf, 65);
        if(aucBuf[0] == FCD_HID_CMD_SET_IF_GAIN) {
            std::cerr <<"If gain set to: "<< gain <<std::endl;
        }
        else {
            std::cerr << "Could not set If gain" <<std::endl;
        }

    }

  } /* namespace fcdproplus */
} /* namespace gr */

