// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Platform dependent socket header
///
/// \file   z21/socket.h
/// \author Vincent Hamp
/// \date   19/04/2024

#pragma once

// https://stackoverflow.com/questions/67726142/how-can-i-use-sys-socket-h-on-windows
// https://stackoverflow.com/questions/3531474/socklen-t-undeclared-when-compiling-c-code
#if defined __has_include
#  if __has_include(<arpa/inet.h>)
#    include <arpa/inet.h>
#  endif
#  if __has_include(<sys/socket.h>)
#    include <sys/socket.h>
#  endif
#  if __has_include(<ws2tcpip.h>)
#    include <ws2tcpip.h>
#  endif
#else
#  error "__has_include not supported"
#endif
