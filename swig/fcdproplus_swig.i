/* -*- c++ -*- */

#define FCDPROPLUS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "fcdproplus_swig_doc.i"

%{
#include "fcdproplus/fcdproplus.h"
#include "fcdproplus/fcdpp_control.h"
#include "fcdproplus/fcd_control.h"
#include "fcdproplus/fcd.h"
%}

%include "fcdproplus/fcdproplus.h"
%include "fcdproplus/fcdpp_control.h"
%include "fcdproplus/fcd_control.h"
%include "fcdproplus/fcd.h"

GR_SWIG_BLOCK_MAGIC2(fcdproplus, fcdproplus);
GR_SWIG_BLOCK_MAGIC2(fcdproplus,fcdpp_control);
GR_SWIG_BLOCK_MAGIC2(fcdproplus,fcd_control);
GR_SWIG_BLOCK_MAGIC2(fcdproplus,fcd);
