#pragma once

#include "ofxJsonUtils.h"
#include "hjson.h"
#include "yaml.h"
#include "cpptoml.h"

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
				ofFile file(path, ofFile::ReadOnly, false);
				ofBuffer buf = file.readToBuffer();
				auto str = buf.getData();

				YAML::Node yaml = YAML::Load(str);
				return yamlToJson(yaml);
			}

			static ofJson loadToml(const std::string &path) {
				//ofFile file(path, ofFile::ReadOnly, false);
				//ofBuffer buf = file.readToBuffer();
				//auto str = buf.getData();

				shared_ptr<cpptoml::table> toml = cpptoml::parse_file(path);
				return tomlToJson(toml);
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
					//auto nodes = yaml.m_pMemory->m_pMemory->m_nodes;
					for (auto it = yaml.begin(); it != yaml.end(); ++it) {
						auto map = *it.m_iterator.m_mapIt;
						auto key = map.first->scalar();

						auto value = yaml[key];
						if (value.IsScalar()) {
							try {
								auto b = value.as<bool>();
								json[key] = b;
								continue;
							}
							catch (YAML::TypedBadConversion<bool> e) {
							}

							try {
								auto d = value.as<double>();
								if (d == (int)d) {
									json[key] = (int)d;
								} else {
									json[key] = d;
								}
								continue;
							}
							catch (YAML::TypedBadConversion<double> e) {
							}

							auto s = value.as<string>();
							json[key] = s;
						}
						else if (value.IsMap()) {
							json[key] = yamlToJson(value);
						}
						else if (value.IsSequence()) {
							json[key] = yamlToJson(value);
						}
						else if (value.IsSequence()) {
							json[key] = yamlToJson(value);
						}
						else if (value.IsNull()) {
							json[key] = NULL;
						}
					}
				}
				else if (yaml.IsSequence()){
					for (int i = 0; i < yaml.size(); ++i) {
						YAML::Node value = yaml[i];

						if (value.IsScalar()) {
							try {
								auto b = value.as<bool>();
								json[i] = b;
								continue;
							}
							catch (YAML::TypedBadConversion<bool> e) {
							}

							try {
								auto d = value.as<double>();
								if (d == (int)d) {
									json[i] = (int)d;
								}
								else {
									json[i] = d;
								}
								continue;
							}
							catch (YAML::TypedBadConversion<double> e) {
							}

							auto s = value.as<string>();
							json[i] = s;
						}
						else if (value.IsMap()) {
							json[i] = yamlToJson(value);
						}
						else if (value.IsSequence()) {
							json[i] = yamlToJson(value);
						}
						else if (value.IsSequence()) {
							json[i] = yamlToJson(value);
						}
						else if (value.IsNull()) {
							json[i] = NULL;
						}
					}
				}
				return json;
			}

			static ofJson tomlToJson(shared_ptr<cpptoml::base> toml) {
				ofJson json;

				if (toml->is_table()) {
					shared_ptr<cpptoml::table> table = toml->as_table();
					for (auto it = table->begin(); it != table->end(); ++it) {
						auto key = it->first;
						auto value = it->second;
						if (value->is_table()) {
							json[key] = tomlToJson(value);
						}
						else if (value->is_array()) {
							json[key] = tomlToJson(value);
						}
						else if (value->is_table_array()) {
							json[key] = tomlToJson(value);
						}
						else if (value->is_value()) {
							auto bv = value->as<bool>();
							if (bv) {
								json[key] = (bool)bv->get();
							}
							else {
								auto v = value->as<double>();
								if (v) {
									auto d = (double)v->get();
									if (d == (int)d) {
										json[key] = (int)d;
									}
									else {
										json[key] = d;
									}
								}
								else {
									auto sv = value->as<string>();
									if (sv) {
										json[key] = (string)sv->get();
									}
									else {
										json[key] = NULL;
									}
								}
							}
						}
					}
				}
				else if (toml->is_array()) {
					shared_ptr<cpptoml::array> arr = toml->as_array();

					int i = 0;
					for (auto it = arr->begin(); it != arr->end(); ++it) {
						auto value = *it;
						if (value->is_table()) {
							json[i] = tomlToJson(value);
						}
						else if (value->is_array()) {
							json[i] = tomlToJson(value);
						}
						else if (value->is_table_array()) {
							json[i] = tomlToJson(value);
						}
						else if (value->is_value()) {
							auto bv = value->as<bool>();
							if (bv) {
								json[i] = (bool)bv->get();
							}
							else {
								auto v = value->as<double>();
								if (v) {
									auto d = (double)v->get();
									if (d == (int)d) {
										json[i] = (int)d;
									}
									else {
										json[i] = d;
									}
								}
								else {
									auto sv = value->as<string>();
									if (sv) {
										json[i] = (string)sv->get();
									}
									else {
										json[i] = NULL;
									}
								}
							}
						}
						++i;
					}
				}
				else if (toml->is_table_array()) {
					shared_ptr<cpptoml::table_array> arr = toml->as_table_array();
					int i = 0;
					for (auto it = arr->begin(); it != arr->end(); ++it) {
						auto value = *it;
						json[i] = tomlToJson(value);
						++i;
					}
				}

				return json;
			}
		};

	}
}

using ofxAnyJson = ofx::AnyJson::ofxAnyJson;