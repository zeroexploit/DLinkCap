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
-vi : Input Format of the Video Stream (mjpeg)
-vc : Video Codec as FFmpeg would take it (mpeg4)
-vq : Video Quality as FFMpeg Scale for the selected Video Codec (5)
-vf : Input Framerate of the Video Stream (-not set-)
-ai : Input Format of the Audio Stream (-not set-)
-ac : Audio Codec as FFmpeg would take it (libmp3lame)
-aq : Audio Quality as FFMpeg Scale for the selected Audio Codec (5)
-t  : Set the recording Duration in hh:mm:ss (-until user interrupt-)
-nm : Do not merge recorded raw Streams (-off-)
-dd : Do not delete the recorded raw Files (-off-)

# Capture Intervalls
If you want to use DLinkCap to capture in Intervalls in a specifig period of time, your only Chance is to use 
it together with Crontabs. At least for now.
