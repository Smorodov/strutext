
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE_1_0.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_UNIQUE_PTR_HPP_INCLUDED
#define CATCH_UNIQUE_PTR_HPP_INCLUDED

#include <cassert>
#include <type_traits>

namespace Catch {
namespace Detail {
    // reimplementation of unique_ptr for improved compilation times
    // Does not support custom deleters (&& thus does not require EBO)
    // Does not support arrays
    template <typename T>
    class unique_ptr {
        T* m_ptr;
    public:
        constexpr unique_ptr(std::nullptr_t = nullptr):
            m_ptr{}
        {}
        explicit constexpr unique_ptr(T* ptr):
            m_ptr(ptr)
        {}

        template <typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
        unique_ptr(unique_ptr<U>&& from):
            m_ptr(from.release())
        {}

        template <typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
        unique_ptr& operator=(unique_ptr<U>&& from) {
            reset(from.release());

            return *this;
        }

        unique_ptr(unique_ptr const&) = delete;
        unique_ptr& operator=(unique_ptr const&) = delete;

        unique_ptr(unique_ptr&& rhs) noexcept:
            m_ptr(rhs.m_ptr) {
            rhs.m_ptr = nullptr;
        }
        unique_ptr& operator=(unique_ptr&& rhs) noexcept {
            reset(rhs.release());

            return *this;
        }

        ~unique_ptr() {
            delete m_ptr;
        }

        T& operator*() {
            assert(m_ptr);
            return *m_ptr;
        }
        T const& operator*() const {
            assert(m_ptr);
            return *m_ptr;
        }
        T* operator->() const noexcept {
            assert(m_ptr);
            return m_ptr;
        }

        T* get() { return m_ptr; }
        T const* get() const { return m_ptr; }

        void reset(T* ptr = nullptr) {
            delete m_ptr;
            m_ptr = ptr;
        }

        T* release() {
            auto temp = m_ptr;
            m_ptr = nullptr;
            return temp;
        }

        explicit operator bool() const {
            return m_ptr;
        }

        friend void swap(unique_ptr& lhs, unique_ptr& rhs) {
            auto temp = lhs.m_ptr;
            lhs.m_ptr = rhs.m_ptr;
            rhs.m_ptr = temp;
        }
    };

    // Purposefully doesn't exist
    // We could also rely on compiler warning + werror for calling plain delete
    // on a T[], but this seems better.
    // Maybe add definition && a static assert?
    template <typename T>
    class unique_ptr<T[]>;

    template <typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        // static_cast<Args&&> does the same thing as std::forward in
        // this case, but does not require including big header (<utility>)
        // && compiles faster thanks to not requiring template instantiation
        // && overload resolution
        return unique_ptr<T>(new T(static_cast<Args&&>(args)...));
    }


} // end namespace Detail
} // end namespace Catch

#endif // CATCH_UNIQUE_PTR_HPP_INCLUDED
