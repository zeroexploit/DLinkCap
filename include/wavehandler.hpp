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
 * File:   wavehandler.hpp
 * Author: Jörn Roddelkopf
 * Version: 1.1 30.08.2016
 */

#ifndef WAVEHANDLER_HPP
#define WAVEHANDLER_HPP

#include <string>
#include <fstream>

/**
 * Provides all needed Funcationality to make the recorded Waves work as
 * expected.
 */
class WaveHandler
{
    private:
        std::string pathToSource;       // Path to the Source Wave File
        
    public:
        WaveHandler(std::string path);  // Default Constructor
        void makeFileWorking(void);     // Edit the Wave Header to correct the length Issue
};

#endif /* WAVEHANDLER_HPP */

