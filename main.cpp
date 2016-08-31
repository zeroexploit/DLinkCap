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

#include "include/main.hpp"

/**
 * Main Entry Point
 * @param argc Number of Arguments passed
 * @param argv List of provided Arguments
 * @return Default Exit Codes
 */
int main(int argc, char** argv) 
{
    std::cout << "DLink Camera Capture Tool\nAuthor: Joern Roddelkopf\nProvided under GNU GPL v3\nCheck: https://github.com/zeroexploit/DLinkCap for Updates etc.\n\n";
    std::cout << "Usage:\n" << "dlinkcap <Video Stream | -> <Audio Stream | -> <Output Format> <Output File> [OPTIONS]\n\n";
    
    if(argc <= 1)
    {
        printHelpScreen();
        return 0;
    }
    
    Daemonizer daemonizer = Daemonizer();
    Recorder recorder = Recorder();
    bool runAsDaemon = false;
    
    for(int i = 0; i < argc; i++)
    {
        std::string argvString = std::string(argv[i]);
        
        if(argvString.compare("-h") == 0 || argvString.compare("--help") == 0)
        {
            printHelpScreen();
            return 0;
        }
        
        if(i == 1)
        {
            if(!recorder.setVideoStream(argvString))
            {
                std::cout << "Invalid Video URL! Exiting...\n";
                return 0;
            }
        }
        else if(i == 2)
        {
            if(!recorder.setAudioStream(argvString))
            {
                std::cout << "Invalid Audio URL! Exiting...\n";
                return 0;
            }
        }
        else if(i == 3)
            recorder.setOutputFormat(argvString);
        else if(i == 4)
            recorder.setOutputPath(argvString);
        
        if(i + 1 < argc && argvString.compare("-vi") == 0)
            recorder.setInputFormat("", std::string(argv[i + 1]));
        else if(i + 1 < argc && argvString.compare("-vc") == 0)
            recorder.setVideoCodec(std::string(argv[i + 1]));
        else if(i + 1 < argc && argvString.compare("-vq") == 0)
            recorder.setVideoQuality(std::string(argv[i + 1]));
        else if(i + 1 < argc && argvString.compare("-vf") == 0)
            recorder.setInputFramerate(std::string(argv[i + 1]));
        else if(i + 1 < argc && argvString.compare("-ai") == 0)
            recorder.setInputFormat(std::string(argv[i + 1]), "");
        else if(i + 1 < argc && argvString.compare("-ac") == 0)
            recorder.setAudioCodec(std::string(argv[i + 1]));
        else if(i + 1 < argc && argvString.compare("-aq") == 0)
            recorder.setAudioQuality(std::string(argv[i + 1]));
        else if(i + 1 < argc && argvString.compare("-t") == 0)
            recorder.setRecordingTime(std::string(argv[i + 1]));
        
        if(argvString.compare("-nm") == 0)
            recorder.setShouldMerge(false);
        else if(argvString.compare("-dd") == 0)
            recorder.setDeleteTmps(false);
        else if(argvString.compare("-d") == 0)
            runAsDaemon = true;
    }
    
    if(runAsDaemon)
        daemonizer.daemonize();
    
    recorder.record();
    
    return 0;
}

/**
 * Output the Help / Information Text
 */
void printHelpScreen(void)
{
    std::cout << "Options:\n"
                    "-vi : Input Format of the Video Stream (mjpeg)\n"
                    "-vc : Video Codec as FFmpeg would take it (mpeg4)\n"
                    "-vq : Video Quality as FFMpeg Scale for the selected Video Codec (5)\n"
                    "-vf : Input Framerate of the Video Stream (12)\n"
                    "-ai : Input Format of the Audio Stream (s16le)"
                    "-ac : Audio Codec as FFmpeg would take it (libmp3lame)\n"
                    "-aq : Audio Quality as FFMpeg Scale for the selected Audio Codec (5)\n"
                    "-t  : Set the recording Duration in hh:mm:ss (01:30:00)\n"
                    "-nm : Do not merge recorded raw Streams\n"
                    "-dd : Do not delete the recorded raw Files (may takes a lot of disc space)\n"
                    "-d  : Run as Daemon in the Background\n\n";
    std::cout << "Purpose:\nCapture Video and Audio from DLink Cameras and merge them into one File with the given Codecs and Quality.\n\n";
    std::cout << "Description:\nThis Tool is designed for DLink (or any other compatible) Cameras that doesn\'t provide a single Stream with Video and Audio, like the DSC-932L. "
                    "It will record the Video and Audio separatly, using the HTTP Protocoll and merge them together at the End of the Recording. As some of the DLink Devices provide "
                    "a wrong WAVE File Header within the Audio Stream causing other Programs to stop after about 32 Seconds, this Tool will rebuild the Header in order to make "
                    "recording of Sound work as desired. The Recording of the Streams is captured as Raw "
                    "Data and will be encoded and merged at the End of the Recording. Input-, Codec- and Quality Paramter must be provided as FFMpeg Paramters!\n\n";
    std::cout << "Parameters:\nVideo Stream | - :\tHttp Address of the Video Stream or \"-\" if no Video should be recorded\nAudio Stream | - :\tHttp Address of the Audio Stream or "
                    "\"-\" if no Audio should be recorded\nVideo Codec | copy :\tVideo Codec to use for encoding as FFmpeg would use it (e.g. libx264) or copy if the original Stream should "
                    "be copied instead of reencoded\nVideo Quality :\t\tQuality of the Video. Use the FFmpeg specifig scale (e.g. 0-51) for the selected Codec\nAudio Codec | copy :\t"
                    "The Audio Codec that should be used as you would parse ist to FFMpeg (e.g. libmp3lame) or copy it the original should be keelt instead of reencoding it\n"
                    "Audio Quality :\t\tQuality of the Audio. Use the FFmpeg specifig scale (e.g. 0 - 9) for the selected Audio Codec\nDuration :\t\tHow long to record (e.g. 01:00:00 "
                    "will record for an hour) or not set will record until the Users terminates the Tool\nOutput File :\t\tPath to the Output File that should be written.\n"
                    "Output Format :\t\t Which Container Format will be used as FFmpeg Format\n\n";
    std::cout << "Dependencies:\n- FFMpeg\n- pThreads\n- wget\n";
}