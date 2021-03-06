/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
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
#include "phase_unwrap_impl.h"

#define pi 3.1415926536

namespace gr {
  namespace AMR {

    phase_unwrap::sptr
    phase_unwrap::make(int samples)
    {
      return gnuradio::get_initial_sptr
        (new phase_unwrap_impl(samples));
    }

    /*
     * The private constructor
     */
    phase_unwrap_impl::phase_unwrap_impl(int samples)
      : gr::sync_block("phase_unwrap",
              gr::io_signature::make(1, 1, sizeof(float)*samples),
              gr::io_signature::make(1, 1, sizeof(float)*samples)),
        p_vlen(samples)
    {
        set_max_noutput_items(1);
    }

    /*
     * Our virtual destructor.
     */
    phase_unwrap_impl::~phase_unwrap_impl()
    {
    }

    int
    phase_unwrap_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];

        float mod_sum = 0;

/*
        out[p_vlen-1] = in[p_vlen-1];
        for (int i = p_vlen-2; i >= 0; i--)
        {
            if (out[i+1] > in[i] + 3.1415926536 + mod_sum)
            {
                mod_sum += 2*3.1415926536;
                out[i] = in[i] + mod_sum;
            }
            else if (out[i+1] < in[i] - 3.1415926536 + mod_sum)
            {
                mod_sum -= 2*3.1415926536;
                out[i] = in[i] + mod_sum;
            }
            else
            {
                out[i] = in[i] + mod_sum;
            }
        }
*/

        out[0] = in[0];
        for (int i = 1; i < p_vlen; i++)
        {
            if (out[i-1] > in[i] + pi + mod_sum)
                mod_sum += 2*pi;
            else if (out[i-1] < in[i] - pi + mod_sum)
                mod_sum -= 2*pi;

            out[i] = in[i] + mod_sum;

        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace AMR */
} /* namespace gr */

