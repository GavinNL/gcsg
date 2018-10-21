#pragma once
#ifndef CGCS_LOG_H
#define CGCS_LOG_H

#if defined GCSG_USE_SPDLOG

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h> // must be included

#define CREATE_LOGGER(a) spdlog::stdout_color_mt(a);

#define _INFO(logger,...)  logger->info(__VA_ARGS__)
#define _DEBUG(logger,...) logger->debug(__VA_ARGS__)

#else

#define CREATE_LOGGER(a) 1

#define _INFO(logger,...)
#define _DEBUG(logger,...)

#endif

#endif



