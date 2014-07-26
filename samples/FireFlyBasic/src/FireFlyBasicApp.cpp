#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "ciFlyCap.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FireFlyBasicApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

  protected:
	ciFlyCap		mCapture;
	gl::Texture		mTexture;
};

void FireFlyBasicApp::setup()
{
	// startup
	mCapture = ciFlyCap(640, 480);
	if (mCapture.connect()) {
		mCapture.setPixelFormat(PixelFormat::PIXEL_FORMAT_RGB8);
		mCapture.start();
	}
}

void FireFlyBasicApp::mouseDown( MouseEvent event )
{
}

void FireFlyBasicApp::update()
{	
	if (mCapture.checkNewFrame()) {
		mTexture = gl::Texture(mCapture.getSurface());
	}
}

void FireFlyBasicApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	if(mTexture) {
		gl::draw(mTexture);
	}
}

CINDER_APP_NATIVE( FireFlyBasicApp, RendererGl )
