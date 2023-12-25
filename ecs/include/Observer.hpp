/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Observer.hpp
*/

#pragma once
#include <functional>
#include <vector>

// namespace eng {
//     struct byteImage {
//         char* watchPtr;
//         size_t size;

//         std::vector<char> prevImage;

//         byteImage(char* ptr, size_t size)
//             : watchPtr(ptr)
//             , size(size)
//         {
//             prevImage.reserve(size);
//             for (int i = 0; i < size; ++i) {
//                 prevImage.push_back(watchPtr[i]);
//             }
//         }

//         bool HasChanged()
//         {
//             for (int i = 0; i < size; ++i) {
//                 if (watchPtr[i] != prevImage[i]) {
//                     return true;
//                 }
//             }
//             return false;
//         }

//         void Update()
//         {
//             for (int i = 0; i < size; ++i) {
//                 prevImage[i] = watchPtr[i];
//             }
//         }
//     };

//     class Observer {
//     public:
//         template <typename... T>
//         void RegisterTarget(T&&... targets, std::function<void(T&&...)> callback)
//         {
//             (_targets.emplace_back(targets, sizeof(targets)), ...);
//             _callback = [callback]() { callback(); };
//         }

//         void Poll()
//         {
//             for (auto& target : _targets) {
//                 if (target.HasChanged()) {
//                     target.Update();
//                     if (_callback)
//                         _callback();
//                 }
//             }
//         }

//     private:
//         std::vector<byteImage> _targets;
//         std::function<void()> _callback = nullptr;
//     };
// } // namespace eng
