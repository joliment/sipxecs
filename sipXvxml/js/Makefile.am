## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libjavascript.la

libjavascript_la_CFLAGS = -DGCC_OPT_BUG -DXP_UNIX -DSVR4 -DSYSV -D_BSD_SOURCE -DPOSIX_SOURCE -DX86_LINX -DEDITLINE -DHAVE_VA_COPY -DVA_COPY=va_copy

libjavascript_la_SOURCES= \
    src/jsapi.c     \
    src/jsxml.c     \
    src/jsiter.c    \
    src/jsarena.c   \
    src/jsarray.c   \
    src/jsatom.c    \
    src/jsbool.c    \
    src/jscntxt.c   \
    src/jsdate.c    \
    src/jsdbgapi.c  \
    src/jsdhash.c   \
    src/jsdtoa.c    \
    src/jsemit.c    \
    src/jsexn.c     \
    src/jsfun.c     \
    src/jsgc.c      \
    src/jshash.c    \
    src/jsinterp.c  \
    src/jslock.c    \
    src/jslog2.c    \
    src/jslong.c    \
    src/jsmath.c    \
    src/jsnum.c     \
    src/jsobj.c     \
    src/jsopcode.c  \
    src/jsparse.c   \
    src/jsprf.c     \
    src/jsregexp.c  \
    src/jsscan.c    \
    src/jsscope.c   \
    src/jsscript.c  \
    src/jsstr.c     \
    src/jsutil.c    \
    src/jsxdrapi.c  \
    src/prmjtime.c \
    src/fdlibm/e_acos.c  \
    src/fdlibm/e_acosh.c  \
    src/fdlibm/e_asin.c  \
    src/fdlibm/e_atan2.c  \
    src/fdlibm/e_atanh.c  \
    src/fdlibm/e_cosh.c  \
    src/fdlibm/e_exp.c  \
    src/fdlibm/e_fmod.c  \
    src/fdlibm/e_gamma.c  \
    src/fdlibm/e_gamma_r.c  \
    src/fdlibm/e_hypot.c  \
    src/fdlibm/e_j0.c  \
    src/fdlibm/e_j1.c  \
    src/fdlibm/e_jn.c  \
    src/fdlibm/e_lgamma.c  \
    src/fdlibm/e_lgamma_r.c  \
    src/fdlibm/e_log10.c  \
    src/fdlibm/e_log.c  \
    src/fdlibm/e_pow.c  \
    src/fdlibm/e_remainder.c  \
    src/fdlibm/e_rem_pio2.c  \
    src/fdlibm/e_scalb.c  \
    src/fdlibm/e_sinh.c  \
    src/fdlibm/e_sqrt.c  \
    src/fdlibm/k_cos.c  \
    src/fdlibm/k_rem_pio2.c  \
    src/fdlibm/k_sin.c  \
    src/fdlibm/k_standard.c  \
    src/fdlibm/k_tan.c  \
    src/fdlibm/s_asinh.c  \
    src/fdlibm/s_atan.c  \
    src/fdlibm/s_cbrt.c  \
    src/fdlibm/s_ceil.c  \
    src/fdlibm/s_copysign.c  \
    src/fdlibm/s_cos.c  \
    src/fdlibm/s_erf.c  \
    src/fdlibm/s_expm1.c  \
    src/fdlibm/s_fabs.c  \
    src/fdlibm/s_finite.c  \
    src/fdlibm/s_floor.c  \
    src/fdlibm/s_frexp.c  \
    src/fdlibm/s_ilogb.c  \
    src/fdlibm/s_isnan.c  \
    src/fdlibm/s_ldexp.c  \
    src/fdlibm/s_lib_version.c  \
    src/fdlibm/s_log1p.c  \
    src/fdlibm/s_logb.c  \
    src/fdlibm/s_matherr.c  \
    src/fdlibm/s_modf.c  \
    src/fdlibm/s_nextafter.c  \
    src/fdlibm/s_rint.c  \
    src/fdlibm/s_scalbn.c  \
    src/fdlibm/s_signgam.c  \
    src/fdlibm/s_significand.c  \
    src/fdlibm/s_sin.c  \
    src/fdlibm/s_tan.c  \
    src/fdlibm/s_tanh.c  \
    src/fdlibm/w_acos.c  \
    src/fdlibm/w_acosh.c  \
    src/fdlibm/w_asin.c  \
    src/fdlibm/w_atan2.c  \
    src/fdlibm/w_atanh.c  \
    src/fdlibm/w_cosh.c  \
    src/fdlibm/w_exp.c  \
    src/fdlibm/w_fmod.c  \
    src/fdlibm/w_gamma.c  \
    src/fdlibm/w_gamma_r.c  \
    src/fdlibm/w_hypot.c  \
    src/fdlibm/w_j0.c  \
    src/fdlibm/w_j1.c  \
    src/fdlibm/w_jn.c  \
    src/fdlibm/w_lgamma.c  \
    src/fdlibm/w_lgamma_r.c  \
    src/fdlibm/w_log10.c  \
    src/fdlibm/w_log.c  \
    src/fdlibm/w_pow.c  \
    src/fdlibm/w_remainder.c  \
    src/fdlibm/w_scalb.c  \
    src/fdlibm/w_sinh.c  \
    src/fdlibm/w_sqrt.c 

nobase_include_HEADERS = \
	src/jsapi.h \
	src/jsxml.h \
	src/jsiter.h \
	src/jsarena.h \
	src/jsarray.h \
	src/jsatom.h \
	src/jsbit.h \
	src/jsbool.h \
	src/jsclist.h \
	src/jscntxt.h \
	src/jscompat.h \
	src/jsconfig.h \
	src/jscpucfg.h \
	src/jsdate.h \
	src/jsdbgapi.h \
	src/jsdhash.h \
	src/jsdtoa.h \
	src/jsemit.h \
	src/jsexn.h \
	src/jsfile.h \
	src/jsfun.h \
	src/jsgc.h \
	src/jshash.h \
	src/jsinterp.h \
	src/jslibmath.h \
	src/jslock.h \
	src/jslong.h \
	src/jsmath.h \
	src/jsnum.h \
	src/jsobj.h \
	src/jsopcode.h \
	src/jsosdep.h \
	src/jsotypes.h \
	src/jsparse.h \
	src/jsprf.h \
	src/jsprvtd.h \
	src/jspubtd.h \
	src/jsregexp.h \
	src/jsscan.h \
	src/jsscope.h \
	src/jsscript.h \
	src/jsstddef.h \
	src/jsstr.h \
	src/jstypes.h \
	src/jsutil.h \
	src/jsxdrapi.h \
	src/prmjtime.h \
	src/resource.h \
	src/fdlibm/fdlibm.h

EXTRA_DIST = \
	src/jskeyword.tbl \
	src/jsopcode.tbl \
	src/jsproto.tbl \
	src/js.msg
