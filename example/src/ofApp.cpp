#include "ofApp.h"
#include "ofxAnyJson.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofJson json = ofxAnyJson::loadJson(ofToDataPath("test.json"));
	ofJson yaml = ofxAnyJson::loadYaml(ofToDataPath("test.yaml"));
	ofJson hjson = ofxAnyJson::loadHjson(ofToDataPath("test.hjson"));
	ofJson toml = ofxAnyJson::loadToml(ofToDataPath("test.toml"));
	ofLog() << "json: " << json;
	ofLog() << "yaml: " << yaml;
	ofLog() << "hjson: " << hjson;
	ofLog() << "toml: " << toml;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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