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
#include <ranges>
#include <stdexcept>

#include "bitfield.hpp"
#include "msr_schema.hpp"

struct msr_query
{
    uint32_t msr_index;
    msr_schema_map schema;

    uint64_t msr_data;
    bitfield<uint64_t> bf;

    auto rdmsr( uint32_t idx )
    {
        __try { msr_data = __readmsr( idx ); }
        __except ( 1 ) { msr_data = 0; }

        return msr_data;
    }

    explicit msr_query( uint32_t index ) : msr_index( index )
    {
        msr_data = rdmsr( index );

        const auto search = msr_schema_list.find( index );
        if ( search != msr_schema_list.end() )
            schema = search->second;
        else
        {
            if ( index >= 0x40000000 && index <= 0x400000ff )
                schema = reserved_msr_address_space;
            else
                schema = msr_schema_invalid;
        }

        bf = bitfield( msr_data );
    }

    struct field_value
    {
        std::string field_name;
        uint64_t value;
    };

    field_value get_by_name_index( const std::pair<std::string, std::size_t>& p ) noexcept
    {
        const msr_schema& reg_schema = schema.at( p.first );
        const msr_field& field = reg_schema.fields[ p.second ];
        const uint64_t reg_value = msr_data;
        if ( field.bit_start == field.bit_end )
        {
            return { field.name, uint64_t( reg_value >> field.bit_start ) & 1 };
        }
        uint32_t mask = ( ( 1U << ( field.bit_end - field.bit_start + 1 ) ) - 1 ) << field.bit_start;
        return { field.name, ( reg_value & mask ) >> field.bit_start };
    }

    auto& get_bitfield( const std::string& reg_name ) { return bf; }
    auto operator[]( size_t index ) { return bf[ index ]; }

    uint64_t get_bit( std::size_t bit_position ) noexcept { return bf[ bit_position ]; }

    uint64_t get_bit_range( std::size_t bit_start, std::size_t bit_end ) noexcept
    {
        uint64_t result = 0;
        for ( std::size_t i = bit_start; i <= bit_end; ++i )
            result |= ( get_bit( i ) << ( i - bit_start ) );

        return result;
    }

    auto find_field_iter( const msr_schema& reg_schema, auto predicate ) noexcept
    {
        const auto field_iter = std::find_if( reg_schema.fields.begin(), reg_schema.fields.end(), predicate );

        if ( field_iter == reg_schema.fields.end() )
            __fastfail( 0x13378 );

        return field_iter;
    }

    bitfield<uint64_t>::proxy_range get_field( const char* field_name ) noexcept
    {
        for ( const auto& reg_schema : schema | std::views::values )
        {
            const auto field_iter = find_field_iter( reg_schema, [ &field_name ] ( const msr_field& field )
            {
                return field.name == field_name;
            } );

            if ( field_iter != reg_schema.fields.end() )
            {
                return bf.get_range( field_iter->bit_start, field_iter->bit_end );
            }
        }

        __fastfail( 0x13379 );
    }

    field_value get_field( std::pair<std::size_t, std::size_t> bit_range ) noexcept
    {
        const msr_schema& reg_schema = schema.begin()->second; // Adjust this if necessary
        const auto field_iter = find_field_iter( reg_schema, [ &bit_range ] ( const msr_field& field )
        {
            return field.bit_start == bit_range.first && field.bit_end == bit_range.second;
        } );

        const uint64_t value = get_bit_range( bit_range.first, bit_range.second );
        return { field_iter->name, value };
    }

    field_value get_field( std::size_t bit_pos ) noexcept
    {
        const msr_schema& reg_schema = schema.begin()->second; // Adjust this if necessary
        const auto field_iter = find_field_iter( reg_schema, [ &bit_pos ] ( const msr_field& field )
        {
            return field.bit_start == bit_pos && field.bit_end == bit_pos;
        } );

        const uint64_t value = get_bit( bit_pos );
        return { field_iter->name, value };
    }

    const msr_schema_map& get_schema() const noexcept { return schema; }
};
