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
#include <array>
#include <intrin.h>
#include <stdexcept>

#include "bitfield.hpp"
#include "cpuid_schema.hpp"

constexpr int32_t   initial_cpuid_function_id = 0x0000;
constexpr int32_t   max_cpuid_function_id = 0x0021;
constexpr uint32_t  ext_cpuid_function_id = 0x80000000;
constexpr uint32_t  ext_cpuid_function_id_max = 0x80000009;

enum e_cpuid_registers
{
    eax = 0,
    ebx = 1,
    ecx = 2,
    edx = 3
};

struct cpu_query
{
private:
    int32_t _function_id;
    int32_t _subfunction_id;
    int32_t info[ 4 ]{};
    std::array<bitfield<int32_t>, 4> bf;
    function_schema schema;

public:
    cpu_query( int32_t function_id, int32_t subfunction_id = 0 ) : _function_id( function_id ), _subfunction_id( subfunction_id )
    {
        __cpuidex( info, function_id, subfunction_id );

        for ( int i = 0; i < 4; ++i )
            bf[ i ] = bitfield( info[ i ] );

        int32_t composite_key = 0xffff0000;
        composite_key |= function_id;

        if ( subfunction_id > 0 && function_id < 0x40000000 )
        {
            // Iterate through the map to find the first schema that matches
            for ( const auto& [mask, schema_func] : cpuid_schema )
            {
                if ( ( subfunction_id == ( mask >> 16 ) ) &&
                     ( function_id == ( mask & 0x0000FFFF ) ) )
                {
                    schema = schema_func;
                    break;
                }

                schema = cpuid_schema[ composite_key ];
            }
        }
        else
        {
            schema = cpuid_schema[ function_id ];
        }
    }

    static std::size_t to_index( const std::string& name )
    {
        if ( name == "eax" ) return eax;
        if ( name == "ebx" ) return ebx;
        if ( name == "ecx" ) return ecx;
        if ( name == "edx" ) return edx;
        throw std::out_of_range( "Invalid register name" );
    }

    static const char* to_string( e_cpuid_registers e )
    {
        switch ( e )
        {
            case eax: return "eax";
            case ebx: return "ebx";
            case ecx: return "ecx";
            case edx: return "edx";
            default: return "unknown";
        }
    }

    struct field_value
    {
        std::string field_name;
        uint32_t value;
    };

    [[nodiscard]] int32_t get_by_name( const std::string& reg_name ) const
    {
        return info[ to_index( reg_name ) ];
    }

    field_value get_by_name_index( const std::pair<std::string, std::size_t>& p )
    {
        const register_schema& reg_schema = schema[ p.first ];
        const cpuid_field& field = reg_schema.fields[ p.second ];
        const int32_t reg_value = info[ to_index( p.first ) ];
        if ( field.bit_start == field.bit_end )
        {
            return { field.name, uint32_t( reg_value >> field.bit_start ) & 1 };
        }
        uint32_t mask = ( ( 1U << ( field.bit_end - field.bit_start + 1 ) ) - 1 ) << field.bit_start;
        return { field.name, ( reg_value & mask ) >> field.bit_start };
    }

    auto& get_bitfield( const std::string& reg_name ) { return bf[ to_index( reg_name ) ]; }
    auto& operator[]( const std::string& reg_name ) { return bf[ to_index( reg_name ) ]; }

    uint32_t get_bit( const std::string& reg_name, std::size_t bit_position ) { return bf[ to_index( reg_name ) ][ bit_position ]; }

    uint32_t get_bit_range( const std::string& reg_name, std::size_t bit_start, std::size_t bit_end )
    {
        uint32_t result = 0;
        for ( std::size_t i = bit_start; i <= bit_end; ++i )
        {
            result |= ( get_bit( reg_name, i ) << ( i - bit_start ) );
        }
        return result;
    }

    auto find_field_iter( const register_schema& reg_schema, auto predicate )
    {
        const auto field_iter = std::find_if( reg_schema.fields.begin(), reg_schema.fields.end(), predicate );

        if ( field_iter == reg_schema.fields.end() )
            throw std::out_of_range( "Invalid field or bit range" );

        return field_iter;
    }

    bitfield<int32_t>::proxy_range get_field( const std::string& reg_name, const char* field_name )
    {
        const register_schema& reg_schema = schema[ reg_name ];
        const auto field_iter = find_field_iter( reg_schema, [ &field_name ] ( const cpuid_field& field )
        {
            return field.name == field_name;
        } );

        return bf[ to_index( reg_name ) ].get_range( field_iter->bit_start, field_iter->bit_end );
    }

    field_value get_field( const std::string& reg_name, std::pair<std::size_t, std::size_t> bit_range )
    {
        const register_schema& reg_schema = schema[ reg_name ];
        const auto field_iter = find_field_iter( reg_schema, [ &bit_range ] ( const cpuid_field& field )
        {
            return field.bit_start == bit_range.first && field.bit_end == bit_range.second;
        } );

        const uint32_t value = get_bit_range( reg_name, bit_range.first, bit_range.second );
        return { field_iter->name, value };
    }

    field_value get_field( const std::string& reg_name, std::size_t bit_pos )
    {
        const register_schema& reg_schema = schema[ reg_name ];
        const auto field_iter = find_field_iter( reg_schema, [ &bit_pos ] ( const cpuid_field& field )
        {
            return field.bit_start == bit_pos && field.bit_end == bit_pos;
        } );

        const uint32_t value = get_bit( reg_name, bit_pos );
        return { field_iter->name, value };
    }

    [[nodiscard]] function_schema get_schema() const { return schema; }
};