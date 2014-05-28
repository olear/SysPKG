/*
# libPKG
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
# libPKG is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 2.1.
*/
#ifndef PKG_H
#define PKG_H

#include "libPKG_global.h"
#include "common.h"

class LIBPKGSHARED_EXPORT PKG
{
public:
    PKG();
    Common common;
};

#endif // PKG_H
