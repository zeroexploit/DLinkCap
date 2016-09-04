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

#include "include/recorder.hpp"

/**
 * Initialise the Recorder with some default Values.
 */
Recorder::Recorder(void)
{
    this->audioCodec = "libmp3lame";
    this->audioQuality = 5;
    this->audioStream = "";
    this->outputPath = "";
    this->recordAudio = false;
    this->recordVideo = false;
    this->videoCodec = "mpeg4";
    this->videoQuality = 0;
    this->videoStream = "";
    this->outputFormat = "mp4";
    this->shouldMerge = true;
    this->deleteTmps = true;
    this->inputFormatVideo = "";
    this->inputFormatAudio = "";
    this->frameRate = 0;
    this->tmpAudioFile = "/tmp/dlinkcap_vid.tmp";
    this->tmpVideoFile = "tmp/dlinkcap_aud.tmp";
    this->recordTime = 0;
    this->shouldEncode = true;
    this->daemonizer = Daemonizer();
}

/**
 * Set the Input Format of the Video and Audio Stream. Usefull to avoid wrong
 * guiessing from FFmpeg. Has no effect on recording but on mergin.
 * @param audioFormat the Audio Format used
 * @param videoFormat the Video Format used
 */
void Recorder::setInputFormat(std::string audioFormat, std::string videoFormat)
{
    if(audioFormat.length() > 0)
        this->inputFormatAudio = audioFormat;
    
    if(videoFormat.length() > 0)
        this->inputFormatVideo = videoFormat;
}

/**
 * Set the Framerate of the Input Stream. Most likely the Video Stream won't
 * run with the correct speed if FFmpeg gueses this by itself.
 * @param inputFramerate Number of Frames per Second of the Input Video Stream
 */
void Recorder::setInputFramerate(std::string inputFramerate)
{
    this->frameRate = std::stoi(inputFramerate);
}

/**
 * Set if the Audio and Video Recordings should be merged together to a Single
 * Audio / Video File. 
 * @param shouldMerge True to merge Audio / Video. False to keep two seperate Files
 */
void Recorder::setShouldMerge(bool shouldMerge)
{
    this->shouldMerge = shouldMerge;
}

/**
 * Set if the Raw Audio and Video Recordings should encoded. Makes sense when 
 * not merged.
 * @param shouldEncode True Encodes the Single Files. False keeps them Raw.
 */
void Recorder::setShouldEncode(bool shouldEncode)
{
    this->shouldEncode = shouldEncode;
}

/**
 * Set if the two seperate recorded Raw Files (Audio and Video) should be 
 * deleted after mergin them or keept for other purposes.
 * @param deleteTmps True to delete the raw Files. False to keep them after mergin.
 */
void Recorder::setDeleteTmps(bool deleteTmps)
{
    this->deleteTmps = deleteTmps;
}

/**
 * Set the Video URL to record from.
 * @param url the URL to use
 * @return True if set succesfully. False if not an HTTP Url.
 */
bool Recorder::setVideoStream(std::string url)
{
    if(url.compare("-") == 0)
    {
        this->videoStream = "";
        this->recordVideo = false;
        
        return true;
    }

    if(url.compare(0, 7, "http://") == 0)
    {
        this->videoStream = url;
        this->recordVideo = true;
        
        return true;
    }

    return false;
}
     
/**
 * Set the Audio URL to record from.
 * @param url the URL to use
 * @return True if succesfull. False if not an HTTP Url.
 */
bool Recorder::setAudioStream(std::string url)
{
    if(url.compare("-") == 0)
    {
        this->audioStream = "";
        this->recordAudio = false;
        
        return true;
    }

    if(url.compare(0, 7, "http://") == 0)
    {
        this->audioStream = url;
        this->recordAudio = true;
        
        return true;
    }

    return false;
}
        
/**
 * Set the Video Codec to use for encoding
 * @param codec the Codec to use
 */
void Recorder::setVideoCodec(std::string codec)
{
    this->videoCodec = codec;
}
        
/**
 * Set the Audio Codec to use for encoding
 * @param codec the Codec to use
 */
void Recorder::setAudioCodec(std::string codec)
{
    this->audioCodec = codec;
}
        
/**
 * Set the Path to the File where the Output should be written to
 * @param output the Output File Path
 */
void Recorder::setOutputPath(std::string output)
{
    this->outputPath = output;
}

/**
 * Set the output File Format. Uses FFMpeg Parameter
 * @param format the Output Format as FFmpeg would expect it
 */
void Recorder::setOutputFormat(std::string format)
{
    this->outputFormat = format;
}
        
/**
 * Set the Quality to use for Video encoding.
 * @param quality the Quality
 */
void Recorder::setVideoQuality(std::string quality)
{
    this->videoQuality = std::stoi(quality);
}
   
/**
 * Set the Quality to use for Audio encoding.
 * @param quality the Quality to use
 */
void Recorder::setAudioQuality(std::string quality)
{
    this->audioQuality = std::stoi(quality);
}
        
