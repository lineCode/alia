#ifndef ALIA_COMPONENTS_STORAGE_HPP
#define ALIA_COMPONENTS_STORAGE_HPP

#include <typeindex>
#include <unordered_map>

namespace alia {

// generic_component_storage is one possible implementation of the underlying
// container for storing components and their associated data.
// :Data is the type used to store component data.
template<class Data>
struct generic_component_storage
{
    std::unordered_map<std::type_index, Data> components;
};

// Does the storage object have a component with the given tag?
template<class Tag, class Data>
bool
has_storage_component(generic_component_storage<Data>& storage)
{
    return storage.components.find(std::type_index(typeid(Tag)))
           != storage.components.end();
}

// Store a component.
template<class Tag, class StorageData, class ComponentData>
void
add_storage_component(
    generic_component_storage<StorageData>& storage, ComponentData&& data)
{
    storage.components[std::type_index(typeid(Tag))]
        = std::forward<ComponentData&&>(data);
}

// Remove a component.
template<class Tag, class Data>
void
remove_storage_component(generic_component_storage<Data>& storage)
{
    storage.components.erase(std::type_index(typeid(Tag)));
}

// Retrieve the data for a component.
template<class Tag, class Data>
Data&
get_storage_component(generic_component_storage<Data>& storage)
{
    return storage.components.at(std::type_index(typeid(Tag)));
}

// Invoke f on each component within the storage object.
template<class Data, class Function>
void
for_each_storage_component(generic_component_storage<Data>& storage, Function f)
{
    for (auto& i : storage.components)
    {
        f(i.second);
    }
}

// any_pointer is a simple way to store pointers to any type in a
// generic_component_storage object.
struct any_pointer
{
    any_pointer()
    {
    }

    template<class T>
    any_pointer(T* ptr) : ptr(ptr)
    {
    }

    template<class T>
    operator T*()
    {
        return reinterpret_cast<T*>(ptr);
    }

    void* ptr;
};

template<class T>
bool
operator==(any_pointer p, T* other)
{
    return reinterpret_cast<T*>(p.ptr) == other;
}
template<class T>
bool
operator==(T* other, any_pointer p)
{
    return other == reinterpret_cast<T*>(p.ptr);
}
template<class T>
bool
operator!=(any_pointer p, T* other)
{
    return reinterpret_cast<T*>(p.ptr) != other;
}
template<class T>
bool
operator!=(T* other, any_pointer p)
{
    return other != reinterpret_cast<T*>(p.ptr);
}

} // namespace alia

#endif