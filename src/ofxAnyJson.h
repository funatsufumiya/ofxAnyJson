#pragma once

#include "ofxJsonUtils.h"
#include "hjson.h"

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
		};

	}
}

using ofxAnyJson = ofx::AnyJson::ofxAnyJson;