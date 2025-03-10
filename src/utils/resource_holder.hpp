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

#ifndef UTIL_RESOURCE_HOLDER_HPP
#define UTIL_RESOURCE_HOLDER_HPP

#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace Terrarium {

    // Utility class for loading resources from different locations.
    // `Resource` template argument should implement copy constructor
    // and loadFromFile method
    template <class Resource>
    class ResourceHolder {
        std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
        std::vector<fs::path> search_path;

        // Returned if resource not found. This is usually some kind of
        // "error resource", like "unknown texture".
        std::shared_ptr<Resource> default_res = std::make_shared<Resource>();

    public:
        void addSearchPath(const fs::path &path) {
            search_path.push_back(path);
        }

        void setDefault(std::shared_ptr<Resource> resource) {
            default_res = resource;
        }

        // Entire freaking same function only to make this class work with sf::Music
        // Thank you sfml devs that made sf::Music::openFromFile instead of
        // loadFromFile
        bool open(const std::string &name) {
            if (resources.find(name) != resources.end()) {
                return true;
            }

            resources.try_emplace(name, std::make_shared<Resource>());

            auto it = std::find_if(
                search_path.begin(),
                search_path.end(),
                [&] (const fs::path &path) { return std::filesystem::exists(path / name); });

            if (it == search_path.end()) {
                // File doesn't even exists

                resources.erase(name);

                return false;
            }

            if (!resources[name]->openFromFile((*it / name).string())) {
                // File exists, but failed to load

                resources.erase(name);

                return false;
            }

            return true;
        }

        bool load(const std::string &name) {
            if (resources.find(name) != resources.end()) {
                return true;
            }

            resources.try_emplace(name, std::make_shared<Resource>());

            auto it = std::find_if(
                search_path.begin(),
                search_path.end(),
                [&] (const fs::path &path) { return std::filesystem::exists(path / name); });

            if (it == search_path.end()) {
                // File doesn't even exists

                resources.erase(name);

                return false;
            }

            if (!resources[name]->loadFromFile((*it / name).string())) {
                // File exists, but failed to load

                resources.erase(name);

                return false;
            }

            return true;
        }

        Resource &get(const std::string &name) {
            if (resources.count(name) != 0) {
                return *resources[name];
            }

            std::cerr<<"Terrarium::ResourceHolder::get: resource '"<<name<<"' not found"<<std::endl;
            return *default_res;
        }

        bool exists(const std::string &name) {
            return resources.count(name) != 0;
        }

        void insert(const std::string &name, std::shared_ptr<Resource> resource) {
            resources[name] = resource;
        }
    };

}

#endif
