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

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct cpuid_field
{
    std::string name;
    std::size_t bit_start;
    std::size_t bit_end;
};

struct register_schema
{
    std::vector<cpuid_field> fields;

    register_schema() = default;
    register_schema( std::initializer_list<cpuid_field> init ) : fields( init ) {}

    const cpuid_field& operator[]( std::size_t i ) const
    {
        return fields[ i ];
    }
};

using function_schema = std::unordered_map<std::string, register_schema>;

inline function_schema schema_00 = {
    {"eax", {
        {"max_input_value_cpuid", 0, 31}
    }},
    {"ebx", {
        {"vendor1", 0, 31}          // "Genu"
    }},
    {"ecx", {
        {"vendor3", 0, 31}          // "ntel"
    }},
    {"edx", {
        {"vendor2", 0, 31}          // "ineI”
    }}
};

///////////////////////////////////////////////////////////////// CPUID FUNCTION 01h SCHEMA START ///////////////////////////////////////////////////////////////// 
//
inline register_schema schema_01_eax = {
    {
        // eax version information: type, family, model, and stepping id
        //
        { "type", 0, 7 },  // bits 07-00: type
        { "family", 8, 15 },  // bits 15-08: family
        { "model", 16, 23 },  // bits 23-16: model
        { "stepping_id", 24, 31 }  // bits 31-24: stepping ID
    }
};

inline register_schema schema_01_ebx = {
    {
        { "brand_index", 0, 7 },                    // bits 07-00: brand index
        { "clflush_line_size", 8, 15 },             // bits 15-08: CLFLUSH line size (Value * 8 = cache line size in bytes; used also by CLFLUSHOPT)
        { "max_logical_processors", 16, 23 },       // bits 23-16: maximum number of addressable IDs for logical processors in this physical package*
        { "initial_apic_id", 24, 31 }               // bits 31-24: initial APIC ID**
    }
};

inline register_schema schema_01_ecx = {
    {
        { "sse3", 0, 0 },                   // bit 0: Streaming SIMD Extensions 3 (SSE3)
        { "pclmulqdq", 1, 1 },              // bit 1: PCLMULQDQ
        { "dtes64", 2, 2 },                 // bit 2: 64-bit DS Area
        { "monitor", 3, 3 },                // bit 3: MONITOR/MWAIT
        { "ds_cpl", 4, 4 },                 // bit 4: CPL Qualified Debug Store
        { "vmx", 5, 5 },                    // bit 5: Virtual Machine Extensions
        { "smx", 6, 6 },                    // bit 6: Safer Mode Extensions
        { "eist", 7, 7 },                   // bit 7: Enhanced Intel SpeedStep® technology
        { "tm2", 8, 8 },                    // bit 8: Thermal Monitor 2
        { "ssse3", 9, 9 },                  // bit 9: Supplemental Streaming SIMD Extensions 3 (SSSE3)
        { "cnxt_id", 10, 10 },              // bit 10: L1 Context ID
        { "sdbg", 11, 11 },                 // bit 11: IA32_DEBUG_INTERFACE MSR for silicon debug
        { "fma", 12, 12 },                  // bit 12: FMA extensions using YMM state
        { "cmpxchg16b", 13, 13 },           // bit 13: CMPXCHG16B Available
        { "xtpr_update_control", 14, 14 },  // bit 14: xTPR Update Control
        { "pdcm", 15, 15 },                 // bit 15: Perfmon and Debug Capability
        { "reserved_16", 16, 16 },          // bit 16: Reserved
        { "pcid", 17, 17 },                 // bit 17: Process-context identifiers
        { "dca", 18, 18 },                  // bit 18: Prefetch data from a memory mapped device
        { "sse4_1", 19, 19 },               // bit 19: SSE4.1
        { "sse4_2", 20, 20 },               // bit 20: SSE4.2
        { "x2apic", 21, 21 },               // bit 21: x2APIC feature
        { "movbe", 22, 22 },                // bit 22: MOVBE instruction
        { "popcnt", 23, 23 },               // bit 23: POPCNT instruction
        { "tsc_deadline", 24, 24 },         // bit 24: TSC-Deadline
        { "aesni", 25, 25 },                // bit 25: AESNI instruction extensions
        { "xsave", 26, 26 },                // bit 26: XSAVE/XRSTOR processor extended states feature, the XSETBV/XGETBV instructions, and XCR0
        { "osxsave", 27, 27 },              // bit 27: OS has set CR4.OSXSAVE[bit 18] to enable XSETBV/XGETBV instructions
        { "avx", 28, 28 },                  // bit 28: AVX instruction extensions
        { "f16c", 29, 29 },                 // bit 29: Processor supports 16-bit floating-point conversion instructions
        { "rdrand", 30, 30 },               // bit 30: Processor supports RDRAND instruction
        { "not_used", 31, 31 }              // bit 31: Always returns 0
    }
};

