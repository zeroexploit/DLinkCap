/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   deamonizer.hpp
 * Author: Jörn Roddelkopf
 * Version: 1.0 31.08.2016
 */


#ifndef DAEMONIZER_H
#define DAEMONIZER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <spawn.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>

/**
 * This Class is used to daemonize this Application and to call external
 * Tools like FFmpeg.
 */
class Daemonizer
{
    public:
        bool daemonize(void);                                                           // Daemonize this Application
        bool runExternal(const char* cmd, std::vector<const char*> argvs, bool wait);   // Run an external Application
};

#endif /* DEAMONIZER_H */

