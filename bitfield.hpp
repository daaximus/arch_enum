/* This project is licensed under the MIT license. See the LICENSE file for more details.
 *
 * Copyright 2023. Derek (Daax) Rynd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <type_traits>
#include <utility>

template <typename T>
struct bitfield
{
    T data = 0;

    bitfield() : data( 0 ) {}
    bitfield( T data ) : data( data ) {}

    bitfield( const bitfield& other ) : data( other.data ) {}

    bitfield& operator=( const bitfield& other )
    {
        if ( this != &other )
            data = other.data;
        return *this;
    }

    bool operator==( const bitfield& other ) const { return data == other.data; }
    bool operator!=( const bitfield& other ) const { return data != other.data; }
    bool operator< ( const bitfield& other ) const { return data < other.data; }
    bool operator> ( const bitfield& other ) const { return data > other.data; }
    bool operator<=( const bitfield& other ) const { return data <= other.data; }
    bool operator>=( const bitfield& other ) const { return data >= other.data; }

    constexpr bool operator[]( std::size_t pos ) const noexcept
    {
        return ( data >> pos ) & 1U;
    }

    template <typename U, std::enable_if_t<std::is_integral_v<U> && sizeof( U ) <= sizeof( T ), int> = 0>
    operator U() const noexcept
    {
        return static_cast< U >( data );
    }

    struct proxy_field
    {
        proxy_field( T& ref, std::size_t p ) : data_ref( ref ), position( p ) {}

        proxy_field& operator=( bool b )
        {
            T mask = T( 1 ) << position;
            data_ref = ( data_ref & ~mask ) | ( T( b ) << position );
            return *this;
        }

        operator bool() const
        {
            return ( data_ref >> position ) & 1U;
        }

        private:
        T& data_ref;
        std::size_t position;
    };

    proxy_field operator[]( std::size_t pos )
    {
        return proxy_field( data, pos );
    }

    struct bit_range
    {
        bit_range( T& ref, std::size_t start, std::size_t end )
            : data_ref( ref ), start_position( start ), len( end - start + 1 )
        {
        }

        bit_range& operator=( T value )
        {
            T mask = ( ( T( 1 ) << len ) - 1 ) << start_position;
            data_ref = ( data_ref & ~mask ) | ( ( value << start_position ) & mask );
            return *this;
        }

        operator T() const
        {
            if ( len == sizeof( T ) * 8 )
                return data_ref;

            return ( data_ref >> start_position ) & ( ( T( 1 ) << len ) - 1 );
        }

        template <typename AsType = uint64_t>
        [[nodiscard]] AsType as() const
        {
            if ( len == std::numeric_limits<AsType>::digits )
                return static_cast< AsType >( data_ref );

            // If trying to shift full-width of type, behavior is undefined... condition above added.
            //
            return static_cast< AsType >( ( data_ref >> start_position ) & ( ( AsType( 1 ) << len ) - 1 ) );
        }

        private:
        T& data_ref;
        std::size_t start_position;
        std::size_t len;
    };

    bit_range range( std::size_t start, std::size_t end )
    {
        return bit_range( data, start, end );
    }

    bit_range operator()( std::pair<std::size_t, std::size_t> p )
    {
        return bit_range( data, p.first, p.second );
    }

    struct proxy_range
    {
        T& data_ref;
        std::size_t start_position;
        std::size_t end_position;

        proxy_range( T& ref, std::size_t start, std::size_t end )
            : data_ref( ref ), start_position( start ), end_position( end )
        {
        }

        bit_range operator[]( std::size_t end )
        {
            return bit_range( data_ref, start_position, end );
        }

        proxy_range& operator=( T value )
        {
            bit_range range( data_ref, start_position, end_position );
            range = value;
            return *this;
        }

        operator T() const
        {
            return bit_range( data_ref, start_position, end_position );
        }
    };

    proxy_range get_range( std::size_t start, std::size_t end )
    {
        return proxy_range( data, start, end );
    }
};
