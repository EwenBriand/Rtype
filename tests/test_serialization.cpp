#include <vector>
#include <string>
#include <criterion/criterion.h>
#include "ResourceManager.hpp"

Test(serialize_vec, test_serialize_vec) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::vector<unsigned char> data;

    ecs::ResourceManager::Serialize<std::vector<int>>(v, data);

    std::vector<int> v2;
    std::vector<unsigned char>::iterator it = data.begin();
    ecs::ResourceManager::Deserialize(data, it, v2);

    cr_assert_eq(v.size(), v2.size());
    for (size_t i = 0; i < v.size(); i++) {
        cr_assert_eq(v[i], v2[i]);
    }
}
