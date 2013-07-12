/* -*- c++ -*- */
/* 
 * Copyright 2013 Volker Schroer, DL1KSV
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


#ifndef INCLUDED_FCDPROPLUS_FCDPROPLUS_H
#define INCLUDED_FCDPROPLUS_FCDPROPLUS_H

#include <fcdproplus/api.h>
#include <gnuradio/hier_block2.h>

#include <string>

namespace gr {
  namespace fcdproplus {

  /*!
   * \brief Funcube Pro+ Dongle source block.
   *
   * This class provides a soure block for the Funcube Pro+ Dongle by wrapping the
   * alsa audio interface and the USB control interface of the Funcube Dongle
   * into one convenient source block.
   * The hadware audio device is autodetected by the card name. If more than one Pro+ are
   * connected it takes the first card.
   * The block was tested with  firmware version 20.3
   *
   * \ingroup fcdproplus
   *
   */
    class FCDPROPLUS_API fcdproplus : virtual public hier_block2
    {
    public:
       typedef boost::shared_ptr<fcdproplus> sptr;

       /*!
        * \brief Return a shared_ptr to a new instance of fcdproplus::fcdproplus.
        *
        * To avoid accidental use of raw pointers, fcdproplus::fcdproplus's
        * constructor is in a private implementation
        * class. fcdproplus::fcdproplus::make is the public interface for
        * creating new instances.
        */
       static sptr make(const std::string device_name = "",int unit =1 );

       /*! \brief Set frequency with resolution defined by unit.
        *  \param freq The frequency in unit Hz
        *
        * Sets the frequency of the Funcube Dongle with Hz or Khz resolution
        * depending on the unit paramater ( 1: Hz , 1000 Khz )
        * applying the frequency correction set by set_freq_corr().
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

       /*! \brief Set new frequency correction.
        *  \param ppm The new frequency correction in parts per million
        *
        * Funcube recommends 0 for the Pro+.
        *
        * Ref: http://www.funcubedongle.com/MyImages/FCD2ManualV4.pdf
        */
       virtual void set_freq_corr(int ppm) = 0;

       /*! \brief Set If gain.
        *  \param gain The If gain value between 0 and 59 db
        *
        */
       virtual void set_if_gain(int gain)=0;

    };

  } // namespace fcdproplus
} // namespace gr

#endif /* INCLUDED_FCDPROPLUS_FCDPROPLUS_H */

