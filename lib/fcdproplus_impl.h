/* -*- c++ -*- */
/* 
 * Copyright 2013 Volker Schroer, DL1KSV.
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

#ifndef INCLUDED_FCDPROPLUS_FCDPROPLUS_IMPL_H
#define INCLUDED_FCDPROPLUS_FCDPROPLUS_IMPL_H

#include <fcdproplus/fcdproplus.h>
#include <gnuradio/audio/source.h>
#include <hidapi/hidapi.h>

namespace gr {
  namespace fcdproplus {

    class fcdproplus_impl : public fcdproplus
    {
    private:
      gr::audio::source::sptr fcd;                   /*!< The audio input source */
      hid_device *d_control_handle;             /*!< handle to control the device, set frequency, etc */
      unsigned int d_freq_req;                           /*!< The latest requested frequency in Khz */
      unsigned char aucBuf[65];                 /*!< Buffers to read/write control messages to the dongle */
      int d_corr;
      int d_unit;


    public:
      fcdproplus_impl(const std::string user_device_name,int unit);
      ~fcdproplus_impl();
      /* Public API functions documented in include/fcdproplus.h */
      void set_freq(float freq);
      void set_lna(int gain);
      void set_mixer_gain(int gain);
      void set_freq_corr(int ppm);
      void set_if_gain(int gain);

    };

  } // namespace fcdproplus
} // namespace gr

#endif /* INCLUDED_FCDPROPLUS_FCDPROPLUS_IMPL_H */

