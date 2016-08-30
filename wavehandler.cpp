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

#include "include/wavehandler.hpp"

/**
 * Default Constructor creates the WaveHandler Object with the given Path to
 * the Source Wave File that should be edited.
 * @param path Path to the Wave File that should be edited
 */
WaveHandler::WaveHandler(std::string path)
{
    this->pathToSource = path;
}

/**
 * Correct the ChunkSize of the Wave File wrongly provided by some DLink 
 * Cameras to make the Duration work properly.
 * The File is opened and the wrong Values are replaced with the real ones.
 */
void WaveHandler::makeFileWorking(void)
{
    // Open Wave File and determine Filesize
    std::fstream fileStream(this->pathToSource.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::in);
    fileStream.seekg(std::ios_base::ate);
    
    const int BYTES_COUNT = sizeof(uint32_t);
    const uint32_t RIFF_OFFSET = 4;
    const uint32_t DATA_OFFSET = 40;
    const uint32_t RIFF_LENGTH = 8;
    const uint32_t DATA_LENGTH = 44;
    
    uint32_t fileSize = fileStream.tellg();
    uint32_t chunkSize = fileSize - RIFF_LENGTH;
    uint32_t dataSize = fileSize - DATA_LENGTH;
    
    // Seek to the 'ChunkSize' in RIFF
    fileStream.seekp(RIFF_OFFSET, std::ios_base::beg);
    fileStream.write(reinterpret_cast<const char *>(&chunkSize), BYTES_COUNT);
    
    // Seekt to the 'ChunkSize' in DATA
    fileStream.seekp(DATA_OFFSET, std::ios_base::beg);
    fileStream.write(reinterpret_cast<const char *>(&dataSize), BYTES_COUNT);
    
    fileStream.close();
}