/**
 * Set the Recording Time.
 * @param duration the Duration
 */
void Recorder::setRecordingTime(std::string duration)
{
    if(duration.compare("-") == 0)
    {
        this->recordTime = 0;
        return;
    }
    
    std::vector<std::string> durationList = split(duration, ":");
    
    if(durationList.size() >= 1)
        this->recordTime += std::stol(durationList.at(durationList.size() - 1));
    
    if(durationList.size() >= 2)
        this->recordTime += std::stol(durationList.at(durationList.size() - 2)) * 60;
    
    if(durationList.size() >= 3)
        this->recordTime += std::stol(durationList.at(durationList.size() - 3)) * 60 * 60;
}

/**
 * Split a String at the given delimiter and  remove the delimiter from the String
 * @param str String to Split
 * @param delim Delimiter where to split
 * @return Vector containing the splitted String
 */
std::vector<std::string> Recorder::split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> tokens;
    
    if(str.size() > 0)
    {
        if(delim.size() > 0)
        {
            std::string::size_type currPos = 0, prevPos = 0;
            
            while((currPos = str.find(delim, prevPos)) != std::string::npos)
            {
                std::string item = str.substr(prevPos, currPos - prevPos);
                
                if(item.size() > 0)
                {
                    replace(item, delim, "");
                    tokens.push_back(item);
                }
                
                prevPos = currPos + 1;
            }
            
            std::string tmp = str.substr(prevPos);
            replace(tmp, delim, "");
            tokens.push_back(tmp);
        }
        else
        {
            std::string tmp(str);
            replace(tmp, delim, "");
            tokens.push_back(tmp);
        }
    }
    
    return tokens;
}

/**
 * Replace a Substring inside a String.
 * @param str String to seach in
 * @param oldStr String to search for
 * @param newStr String to replace with
 */
void Recorder::replace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
    size_t pos = 0;

    while((pos = str.find(oldStr, pos)) != std::string::npos)
    {
      str.replace(pos, oldStr.length(), newStr);
      pos += newStr.length();
    }
}

/**
 * Start recording with the given Parameters.
 */
