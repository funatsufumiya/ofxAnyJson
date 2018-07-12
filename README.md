# ofxAnyJson

Loads any json-like formats as `ofJson`.

- yaml
- hjson
- toml

(inspired by [any-json](https://github.com/any-json/any-json))

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
	ofJson toml = ofxAnyJson::loadToml(ofToDataPath("test.toml"));
	ofLog() << "json: " << json;
	ofLog() << "yaml: " << yaml;
	ofLog() << "hjson: " << hjson;
	ofLog() << "toml: " << toml;
}
```