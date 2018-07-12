# ofxAnyJson

Loads any json-like formats as `ofJson`.

- yaml
- hjson

## Dependency

[ofxJsonUtils](https://github.com/2bbb/ofxJsonUtils)

## Usage

```cpp
#include "ofxAnyJson.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofJson json = ofxAnyJson::loadJson(ofToDataPath("test.json"));
	ofJson yaml = ofxAnyJson::loadYaml(ofToDataPath("test.yaml"));
	ofJson hjson = ofxAnyJson::loadHjson(ofToDataPath("test.hjson"));
	ofLog() << "json: " << json;
	ofLog() << "yaml: " << yaml;
	ofLog() << "hjson: " << hjson;
}
```