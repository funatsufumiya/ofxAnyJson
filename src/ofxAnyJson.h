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

			static ofJson loadHjsonString(const std::string &str) {
				Hjson::Value hjson = Hjson::Unmarshal(str);
				return hjsonToJson(hjson);
			}

			static ofJson loadYaml(const std::string &path) {
				ofFile file(path, ofFile::ReadOnly, false);
				ofBuffer buf = file.readToBuffer();
				auto str = buf.getData();
				return parseYaml(str);
			}

			static ofJson loadYamlString(const std::string &str) {
				return parseYaml(str);
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

			//static ofJson yamlToJson(const YAML::Node &yaml) {
			//	ofJson json;
			//}

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

			static ofJson parseYaml(const std::string str) {
				size_t str_len = str.size();
				const unsigned char* cstr =(const unsigned char*)str.data();

				yaml_parser_t parser;
				yaml_token_t  token;

				/* Initialize parser */
				if (!yaml_parser_initialize(&parser))
					throw runtime_error("Failed to initialize parser!");

				/* Set input string */
				yaml_parser_set_input_string(&parser, cstr, str_len);

				ofJson json = ofJson::object();
				ofJson *cur = &json;

				vector<ofJson*> stack;

				string key;

				yaml_event_t event;

				bool should_have_key = false;
				bool prev_key = false;
				bool is_document_root = true;

				do {
					if (!yaml_parser_parse(&parser, &event)) {
						ofLogError() << "YAML parser error: " << parser.error;
						throw runtime_error("YAML parser error");
					}

					switch (event.type)
					{
					case YAML_NO_EVENT: break;
					case YAML_STREAM_START_EVENT: break;
					case YAML_STREAM_END_EVENT: break;
					case YAML_DOCUMENT_START_EVENT: break;
					case YAML_DOCUMENT_END_EVENT: break;
					case YAML_SEQUENCE_START_EVENT:
						{
							if (cur->is_object()) {
								(*cur)[key] = ofJson::array();
								stack.push_back(cur);
								cur = &(*cur)[key];
							}
							else if (cur->is_array()) {
								int idx = cur->size();
								(*cur)[idx] = ofJson::array();
								stack.push_back(cur);
								cur = &(*cur)[idx];
							}
						}
						break;
					case YAML_SEQUENCE_END_EVENT:
						{
							cur = stack.at(stack.size() - 1);
							stack.pop_back();

							if (cur->is_object()) {
								should_have_key = true;
							}
						}
						break;
					case YAML_MAPPING_START_EVENT:
						{
							if (is_document_root) {
								is_document_root = false;
							}else{
								if (cur->is_object()) {
									(*cur)[key] = ofJson::object();
									stack.push_back(cur);
									cur = &(*cur)[key];
								}
								else if (cur->is_array()) {
									int idx = cur->size();
									(*cur)[idx] = ofJson::object();
									stack.push_back(cur);
									cur = &(*cur)[idx];
								}
							}
							should_have_key = true;
						}
						break;
					case YAML_MAPPING_END_EVENT:
						{
							if (stack.size() > 0) {
								cur = stack.at(stack.size() - 1);
								stack.pop_back();

								if (cur->is_object()) {
									should_have_key = true;
								}
							}
						}
						break;
					case YAML_ALIAS_EVENT:
						{
							// ofLog() << "Got alias (anchor %s)\n" << event.data.alias.anchor;
							ofLogError() << "YAML Parser: Sorry, alias is not supported yet";
							throw runtime_error("YAML Parser: Sorry, alias is not supported yet");
						}
						break;
					case YAML_SCALAR_EVENT:
						{
							const char* _s = (const char*)event.data.scalar.value;
							if (should_have_key) {
								key = (const char*)event.data.scalar.value;
								should_have_key = false;
								prev_key = true;
							}
							else {
								string s = (const char*)event.data.scalar.value;
								bool is_bool = false;
								bool is_int = false;
								bool is_double = false;

								if ( (s == "true" || s == "false" || s == "yes" || s == "no")
									&& event.data.scalar.style != YAML_SINGLE_QUOTED_SCALAR_STYLE
									&& event.data.scalar.style != YAML_DOUBLE_QUOTED_SCALAR_STYLE) {
									
									is_bool = true;

								} else {
									try {
										double d = stod(s);
										if (d == (int)d) {
											is_int = true;
										}
										else {
											is_double = true;
										}
									}
									catch (...) {}
								}

								if (cur->is_object()) {
									if (is_bool) {
										(*cur)[key] = (s == "true" || s == "yes") ? true : false;
									}else if (is_int) {
										(*cur)[key] = stoi(s);
									}else if (is_double) {
										(*cur)[key] = stod(s);
									}else {
										(*cur)[key] = s;
									}

									if (prev_key) {
										should_have_key = true;
									}

									prev_key = false;
								}
								else if (cur->is_array()) {
									if (is_bool) {
										cur->push_back( (s == "true" || s == "yes") ? true : false );
									}else if (is_int) {
										cur->push_back( stoi(s) );
									}else if (is_double) {
										cur->push_back( stod(s) );
									}else {
										cur->push_back( s );
									}
								}
							}
						}
						break;
					}
					if (event.type != YAML_SCALAR_EVENT) {
						prev_key = false;
					}
					if (event.type != YAML_STREAM_END_EVENT) {
						yaml_event_delete(&event);
					}

				} while (event.type != YAML_STREAM_END_EVENT);

				yaml_event_delete(&event);

				// Cleanup
				yaml_parser_delete(&parser);

				return json;
			}
		};

	}
}

using ofxAnyJson = ofx::AnyJson::ofxAnyJson;