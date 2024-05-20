// 5/19/2024
// Spencer Banasik

// The purpose of this class is to provide an quick and easy method of 
// having a pool of vectors together in a 1d vector for contiguous iteration
// while ensuring that no references to the pool are invalidated
#ifndef SB_VECTOR_POOL_HPP
#define SB_VECTOR_POOL_HPP

#include <vector>
#include <unordered_map>
#include <stdint.h>

namespace sb_lib {

template<typename T>
class VectorPool {
public:

    VectorPool() {
        id_counter = 0;
    }

    uint32_t add_data(const std::vector<T>& data) {

        uint32_t id = generate_id();
        id_to_idx[id] = this->data.size();
        id_to_size[id] = data.size();
        id_order.push_back(id);

        this->data.insert(this->data.end(), data.begin(), data.end());

        return id;
    }

    void add_element_end(const uint32_t id, const T& data) {

        uint32_t size = id_to_size[id];

        if (id == (id_counter - 1)) {
            this->data.push_back(data);
            id_to_size[id] = size + 1;
            return;
        }

        uint32_t offset = id_to_idx[id];
        offset = offset + size - 1;
        this->data.insert(this->data.begin() + offset, data);

        this->update_references(id, size + 1);
        id_to_size[id] = size + 1;
    }

    void set_data(const uint32_t id, const std::vector<T>& data) {

        uint32_t idx = id_to_idx[id];
        uint32_t last = idx + id_to_size[id] - 1;

        this->data.erase(this->data.begin() + idx, this->data.begin() + last);
        this->data.insert(this->data.begin() + idx, data.begin(), data.end());

        this->update_references(id, data.size());

        id_to_size[id] = data.size();
    }

    void remove_data(const uint32_t id) {

        uint32_t idx = id_to_idx[id];
        uint32_t last = idx + id_to_size[id] - 1;

        this->data.erase(this->data.begin() + idx, this->data.begin() + last);
        this->update_references(id, 0);

    }

    void clear() {
        id_counter = 0;
        id_to_idx.clear();
        id_to_size.clear();
        id_order.clear();
        data.clear();
    }

    std::size_t size() {
        return data.size();
    }

    void reserve(const uint32_t size) {
        data.reserve(size);
    }

    // Expect the user to not manipulate this
    std::vector<T>& pool_data() {
        return data;
    }

private:

    uint32_t generate_id() {
        return id_counter++;
    }


    void update_references(uint32_t id, uint32_t new_size) {

        int32_t diff = new_size - id_to_size[id];
        std::uint32_t target = 0;

        bool update = false;
        for (std::uint32_t idx = 0; idx < id_order.size(); idx++) {

            if (update == true) {

                // TODO: if we're updating references, we now need to offset everything according to the new size, relative to the old size.
                id_to_idx[idx] = id_to_idx[idx] + diff;



            }
            if (id == id_order[idx]) {
                update = true;
                target = idx;
            }

        }

        if (new_size == 0) {
            id_to_idx.erase(id);
            id_to_size.erase(id);
            id_order.erase(id_order.begin() + target);
        }
    }

    uint32_t id_counter;
    std::unordered_map<std::uint32_t, std::uint32_t> id_to_idx;
    std::unordered_map<std::uint32_t, std::uint32_t> id_to_size;
    std::vector<std::uint32_t> id_order;
    std::vector<T> data;

};
};
#endif