inline register_schema schema_01_edx = {
    {
        { "fpu_on_chip", 0, 0 },  // bit 0: Floating-Point Unit On-Chip
        { "vme", 1, 1 },  // bit 1: Virtual 8086 Mode Enhancements
        { "de", 2, 2 },  // bit 2: Debugging Extensions
        { "pse", 3, 3 },  // bit 3: Page Size Extension
        { "tsc", 4, 4 },  // bit 4: Time Stamp Counter
        { "msr", 5, 5 },  // bit 5: Model Specific Registers RDMSR and WRMSR Instructions
        { "pae", 6, 6 },  // bit 6: Physical Address Extension
        { "mce", 7, 7 },  // bit 7: Machine Check Exception
        { "cx8", 8, 8 },  // bit 8: CMPXCHG8B Instruction
        { "apic", 9, 9 },  // bit 9: APIC On-Chip
        { "reserved_10", 10, 10 },  // bit 10: Reserved
        { "sep", 11, 11 },  // bit 11: SYSENTER and SYSEXIT Instructions
        { "mtrr", 12, 12 },  // bit 12: Memory Type Range Registers
        { "pge", 13, 13 },  // bit 13: Page Global Bit
        { "mca", 14, 14 },  // bit 14: Machine Check Architecture
        { "cmov", 15, 15 },  // bit 15: Conditional Move Instructions
        { "pat", 16, 16 },  // bit 16: Page Attribute Table
        { "pse_36", 17, 17 },  // bit 17: 36-Bit Page Size Extension
        { "psn", 18, 18 },  // bit 18: Processor Serial Number
        { "clflush", 19, 19 },  // bit 19: CLFLUSH Instruction
        { "reserved_20", 20, 20 }  // bit 20: Reserved
    }
};

