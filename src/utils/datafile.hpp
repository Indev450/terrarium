/*
 * Copyright 2022 Indev
 *
 * This file is part of TerrariumEngine
 *
 * TerrariumEngine is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#ifndef UTIL_DATAFILE_HPP
#define UTIL_DATAFILE_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
#include <fstream>
#include <utility>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

namespace Terrarium {

    // Human-readable file format for saving and loading data.
    // Heavily inspired by javidx9's file format (see https://youtube.com/watch?v=jlS1Y2-yKV0)
    class Datafile {
        std::vector<std::string> content;

        std::vector<std::pair<std::string, std::shared_ptr<Datafile>>> children;
        std::unordered_map<std::string, size_t> children_ids;

    public:
        Datafile() = default;

        // Do deep copy of other datafile object.
        Datafile &operator=(const Datafile &other) {
            // Content can be copied just like that.
            content = other.content;

            for (auto &pair: other.children) {
                // We don't want to copy pointers, we want to copy objects
                // themselves.
                operator[](pair.first) = *pair.second;
            }

            return *this;
        }

        Datafile(const Datafile &other) {
            *this = other;
        }

        void setString(const std::string &value, size_t i = 0) {
            if (content.size() <= i) content.resize(i+1);

            content[i] = value;
        }

        std::string getString(size_t i = 0) const {
            if (content.size() <= i) return "";

            return content[i];
        }

        void setReal(double value, size_t i = 0) {
            setString(std::to_string(value), i);
        }

        double getReal(size_t i = 0) const {
            return std::atof(getString(i).c_str());
        }

        void setInt(int value, size_t i = 0) {
            setString(std::to_string(value), i);
        }

        int getInt(size_t i = 0) const {
            return std::atoi(getString(i).c_str());
        }

        void setBool(bool value, size_t i = 0) {
            setInt(value ? 1 : 0, i);
        }

        bool getBool(size_t i = 0) {
            return getInt(i) != 0;
        }

        size_t getValueCount() const {
            return content.size();
        }

        bool childExists(const std::string &child_name) const {
            return children_ids.count(child_name) != 0;
        }

        bool empty() const {
            return content.empty() && children.empty();
        }

        const std::vector<std::pair<std::string, std::shared_ptr<Datafile>>> &getChildren() const {
            return children;
        }

        Datafile &operator[](const std::string &child_name) {
            if (!childExists(child_name)) {
                children_ids[child_name] = children.size();
                children.push_back({ child_name, std::make_unique<Datafile>() });
            }

            return *children[children_ids[child_name]].second;
        }

        bool saveToFile(const fs::path &filename,
                        const std::string &indent = "    ",
                        char listSep = ',') {

            std::ofstream file(filename);

            if (!file.is_open()) return false;

            saveToStream(file, indent, listSep);
            file.close();
            return true;
        }

        bool loadFromFile(const fs::path &filename, char listSep = ',') {
            std::ifstream file(filename);

            if (!file.is_open()) return false;

            loadFromStream(file, listSep);
            file.close();
            return true;
        }

        void saveToStream(std::ostream &output,
                          const std::string &indent = "",
                          char listSep = ',') {
            std::string sep = listSep + std::string(" ");

            size_t indent_level = 0;

            std::function<void(const Datafile&, std::ostream&)> write_node = [&] (const Datafile &data, std::ostream &output) {
                auto mkindent = [] (const std::string &s, size_t n) {
                    std::string out;

                    for (size_t i = 0; i < n; ++i) { out += s; };

                    return out;
                };

                for (auto const& property: data.children) {
                    const auto &child = *property.second;

                    if (child.children.empty()) {
                        output<<mkindent(indent, indent_level)<<property.first<<" = ";

                        size_t num_values = child.getValueCount();

                        for (size_t i = 0; i < num_values; ++i) {
                            size_t x = child.getString(i).find_first_of(listSep);

                            if (x != std::string::npos) {
                                output<<'"'<<child.getString(i)<<'"';
                            } else {
                                output<<child.getString(i);
                            }

                            if (i != num_values-1) {
                                output<<sep;
                            }
                        }

                        output<<'\n';
                    } else {
                        output<<'\n'<<mkindent(indent, indent_level)<<property.first<<'\n';
                        output<<mkindent(indent, indent_level)<<"{\n";
                        ++indent_level;

                        write_node(child, output);

                        output<<mkindent(indent, indent_level)<<"}\n\n";
                    }
                }

                if (indent_level) { --indent_level; }
            };

            write_node(*this, output);
        }

        void loadFromStream(std::istream &input, char listSep = ',') {
            std::string property_name, property_value;

            std::stack<std::reference_wrapper<Datafile>> data_path;
            data_path.push(*this);

            while (!input.eof()) {
                std::string line;

                std::getline(input, line);

                auto trim = [](std::string &str) {
                    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
                    str.erase(str.find_last_not_of(" \t\n\r\f\v")+1);
                };

                trim(line);

                if (!line.empty()) {
                    size_t x = line.find_first_of('=');

                    // name = values
                    if (x != std::string::npos) {
                        property_name = line.substr(0, x);
                        trim(property_name);

                        property_value = line.substr(x+1, line.size());
                        trim(property_value);

                        bool inside_quotes = false;
                        bool was_in_quotes = false;
                        std::string token;
                        size_t token_num = 0;

                        for (auto c: property_value) {
                            if (c == '\"') {
                                inside_quotes = !inside_quotes;
                                was_in_quotes = true;
                            } else {
                                if (inside_quotes) {
                                    token.append(1, c);
                                } else {
                                    if (c == listSep) {
                                        if (!was_in_quotes) {
                                            trim(token);
                                        }
                                        data_path.top().get()[property_name].setString(token, token_num);

                                        token.clear();
                                        was_in_quotes = false;
                                        ++token_num;
                                    } else {
                                        token.append(1, c);
                                    }
                                }
                            }
                        }

                        if (!was_in_quotes) {
                            trim(token);
                        }

                        if (!token.empty()) {
                            data_path.top().get()[property_name].setString(token, token_num);
                        }
                    } else {
                        if (line[0] == '{') {
                            data_path.push(data_path.top().get()[property_name]);
                        } else if (line[0] == '}') {
                            data_path.pop();
                        } else {
                            property_name = line;
                        }
                    }
                }
            }
        }
    };
}

#endif
