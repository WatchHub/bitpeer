/*
 Copyright (c) 2013, Tom van der Woerdt
 */

#pragma once
#include <event2/event.h>

ev_uint64_t bp_readvarint(const unsigned char *buffer, size_t *position, size_t length);
struct sockaddr_in6 bp_in4to6(struct sockaddr_in *orig);
