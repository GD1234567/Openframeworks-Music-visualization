#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){	 

	// load in sounds:
	beat.load("1.wav");
	//ow.load("sounds/ow.mp3");
	//dog.load("sounds/dog.mp3");
	//rooster.load("sounds/rooster.mp3");
	
	// we will bounce a circle using these variables:
	px = 300;
	py = 300;
	vx = 0;
	vy = 0;	

	// the fft needs to be smoothed out, so we create an array of floats
	// for that purpose:
	fftSmoothed = new float[8192];

	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}
	
	nBandsToGet = 256;
	tx = new float[nBandsToGet];//x坐标数组
	ty = new float[nBandsToGet];//y坐标数组
	beat.play();
	for (int i = 0; i < nBandsToGet; i++) {//所有的
		tx[i] = 1120 + cos(r) * 300;//x坐标
		ty[i] = 350 + sin(r) * 300;//y坐标
		r += 2 * PI / nBandsToGet;//增加
	}
}


//--------------------------------------------------------------
void ofApp::update(){
	
	

	// update the sound playing system:
	ofSoundUpdate();	
	
	// (1) we increase px and py by adding vx and vy
	px += vx;
	py += vy;
	
	// (2) check for collision, and trigger sounds:
	// horizontal collisions:
	//if (px < 0){
		//px = 0;
		//vx *= -1;
		//dog.play();
	//} else if (px > ofGetWidth()){
		//px = ofGetWidth();
		//vx *= -1;
		//ow.play();
	//}
	// vertical collisions:
	//if (py < 0 ){
		//py = 0;
		//vy *= -1;
		//rooster.play();
	//} else 
	//if (py > ofGetHeight()){
		//py = ofGetHeight();
		//vy *= -1;
		
	//}
	// (3) slow down velocity:
	//vx 	*= 1f;
	//vy 	*= 1f;

	// (4) we use velocity for volume of the samples:
	float vel = sqrt(vx*vx + vy*vy);
	//ow.setVolume(MIN(vel/5.0f, 1));
	//beat.setVolume(MIN(vel/5.0f, 1));
	//dog.setVolume(MIN(vel/5.0f, 1));
	//rooster.setVolume(MIN(vel/5.0f, 1));

	// (5) grab the fft, and put in into a "smoothed" array,
	//		by taking maximums, as peaks and then smoothing downward
	float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed calue sink to zero:
		//fftSmoothed[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		//if (fftSmoothed[i] < val[i]) 
			fftSmoothed[i] = val[i];
		
	}


}

//--------------------------------------------------------------
void ofApp::draw(){

	w = 0;// 2 * PI;// / nBandsToGet;;
	//ofEnableAlphaBlending();
		//ofSetColor(255,255,255,100);
		//ofDrawRectangle(100,ofGetHeight()-300,5*128,200);
	//ofDisableAlphaBlending();
	
	// draw the fft resutls:
	ofBackground(0,0,0);
	
	float width = (float)(9*128) / nBandsToGet;
	for (int i = 0;i < nBandsToGet; i++){
		int a = 2 * (int)(fftSmoothed[i]*100);
		ofColor c;
		c.setHsb(i*1,255,255);
		ofSetColor(c);
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		//ofDrawRectangle(i*width,0, width,(fftSmoothed[i] * 200));
		for (int j = 0; j < a; j++)
		{
			//fill(j*1.3, 255, 255);
			ofDrawRectangle(i*10, ofGetHeight()-j*6, 8, 4);
		}
	}
	ofSetLineWidth(2);
	//ofTranslate(-1020, -350);
	//ofRotate(-1.5*PI);
	//ofTranslate(1020, 350);
	for (int i = 0; i <nBandsToGet; i++) {//所有的
		ofColor c;
		c.setHsb(i * 1, 255, 255);
		ofSetColor(c);
		
		if (i == nBandsToGet - 1) {//没到边界
			ofDrawLine(tx[i] + fftSmoothed[i] *150*cos(w), ty[i] + fftSmoothed[i] *150*sin(w), tx[0] + fftSmoothed[0] *150*cos(w), ty[0] + fftSmoothed[0] *150*sin(w));//线条
			ofDrawLine(tx[i] - fftSmoothed[i] *150*cos(w), ty[i] - fftSmoothed[i] *150*sin(w), tx[0] - fftSmoothed[0] *150*cos(w), ty[0] - fftSmoothed[0] *150*sin(w));//线条
		}
		else//否则
		{
			ofDrawLine(tx[i] + fftSmoothed[i] *150*cos(w), ty[i] + fftSmoothed[i] *150*sin(w), tx[i + 1] + fftSmoothed[i+1] *150*cos(w), ty[i + 1] + fftSmoothed[i + 1] *150*sin(w));//线条
			ofDrawLine(tx[i] - fftSmoothed[i] *150*cos(w), ty[i] - fftSmoothed[i] *150*sin(w), tx[i + 1] - fftSmoothed[i + 1] *150*cos(w), ty[i + 1] - fftSmoothed[i + 1] *150*sin(w));//线条
		}
		w += 2 * PI / nBandsToGet;//增加
	}
	//
	
	// finally draw the playing circle:

	//ofEnableAlphaBlending();
		//ofSetColor(255,255,255,20);
		//ofDrawCircle(px, py,50);
	//ofDisableAlphaBlending();
	
	//ofSetHexColor(0xffffff);
	//ofDrawCircle(px, py,8);
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	// add into vx and vy a small amount of the change in mouse:
	vx += (x - prevx) / 20.0f;
	vy += (y - prevy) / 20.0f;
	// store the previous mouse position:
	prevx = x;
	prevy = y;
}
 
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	prevx = x;
	prevy = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

