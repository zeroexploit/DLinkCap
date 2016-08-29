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

#include "recorder.hpp"

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
    this->videoCodec = "libx264";
    this->videoQuality = 5;
    this->videoStream = "";
    this->outputFormat = "mp4";
}

/**
 * Set the Video URL to record from.
 * @param url Pointer to the URL to use
 * @return True if set succesfully. False if not an HTTP Url.
 */
bool Recorder::setVideoStream(std::string* url)
{
    if(url->compare("-") == 0)
    {
        this->videoStream = "";
        this->recordVideo = false;
        
        return true;
    }

    if(url->compare(0, 7, "http://") == 0)
    {
        this->videoStream = *url;
        this->recordVideo = true;
        
        return true;
    }

    return false;
}
     
/**
 * Set the Audio URL to record from.
 * @param url Pointer to the URL to use
 * @return True if succesfull. False if not an HTTP Url.
 */
bool Recorder::setAudioStream(std::string* url)
{
    if(url->compare("-") == 0)
    {
        this->audioStream = "";
        this->recordAudio = false;
        
        return true;
    }

    if(url->compare(0, 7, "http://") == 0)
    {
        this->audioStream = *url;
        this->recordAudio = true;
        
        return true;
    }

    return false;
}
        
/**
 * Set the Video Codec to use for encoding
 * @param codec Pointer to the Codec to use
 */
void Recorder::setVideoCodec(std::string* codec)
{
    this->videoCodec = *codec;
}
        
/**
 * Set the Audio Codec to use for encoding
 * @param codec Pointer to the Codec to use
 */
void Recorder::setAudioCodec(std::string* codec)
{
    this->audioCodec = *codec;
}
        
/**
 * Set the Path to the File where the Output should be written to
 * @param output Pointer to the Output File Path
 */
void Recorder::setOutputPath(std::string* output)
{
    this->outputPath = *output;
}

/**
 * Set the output File Format. Uses FFMpeg Parameter
 * @param format Pointer to the Output Format as FFmpeg would expect it
 */
void Recorder::setOutputFormat(std::string* format)
{
    this->outputFormat = *format;
}
        
/**
 * Set the Quality to use for Video encoding.
 * @param quality Pointer to the Quality
 */
void Recorder::setVideoQuality(std::string* quality)
{
    this->videoQuality = std::stoi(*quality);
}
   
/**
 * Set the Quality to use for Audio encoding.
 * @param quality Pointer to the Quality to use
 */
void Recorder::setAudioQuality(std::string* quality)
{
    this->audioQuality = std::stoi(*quality);
}
        
/**
 * Set the Recording Time.
 * @param duration Pointer to the Duration
 */
