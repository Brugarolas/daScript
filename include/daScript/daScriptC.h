#pragma once

//if target is not defined, try to auto-detect target
#ifndef _TARGET_SIMD_SSE
    #if __SSE4_1__ || defined(__AVX__) || defined(__AVX2__)
        #define _TARGET_SIMD_SSE 4
    #elif __SSSE3__
        #define _TARGET_SIMD_SSE 3
    #elif defined(__SSE2__) || defined(_M_AMD64) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
        #define _TARGET_SIMD_SSE 2
    #endif
#endif

#if !defined(_TARGET_SIMD_SSE) && !defined(_TARGET_SIMD_NEON)
    #if defined(__ARM_NEON) || defined(__ARM_NEON__)
        #define _TARGET_SIMD_NEON 1
    #endif
#endif

#if defined(_TARGET_SIMD_SSE)
    #include <emmintrin.h>
    typedef __m128 vec4f;
    typedef __m128i vec4i;
#elif defined(_TARGET_SIMD_NEON) // PSP2, iOS
    #include <arm_neon.h>
    typedef float32x4_t vec4f;
    typedef int32x4_t   vec4i;
#else
    #error unsupported target
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dasTextOutputHandle das_text_writer;
typedef struct dasModuleGroupHandle das_module_group;
typedef struct dasFileAccessHandle das_file_access;
typedef struct dasProgram das_program;
typedef struct dasContext das_context;
typedef struct dasFunction das_function;
typedef struct dasError das_error;
typedef struct dasModule das_module;
typedef struct dasNode das_node;

typedef vec4f (das_interop_function) ( das_context * ctx, das_node * node, vec4f * arguments );

void das_initialize();
void das_shutdown();

das_text_writer * das_text_make_printer();
das_text_writer * das_text_make_writer();
void das_text_release ( das_text_writer * output );
void das_text_output ( das_text_writer * output, char * text );

das_module_group * das_modulegroup_make ();
void das_modulegroup_release ( das_module_group * group );

das_file_access * das_fileaccess_make_default (  );
das_file_access * das_fileaccess_make_project ( const char * project_file  );
void das_fileaccess_release ( das_file_access * access );

void das_get_root ( char * root, int maxbuf );

das_program * das_program_compile ( char * program_file, das_file_access * access, das_text_writer * tout, das_module_group * libgroup );
void das_program_release ( das_program * program );
int das_program_err_count ( das_program * program );
int das_program_context_stack_size ( das_program * program );
int das_program_simulate ( das_program * program, das_context * ctx, das_text_writer * tout );
das_error * das_program_get_error ( das_program * program, int index );

void das_error_output ( das_error * error, das_text_writer * tout );
void das_error_report ( das_error * error, char * text, int maxLength );

das_context * das_context_make ( int stackSize );
void das_context_release ( das_context * context );
das_function * das_context_find_function ( das_context * context, char * name );
vec4f das_context_eval_with_catch ( das_context * context, das_function * fun, vec4f * arguments );
char * das_context_get_exception ( das_context * context );

das_module * das_module_create ( char * name );
void das_module_bind_interop_function ( das_module * mod, das_module_group * lib, das_interop_function * fun, char * name, char * cppName, char** args );

int    das_argument_int ( vec4f arg );
float  das_argument_float ( vec4f arg );
char * das_argument_string ( vec4f arg );

vec4f das_result_void ();
vec4f das_result_int ( int r );
vec4f das_result_float ( float r );
vec4f das_result_string ( char * r );

#ifdef __cplusplus
}
#endif
