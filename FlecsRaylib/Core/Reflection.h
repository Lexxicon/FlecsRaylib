#pragma once
#include <vcruntime_typeinfo.h>
#include <vector>

#include "flecs.h"
#include "boost/pfr/core.hpp"
#include "boost/pfr/core_name.hpp"

template<typename>
struct inner;

template<template<typename>class outter_t, typename inner_t>
struct inner<outter_t<inner_t> >{
    typedef typename std::remove_cv<typename std::remove_reference<inner_t>::type>::type type;
};
template<typename _t>
using inner_t = typename inner<std::remove_reference_t<_t>>::type;

namespace Reflection
{
    namespace Detail
    {
        template <class T>
        struct is_std_vector { static constexpr bool value=false; };

        template <class T>
        struct is_std_vector<std::vector<T>> { static constexpr bool value=true; };
        
        // Reusable reflection support for std::vector
        template <typename Elem, typename Vector = std::vector<Elem>> 
        flecs::opaque<Vector, Elem> std_vector_support(flecs::world& world)
        {
            return flecs::opaque<Vector, Elem>()
                .as_type(world.vector<Elem>())
                // Forward elements of std::vector value to serializer
                .serialize([](const flecs::serializer *s, const Vector *data) {
                    for (const auto& el : *data) {
                        s->value(el);
                    }
                    return 0;
                })
                // Return vector count
                .count([](const Vector *data) {
                    return data->size();
                })
                // Resize contents of vector
                .resize([](Vector *data, size_t size) {
                    data->resize(size);
                })
                // Ensure element exists, return pointer
                .ensure_element([](Vector *data, size_t elem) {
                    if (data->size() <= elem) {
                        data->resize(elem + 1);
                    }

                    return &data->data()[elem];
                });
        }

        
        template<typename T>
        void GenerateImplicitReflectionBinds(flecs::world& World)
        {
            flecs::untyped_component Cmp = World.component<T>();
            if(Cmp.has<flecs::MetaType>() || Cmp.has<EcsOpaque>())
            {
                return;
            }

            if constexpr (is_std_vector<T>::value)
            {
                World.component<T>()
                    .opaque(std_vector_support<typename T::value_type>);
                        
                if constexpr (std::is_aggregate_v<typename T::value_type>)
                {
                    GenerateImplicitReflectionBinds<typename T::value_type>(World);
                }
            }
            else
            {
                const auto Names = boost::pfr::names_as_array<T>();
                T* Dummy = nullptr;
            
                boost::pfr::for_each_field(*Dummy,[&]<typename MemberType>(const MemberType& value, std::size_t i)
                {
                    printf("Testing %s %s\n", Names[i].data(), typeid(MemberType).name());
                    if(
                        !World.component<MemberType>().template has<flecs::MetaType>()
                        && !World.component<MemberType>().template has<EcsOpaque>())
                    {
                        if constexpr (std::is_aggregate_v<MemberType>)
                        {
                            GenerateImplicitReflectionBinds<MemberType>(World);
                        }
                        else if constexpr (is_std_vector<MemberType>::value)
                        {
                            World.component<MemberType>()
                                .opaque(std_vector_support<typename MemberType::value_type>);
                        
                            if constexpr (std::is_aggregate_v<typename MemberType::value_type>)
                            {
                                GenerateImplicitReflectionBinds<typename MemberType::value_type>(World);
                            }
                        }
                        else if constexpr (!std::is_pointer_v<MemberType>)
                        {
                            printf("Skipping %s\n", typeid(MemberType).name());
                            return;
                        }
                    }
                    const void* Vptr = static_cast<const void*>(&value);
                    ptrdiff_t Offset = static_cast<const uint8_t*>(Vptr) - reinterpret_cast<const uint8_t*>(Dummy);
                    Cmp.member<MemberType>(Names[i].data(), std::extent_v<MemberType>, Offset);
                });
            }
        }
    }

    template<typename T>
    void Register(flecs::world& World)
    {
        Detail::GenerateImplicitReflectionBinds<T>(World);
    }
}