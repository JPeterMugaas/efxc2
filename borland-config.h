#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#define PROGRAM_DESCRIPTION "Enhanced fxc2"
#define PROGRAM_VERSION "0.0.13.255"
#define PROGRAM_COPYRIGHT "Copyright (c) 2024 J. Peter Mugaas"

#define USE_FMT_LIBRARY
#ifdef USE_FMT_LIBRARY
#define M_FORMAT fmt::format
#else
#define M_FORMAT std::format
#endif
#endif   /*CONFIG_H_INCLUDED*/