inline function_schema schema_01 = {
    {"eax", schema_01_eax},
    {"ebx", schema_01_ebx},
    {"ecx", schema_01_ecx},
    {"edx", schema_01_edx},
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 01h SCHEMA END /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 02h SCHEMA START ///////////////////////////////////////////////////////////////
//
inline function_schema schema_02 = {
    {"eax", { {"cache_and_tlb_information", 0, 31}, } },
    {"ebx", { {"cache_and_tlb_information", 0, 31}, } },
    {"ecx", { {"cache_and_tlb_information", 0, 31}, } },
    {"edx", { {"cache_and_tlb_information", 0, 31}, } },
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 02h SCHEMA END //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 03h SCHEMA START ////////////////////////////////////////////////////////////////
//
inline function_schema schema_03 = {
    {"eax", {
        {"reserved_eax", 0, 31}
    }},
    {"ebx", {
        {"reserved_ebx", 0, 31}
    }},
    {"ecx", {
        {"processor_serial_number_low", 0, 31}   // Only available in Pentium III
    }},
    {"edx", {
        {"processor_serial_number_high", 0, 31}  // Only available in Pentium III
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 03h SCHEMA END //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 04h SCHEMA START ////////////////////////////////////////////////////////////////
//
inline function_schema schema_04h = {
    {"eax", {
        {"cache_type", 0, 4},
        {"cache_level", 5, 7},
        {"self_initializing", 8, 8},
        {"fully_associative", 9, 9},
        {"reserved_eax1", 10, 13},
        {"logical_processors", 14, 25},
        {"cores_per_package", 26, 31}
    }},
    {"ebx", {
        {"line_size", 0, 11},
        {"partitions", 12, 21},
        {"ways_of_associativity", 22, 31}
    }},
    {"ecx", {
        {"number_of_sets", 0, 31}
    }},
    {"edx", {
        {"write_back_invalidate", 0, 0},
        {"cache_inclusiveness", 1, 1},
        {"complex_cache_indexing", 2, 2},
        {"reserved_edx", 3, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 04h SCHEMA END //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 05h SCHEMA START ////////////////////////////////////////////////////////////////
//
inline function_schema schema_05 = {
    {"eax", {
        {"smallest_monitor_line", 0, 15},
        {"reserved_eax", 16, 31}
    }},
    {"ebx", {
        {"largest_monitor_line", 0, 15},
        {"reserved_ebx", 16, 31}
    }},
    {"ecx", {
        {"monitor_mwait_ext_enum", 0, 0},
        {"interrupts_break_event", 1, 1},
        {"reserved_ecx", 2, 31}
    }},
    {"edx", {
        {"c0_sub_cstates", 0, 3},
        {"c1_sub_cstates", 4, 7},
        {"c2_sub_cstates", 8, 11},
        {"c3_sub_cstates", 12, 15},
        {"c4_sub_cstates", 16, 19},
        {"c5_sub_cstates", 20, 23},
        {"c6_sub_cstates", 24, 27},
        {"c7_sub_cstates", 28, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 05h SCHEMA END //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 06h SCHEMA START ////////////////////////////////////////////////////////////////
//
inline register_schema schema_06_eax = {
{
    { "digital_temperature_sensor", 0, 0 },
    { "turbo_boost", 1, 1 },
    { "arat", 2, 2 },
    { "reserved_eax_03", 3, 3 },
    { "pln", 4, 4 },
    { "ecmd", 5, 5 },
    { "ptm", 6, 6 },
    { "hwp_base_registers", 7, 7 },
    { "hwp_notification", 8, 8 },
    { "hwp_activity_window", 9, 9 },
    { "hwp_energy_performance_preference", 10, 10 },
    { "hwp_package_level_request", 11, 11 },
    { "reserved_eax_12", 12, 12 },
    { "hdc_base_registers", 13, 13 },
    { "turbo_boost_max_3", 14, 14 },
    { "hwp_capabilities", 15, 15 },
    { "hwp_peci_override", 16, 16 },
    { "flexible_hwp", 17, 17 },
    { "fast_access_hwp_request", 18, 18 },
    { "hw_feedback", 19, 19 },
    { "ignore_idle_logical_processor_hwp", 20, 20 },
    { "reserved_eax_21_22", 21, 22 },
    { "thread_director", 23, 23 },
    { "therm_interrupt", 24, 24 },
    { "reserved_eax_25_31", 25, 31 }
}
};

inline register_schema schema_06_ebx = {
    {
        { "number_of_interrupt_thresholds", 0, 3 },
        { "reserved_ebx", 4, 31 },
    },
};

inline register_schema schema_06_ecx = {
    {
        { "hardware_coord_feedback", 0, 0 },
        { "reserved_ecx_01_02", 1, 2 },
        { "performance_energy_bias_preference", 3, 3 },
        { "reserved_ecx_04_07", 4, 7 },
        { "thread_director_classes", 8, 15 },
        { "reserved_ecx", 16, 31 },
    },
};

inline register_schema schema_06_edx = {
    {
        { "performance_capability_reporting", 0, 0 },
        { "energy_efficiency_capability_reporting", 1, 1 },
        { "reserved_edx_02_07", 2, 7 },
        { "hardware_feedback_interface_structure_size", 8, 11 },
        { "logical_processor_row_index", 16, 31 }
    }
};

inline function_schema schema_06 = {
    {"eax", schema_06_eax},
    {"ebx", schema_06_ebx},
    {"ecx", schema_06_ecx},
    {"edx", schema_06_edx},
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 06h SCHEMA END //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 07h SCHEMA START ////////////////////////////////////////////////////////////////
//
// cpuid eax = 07h, ecx = 00h
//
inline function_schema schema_07_00 = {
    {"eax", {
        {"maximum_sub_leaves", 0, 31},
    }},
    {"ebx", {
        {"fsgsbase", 0, 0},
        {"ia32_tsc_adjust", 1, 1},
        {"sgx", 2, 2},
        {"bmi1", 3, 3},
        {"hle", 4, 4},
        {"avx2", 5, 5},
        {"fdp_excptn_only", 6, 6},
        {"smep", 7, 7},
        {"bmi2", 8, 8},
        {"enhanced_rep_movsb/stosb", 9, 9},
        {"invpcid", 10, 10},
        {"rtm", 11, 11},
        {"rdt-m", 12, 12},
        {"deprecates_fpu_cs_and_fpu_ds_values", 13, 13},
        {"mpx", 14, 14},
        {"rdt-a", 15, 15},
        {"avx512f", 16, 16},
        {"avx512dq", 17, 17},
        {"rdseed", 18, 18},
        {"adx", 19, 19},
        {"smap", 20, 20},
        {"avx512_ifma", 21, 21},
        {"reserved_ebx_22", 22, 22},
        {"clflushopt", 23, 23},
        {"clwb", 24, 24},
        {"intel_processor_trace", 25, 25},
        {"avx512pf", 26, 26},
        {"avx512er", 27, 27},
        {"avx512cd", 28, 28},
        {"sha", 29, 29},
        {"avx512bw", 30, 30},
        {"avx512vl", 31, 31}
    }},
    {"ecx", {
        {"prefetchwt1", 0, 0},
        {"avx512_vbmi", 1, 1},
        {"umip", 2, 2},
        {"pku", 3, 3},
        {"ospke", 4, 4},
        {"waitpkg", 5, 5},
        {"avx512_vbmi2", 6, 6},
        {"cet_ss", 7, 7},
        {"gfni", 8, 8},
        {"vaes", 9, 9},
        {"vpclmulqdq", 10, 10},
        {"avx512_vnni", 11, 11},
        {"avx512_bitalg", 12, 12},
        {"tme_en", 13, 13},
        {"avx512_vpopcntdq", 14, 14},
        {"reserved_ecx_15", 15, 15},
        {"la57", 16, 16},
        {"mawau", 17, 21},
        {"rdpid_and_ia32_tsc_aux", 22, 22},
        {"kl", 23, 23},
        {"bus_lock_detect", 24, 24},
        {"cldemote", 25, 25},
        {"reserved_ecx_26", 26, 26},
        {"movdiri", 27, 27},
        {"movdir64b", 28, 28},
        {"enqcmd", 29, 29},
        {"sgx_lc", 30, 30},
        {"pks", 31, 31}
    }},
    {"edx", {
        {"reserved_edx_00", 0, 0},
        {"sgx-keys", 1, 1},
        {"avx512_4vnniw", 2, 2},
        {"avx512_4fmaps", 3, 3},
        {"fast_short_rep_mov", 4, 4},
        {"uintr", 5, 5},
        {"reserved_edx_06-07", 6, 7},
        {"avx512_vp2intersect", 8, 8},
        {"srbds_ctrl", 9, 9},
        {"md_clear", 10, 10},
        {"rtm_always_abort", 11, 11},
        {"reserved_edx_12", 12, 12},
        {"rtm_force_abort", 13, 13},
        {"serialize", 14, 14},
        {"hybrid", 15, 15},
        {"tsxldtrk", 16, 16},
        {"reserved_edx_17", 17, 17},
        {"pconfig", 18, 18},
        {"architectural_lbrs", 19, 19},
        {"cet_ibt", 20, 20},
        {"reserved_edx_21", 21, 21},
        {"amx-bf16", 22, 22},
        {"avx512_fp16", 23, 23},
        {"amx-tile", 24, 24},
        {"amx-int8", 25, 25},
        {"ibrs_and_ibpb", 26, 26},
        {"stibp", 27, 27},
        {"l1d_flush", 28, 28},
        {"ia32_arch_capabilities", 29, 29},
        {"ia32_core_capabilities", 30, 30},
        {"ssbd", 31, 31}
    }}
};

// cpuid eax = 07h, ecx = 01h
//
inline function_schema schema_07_01 = {
    {"eax", {
        {"reserved_eax1", 0, 3},
        {"avx_vnni", 4, 4},
        {"avx512_bf16", 5, 5},
        {"reserved_eax2", 6, 9},
        {"fast_zero_rep_movsb", 10, 10},
        {"fast_short_rep_stosb", 11, 11},
        {"fast_short_rep_cmpsb_scasb", 12, 12},
        {"reserved_eax3", 13, 21},
        {"hreset", 22, 22},
        {"reserved_eax4", 23, 31}
    }},
    {"ebx", {
        {"ia32_ppin_ppin_ctl_msr", 0, 0},
        {"reserved_ebx", 1, 31}
    }},
    {"ecx", {
        {"reserved_ecx", 0, 31}
    }},
    {"edx", {
        {"reserved_edx1", 0, 17},
        {"cet_sss", 18, 18},
        {"reserved_edx2", 19, 31}
    }}
};

// cpuid eax = 07h, ecx = 02h
//
inline function_schema schema_07_02 = {
    {"eax", {
        {"reserved_eax", 0, 31}
    }},
    {"ebx", {
        {"reserved_ebx", 0, 31}
    }},
    {"ecx", {
        {"reserved_ecx", 0, 31}
    }},
    {"edx", {
        {"psfd", 0, 0},
        {"ipred_ctrl", 1, 1},
        {"rrsba_ctrl", 2, 2},
        {"ddpd_u", 3, 3},
        {"bhi_ctrl", 4, 4},
        {"mcdt_no", 5, 5},
        {"reserved_edx", 6, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 07h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 09h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_09 = {
    {"eax", {
        {"ia32_platform_dca_cap", 0, 31}
    }},
    {"ebx", {
        {"reserved_ebx", 0, 31}
    }},
    {"ecx", {
        {"reserved_ecx", 0, 31}
    }},
    {"edx", {
        {"reserved_edx", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 09h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 0Ah SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_0a = {
    {"eax", {
        {"version_id", 0, 7},
        {"general_purpose_counter_per_lp", 8, 15},
        {"general_purpose_counter_bit_width", 16, 23},
        {"length_ebx_bit_vector", 24, 31}
    }},
    {"ebx", {
        {"core_cycle_event_na", 0, 0},
        {"instruction_retired_event_na", 1, 1},
        {"reference_cycles_event_na", 2, 2},
        {"last_level_cache_reference_event_na", 3, 3},
        {"last_level_cache_misses_event_na", 4, 4},
        {"branch_instruction_retired_event_na", 5, 5},
        {"branch_mispredict_retired_event_na", 6, 6},
        {"top_down_slots_event_na", 7, 7},
        {"reserved_ebx", 8, 31}
    }},
    {"ecx", {
        {"supported_fixed_counters_bitmask", 0, 31}
    }},
    {"edx", {
        {"number_fixed_function_counters", 0, 4},
        {"fixed_function_counters_bit_width", 5, 12},
        {"reserved_edx1", 13, 14},
        {"anythread_deprecation", 15, 15},
        {"reserved_edx2", 16, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 0Ah SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 0Dh SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_0d_00 = {
    {"eax", {
        {"x87_state", 0, 0},
        {"sse_state", 1, 1},
        {"avx_state", 2, 2},
        {"mpx_state", 3, 4},
        {"avx_512_state", 5, 7},
        {"ia32_xss_1", 8, 8},
        {"pkru_state", 9, 9},
        {"ia32_xss_2", 10, 16},
        {"tilecfg_state", 17, 17},
        {"tiledata_state", 18, 18},
        {"reserved_eax", 19, 31}
    }},
    {"ebx", {
        {"max_size_enabled_features", 0, 31}
    }},
    {"ecx", {
        {"max_size_all_supported_features", 0, 31}
    }},
    {"edx", {
        {"xcr0_upper_32_bits", 0, 31}
    }}
};

inline function_schema schema_0d_01 = {
    {"eax", {
        {"xsaveopt_available", 0, 0},
        {"xsavec_and_compacted_xrstor", 1, 1},
        {"xgetbv_ecx1", 2, 2},
        {"xsaves_xrstors_and_ia32_xss", 3, 3},
        {"xfd_support", 4, 4},
        {"reserved_eax", 5, 31}
    }},
    {"ebx", {
        {"xsave_area_size", 0, 31}
    }},
    {"ecx", {
        {"xcr0_used", 0, 7},
        {"pt_state", 8, 8},
        {"xcr0_used_2", 9, 9},
        {"pasid_state", 10, 10},
        {"cet_user_state", 11, 11},
        {"cet_supervisor_state", 12, 12},
        {"hdc_state", 13, 13},
        {"uintr_state", 14, 14},
        {"lbr_state", 15, 15},
        {"hwp_state", 16, 16},
        {"xcr0_used_3", 17, 18},
        {"reserved_ecx", 19, 31}
    }},
    {"edx", {
        {"ia32_xss_upper_32_bits", 0, 31}
    }}
};

inline function_schema schema_0d_ff = {
    {"eax", {
        {"feature_save_area_size", 0, 31}
    }},
    {"ebx", {
        {"feature_save_area_offset", 0, 31}
    }},
    {"ecx", {
        {"support_in_ia32_xss", 0, 0},
        {"alignment_in_compacted_xsave", 1, 1},
        {"reserved_ecx", 2, 31}
    }},
    {"edx", {
        {"reserved_edx", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 0Dh SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 0Fh SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_0f = {
    {"eax", {
        {"reserved_eax", 0, 31}
    }},
    {"ebx", {
        {"max_rmid_range", 0, 31}
    }},
    {"ecx", {
        {"reserved_ecx", 0, 31}
    }},
    {"edx", {
        {"reserved_edx_bit0", 0, 0},
        {"l3_cache_rdtm", 1, 1},
        {"reserved_edx", 2, 31}
    }}
};

inline function_schema schema_0f_01 = {
    {"eax", {
        {"reserved_eax", 0, 31}
    }},
    {"ebx", {
        {"conversion_factor", 0, 31}
    }},
    {"ecx", {
        {"max_rmid_range_res_type", 0, 31}
    }},
    {"edx", {
        {"l3_occupancy_monitoring", 0, 0},
        {"l3_total_bandwidth_monitoring", 1, 1},
        {"l3_local_bandwidth_monitoring", 2, 2},
        {"reserved_edx", 3, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 0Fh SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 10h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_10 = {
    {"eax", {
        {"reserved_eax", 0, 31}
    }},
    {"ebx", {
        {"reserved_b0", 0, 0},
        {"l3_cache_allocation", 1, 1},
        {"l2_cache_allocation", 2, 2},
        {"memory_bandwidth_allocation", 3, 3},
        {"reserved_ebx", 4, 31}
    }},
    {"ecx", {
        {"reserved_ecx", 0, 31}
    }},
    {"edx", {
        {"reserved_edx", 0, 31}
    }}
};

inline function_schema schema_10_01 = {
    {"eax", {
        {"length_of_capacity_bitmask", 0, 4},
        {"reserved_eax", 5, 31}
    }},
    {"ebx", {
        {"isolation_contention", 0, 31}
    }},
    {"ecx", {
        {"reserved_c0_1", 0, 1},
        {"cdp_support", 2, 2},
        {"reserved_ecx", 3, 31}
    }},
    {"edx", {
        {"highest_cos_number", 0, 15},
        {"reserved_edx", 16, 31}
    }}
};

inline function_schema schema_10_02 = {
    {"eax", {
        {"length_of_capacity_bitmask", 0, 4},
        {"reserved_eax", 5, 31}
    }},
    {"ebx", {
        {"isolation_contention", 0, 31}
    }},
    {"ecx", {
        {"reserved_ecx", 0, 31}
    }},
    {"edx", {
        {"highest_cos_number", 0, 15},
        {"reserved_edx", 16, 31}
    }}
};

inline function_schema schema_10_03 = {
    {"eax", {
        {"max_mba_throttling", 0, 11},
        {"reserved_eax", 12, 31}
    }},
    {"ebx", {
        {"reserved_ebx", 0, 31}
    }},
    {"ecx", {
        {"reserved_ecx", 0, 1},
        {"response_of_delay_values_is_linear", 2, 2},
        {"reserved_ecx", 3, 31}
    }},
    {"edx", {
        {"highest_cos_number", 0, 15},
        {"reserved_edx", 16, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 10h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 11h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_0b = {
    {"eax", {
        {"x2apic_shift", 0, 4},
        {"reserved", 5, 31}
    }},
    {"ebx", {
        {"logical_processors", 0, 15},
        {"reserved", 16, 31}
    }},
    {"ecx", {
        {"sub_leaf_index", 0, 7},
        {"domain_type", 8, 15},
        {"reserved", 16, 31}
    }},
    {"edx", {
        {"x2apic_id_current_lp", 0, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 11h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 12h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_12_00 = {
    {"eax", {
        {"sgx1", 0, 0},
        {"sgx2", 1, 1},
        {"reserved_eax", 2, 4},
        {"enclv_support", 5, 5},
        {"encls_support", 6, 6},
        {"enclu_support", 7, 7},
        {"reserved_eax", 8, 9},
        {"encls_eupdatesvn", 10, 10},
        {"enclu_edeccssa", 11, 11},
        {"reserved_eax", 12, 31}
    }},
    {"ebx", {
        {"miscselect", 0, 31}
    }},
    {"ecx", {
        {"reserved_ecx", 0, 31}
    }},
    {"edx", {
        {"max_enclave_size_not64", 0, 7},
        {"max_enclave_size_64", 8, 15},
        {"reserved_edx", 16, 31}
    }}
};

inline function_schema schema_12_01 = {
    {"eax", {
        {"secs_attributes_31_0", 0, 31}
    }},
    {"ebx", {
        {"secs_attributes_63_32", 0, 31}
    }},
    {"ecx", {
        {"secs_attributes_95_64", 0, 31}
    }},
    {"edx", {
        {"secs_attributes_127_96", 0, 31}
    }}
};

inline function_schema schema_12_02 = {
    {"eax", {
        {"sub_leaf_type", 0, 3},
        {"physical_address_base_31_12", 12, 31}
    }},
    {"ebx", {
        {"physical_address_base_51_32", 0, 19}
    }},
    {"ecx", {
        {"epc_section_property_encoding", 0, 3},
        {"epc_section_size_31_12", 12, 31}
    }},
    {"edx", {
        {"epc_section_size_51_32", 0, 19}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 12h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 14h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_14_00 = {
    {"eax", {
        {"max_sub_leaf_supported", 0, 31}
    }},
    {"ebx", {
        {"cr3_filter", 0, 0},
        {"configurable_psb_cycle_accurate", 1, 1},
        {"ip_tracestop_filter_preservation", 2, 2},
        {"mtc_timing_packet", 3, 3},
        {"ptwrite_support", 4, 4},
        {"power_event_trace", 5, 5},
        {"psb_pmi_preservation", 6, 6},
        {"event_trace_packet_gen", 7, 7},
        {"disable_tnt_packet_gen", 8, 8}
    }},
    {"ecx", {
        {"enable_tracing_topa", 0, 0},
        {"topa_var_entries", 1, 1},
        {"single_range_output", 2, 2},
        {"trace_transport_output", 3, 3},
        {"lip_values_include_cs", 31, 31}
    }},
    {"edx", {{"reserved", 0, 31}}},
};
inline function_schema schema_14_01 = {
    {"eax", {
        {"num_addr_ranges", 0, 2},
        {"reserved", 3, 15},
        {"supported_mtc_period", 16, 31}
    }},
    {"ebx", {
        {"cycle_threshold_values", 0, 15},
        {"config_psb_freq", 16, 31}
    }},
    {"ecx", {{"reserved", 0, 31}}},
    {"edx", {{"reserved", 0, 31}}}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 14h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 15h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_15 = {
    {"eax", {
        {"denominator_tsc_core_crystal_clock_ratio", 0, 31}
    }},
    {"ebx", {
        {"numerator_tsc_core_crystal_clock_ratio", 0, 31}
    }},
    {"ecx", {
        {"core_crystal_clock_freq", 0, 31}
    }},
    {"edx", {{"reserved", 0, 31}}}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 15h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 16h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_16 = {
    {"eax", {
        {"processor_base_frequency_mhz", 0, 15}
    }},
    {"ebx", {
        {"maximum_frequency_mhz", 0, 15}
    }},
    {"ecx", {
        {"bus_reference_frequency_mhz", 0, 15}
    }},
    {"edx", {{"reserved", 0, 31}}}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 16h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 17h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_17_00 = {
    {"eax", {
        {"max_socid_index", 0, 31}
    }},
    {"ebx", {
        {"soc_vendor_id", 0, 15},
        {"is_vendor_scheme", 16, 16}
    }},
    {"ecx", {
        {"project_id", 0, 31}
    }},
    {"edx", {
        {"stepping_id", 0, 31}
    }}
};

// for ecx = 1...3 (07h is the sub-index used because bits 0,1,2 set in 07h)
//
inline function_schema schema_17_07 = {
    {"eax", {
        {"soc_vendor_brand_str", 0, 31}
    }},
    {"ebx", {
        {"soc_vendor_brand_str", 0, 31}
    }},
    {"ecx", {
        {"soc_vendor_brand_str", 0, 31}
    }},
    {"edx", {
        {"soc_vendor_brand_str", 0, 31}
    }}
};

// for ecx > maxsocid_index
//
inline function_schema schema_17_ff = {
    {"eax", {
        {"reserved", 0, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 17h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 18h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_18_00 = {
    {"eax", {
        {"max_input_value", 0, 31}
    }},
    {"ebx", {
        {"page_4k_support", 0, 0},
        {"page_2M_support", 1, 1},
        {"page_4M_support", 2, 2},
        {"page_1G_support", 3, 3},
        {"reserved1", 4, 7},
        {"partitioning", 8, 10},
        {"reserved2", 11, 15},
        {"ways_of_associativity", 16, 31}
    }},
    {"ecx", {
        {"number_of_sets", 0, 31}
    }},
    {"edx", {
        {"translation_cache_type", 0, 4},
        {"translation_cache_level", 5, 7},
        {"fully_associative_structure", 8, 8},
        {"reserved3", 9, 13},
        {"max_addressable_ids_for_lp", 14, 25},
        {"reserved4", 26, 31}
    }}
};

inline function_schema schema_18_01 = {
    {"eax", {
        {"reserved", 0, 31}
    }},
    {"ebx", {
        {"page_4k_support", 0, 0},
        {"page_2M_support", 1, 1},
        {"page_4M_support", 2, 2},
        {"page_1G_support", 3, 3},
        {"reserved1", 4, 7},
        {"partitioning", 8, 10},
        {"reserved2", 11, 15},
        {"ways_of_associativity", 16, 31}
    }},
    {"ecx", {
        {"number_of_sets", 0, 31}
    }},
    {"edx", {
        {"translation_cache_type", 0, 4},
        {"translation_cache_level", 5, 7},
        {"fully_associative_structure", 8, 8},
        {"reserved3", 9, 13},
        {"max_addressable_ids_for_lp", 14, 25},
        {"reserved4", 26, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 18h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 19h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_19 = {
    {"eax", {
        {"kl_cpl0_only_supported", 0, 0},
        {"kl_no_encrypt_supported", 1, 1},
        {"kl_no_decrypt_supported", 2, 2},
        {"reserved", 3, 31}
    }},
    {"ebx", {
        {"aeskle", 0, 0},
        {"reserved1", 1, 1},
        {"wide_key_locker_supported", 2, 2},
        {"reserved2", 3, 3},
        {"platform_support_key_locker_msrs", 4, 4},
        {"reserved3", 5, 31}
    }},
    {"ecx", {
        {"nobackup_supported", 0, 0},
        {"key_source_encoding_supported", 1, 1},
        {"reserved", 2, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 19h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Ah SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_1a = {
    {"eax", {
        {"native_model_id", 0, 23},
        {"core_type", 24, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Ah SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Bh SCHEMA START /////////////////////////////////////////////////////////////
//
//  For details on this sub-leaf, see “INPUT EAX = 1BH: Returns PCONFIG Information” on page 3-252.
//
//  NOTE:
//      Leaf 1BH is supported if CPUID.(EAX=07H, ECX=0H):EDX[18] = 1.
inline function_schema schema_1b = {
    {"eax", {
        {"pconfig_data", 0, 31}
    }},
    {"ebx", {
        {"pconfig_data", 0, 31}
    }},
    {"ecx", {
        {"pconfig_data", 0, 31}
    }},
    {"edx", {
        {"pconfig_data", 0, 31}
    }}
};

inline function_schema schema_1b_ff = {
    {"eax", {
        {"pconfig_target_id", 0, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Bh SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Ch SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_1c = {
    {"eax", {
        {"supported_lbr_depth_values", 0, 7},
        {"reserved", 8, 29},
        {"deep_c_state_reset", 30, 30},
        {"ip_values_contain_lip", 31, 31}
    }},
    {"ebx", {
        {"cpl_filtering_supported", 0, 0},
        {"branch_filtering_supported", 1, 1},
        {"call_stack_mode_supported", 2, 2},
        {"reserved", 3, 31}
    }},
    {"ecx", {
        {"mispredict_bit_supported", 0, 0},
        {"timed_lbrs_supported", 1, 1},
        {"branch_type_field_supported", 2, 2},
        {"reserved", 3, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Ch SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Dh SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_1d_00 = {
    {"eax", {
        {"max_palette", 0, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};

inline function_schema schema_1d_01 = {
    {"eax", {
        {"palette_1_total_tile_bytes", 0, 15},
        {"palette_1_bytes_per_tile", 16, 31}
    }},
    {"ebx", {
        {"palette_1_bytes_per_row", 0, 15},
        {"palette_1_max_names", 16, 31}
    }},
    {"ecx", {
        {"palette_1_max_rows", 0, 15},
        {"reserved", 16, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Dh SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Eh SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_1e = {
    {"eax", {
        {"reserved", 0, 31}
    }},
    {"ebx", {
        {"tmul_maxk", 0, 7},
        {"tmul_maxn", 8, 23},
        {"reserved", 24, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Eh SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Fh SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_1f = {
    {"eax", {
        {"x2apic_id_shift", 0, 4},
        {"reserved", 5, 31}
    }},
    {"ebx", {
        {"logical_processors_within_domain", 0, 15},
        {"reserved", 16, 31}
    }},
    {"ecx", {
        {"sub_leaf_index", 0, 7},
        {"domain_type", 8, 15},
        {"reserved", 16, 31}
    }},
    {"edx", {
        {"x2apic_id", 0, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 1Fh SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 20h SCHEMA START /////////////////////////////////////////////////////////////
//
inline function_schema schema_20 = {
    {"eax", {
        {"max_sub_leaves_supported", 0, 31}
    }},
    {"ebx", {
        {"hreset_support", 0, 0},
        {"reserved", 1, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 20h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 21h SCHEMA START /////////////////////////////////////////////////////////////
//
//
// Invalid. No existing or future CPU will return processor identification or feature information if the initial
// EAX value is 21H. If the value returned by CPUID.0:EAX (the maximum input value for basic CPUID information) is at least 21H;
// 0 is returned in the registers EAX, EBX, ECX, and EDX. Otherwise, the data for the highest basic information leaf is returned.
//

inline function_schema schema_21 = {
    {"eax", {
        {"invalid", 0, 31}
    }},
    {"ebx", {
        {"invalid", 0, 31}
    }},
    {"ecx", {
        {"invalid", 0, 31}
    }},
    {"edx", {
        {"invalid", 0, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 21h SCHEMA END ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 40000000-4FFFFFFFh SCHEMA START //////////////////////////////////////////////
//
inline function_schema schema_40 = {
    {"eax", {
        {"invalid", 0, 31}
    }},
    {"ebx", {
        {"invalid", 0, 31}
    }},
    {"ecx", {
        {"invalid", 0, 31}
    }},
    {"edx", {
        {"invalid", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 40000000-4FFFFFFFh SCHEMA END ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000000h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000000 = {
    {"eax", {
        {"maximum_input_value", 0, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000000h SCHEMA END /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000001h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000001 = {
    {"eax", {
        {"extended_processor_signature_and_feature_bits", 0, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"lahf_sahf_in_64_bit", 0, 0},
        {"reserved_1", 1, 4},
        {"lzcnt", 5, 5},
        {"reserved_2", 6, 7},
        {"prefetchw", 8, 8},
        {"reserved_3", 9, 31}
    }},
    {"edx", {
        {"reserved_1", 0, 10},
        {"syscall_sysret", 11, 11},
        {"reserved_2", 12, 19},
        {"execute_disable_bit_available", 20, 20},
        {"reserved_3", 21, 25},
        {"1gbyte_pages", 26, 26},
        {"rdtscp_ia32_tsc_aux", 27, 27},
        {"reserved_4", 28, 28},
        {"intel_64_architecture", 29, 29},
        {"reserved_5", 30, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000001h SCHEMA END /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000002h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000002 = {
    {"eax", {
        {"processor_brand_string_1", 0, 31}
    }},
    {"ebx", {
        {"processor_brand_string_2", 0, 31}
    }},
    {"ecx", {
        {"processor_brand_string_3", 0, 31}
    }},
    {"edx", {
        {"processor_brand_string_4", 0, 31}
    }}
};


//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000002h SCHEMA END /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000003h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000003 = {
    {"eax", {
        {"processor_brand_string_1", 0, 31}
    }},
    {"ebx", {
        {"processor_brand_string_2", 0, 31}
    }},
    {"ecx", {
        {"processor_brand_string_3", 0, 31}
    }},
    {"edx", {
        {"processor_brand_string_4", 0, 31}
    }}
};


//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000003h SCHEMA END /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000004h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000004 = {
    {"eax", {
        {"processor_brand_string_1", 0, 31}
    }},
    {"ebx", {
        {"processor_brand_string_2", 0, 31}
    }},
    {"ecx", {
        {"processor_brand_string_3", 0, 31}
    }},
    {"edx", {
        {"processor_brand_string_4", 0, 31}
    }}
};


//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000004h SCHEMA END /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000005h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000005 = {
    {"eax", {
        {"maximum_input_value", 0, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};
//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000005h SCHEMA END /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000006h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000006 = {
    {"eax", {
        {"reserved", 0, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"cache_line_size", 0, 7},
        {"reserved", 8, 11},
        {"l2_associativity", 12, 15},
        {"cache_size", 16, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};

//
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000006h SCHEMA END /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// CPUID FUNCTION 80000007h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000007 = {
    {"eax", {
        {"reserved", 0, 31}
    }},
    {"ebx", {
        {"reserved", 0, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 7},
        {"invariant_tsc", 8, 8},
        {"reserved", 9, 31}
    }}
};
//
//////////////////////////////////////////////////////////////////// CPUID FUNCTION 80000007h SCHEMA END /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////// CPUID FUNCTION 80000007h SCHEMA START ///////////////////////////////////////////////////////
//
inline function_schema schema_80000008 = {
    {"eax", {
        {"physical_address_bits", 0, 7},
        {"linear_address_bits", 8, 15},
        {"reserved", 16, 31}
    }},
    {"ebx", {
        {"reserved", 0, 8},
        {"wbnoinvd", 9, 9},
        {"reserved", 10, 31}
    }},
    {"ecx", {
        {"reserved", 0, 31}
    }},
    {"edx", {
        {"reserved", 0, 31}
    }}
};
//
//////////////////////////////////////////////////////////////////// CPUID FUNCTION 80000007h SCHEMA END /////////////////////////////////////////////////////////

inline std::unordered_map<int32_t, function_schema> cpuid_schema = {
    {0x00000000, schema_00},
    {0x00000001, schema_01},
    {0x00000002, schema_02},
    {0x00000003, schema_03},
    {0x00000005, schema_05},
    {0x00000006, schema_06},
    {0x00000007, schema_07_00},
    {0x00010007, schema_07_01},
    {0x00020007, schema_07_02},
    {0x00000009, schema_09},
    {0x0000000a, schema_0a},
    {0x0000000b, schema_0b},
    {0x0000000d, schema_0d_00},
    {0x0001000d, schema_0d_01},
    {0xffff000d, schema_0d_ff}, // because it applies for any ecx = n, where n > 1
    {0x0000000f, schema_0f},
    {0x00000010, schema_10},
    {0x00010010, schema_10_01},
    {0x00020010, schema_10_02},
    {0x00030010, schema_10_03},
    {0x00000012, schema_12_00},
    {0x00010012, schema_12_01},
    {0x00020012, schema_12_02},
    {0x00000014, schema_14_00},
    {0x00010014, schema_14_01},
    {0x00000015, schema_15},
    {0x00000016, schema_16},
    {0x00000017, schema_17_00},
    {0x00010017, schema_17_07},
    {0x00020017, schema_17_07},
    {0x00030017, schema_17_07},
    {0xffff0017, schema_17_ff},
    {0x00000018, schema_18_00},
    {0x00010018, schema_18_01},
    {0x00000019, schema_19},
    {0x0000001a, schema_1a},
    {0x0000001b, schema_1b},
    {0xffff001b, schema_1b_ff},
    {0x0000001c, schema_1c},
    {0x0000001d, schema_1d_00},
    {0x0001001d, schema_1d_01},
    {0x0000001e, schema_1e},
    {0x0000001f, schema_1f},
    {0x00000020, schema_20},
    {0x00000021, schema_21},
    {0x40000000, schema_40},
    {0x80000000, schema_80000000},
    {0x80000001, schema_80000001},
    {0x80000002, schema_80000002},
    {0x80000003, schema_80000003},
    {0x80000004, schema_80000004},
    {0x80000005, schema_80000005},
    {0x80000006, schema_80000006},
    {0x80000007, schema_80000007},
    {0x80000008, schema_80000008},
};