/* -*- c++ -*- */
/*
 * Copyright 2011-2013 Free Software Foundation, Inc.
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_FCDPROPLUS_FCD_IMPL_H
#define INCLUDED_FCDPROPLUS_FCD_IMPL_H

#include <fcdproplus/fcd.h>
#include <fcdproplus/fcd_control.h>
#include <gnuradio/audio/source.h>
#include <gnuradio/logger.h>
#include <gnuradio/prefs.h>

namespace gr {
  namespace fcdproplus {

    class fcd_impl : public fcd
    {
    public:
      fcd_impl(const std::string device_name = "");
      ~fcd_impl();

      /* Public API functions documented in include/fcdproplus/fcd.h */
      void set_freq(float freq);
      void set_lna_gain(float gain);
      void set_mixer_gain(float gain);
      void set_freq_corr(int ppm);
      void set_dc_corr(double _dci, double _dcq);
      void set_iq_corr(double _gain, double _phase);

    private:
      gr::audio::source::sptr fcd_audio;  /*!< The audio input source */
      int d_freq_corr;                    /*!< The frequency correction in ppm */
      int d_freq_req;                     /*!< The latest requested frequency in Hz */
      fcd_control::sptr fcd_control_block;/*!< The fcd control block */
      gr::logger_ptr d_logger;
    };

  } /* namespace fcd */
} /* namespace gr */

#endif /* INCLUDED_FCDPROPLUS_FCD_IMPL_H */
