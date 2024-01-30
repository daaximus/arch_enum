# ArchEnum

This project provides a convenient way to query information about the CPU using the CPUID and MSR (Model Specific Register) instructions. 

It includes definitions for a variety of known CPUID leaves and MSR registers, allowing you to easily inspect the data returned by these instructions in a structured way. The information is also displayed in a manner that makes it possible to generate a report, and diff various machines quickly. As an example here is the output of two machines, one a VM and one that isn't:

![https://i.imgur.com/5UEYMdu.png](https://i.imgur.com/5UEYMdu.png)

## Usage

You can use the `cpu_query` class to perform a CPUID query. Simply instantiate an object of this class, passing the desired function and sub-function IDs to the constructor. You can then use the various methods of this class to inspect the data returned by the CPUID instruction.

Examples:

```cpp
// Get masked value of bit range in eax.
//
cpu_query cpu( 0x0a, 0 );
auto fv = cpu[ "eax" ].range(8, 15).as<uint8_t>();
printf( "%Xh\n", fv );

// Get name and value of bits 16:23 in eax for cpuid.0xa
//
auto field = cpu.get_field( "eax", { 16,23 } );
printf( "%s %Xh\n", field.field_name.c_str(), field.value );

// More involved example to enumerate all CPUID leaves and sub functions.
//
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

    std::unordered_map<int, int> sub_leaves_map = { {7, 3}, {0x0d, 3}, 
                                                   {0x12, 3}, {0x10, 4}, 
                                                   {0x17, 4}, {0x18, 2}, 
                                                   {0x1b, 2}, {0x1d, 2}, 
                                                   {0x14, 2}, };
    
	/* can also test with to see behavior; replace max_cpuid_leaf => max_cpuid_function_id*/
    for ( auto it = initial_cpuid_function_id; it <= max_cpuid_leaf ; it++ )
    {
        const int sub_leaves = sub_leaves_map.contains( it ) ? sub_leaves_map[ it ] : 1;

        for ( auto sub = 0; sub < sub_leaves; sub++ )
            print_cpuid( it, sub );
    }

    for ( auto it = ext_cpuid_function_id; it < ext_cpuid_function_id_max; it++ )
        print_cpuid( it );
}
```

The output of the `enum_cpuid_leaves` function is shown here:
## CPUID [0.0]
               [eax] {16h}
                                                 max_input_value_cpuid {    0:31   }               = 16h
               [ebx] {756E6547h}
                                                               vendor1 {    0:31   }               = 756E6547h
               [ecx] {6C65746Eh}
                                                               vendor3 {    0:31   }               = 6C65746Eh
               [edx] {49656E69h}
                                                               vendor2 {    0:31   }               = 49656E69h

--------------------------------------------------------------------------------------------------------------------------------

## CPUID [1.0]
               [eax] {A0655h}
                                                                  type {    0:7    }               = 55h
                                                                family {    8:15   }               = 6h
                                                                 model {   16:23   }               = Ah
                                                           stepping_id {   24:31   }               = 0h
               [ebx] {D200800h}
                                                           brand_index {    0:7    }               = 0h
                                                     clflush_line_size {    8:15   }               = 8h
                                                max_logical_processors {   16:23   }               = 20h
                                                       initial_apic_id {   24:31   }               = Dh
               [ecx] {7FFAFBBFh}
                                                                  sse3 {    0:0    }               = 1h
                                                             pclmulqdq {    1:1    }               = 1h
                                                                dtes64 {    2:2    }               = 1h
                                                               monitor {    3:3    }               = 1h
                                                                ds_cpl {    4:4    }               = 1h
                                                                   vmx {    5:5    }               = 1h
                                                                   smx {    6:6    }               = 0h
                                                                  eist {    7:7    }               = 1h
                                                                   tm2 {    8:8    }               = 1h
                                                                 ssse3 {    9:9    }               = 1h
                                                               cnxt_id {   10:10   }               = 0h
                                                                  sdbg {   11:11   }               = 1h
                                                                   fma {   12:12   }               = 1h
                                                            cmpxchg16b {   13:13   }               = 1h
                                                   xtpr_update_control {   14:14   }               = 1h
                                                                  pdcm {   15:15   }               = 1h
                                                           reserved_16 {   16:16   }               = 0h
                                                                  pcid {   17:17   }               = 1h
                                                                   dca {   18:18   }               = 0h
                                                                sse4_1 {   19:19   }               = 1h
                                                                sse4_2 {   20:20   }               = 1h
                                                                x2apic {   21:21   }               = 1h
                                                                 movbe {   22:22   }               = 1h
                                                                popcnt {   23:23   }               = 1h
                                                          tsc_deadline {   24:24   }               = 1h
                                                                 aesni {   25:25   }               = 1h
                                                                 xsave {   26:26   }               = 1h
                                                               osxsave {   27:27   }               = 1h
                                                                   avx {   28:28   }               = 1h
                                                                  f16c {   29:29   }               = 1h
                                                                rdrand {   30:30   }               = 1h
                                                              not_used {   31:31   }               = 0h
               [edx] {BFEBFBFFh}
                                                           fpu_on_chip {    0:0    }               = 1h
                                                                   vme {    1:1    }               = 1h
                                                                    de {    2:2    }               = 1h
                                                                   pse {    3:3    }               = 1h
                                                                   tsc {    4:4    }               = 1h
                                                                   msr {    5:5    }               = 1h
                                                                   pae {    6:6    }               = 1h
                                                                   mce {    7:7    }               = 1h
                                                                   cx8 {    8:8    }               = 1h
                                                                  apic {    9:9    }               = 1h
                                                           reserved_10 {   10:10   }               = 0h
                                                                   sep {   11:11   }               = 1h
                                                                  mtrr {   12:12   }               = 1h
                                                                   pge {   13:13   }               = 1h
                                                                   mca {   14:14   }               = 1h
                                                                  cmov {   15:15   }               = 1h
                                                                   pat {   16:16   }               = 1h
                                                                pse_36 {   17:17   }               = 1h
                                                                   psn {   18:18   }               = 0h
                                                               clflush {   19:19   }               = 1h
                                                           reserved_20 {   20:20   }               = 0h

-------------------------------------------------------------------

**Intel(R) Core(TM) i9-10850K CPU @ 3.60GHz**

-------------------------------------------------------------------

`[[OTHER LEAF OUTPUT REMOVED FOR BREVITY]]`

-------------------------------------------------------------------

Similarly, you can use the `msr_query` class to perform an MSR query. Instantiate an object of this class, passing the desired MSR index to the constructor. You can then use the various methods of this class to inspect the data returned by the `RDMSR` instruction.

```cpp
// [[Example for KM display of all MSRs that have a schema in the list]]
//
for ( const auto& index : msr_schema_list | std::views::keys )
{
    msr_query msr( index );
    for ( auto& [name, schema] : msr.get_schema() )
    {
        serial::dbgp( "%15c[%s] {%llXh}\n", ' ', name.c_str(), msr.bf.data );

        for ( auto& field : schema.fields )
        {
            auto field_value = msr.bf.range( field.bit_start, field.bit_end );
            serial::dbgp( "%70s {%5llu:%-5llu} %15s %llXh\n",
                                               field.name.c_str(),
                                               field.bit_start,
                                               field.bit_end,
                                               "=",
                                               static_cast< uint64_t >( field_value ) );
        }
    }
}
```

**Results:**

![https://i.imgur.com/2CennXN.png](https://i.imgur.com/2CennXN.png)

![https://i.imgur.com/6DUCJOX.png](https://i.imgur.com/6DUCJOX.png)

## Building

This project is written to be compatible with c++20, so a compatible compiler is required. It has been tested with MSVC and Clang on Windows.

## Limitations

This project does not include definitions for all possible CPUID leaves and MSR registers. It only includes the ones that are most commonly used / supported; for MSRs I just picked a bunch of test Architectural MSRs from the documentation. I will add more in the future, but you can also quickly add them.

If a CPUID leaf or MSR register is not included in the definitions, you can still perform a query for it, but the data will not be structured and will need to be interpreted manually.

## License

This project is licensed under the MIT license. See the LICENSE file for more details.

```Copyright <YEAR> <COPYRIGHT HOLDER>
Copyright 2023. daaximus.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

```

