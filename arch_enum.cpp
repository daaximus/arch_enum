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

#include <intrin.h>
#include <iostream>
#include <string>
#include <unordered_map>

#include "cpuid.hpp"
#include "msr.hpp"

void print_cpuid( int function_id, int subfunction_id = 0 )
{
    printf( "## CPUID [%lX.%lX]\n", function_id, subfunction_id );
    cpu_query cpuid( function_id, subfunction_id );

    if ( cpuid.get_schema().empty() )
        printf( "%s |=> LEAF DOES NOT EXIST / INVALID / UNSUPPORTED\n", std::string( 15, ' ' ).c_str() );

    for ( auto& [name, idx] : cpuid.get_schema() )
    {
        auto& cpuid_reg = cpuid[ name ];
        printf( "%15c[%s] {%Xh}\n", ' ', name.c_str(), cpuid_reg.data );

        for ( auto& reg : idx.fields )
        {
            auto field_value = cpuid_reg( { reg.bit_start, reg.bit_end } );
            printf( "%70s {%5llu:%-5llu} %15s %llXh\n",
                    reg.name.c_str(),
                    reg.bit_start,
                    reg.bit_end,
                    "=",
                    field_value.as<uint64_t>() );
        }
    }
    // don't judge
    printf( "\n%s\n\n", std::string( 128, '-' ).c_str() );
}

void enum_cpuid_leaves()
{
    cpu_query tmp( initial_cpuid_function_id );
    [[maybe_unused]] int32_t max_cpuid_leaf = tmp.get_bit_range( "eax", 0, 31 );

    std::unordered_map<int, int> sub_leaves_map = { {7, 3}, {0x0d, 3}, {0x12, 3}, {0x10, 4}, {0x17, 4}, {0x18, 2}, {0x1b, 2}, {0x1d, 2}, {0x14, 2}, };

    for ( auto it = initial_cpuid_function_id; it <= max_cpuid_leaf /* can also test with to see behavior => max_cpuid_function_id*/; it++ )
    {
        const int sub_leaves = sub_leaves_map.contains( it ) ? sub_leaves_map[ it ] : 1;

        for ( auto sub = 0; sub < sub_leaves; sub++ )
            print_cpuid( it, sub );
    }

    for ( auto it = ext_cpuid_function_id; it < ext_cpuid_function_id_max; it++ )
        print_cpuid( it );
}

auto get_cpu_brand()
{
    std::string brand_name;

    for ( auto it = 0x80000002; it < 0x80000006; it++ )
    {
        cpu_query brand_leaf( it );
        for ( auto& [name, idx] : brand_leaf.get_schema() )
        {
            uint32_t cpuid_data = brand_leaf[ name ].data;
            brand_name.append( reinterpret_cast< char* >( &cpuid_data ), 4 );
        }
    }

    return brand_name;
}

int main( int argc, char** argv, char** envp )
{
    // [[Example for KM display of all MSRs that have a schema in the list]]
    //
    // for ( const auto& index : msr_schema_list | std::views::keys )
    // {
    //     msr_query msr( index );
    //     for ( auto& [name, schema] : msr.get_schema() )
    //     {
    //         serial::dbgp( "%15c[%s] {%llXh}\n", ' ', name.c_str(), msr.bf.data );
    // 
    //         for ( auto& field : schema.fields )
    //         {
    //             auto field_value = msr.bf.range( field.bit_start, field.bit_end );
    //             serial::dbgp( "%70s {%5llu:%-5llu} %15s %llXh\n",
    //                                                field.name.c_str(),
    //                                                field.bit_start,
    //                                                field.bit_end,
    //                                                "=",
    //                                                static_cast< uint64_t >( field_value ) );
    //         }
    //     }
    // }


    // [[Example for UM display of all CPUID leaves and their associated bitfields / values]]
    //
    enum_cpuid_leaves();
    
    const auto cpu_brand = get_cpu_brand();
    printf( "%s\n", cpu_brand.c_str() );

    // Other examples
    //
    cpu_query cpu( 0x0a, 0 );
    auto fv = cpu[ "eax" ].range(8, 15).as<uint8_t>();
    printf( "%Xh\n", fv );

    auto field = cpu.get_field( "eax", { 16,23 } );
    printf( "%s %Xh\n", field.field_name.c_str(), field.value );

    getchar();

    return 0;
}
