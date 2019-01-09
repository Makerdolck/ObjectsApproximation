#ifndef __VSN_OPENGL_H
#define __VSN_OPENGL_H

#include "vsn_global.h"
#include "vsn_platform.h"

#ifndef VSN_NO_OPENGL

// Windows always needs this to ensure that APIENTRY gets defined
#if defined(VSN_PLATFORM_WINDOWS)
# include "vsn_windows.h"
#endif

// Note: Mac OSX is a "controlled platform" for OpenGL ABI so we
// use the system provided headers there. Controlled means that the
// headers always match the actual driver implementation so there
// is no possibility of drivers exposing additional functionality
// from the system headers. Also it means that the vendor can
// (and does) make different choices about some OpenGL types. For
// e.g. Apple uses void* for GLhandleARB whereas other platforms
// use unsigned int.
//
// For the "uncontrolled" Windows and Linux platforms we use the
// official Khronos headers. On these platforms this gives
// access to additional functionality the drivers may expose but
// which the system headers do not.

#if defined(VSN_OPENGL_ES_2)
# if defined(VSN_PLATFORM_MAC) // iOS
#  if defined(VSN_OPENGL_ES_3)
#   include <OpenGLES/ES3/gl.h>
#   include <OpenGLES/ES3/glext.h>
#  else
#   include <OpenGLES/ES2/gl.h>
#   include <OpenGLES/ES2/glext.h>
#  endif

/*
   OES_EGL_image_external is not included in the Apple provided
   system headers yet, but we define the missing typedef so that
   the qopenglextensions.cpp code will magically work once Apple
   include the extension in their drivers.
*/
typedef void* GLeglImageOES;

# else // "uncontrolled" ES2 platforms

// In "es2" builds (VSN_OPENGL_ES_2) additional defines indicate if ES
// 3.0 or higher is available. In this case include the corresponding
// header. These are backwards compatible and it should be safe to
// include headers on top of each other, meaning that applications can
// include gl2.h even if gl31.h gets included here.

// This compile time differentation is important inside Qt because,
// unlike desktop GL, GLES is different when it comes to versioning
// and extensions: Standard functions that are new in a given version
// are always available in a version-specific header and are not
// guaranteed to be dynamically resolvable via eglGetProcAddress (and
// are typically not available as extensions even if they were part of
// an extension for a previous version).

#  if defined(VSN_OPENGL_ES_3_1)
#   include <GLES3/gl31.h>
#  elif defined(VSN_OPENGL_ES_3)
#   include <GLES3/gl3.h>
#  else
#   include <GLES2/gl2.h>
#endif

/*
   Some GLES2 implementations (like the one on Harmattan) are missing the
   typedef for GLchar. Work around it here by adding it. The Kkronos headers
   specify GLChar as a typedef to char, so if an implementation already
   provides it, then this doesn't do any harm.
*/
typedef char GLchar;

#  include <vsn_opengles2ext.h>
# endif // VSN_PLATFORM_MAC
#else // non-ES2 platforms
# if defined(VSN_PLATFORM_MAC)
#  include <OpenGL/gl.h>
#  if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
#   define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#   include <OpenGL/gl3.h>
#  endif
#  include <OpenGL/glext.h>
# else
#  define GL_GLEXT_LEGACY // Prevents GL/gl.h from #including system glext.h
#  include <GL/gl.h>
#  include "vsn_openglext.h"
# endif // VSN_PLATFORM_MAC
#endif // VSN_OPENGL_ES_2

// Desktops, apart from Mac OS X prior to 10.7 can support OpenGL 3.
// Desktops, apart from Mac OS X prior to 10.9 can support OpenGL 4.
#if !defined(VSN_OPENGL_ES_2)
# if !defined(VSN_PLATFORM_MAC) || (defined(VSN_PLATFORM_MAC) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7)
#  define VSN_OPENGL_3
#  define VSN_OPENGL_3_2
# endif
# if !defined(VSN_PLATFORM_MAC) || (defined(VSN_PLATFORM_MAC) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_9)
#  define VSN_OPENGL_4
# endif
# if !defined(VSN_PLATFORM_MAC)
#  define VSN_OPENGL_4_3
# endif
#endif

VSN_BEGIN_NAMESPACE


// When all else fails we provide sensible fallbacks - this is needed to
// allow compilation on OS X 10.6
#if !defined(VSN_OPENGL_ES_2)

// OS X 10.6 doesn't define these which are needed below
// OS X 10.7 and later defien them in gl3.h
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif


// This block is copied from glext.h and defines the types needed by
// a few extension classes.

#include <stddef.h>
#ifndef GL_VERSION_2_0
/* GL type for program/shader text */
typedef char GLchar;
#endif

#ifndef GL_VERSION_1_5
/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
#endif

#ifndef GL_ARB_vertex_buffer_object
/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;
#endif

#ifndef GL_ARB_shader_objects
/* GL types for program/shader text and shader object handles */
typedef char GLcharARB;
typedef unsigned int GLhandleARB;
#endif

/* GL type for "half" precision (s10e5) float data in host memory */
#ifndef GL_ARB_half_float_pixel
typedef unsigned short GLhalfARB;
#endif

#ifndef GL_NV_half_float
typedef unsigned short GLhalfNV;
#endif

#ifndef GLEXT_64_TYPES_DEFINED
/* This code block is duplicated in glxext.h, so must be protected */
#define GLEXT_64_TYPES_DEFINED
/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
typedef long int int64_t;
typedef unsigned long int uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && (defined(__GNUC__) || (defined(_MSC_VER) && _MSC_VER >= 1600))
#include <stdint.h>
#elif defined(_WIN32)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/* Fallback if nothing above works */
#include <inttypes.h>
#endif
#endif

#ifndef GL_EXT_timer_query
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
#endif

#ifndef GL_ARB_sync
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;
#endif

#ifndef GL_ARB_cl_event
/* These incomplete types let us declare types compatible with OpenCL's cl_context and cl_event */
struct _cl_context;
struct _cl_event;
#endif

#ifndef GL_ARB_debug_output
typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const GLvoid *userParam);
#endif

#ifndef GL_AMD_debug_output
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,GLvoid *userParam);
#endif

#ifndef GL_KHR_debug
typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const GLvoid *userParam);
#endif

#ifndef GL_NV_vdpau_interop
typedef GLintptr GLvdpauSurfaceNV;
#endif

// End of block copied from glext.h
#endif


// Types that aren't defined in all system's gl.h files.
typedef ptrdiff_t vsngl_GLintptr;
typedef ptrdiff_t vsngl_GLsizeiptr;


#if defined(APIENTRY) && !defined(VSN_OPENGLFUNC_APIENTRY)
#   define VSN_OPENGLFUNC_APIENTRY APIENTRY
#endif

# ifndef VSN_OPENGLFUNC_APIENTRYPTR
#   ifdef VSN_OPENGLFUNC_APIENTRY
#     define VSN_OPENGLFUNC_APIENTRYPTR VSN_OPENGLFUNC_APIENTRY *
#   else
#     define VSN_OPENGLFUNC_APIENTRY
#     define VSN_OPENGLFUNC_APIENTRYPTR *
#   endif
# endif

VSN_END_NAMESPACE

#endif // VSN_NO_OPENGL

#endif // __VSN_OPENGL_H