void Recorder::record(void)
{
    std::cout << "Start recording...\n";
    
    if(this->recordAudio == false && this->recordVideo == false)
    {
        std::cout << "No Video AND No Audio Stream given! Nothing to do here...\n";
        return;
    }
    
    if(this->recordVideo)
    {
        std::cout << "Recording Video Stream...\n";
        
        // If Video AND Audio should be recorded -> Run a new Thread for parallel recording
        if(this->recordAudio)
        {
            this->videoThread = std::thread(&Recorder::recordVideoStream, this);
            this->videoThread.detach();
        }
        else
        {
            recordVideoStream();
        }
    }
    
    if(this->recordAudio)
    {
        std::cout << "Recording Audio Stream...\n";
        recordAudioStream();
        
        std::cout << "Editing Wave File to make it work correctly...\n";
        WaveHandler wavHandler(this->tmpAudioFile);
        wavHandler.makeFileWorking();
        
        // Wait 10 Seconds to let the other Thread finish -> Sync it in the Future to avoid this
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    
    if(this->shouldMerge)
    {
        std::cout << "Merge Audio and Video...\n";
        mergeAudioVideo();
        std::cout << "Output written to: " + this->outputPath + "\n";
    }
    else
    {
        std::cout << "Mergin disabled...\n";
        
        if(this->recordAudio && this->shouldEncode)
        {
            std::cout << "Encoding Audio...\n";

            bool tmpBool = this->recordVideo;
            this->recordVideo = false;
            std::string tmpPath = this->outputPath;
            this->outputPath = this->outputPath + ".audio";
            
            mergeAudioVideo();
            std::cout << "Audio File written: " + this->outputPath + "\n";
            
            this->recordVideo = tmpBool;
            this->outputPath = tmpPath;           
        }
        else
        {
            std::cout << "No Encoding is performed...\nRaw-Audio File written: " + this->tmpAudioFile + "\n";
        }
        
        if(this->recordVideo && this->shouldEncode)
        {
            std::cout << "Encoding Video...\n";
            
            bool tmpBool = this->recordAudio;
            this->recordAudio = false;
            std::string tmpPath = this->outputPath;
            this->outputPath = this->outputPath + ".video";;
            
            mergeAudioVideo();
            
            std::cout << "Video File written: " + this->outputPath + "\n";
            
            this->recordAudio = tmpBool;
            this->outputPath = tmpPath;
        }
        else
        {
            std::cout << "No Encoding is performed...\nRaw-Video File written: " + this->tmpVideoFile + "\n";
        }
    }
    
    if(this->deleteTmps)
    {
        std::cout << "Deleting temporary Files...\n";
                
        removeTmpFiles();
    }
        
    std::cout << "Recording done!\n";
}

/**
 * Record the Audio Stream using wget
 */
void Recorder::recordAudioStream(void)
{
    std::vector<std::string> argvs;
    std::string command = "";
     
    if(this->recordTime > 0)
    {
        command = "timeout";
        argvs.push_back(std::to_string(this->recordTime));
        argvs.push_back("wget");
    }
    else
        command = "wget";
    
    argvs.push_back("-qO-");
    argvs.push_back(this->audioStream);
    
    this->tmpAudioFile = this->outputPath;
    this->tmpAudioFile = this->tmpAudioFile.substr(0, this->tmpAudioFile.find_last_of("/")) + "/aud_" + this->tmpAudioFile.substr(this->tmpAudioFile.find_last_of("/") + 1, this->tmpAudioFile.find_last_of(".")) + ".raw";
    
    argvs.push_back(">");
    argvs.push_back(this->tmpAudioFile);

    if(!this->daemonizer.runExternal(command, argvs, true))
        std::cout << "There was an error recording the Audio Stream!\n";
    else
        std::cout << "Audio Record complete!\n";
}

/**
 * Record the Video Stream using FFmpeg. No Encoding is performed while
 * capturing.
 */
void Recorder::recordVideoStream(void)
{
    std::string command = "ffmpeg";
    std::vector<std::string> argvs;
    
    argvs.push_back("-y");
    argvs.push_back("-loglevel");
    argvs.push_back("quiet");
    
    if(this->inputFormatVideo.length() > 0)
    {
        argvs.push_back("-f");
        argvs.push_back(this->inputFormatVideo);
    }
            
    if(this->frameRate > 0)
    {
        argvs.push_back("-framerate");
        argvs.push_back(std::to_string(this->frameRate));
    }

    argvs.push_back("-re");
    argvs.push_back("-use_wallclock_as_timestamps");
    argvs.push_back("1");
    argvs.push_back("-i");
    argvs.push_back(this->videoStream);
    argvs.push_back("-an");
    argvs.push_back("-c:v");
    argvs.push_back("copy");
    
    this->tmpVideoFile = this->outputPath;
    this->tmpVideoFile = this->tmpVideoFile.substr(0, this->tmpVideoFile.find_last_of("/")) + "/vid_" + this->tmpVideoFile.substr(this->tmpVideoFile.find_last_of("/") + 1, this->tmpVideoFile.find_last_of(".")) + ".raw";
    
    if(this->recordTime > 0)
    {
        argvs.push_back("-t");
        argvs.push_back(std::to_string(this->recordTime));
    }
    
    argvs.push_back("-f");
    argvs.push_back(this->outputFormat);
    argvs.push_back(this->tmpVideoFile);

    if(!this->daemonizer.runExternal(command, argvs, true))
       std::cout << "There was an error recording the Video Stream!\n";
    else
        std::cout << "Video Record complete!\n";
}

/**
 * Calls FFmpeg to merge and encode the recorded Streams together into a Single
 * File. Or just encode a single File.
 */
void Recorder::mergeAudioVideo(void)
{
    std::string ffmpeg = "ffmpeg";
    std::vector<std::string> argvs;
    
    argvs.push_back("-y");
    argvs.push_back("-loglevel");
    argvs.push_back("quiet");
    
    if(this->recordVideo)
    {
        argvs.push_back("-i");
        argvs.push_back(this->tmpVideoFile);
    }
    
    if(this->inputFormatAudio.length() > 0 && this->recordAudio)
    {
        argvs.push_back("-f");
        argvs.push_back(this->inputFormatAudio);
    }
    
    if(this->recordAudio)
    {
        argvs.push_back("-i");
        argvs.push_back(this->tmpAudioFile);
    }
    else
    {
        argvs.push_back("-an");
    }
    
    if(this->recordVideo)
    {
        argvs.push_back("-c:v");
        argvs.push_back(this->videoCodec);
    
        if(this->videoCodec.compare("copy") != 0 && this->videoQuality != 0)
        {
            // If libx264 is used qscale doesn't work and is replaced with crf
            if(this->videoCodec.compare("libx264") == 0)
            {
                argvs.push_back("-crf");
                argvs.push_back(std::to_string(this->videoQuality));
            }
            else
            {
                argvs.push_back("-qscale:v");
                argvs.push_back(std::to_string(this->videoQuality));
            }
        }
    }
    
    if(this->recordAudio)
    {
        argvs.push_back("-c:a");
        argvs.push_back(this->audioCodec);
        argvs.push_back("-qscale:a");
        argvs.push_back(std::to_string(this->audioQuality));
    }
    
    argvs.push_back("-f");
    argvs.push_back(this->outputFormat);
    argvs.push_back(this->outputPath);
    
    this->daemonizer.runExternal(ffmpeg, argvs, true);
}

/**
 * Removes the temporary Files containing the captured raw Streams.
 */
void Recorder::removeTmpFiles(void)
{
    std::remove(this->tmpVideoFile.c_str());
    std::remove(this->tmpAudioFile.c_str());
}