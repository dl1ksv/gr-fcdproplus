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

#ifndef INCLUDED_FCDPROPLUS_CONTROL_H
#define INCLUDED_FCDPROPLUS_CONTROL_H

#include <fcdproplus/api.h>
#include <gnuradio/block.h>


namespace gr {
  namespace fcdproplus {

    class FCDPROPLUS_API fcdpp_control : virtual public block
    {
    public:
      // gr::blocks::message_debug::sptr
      typedef boost::shared_ptr<fcdpp_control> sptr;

      static sptr make();

       /*! \brief Sets the frequency .
        *  \param freq The frequency in unit Hz
        *
        * Sets the frequency of the Funcube Dongle with Hz resolution
        *
        */
       virtual void set_freq(float freq) = 0;

       /*! \brief Switches the LNA on/off.
        *  \param gain  lna on/off.
        *
        */
       virtual void set_lna(int gain) = 0;

       /*! \brief Switches mixer gain onf/off.
        *  \param gain true means on.
        *
        */
       virtual void set_mixer_gain(int gain) = 0;

       /*! \brief Set If gain.
        *  \param gain The If gain value between 0 and 59 db
        *
        */
       virtual void set_if_gain(int gain)=0;

    };
  } // namespace fcdproplus
} // namespace gr


#endif /* INCLUDED_FCDPROPLUS_CONTROL_H */
