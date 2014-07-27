ciFlyCap
========

Based on BanTheRewind's ciFlyCap, a cinder wrapper for FlyCapture2
https://github.com/meshula/BanTheRewind/tree/master/blocks/ciFlyCap

I made a small fix to how the images are grabbed off the camera then converted into the requested format.  Additionally the native FlyCap callback is used rather than a polling thread.

Because I want these images in a 64bit Cinder project, and because FlyCapture only works in Visual Studio 2010, I made an example that uses memory mapped files to communicate.

I tested on Windows 7 using a FireFly MV USB 2.0 camera

This includes FlyCapture2.6.3.4, which was the most up to date version as of 7/20/2014

DEPENCIES:
	
	Visual Studio 2010
	FlyCapture2.6.3.4


NOTES:
	
	I tested on a machine with flycapture installed, so the app knows where to find the .dll
	If you run this on a machine w/o flycapture you'll need to copy the .dll into your path


ISSUES:

	Currently using two CiFlyCap objects crashes.  Unsure what the issue is, but I have an
	example in samples/RawMultiTest that shows multiple cameras fundamentally work fine.