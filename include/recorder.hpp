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
 * File:   recorder.hpp
 * Author: Jörn Roddelkopf
 * Version: 1.3 01.09.2016
 */

#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <string>
#include <string.h>
#include <vector>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "wavehandler.hpp"
#include "daemonizer.h"

/**
 * The Recorder Class will handle the recording of Video / Audio Streams.
 * This is most likely the Main Class of this Tool.
 */
class Recorder
{
    private:
        std::string videoStream;    // Url of the Video Stream
        std::string audioStream;    // Url of the Audio Stream
        std::string videoCodec;     // Video Codec to use (FFMpeg Style)
        std::string audioCodec;     // Audio Codec to use (FFMpeg Style)
        std::string outputPath;     // Path to the Output File
        std::string outputFormat;   // Output Format
        std::string inputFormatVideo;    // Input Format for Video Stream
        std::string inputFormatAudio;    // Input Format for Audio Stream
        std::string tmpAudioFile;   // Path to the temporary Audio File
        std::string tmpVideoFile;   // Path to the temporary Video File
        int frameRate;              // Framerate (Input Stream)
        int videoQuality;           // Video Quality (as FFMpeg Scale)
        int audioQuality;           // Audio Quality (as FFMpeg Scale)
        long recordTime;            // Time to record
        bool recordVideo;           // Record Video Stream?
        bool recordAudio;           // Record Audio Stream?
        bool shouldMerge;           // Merge Audio / Video or keep single Files?
        bool shouldEncode;          // Should the Raw-Files should be encoded?
        bool deleteTmps;            // Delete or keep the temporary recorded Files after mergeing them. Always False if shouldMerge = false.
        std::thread videoThread;    // Thread for capturing the Video
        Daemonizer daemonizer;      // Daemonizer for calling external Tools
        
    private:
        std::vector<std::string> split(const std::string &str, const std::string &delim);
        void replace(std::string& str, const std::string& oldStr, const std::string& newStr);
        void recordVideoStream(void);
        void recordAudioStream(void);
        void mergeAudioVideo(void);
        void removeTmpFiles(void);
        
    public:
        Recorder(void);             // Default Constructor
        bool setVideoStream(std::string url);
        bool setAudioStream(std::string url);
        void setVideoCodec(std::string codec);
        void setAudioCodec(std::string codec);
        void setOutputPath(std::string output);
        void setOutputFormat(std::string format);
        void setVideoQuality(std::string quality);
        void setAudioQuality(std::string quality);
        void setRecordingTime(std::string duration);
        void setShouldMerge(bool shouldMerge);
        void setDeleteTmps(bool deleteTmps);
        void setInputFormat(std::string audioFormat, std::string videoFormat);
        void setInputFramerate(std::string inputFramerate);
        void setShouldEncode(bool shouldEncode);
        void record(void);
};

#endif /* RECORDER_HPP */

