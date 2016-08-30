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

#include "wavehandler.hpp"

WaveHandler::WaveHandler(std::string path)
{
    this->pathToSource = path;
}

bool WaveHandler::isEditRequired(void)
{
    return true;
}

void WaveHandler::makeFileWorking(void)
{
    std::ifstream fileIn(this->pathToSource.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
    uint32_t dataSizeRiff = fileIn.tellg() - 8; 
    uint32_t dataSizeData = fileIn.tellg() - 44; 
    
    fileIn.seekg(4);
    uint32_t tmpRiff = 0;
    uint32_t tmpData = 0;
    fileIn.read(reinterpret_cast<char*>(&tmpRiff), sizeof(uint32_t));
    fileIn.seekg(40);
    fileIn.read(reinterpret_cast<char*>(&tmpData), sizeof(uint32_t));
    
    std::ofstream fileOut((this->pathToSource + ".edited").c_str(), std::ios::out|std::ios::binary);
    std::string riff = "RIFF";
    std::string wave = "WAVE";
    std::string fmt = "fmt ";
    uint32_t fmtLength = 16;
    
    fileIn.seekg(0);
    uint32_t tmpShit = 0;
    fileIn.read(reinterpret_cast<char*>(&tmpShit), 4);
    fileIn.read(reinterpret_cast<char*>(&tmpShit), 4);
    fileIn.read(reinterpret_cast<char*>(&tmpShit), 4);
    fileIn.read(reinterpret_cast<char*>(&tmpShit), 4);
    fileIn.read(reinterpret_cast<char*>(&tmpShit), 4);
    
    int format = 1;
    int channels = 1;
    int rate = 16000;
    int block = (16 * 1) / 8;
    int bits = 16;
    int bytesS = rate * block;
    fileIn.read(reinterpret_cast<char*>(&format), 2);
    fileIn.read(reinterpret_cast<char*>(&channels), 2);
    fileIn.read(reinterpret_cast<char*>(&rate), 4);
    fileIn.read(reinterpret_cast<char*>(&bytesS), 4);
    fileIn.read(reinterpret_cast<char*>(&block), 2);
    fileIn.read(reinterpret_cast<char*>(&bits), 2);
    fileIn.read(reinterpret_cast<char*>(&tmpShit), 4);
    fileIn.read(reinterpret_cast<char*>(&tmpShit), 4);
    std::string data = "data";

    // RIFF
    fileOut.write(reinterpret_cast<const char *>(riff.c_str()), 4);
    fileOut.write(reinterpret_cast<const char *>(&dataSizeRiff), 4);
    fileOut.write(reinterpret_cast<const char *>(wave.c_str()), 4);
    
    // FMT
    fileOut.write(reinterpret_cast<const char *>(fmt.c_str()), 4);
    fileOut.write(reinterpret_cast<const char *>(&fmtLength), 4);
    fileOut.write(reinterpret_cast<const char *>(&format), 2);
    fileOut.write(reinterpret_cast<const char *>(&channels), 2);
    fileOut.write(reinterpret_cast<const char *>(&rate), 4);
    fileOut.write(reinterpret_cast<const char *>(&bytesS), 4);
    fileOut.write(reinterpret_cast<const char *>(&block), 2);
    fileOut.write(reinterpret_cast<const char *>(&bits), 2);
    
    // Data
    fileOut.write(reinterpret_cast<const char *>(data.c_str()), 4);
    fileOut.write(reinterpret_cast<const char *>(&dataSizeData), 4);
    
    int tmpBit = 0;
    
    fileIn.seekg(44);
    
    while(fileIn.tellg() < dataSizeData)
    {
        fileIn.read(reinterpret_cast<char*>(&tmpBit), block);
        fileOut.write(reinterpret_cast<const char *>(&tmpBit), block);
    }
    
    fileIn.close();
    fileOut.close();
    
    system(("rm " + this->pathToSource).c_str());
    system(("mv " + this->pathToSource + ".edited " + this->pathToSource).c_str());
}