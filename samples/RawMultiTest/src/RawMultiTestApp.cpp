#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "FlyCapture2.h"
#include <boost/thread/mutex.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace FlyCapture2;

class RawMultiTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void shutdown();

protected:

	void showError();
	static void onImageGrabbed(Image *pImage, const void *pCallbackData);

	struct callbackData {
		Surface8u targetSurface;
		mutex surfaceMutex;
	};

	BusManager				mBusManager;
	FlyCapture2::Camera		*mCamera1;
	FlyCapture2::Camera		*mCamera2;
	Error					mErr;

	callbackData			mCam1Data;
	callbackData			mCam2Data;

	gl::Texture				mTex1;
	gl::Texture				mTex2;
};

void RawMultiTestApp::setup()
{

	setFrameRate(60);
	setWindowSize(1280, 480);

	PGRGuid	guid;

	//////////////////
	// setup camera 1
	mErr = mBusManager.GetCameraFromIndex(0, &guid);
    if (mErr != PGRERROR_OK) {
		showError();
		quit();
	}

	mCamera1 = new FlyCapture2::Camera();
	mErr = mCamera1->Connect(&guid);
    if (mErr != PGRERROR_OK) {
		showError();
		mCamera1 = 0;
		quit();
	}

	mCam1Data.targetSurface = Surface8u(640, 480, false);
	mErr = mCamera1->StartCapture(onImageGrabbed, &mCam1Data);
    if (mErr != PGRERROR_OK) {
		showError();
		quit();
	}

	//////////////////
	// setup camera 2
	mErr = mBusManager.GetCameraFromIndex(1, &guid);
    if (mErr != PGRERROR_OK) {
		showError();
		quit();
	}

	mCamera2 = new FlyCapture2::Camera();
	mErr = mCamera2->Connect(&guid);
    if (mErr != PGRERROR_OK) {
		showError();
		mCamera2 = 0;
		quit();
	}

	mCam2Data.targetSurface = Surface8u(640, 480, false);
	mErr = mCamera2->StartCapture(onImageGrabbed, &mCam2Data);
    if (mErr != PGRERROR_OK) {
		showError();
		quit();
	}

}

void RawMultiTestApp::onImageGrabbed(Image *pImage, const void *pCallbackData)
{
	Image convertedImage;
	callbackData *data = (callbackData*)pCallbackData;
	pImage->Convert(PixelFormat::PIXEL_FORMAT_RGB8, &convertedImage);

	boost::mutex::scoped_lock lock(data->surfaceMutex);
	memcpy(data->targetSurface.getData(), convertedImage.GetData(), convertedImage.GetCols() * convertedImage.GetRows() * 3);
}

void RawMultiTestApp::shutdown()
{
	if( mCamera1 ) {
		delete mCamera1; 
	}
	if( mCamera2 ) {
		delete mCamera2;
	}
}

// Show error
void RawMultiTestApp::showError()
{
	console() << "ERROR ciFlyCap: " << mErr.GetDescription() << "\n";
}

void RawMultiTestApp::mouseDown( MouseEvent event )
{
}

void RawMultiTestApp::update()
{
	{
		boost::mutex::scoped_lock lock(mCam1Data.surfaceMutex);
		mTex1 = mCam1Data.targetSurface;
	}
	{
		boost::mutex::scoped_lock lock(mCam2Data.surfaceMutex);
		mTex2 = mCam2Data.targetSurface;
	}

}

void RawMultiTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	if( mTex1 ) { gl::draw(mTex1); }
	if( mTex2 ) { gl::draw(mTex2, Vec2i(mTex1.getWidth(), 0)); }
}

CINDER_APP_NATIVE( RawMultiTestApp, RendererGl )
