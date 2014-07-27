#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "ciFlyCap.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DualTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

  protected:
	ciFlyCap		mCaptureL;
	ciFlyCap		mCaptureR;
	gl::Texture		mTextureL;
	gl::Texture		mTextureR;
};

void DualTestApp::setup()
{
	// startup
	mCaptureL = ciFlyCap(640, 480, 0);
	if (mCaptureL.connect()) {
		mCaptureL.setPixelFormat(PixelFormat::PIXEL_FORMAT_RGB8);
		mCaptureL.start();
	}
	mCaptureR = ciFlyCap(640, 480, 1);
	if (mCaptureR.connect()) {
		mCaptureR.setPixelFormat(PixelFormat::PIXEL_FORMAT_RGB8);
		mCaptureR.start();
	}

	setWindowSize(1280, 480);
}

void DualTestApp::mouseDown( MouseEvent event )
{
}

void DualTestApp::update()
{

	if (mCaptureL.checkNewFrame()) {
		mTextureL = gl::Texture(mCaptureL.getSurface());
	}
	if (mCaptureR.checkNewFrame()) {
		mTextureR = gl::Texture(mCaptureR.getSurface());
	}
}

void DualTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	if(mTextureL) {
		gl::draw(mTextureL);
	}
	if(mTextureR) {
		gl::draw(mTextureR, Vec2i(mTextureL.getWidth(), 0));
	}
}

CINDER_APP_NATIVE( DualTestApp, RendererGl )
