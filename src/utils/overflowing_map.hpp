#ifndef OVERFLOWING_MAP_HPP
#define OVERFLOWING_MAP_HPP

#include <unordered_map>
#include <memory>

namespace Terrarium {

    // Utility class that stores data associated with id that can be overflowed,
    // such as uint16_t
    template <class Key, class Value>
    class OverflowingMap {
        Key last_key = 0;

        bool isKeyFree(Key key) {
            return key != 0 && map.find(key) == map.end();
        }

        Key getFreeKey() {
            Key start_key = last_key;

            while (!isKeyFree(++last_key)) {
                if (last_key == start_key) {
                    return 0;
                }
            }

            return last_key;
        }

    protected:
        std::unordered_map<Key, std::shared_ptr<Value>> map;

    public:
        Key add(std::shared_ptr<Value> value) {
            Key key = getFreeKey();

            if (key == 0) {
                return 0;
            }

            map[key] = value;

            return key;
        }

        std::shared_ptr<Value> get(Key key) {
            if (isKeyFree(key)) {
                return nullptr;
            }

            return map[key];
        }

        void del(Key key) {
            map.erase(key);
        }
    };

} // namespace Terrarium

#endif
