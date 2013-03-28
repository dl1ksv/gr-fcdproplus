/* -*- c++ -*- */

#define FCDPROPLUS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "fcdproplus_swig_doc.i"

%{
#include "fcdproplus/fcdproplus.h"
%}


%include "fcdproplus/fcdproplus.h"
GR_SWIG_BLOCK_MAGIC2(fcdproplus, fcdproplus);
