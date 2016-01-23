/**
 * @file any.h
 * @brief from boost.any with c++11
 * @author lonsharn
 * @version 1.0
 * @date 2015-11-05
 */
#pragma once

#include <algorithm>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

template <bool B, class T = void>
struct disable_if_c {
    typedef T type;
};

template <class T>
struct disable_if_c<true, T> {};

template <class Cond, class T = void> 
struct disable_if : public disable_if_c<Cond::value, T> {};

class any
{
    public: // structors
        any() 
            : content(0)
        {
        }

        template<typename ValueType>
            any(const ValueType & value)
            : content(new holder<typename std::remove_cv<typename std::decay<const ValueType>::type>::type>(value))
            {
            }

        any(const any & other)
            : content(other.content ? other.content->clone() : 0)
            {
            }

        // Move constructor
        any(any&& other) 
            : content(other.content)
        {
            other.content = 0;
        }

        // Perfect forwarding of ValueType
        template<typename ValueType>
            any(ValueType&& value
                    , typename disable_if<std::is_same<any&, ValueType>, ValueType >::type* = 0 // disable if value has type `any&`
                    , typename disable_if<std::is_const<ValueType>, ValueType >::type* = 0) // disable if value has type `const ValueType&&`
            : content(new holder< typename std::decay<ValueType>::type >(static_cast<ValueType&&>(value)))
            {
            }

        ~any() 
        {
            delete content;
        }
    public: // modifiers
        any & swap(any & rhs) 
        {
            std::swap(content, rhs.content);
            return *this;
        }

        any & operator=(const any& rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        // move assignement
        any & operator=(any&& rhs) 
        {
            rhs.swap(*this);
            any().swap(rhs);
            return *this;
        }

        // Perfect forwarding of ValueType
        template <class ValueType>
            any & operator=(ValueType&& rhs)
            {
                any(static_cast<ValueType&&>(rhs)).swap(*this);
                return *this;
            }

    public: // queries

        bool empty() const 
        {
            return !content;
        }

        void clear() 
        {
            any().swap(*this);
        }

        const std::type_info& type() const 
        {
            return content ? content->type() : typeid(void);
        }
    private: // types
        class placeholder
        {
            public: // structors
                virtual ~placeholder()
                {
                }
            public: // queries
                virtual const std::type_info& type() const = 0;
                virtual placeholder * clone() const = 0;
        };

        template<typename ValueType>
            class holder : public placeholder
        {
            public: // structors
                holder(const ValueType & value)
                    : held(value)
                {
                }

                holder(ValueType&& value)
                    : held(static_cast< ValueType&& >(value))
                {
                }
            public: // queries
                virtual const std::type_info& type() const 
                {
                    return typeid(ValueType);
                }
                virtual placeholder * clone() const
                {
                    return new holder(held);
                }
            public: // representation
                ValueType held;
            private: // intentionally left unimplemented
                holder & operator=(const holder &);
        };

    private: // representation
        template<typename ValueType>
            friend ValueType * any_cast(any *) ;
        template<typename ValueType>
            friend ValueType * unsafe_any_cast(any *) ;
        placeholder * content;
};

inline void swap(any & lhs, any & rhs) 
{
    lhs.swap(rhs);
}

class bad_any_cast :
    public std::bad_cast
{
    public:
        virtual const char * what() const noexcept
        {
            return "any::bad_any_cast: "
                "failed conversion using any::any_cast";
        }
};

    template<typename ValueType>
ValueType * any_cast(any * operand) 
{
    return operand && operand->type() == typeid(ValueType)
        ? &static_cast<any::holder<typename std::remove_cv<ValueType>::type> *>(operand->content)->held
        : 0;
}

    template<typename ValueType>
inline const ValueType * any_cast(const any * operand) 
{
    return any_cast<ValueType>(const_cast<any *>(operand));
}

    template<typename ValueType>
ValueType any_cast(any & operand)
{
    typedef typename std::remove_reference<ValueType>::type nonref;


    nonref * result = any_cast<nonref>(&operand);
    if(!result)
        std::bad_exception(bad_any_cast());

    // Attempt to avoid construction of a temporary object in cases when 
    // `ValueType` is not a reference. Example:
    // `static_cast<std::string>(*result);` 
    // which is equal to `std::string(*result);`
    typedef typename std::conditional<std::is_reference<ValueType>::value, ValueType, typename std::add_rvalue_reference<ValueType>::type>::type ref_type;

    return static_cast<ref_type>(*result);
}

    template<typename ValueType>
inline ValueType any_cast(const any & operand)
{
    typedef typename std::remove_reference<ValueType>::type nonref;
    return any_cast<const nonref &>(const_cast<any &>(operand));
}

    template<typename ValueType>
inline ValueType any_cast(any&& operand)
{
    static_assert(
            std::is_rvalue_reference<ValueType&&>::value /*true if ValueType is rvalue or just a value*/
            || std::is_const< typename std::remove_reference<ValueType>::type >::value,
            "any::any_cast shall not be used for getting nonconst references to temporary objects" 
            );
    return any_cast<ValueType>(operand);
}

// Note: The "unsafe" versions of any_cast are not part of the
// public interface and may be removed at any time. They are
// required where we know what type is stored in the any and can't
// use typeid() comparison, e.g., when our types may travel across
// different shared libraries.
    template<typename ValueType>
inline ValueType * unsafe_any_cast(any * operand) 
{
    return &static_cast<any::holder<ValueType> *>(operand->content)->held;
}

    template<typename ValueType>
inline const ValueType * unsafe_any_cast(const any * operand) 
{
    return unsafe_any_cast<ValueType>(const_cast<any *>(operand));
}
