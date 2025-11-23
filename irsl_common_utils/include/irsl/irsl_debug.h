#ifndef __IRSL_DEBUG_H__
#define __IRSL_DEBUG_H__

#ifdef _WIN32
#define __PRETTY_FUNCTION__  __FUNCSIG__
#endif

#include <iostream>

#ifdef IRSL_DEBUG
#define IRSL_DEBUG_SIMPLE_NL(args) std::cerr << args
#define IRSL_DEBUG_SIMPLE(args) std::cerr << args << std::endl
#define IRSL_DEBUG_STREAM_NL(args) \
    std::cerr << "[" << __PRETTY_FUNCTION__ << "]" << args
#define IRSL_DEBUG_STREAM(args) \
    std::cerr << "[" << __PRETTY_FUNCTION__ << "]" << args << std::endl
#define IRSL_DEBUG_PRINT() \
    std::cerr << "[" << __PRETTY_FUNCTION__ << "]" << std::endl
#else
#define IRSL_DEBUG_SIMPLE_NL(args)
#define IRSL_DEBUG_SIMPLE(args)
#define IRSL_DEBUG_STREAM_NL(args)
#define IRSL_DEBUG_STREAM(args)
#define IRSL_DEBUG_PRINT()
#endif

#define IRSL_INFO_SIMPLE_NL(args) std::cout << args
#define IRSL_INFO_SIMPLE(args) std::cout << args << std::endl
#define IRSL_INFO_STREAM_NL(args) \
    std::cout << "[" << __PRETTY_FUNCTION__ << "]" << args
#define IRSL_INFO_STREAM(args) \
    std::cout << "[" << __PRETTY_FUNCTION__ << "]" << args << std::endl
#define IRSL_INFO_PRINT() \
    std::cout << "[" << __PRETTY_FUNCTION__ << "]" << std::endl

#define IRSL_ERROR_SIMPLE_NL(args) std::cerr << args
#define IRSL_ERROR_SIMPLE(args) std::cerr << args << std::endl
#define IRSL_ERROR_STREAM_NL(args) \
    std::cerr << "[" << __PRETTY_FUNCTION__ << "]" << args
#define IRSL_ERROR_STREAM(args) \
    std::cerr << "[" << __PRETTY_FUNCTION__ << "]" << args << std::endl
#define IRSL_ERROR_PRINT() \
    std::cerr << "[" << __PRETTY_FUNCTION__ << "]" << std::endl

#endif
