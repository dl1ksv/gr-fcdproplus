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

#ifndef INCLUDED_FCDPROPLUS_CONTROL_IMPL_H
#define INCLUDED_FCDPROPLUS_CONTROL_IMPL_H

#include <fcdproplus/fcdpp_control.h>

#ifdef SYSTEM_HIDAPI
#include <hidapi/hidapi.h>
#else
#include "hidapi.h"
#endif

namespace gr {
  namespace fcdproplus {

    class fcdpp_control_impl : public fcdpp_control
    {
    private:

      hid_device *d_control_handle;             /*!< handle to control the device, set frequency, etc */
      unsigned char aucBuf[65];                 /*!< Buffers to read/write control messages to the dongle */

    public:
      fcdpp_control_impl();
      ~fcdpp_control_impl();
      /* Public API functions documented in include/control.h */
      void set_freq(float freq);
      void set_lna(int gain);
      void set_mixer_gain(int gain);
      void set_if_gain(int gain);
      void set_frequency_msg(pmt::pmt_t msg);


    };

  } // namespace fcdproplus
} // namespace gr

#endif /* INCLUDED_FCDPROPLUS_CONTROL_H */

