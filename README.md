# DLinkCap
Command Line Tool for recording Video and Audio from Http based Webcam Streams on Linux Systems.

# Description
DLinkCap is a tiny Command Line Tool for recording Audio and / or Video from HTTP based Webcam Streams. Originally intended for 
DLink IP-Cams that can't provide a single Stream containing both Video and Audio. Nevertheless, it is or at least should be 
possible to capture the Streams from any Vendor or Device as long as they are provided through HTTP Protocoll. The Video 
and Audio Stream will be recorded and merged together to a single File.
Additionally, this Tool will rebuild and fix the WAVE Header of the Audio Stream as some of the DLink Cams provide a wrong 
Information about the Playback Length of the Audio, leading to Errors in other Applications.

# When to use DLinkCap
- If you got a DLink Camera (or any other) that can't provide Video and Audio in a single Stream.
- Your Camera supports HTTP as Streaming Protocoll.
- The Audio Stream of your DLink Cam is messed up and got a wrong Length Information. 
- You want a simple Tool to record your Webcam on a Linux System.

# When NOT to use DLinkCap
- If your Webcam provides a Stream including Audio AND Video (RTP Stream for instance) you could easyle capture it with other
Tools out there, but DLinkCap won't work with this.
- If your Webcam requires any other Streaming Protocoll than HTTP. Seriously, nothing else will work!

# Dependencies
DLinkCap requires the following external Programs / Librarys to be installt and available:
- ffmpeg
- wget
- pthread

# Usage
DLinkCap is used as following:

dlinkcap (Video URL) (Audio URL) (Output Format) (Output Path) [Options]

Example:

dlinkcap http://user:pass@192.168.1.11/video.cgi http://user:pass@192.168.1.11/audio.cgi mp4 /var/record.mp4

That will auth with the Webcam at 192.168.11 and record the Video/Audio to /var/record.mp4 using all default Settings. As
these may not fullfill your needs it is possible to use the following Options to specify the Output of DLinkCap. But 
remember: The Order of the first four Parameters is fixed! Don't mix it up with the Options! Stick to the given Order!
However, the Order of the Options doesn't matter and is up to you.

# Options
These are the Options you can use to customize the Output of DLinkCap. The Values at the End represent the default Setting:
- -vi : Input Format of the Video Stream (-auto detected-)
- -vc : Video Codec as FFmpeg would take it (mpeg4)
- -vq : Video Quality as FFMpeg Scale for the selected Video Codec (5)
- -vf : Input Framerate of the Video Stream (-not set-)
- -ai : Input Format of the Audio Stream (-auto detect-)
- -ac : Audio Codec as FFmpeg would take it (libmp3lame)
- -aq : Audio Quality as FFMpeg Scale for the selected Audio Codec (5)
- -t  : Set the recording Duration in hh:mm:ss (-until user interrupt-)
- -nm : Do not merge recorded raw Streams (-off-)
- -dd : Do not delete the recorded raw Files (-off-)
- -d  : Run as Daemon

Example:

dlinkcap http://user:pass@192.168.1.11/video.cgi http://user:pass@192.168.1.11/audio.cgi mp4 /var/record.mp4 -vi mjpeg -vc libx264 -vq 24 -vf 20 -ai s26le -ac libmp3 -aq 2 -t 01:30:00

In this Example, DLinkCap will record for 1 hour and 30 minutes. The mjpeg Video Input Stream will be encoded with x264 Codec (Quality 24) and contains 20 Frames per Second. The Audio s16le (16 Bit PCM - Little Endian) will be converted to Mp3 with a Quality Setting of 2. The Output File will be an MP4 at the given location. I would highly recommend to avoid the input format specifiers as they might mess up the result if set incorrectly. DLinkCap should be able to detect them in most of the cases.

# Capture Intervalls
If you want to use DLinkCap to capture in Intervalls in a specifig period of time, your only Chance is to use 
it together with Crontabs. At least for now.
