#ifndef COIN_GLUE_CG_H
#define COIN_GLUE_CG_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

#if !defined(HAVE_DYNAMIC_LINKING) && defined(HAVE_CGLIB)

/* FIXME: static linking with Cg library has not been tested
   yet. 20050125 mortene.*/

#define CGLIB_RUNTIME_LINKING 0

#include <Cg/cg.h>
#include <Cg/cgGL.h>

#else /* HAVE_DYNAMIC_LINKING || !HAVE_CGLIB */

#define CGLIB_RUNTIME_LINKING 1

/*
   We need some Cg structs and defines, so set them up here for
   runtime linking support, as we want to avoid including
   Cg/<whatever>.h.
*/

/* typedef struct _CGprogram * CGprogram; XXX */
typedef void * CGprogram;

/* typedef struct _CGcontext * CGcontext; XXX */
typedef void * CGcontext;

/* typedef struct _CGparameter *CGparameter; XXX */
typedef void * CGparameter;

typedef enum {
  CG_PROFILE_ARBVP1 = 6150,
  CG_PROFILE_ARBFP1 = 7000
} CGprofile;

typedef enum {
  CG_NO_ERROR = 0
} CGerror;

typedef enum {
  CG_SOURCE = 4112
} CGenum;

typedef enum {
  CG_GL_MATRIX_IDENTITY = 0,
  CG_GL_MATRIX_TRANSPOSE = 1,
  CG_GL_MATRIX_INVERSE = 2,
  CG_GL_MATRIX_INVERSE_TRANSPOSE = 3,

  CG_GL_MODELVIEW_MATRIX = 4,
  CG_GL_PROJECTION_MATRIX = 5,
  CG_GL_TEXTURE_MATRIX = 6,
  CG_GL_MODELVIEW_PROJECTION_MATRIX = 7,

  CG_GL_VERTEX = 8,
  CG_GL_FRAGMENT = 9
} CGGLenum;

typedef enum {
  CG_UNKNOWN_TYPE = 0,

  CG_FLOAT = 1045,
  CG_FLOAT1 = 1091,
  CG_FLOAT2 = 1046,
  CG_FLOAT3 = 1047,
  CG_FLOAT4 = 1048,
  CG_FLOAT2x2 = 1054,
  CG_FLOAT3x3 = 1059,
  CG_FLOAT4x4 = 1064,
  CG_SAMPLER1D = 1065,
  CG_SAMPLER2D = 1066,
  CG_SAMPLER3D = 1067,
  CG_SAMPLERRECT = 1068,
  CG_SAMPLERCUBE = 1069
} CGtype;

typedef int CGbool;
typedef void (* CGerrorCallbackFunc)(void);

#endif /* HAVE_DYNAMIC_LINKING */

int cc_cgglue_available(void);

CGcontext glue_cgCreateContext(void);
void glue_cgDestroyContext(CGcontext);
CGbool glue_cgIsContext(CGcontext);
const char * glue_cgGetLastListing(CGcontext);

CGprogram glue_cgCreateProgram(CGcontext, CGenum, const char *, CGprofile,
			       const char *, const char **);
void glue_cgDestroyProgram(CGprogram);
CGbool glue_cgIsProgram(CGprogram);

const char * glue_cgGetProfileString(CGprofile);

CGerror glue_cgGetError(void);
const char * glue_cgGetErrorString(CGerror);
void glue_cgSetErrorCallback(CGerrorCallbackFunc);

CGbool glue_cgIsParameter(CGparameter);
CGtype glue_cgGetParameterType(CGparameter);
CGparameter glue_cgGetNamedParameter(CGprogram, const char *);

const char * glue_cgGetTypeString(CGtype);

CGbool glue_cgGLIsProfileSupported(CGprofile);
void glue_cgGLEnableProfile(CGprofile);
void glue_cgGLDisableProfile(CGprofile);
CGprofile glue_cgGLGetLatestProfile(CGGLenum);

void glue_cgGLLoadProgram(CGprogram);
void glue_cgGLBindProgram(CGprogram);

void glue_cgGLSetParameter1f(CGparameter, float);
void glue_cgGLSetParameter2f(CGparameter, float, float);
void glue_cgGLSetParameter3f(CGparameter, float, float, float);
void glue_cgGLSetParameter4f(CGparameter, float, float, float, float);
void glue_cgGLSetStateMatrixParameter(CGparameter, CGGLenum, CGGLenum);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_CG_H */