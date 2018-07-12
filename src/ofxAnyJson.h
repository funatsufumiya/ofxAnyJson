#pragma once

#include "ofxJsonUtils.h"
#include "hjson.h"
#include "yaml.h"

namespace ofx {
	namespace AnyJson {

		class ofxAnyJson {
		public:
			static ofJson loadHjson(const std::string &path) {
				ofFile file(path, ofFile::ReadOnly, false);
				ofBuffer buf = file.readToBuffer();
				auto str = buf.getData();

				Hjson::Value hjson = Hjson::Unmarshal(str);

				return hjsonToJson(hjson);
			}

			static ofJson loadYaml(const std::string &path) {
				YAML::Node yaml = YAML::LoadFile(path);
				return yamlToJson(yaml);
			}

			static ofJson loadJson(const std::string &path) {
				return ofLoadJson(path);
			}

		private:
			static ofJson hjsonToJson(const Hjson::Value &hjson) {
				ofJson json;

				if (hjson.type() == Hjson::Value::MAP) {
					for (auto it = hjson.begin(); it != hjson.end(); ++it) {
						string key = it->first;
						Hjson::Value value = it->second;

						switch (value.type()) {
						case Hjson::Value::STRING:
							json[key] = value.to_string();
							break;
						case Hjson::Value::DOUBLE:
							{
								double d = value.to_double();
								if (d == (int)d) {
									json[key] = (int)d;
								}
								else {
									json[key] = d;
								}
							}
							break;
						case Hjson::Value::BOOL:
							json[key] = (bool)value;
							break;
						case Hjson::Value::HJSON_NULL:
							json[key] = NULL;
							break;
						case Hjson::Value::VECTOR:
							json[key] = hjsonToJson(value);
							break;
						case Hjson::Value::MAP:
							json[key] = hjsonToJson(value);
							break;
						}

					}
				} else if (hjson.type() == Hjson::Value::VECTOR) {
					for (int i = 0; i< hjson.size(); ++i){
						Hjson::Value value = hjson[i];

						switch (value.type()) {
						case Hjson::Value::STRING:
							json[i] = value.to_string();
							break;
						case Hjson::Value::DOUBLE:
							{
								double d = value.to_double();
								if (d == (int)d) {
									json[i] = (int)d;
								}
								else {
									json[i] = d;
								}
							}
							break;
						case Hjson::Value::BOOL:
							json[i] = (bool)value;
							break;
						case Hjson::Value::HJSON_NULL:
							json[i] = NULL;
							break;
						case Hjson::Value::VECTOR:
							json[i] = hjsonToJson(value);
							break;
						case Hjson::Value::MAP:
							json[i] = hjsonToJson(value);
							break;
						}
					}
				}

				return json;
			}

			static ofJson yamlToJson(const YAML::Node &yaml) {

				ofJson json;
				if (yaml.IsMap()){
					//for (int i = 0; i < yaml.size(); ++i) {
					//	YAML::Node value = yaml[i];
					//}
					for (auto it = yaml.begin(); it != yaml.end(); ++it) {
						//auto it2 = it;
						//it->first.as<std::string>();
						//auto key = it->first;
						//auto value = it->second;
					}
				}
				else if (yaml.IsSequence()){
					for (int i = 0; i < yaml.size(); ++i) {
						YAML::Node value = yaml[i];
					}
				}
				return ofJson();
			}
		};

	}
}

using ofxAnyJson = ofx::AnyJson::ofxAnyJson;