void Recorder::setRecordingTime(std::string* duration)
{
    if(duration->compare("-") == 0)
    {
        this->recordTime = 0;
        return;
    }
    
    std::vector<std::string> durationList = split(*duration, ":");
    
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
 * Replace a Substring inside String.
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
 * @return True if recording was succesfull. False if somethin failed.
 */
bool Recorder::record(void)
{
    if(this->recordVideo)
    {
        std::cout << "Recording Video Stream...\n";
        
        if(this->recordAudio)
        {
            std::thread videoThread(&Recorder::recordVideoStream, this);
            videoThread.detach();
        }
        else
        {
            recordVideoStream();
        }
    }
    
    if(this->recordAudio)
    {
        std::cout << "Recording Audio Stream...\n";
        std::string audio = recordAudioStream();
        
        WaveHandler wavHandler(audio);
        
        if(wavHandler.isEditRequired())
        {
            std::cout << "Editing Wave File to make it work correctly...\n";
            wavHandler.makeFileWorking();
        }
    }
    
    if(this->recordAudio && this->recordVideo)
    {
        std::cout << "Mergin Audio and Video...\n";
        mergeAudioVideo();
        removeTmpFiles();
    }
    
    return true;
}

/**
 * Record the Audio Stream using wget
 */
std::string Recorder::recordAudioStream(void)
{
    char* buff = new char[4096];
    std::string command = "";
     
    if(this->recordTime > 0)
        command = "timeout " + std::to_string(this->recordTime) + " wget -qO- " + this->audioStream;
    else
        command = "wget -qO- " + this->audioStream;
    
    std::string tmpFileName = this->outputPath;
    tmpFileName = tmpFileName.substr(0, tmpFileName.find_last_of("/")) + "/aud_" + tmpFileName.substr(tmpFileName.find_last_of("/") + 1);

    FILE* ffmpeg = popen(command.c_str(), "r");
    FILE* output = fopen(tmpFileName.c_str(), "w");
	
    if(ffmpeg == NULL || output == NULL)
    {
        std::cout << "Error with output!\n";
        return "";
    }
    
    size_t readed;

    while((readed = fread(buff, sizeof(char), sizeof(buff), ffmpeg)) > 0)
    {
        if(fwrite(buff , sizeof(char), readed, output) < 0)
            break;
    }

    pclose(ffmpeg);
    fclose(output);
    delete buff;
    buff = 0;
    
    return tmpFileName;
}

/**
 * Record the Video Stream using FFMpeg.
 */
void Recorder::recordVideoStream(void)
{
   char* buff = new char[4096];
    std::string command = "";
     
    if(this->recordTime > 0)
        command = "timeout " + std::to_string(this->recordTime) + " wget -qO- " + this->videoStream;
    else
        command = "wget -qO- " + this->videoStream;
    
    std::string tmpFileName = this->outputPath;
    tmpFileName = tmpFileName.substr(0, tmpFileName.find_last_of("/")) + "/vid_" + tmpFileName.substr(tmpFileName.find_last_of("/") + 1);

    FILE* ffmpeg = popen(command.c_str(), "r");
    FILE* output = fopen(tmpFileName.c_str(), "w");
	
    if(ffmpeg == NULL || output == NULL)
    {
        std::cout << "Error with output!\n";
        return;
    }
    
    size_t readed;

    while((readed = fread(buff, sizeof(char), sizeof(buff), ffmpeg)) > 0)
    {
        if(fwrite(buff , sizeof(char), readed, output) < 0)
            break;
    }

    pclose(ffmpeg);
    fclose(output);
    delete buff;
    buff = 0;
}

void Recorder::mergeAudioVideo(void)
{
    std::string tmpVideoName = this->outputPath;
    tmpVideoName = tmpVideoName.substr(0, tmpVideoName.find_last_of("/")) + "/vid_" + tmpVideoName.substr(tmpVideoName.find_last_of("/") + 1);
    
    std::string tmpFileName = this->outputPath;
    tmpFileName = tmpFileName.substr(0, tmpFileName.find_last_of("/")) + "/aud_" + tmpFileName.substr(tmpFileName.find_last_of("/") + 1);
    
    std::string command = "ffmpeg -y -loglevel quiet -f mjpeg -framerate 12 -i " + tmpVideoName + " -i " + tmpFileName
            + " -c:v " + this->videoCodec + " -qscale:v " + std::to_string(this->videoQuality) + " -c:a " + this->audioCodec + " -qscale:a " + 
            std::to_string(this->audioQuality) + " -f " +this->outputFormat + " " + this->outputPath;
    
    system(command.c_str());
}

void Recorder::removeTmpFiles(void)
{
    std::string tmpVideoName = this->outputPath;
    tmpVideoName = tmpVideoName.substr(0, tmpVideoName.find_last_of("/")) + "/vid_" + tmpVideoName.substr(tmpVideoName.find_last_of("/") + 1);
    
    std::string tmpFileName = this->outputPath;
    tmpFileName = tmpFileName.substr(0, tmpFileName.find_last_of("/")) + "/aud_" + tmpFileName.substr(tmpFileName.find_last_of("/") + 1);
    
    system(std::string("rm " + tmpVideoName).c_str());
    system(std::string("rm " + tmpFileName).c_str());
}