// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COBALT_MEDIA_CAST_TEST_UTILITY_NET_UTILITY_H_
#define COBALT_MEDIA_CAST_TEST_UTILITY_NET_UTILITY_H_

#include "net/base/ip_endpoint.h"

namespace cobalt {
namespace media {
namespace cast {
namespace test {

// Determine a unused UDP port for the in-process receiver to listen on.
// Method: Bind a UDP socket on port 0, and then check which port the
// operating system assigned to it.
net::IPEndPoint GetFreeLocalPort();

}  // namespace test
}  // namespace cast
}  // namespace media
}  // namespace cobalt

#endif  // COBALT_MEDIA_CAST_TEST_UTILITY_NET_UTILITY_H_
