/* -*- c++ -*- */
/* 
 * Copyright 2013, 2018 Volker Schroer, DL1KSV
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


#include <iostream>
#include <fstream>
#include <exception>



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

      prefs *p = prefs::singleton();
      std::string config_file = p->get_string("LOG", "log_config", "");
      std::string log_level = p->get_string("LOG", "log_level", "off");
      std::string log_file = p->get_string("LOG", "log_file", "");

      GR_CONFIG_LOGGER(config_file);

      GR_LOG_GETLOGGER(LOG, "gr_log." + alias());
      GR_LOG_SET_LEVEL(LOG, log_level);
      if(log_file.size() > 0) {
        if(log_file == "stdout") {
          GR_LOG_SET_CONSOLE_APPENDER(LOG, "cout","gr::log :%p: %c{1} - %m%n");
        }
        else if(log_file == "stderr") {
          GR_LOG_SET_CONSOLE_APPENDER(LOG, "cerr","gr::log :%p: %c{1} - %m%n");
        }
        else {
          GR_LOG_SET_FILE_APPENDER(LOG, log_file , true,"%r :%p: %c{1} - %m%n");
        }
      }

      d_logger = LOG;


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
                GR_LOG_INFO(d_logger,boost::format("Could not open device: %1%") % user_device_name);
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
                        hw_id << "hw:" << id<<",0"; // build alsa identifier
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
          GR_LOG_INFO(d_logger,boost::format("Audio device %1% opened") % device_name);
        }
        else  {
            GR_LOG_INFO(d_logger,boost::format("Funcube Dongle Pro+ found as: %1%") % device_name);
        }


        /* block to convert stereo audio to a complex stream */
        f2c = gr::blocks::float_to_complex::make(1);

        connect(fcd, 0, f2c, 0);
        connect(fcd, 1, f2c, 1);
        connect(f2c, 0, self(), 0);

        fcd_control = fcdpp_control::make();

      message_port_register_hier_in(pmt::mp("freq"));
      msg_connect(self(),pmt::mp("freq"),fcd_control,pmt::mp("freq"));
    }
    /*
     * Our virtual destructor.
     */
    fcdproplus_impl::~fcdproplus_impl()
    {

    }

    void
    fcdproplus_impl::set_freq(float freq)
    {
        float setfreq;
        if(d_freq_req == (int) freq)
            return; // Frequency did not change
        d_freq_req=(int) freq;
        if(d_corr == 0) {
            setfreq= freq*d_unit;
        }
        else {
            setfreq=((float)d_unit+((float) d_corr)/(1000000./d_unit))*freq;
        }
        fcd_control->set_freq(setfreq);
    }

    void
    fcdproplus_impl::set_lna(int gain)
    {
      fcd_control->set_lna(gain);
    }

    void
    fcdproplus_impl::set_mixer_gain(int gain)
    {
      fcd_control->set_mixer_gain(gain);
    }

    void
    fcdproplus_impl::set_freq_corr(int ppm)
    {
        float freq;
        if(d_corr == ppm)
            return;
        d_corr=ppm;
        GR_LOG_INFO(d_logger,boost::format("Set frequency correction to: %1% ppm ") % ppm );
        freq=d_freq_req;
        d_freq_req=0;
        set_freq(freq);
    }

    void
    fcdproplus_impl::set_if_gain(int gain)
    {
        if( (gain < 0) || gain > 59) {
            GR_LOG_WARN(d_logger,boost::format("Invalid If gain value: %1%") % gain );
            return;
        }
        fcd_control->set_if_gain(gain);

    }

  } /* namespace fcdproplus */
} /* namespace gr */

