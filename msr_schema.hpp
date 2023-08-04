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
#include <string>
#include <unordered_map>
#include <vector>

struct msr_field
{
    std::string name;
    std::size_t bit_start;
    std::size_t bit_end;
};

struct msr_schema
{
    std::vector<msr_field> fields;

    msr_schema() = default;
    msr_schema( std::initializer_list<msr_field> init ) : fields( init ) {}

    const msr_field& operator[]( std::size_t i ) const
    {
        return fields[ i ];
    }
};

using msr_schema_map = std::unordered_map<std::string, msr_schema>;

inline msr_schema_map msr_schema_invalid = {
    {"invalid_msr", {
        {"invalid", 0, 64},
    }}
};

inline msr_schema_map msr_schema_17h = {
    {"ia32_platform_id", {
        {"reserved1", 0, 49},
        {"platform_id", 50, 52},
        {"reserved2", 53, 63}
    }}
};

inline msr_schema_map msr_schema_1bh = {
    {"ia32_apic_base", {
        {"reserved1", 0, 7},
        {"bsp_flag", 8, 8},
        {"reserved2", 9, 9},
        {"enable_x2apic_mode", 10, 10},
        {"apic_global_enable", 11, 11},
        {"apic_Base", 12, 38},       // Assuming MAXPHYADDR as 39, you may need to adjust this.
        {"reserved3", 39, 63}
    }}
};

inline msr_schema_map msr_schema_3ah = {
    {"ia32_feature_control", {
        {"lock_bit", 0, 0},
        {"enable_vmx_inside_smx", 1, 1},
        {"enable_vmx_outside_smx", 2, 2},
        {"reserved1", 3, 7},
        {"senter_local_function_enables", 8, 14},
        {"senter_global_enable", 15, 15},
        {"reserved2", 16, 16},
        {"sgx_launch_control_enable", 17, 17},
        {"sgx_global_enable", 18, 18},
        {"reserved3", 19, 19},
        {"lmce_on", 20, 20},
        {"reserved4", 21, 63}
    }}
};

inline msr_schema_map msr_schema_10ah = {
    {"ia32_arch_capabilities", {
        {"rdcl_no", 0, 0},
        {"ibrs_all", 1, 1},
        {"rsba", 2, 2},
        {"skip_l1dfl_vmentry", 3, 3},
        {"ssb_no", 4, 4},
        {"mds_no", 5, 5},
        {"if_pschange_mc_no", 6, 6},
        {"tsx_ctrl", 7, 7},
        {"taa_no", 8, 8},
        {"mcu_control", 9, 9},
        {"misc_package_ctls", 10, 10},
        {"energy_filtering_ctl", 11, 11},
        {"doitm", 12, 12},
        {"sbdp_ssdp_no", 13, 13},
        {"fbsdp_no", 14, 14},
        {"psdp_no", 15, 15},
        {"reserved1", 16, 16},
        {"fb_clear", 17, 17},
        {"fb_clear_ctrl", 18, 18},
        {"rrsba", 19, 19},
        {"bhi_no", 20, 20},
        {"xapic_disable_status", 21, 21},
        {"reserved2", 22, 22},
        {"overclocking_status", 23, 23},
        {"pbrsb_no", 24, 24},
        {"reserved3", 25, 63}
    }}
};

inline msr_schema_map msr_schema_48h = {
    {"ia32_spec_ctrl", {
        {"indirect_branch_restricted_speculation_ibrs", 0, 0},
        {"single_thread_indirect_branch_predictors_stibp", 1, 1},
        {"speculative_store_bypass_disable_ssbd", 2, 2},
        {"ipred_dis_u", 3, 3},
        {"ipred_dis_s", 4, 4},
        {"rrsba_dis_u", 5, 5},
        {"rrsba_dis_s", 6, 6},
        {"psfd", 7, 7},
        {"ddpd_u", 8, 8},
        {"reserved", 9, 9},
        {"bhi_dis_s", 10, 10}
    }}
};

inline msr_schema_map msr_schema_8ch = {
    {"ia32_sgxlepubkeyhash0", {
        {"ia32_sgxlepubkeyhash_63_0", 0, 63}
    }}
};

inline msr_schema_map msr_schema_8dh = {
    {"ia32_sgxlepubkeyhash1", {
        {"ia32_sgxlepubkeyhash_127_64", 0, 63}
    }}
};

inline msr_schema_map msr_schema_8eh = {
    {"ia32_sgxlepubkeyhash2", {
        {"ia32_sgxlepubkeyhash_191_128", 0, 63}
    }}
};

inline msr_schema_map msr_schema_8fh = {
    {"ia32_sgxlepubkeyhash3", {
        {"ia32_sgxlepubkeyhash_255_192", 0, 63}
    }}
};

inline msr_schema_map msr_schema_9bh = {
    {"ia32_smm_monitor_ctl", {
        {"valid", 0, 0},
        {"reserved1", 1, 1},
        {"controls_smi_unblocking_by_vmxoff", 2, 2},
        {"reserved2", 3, 10},
        {"mseg_base", 11, 31},
        {"reserved3", 32, 63}
    }}
};

inline msr_schema_map msr_schema_9eh = {
    {"ia32_smbase", {
        {"smram_image_base_address", 0, 63}
    }}
};

inline msr_schema_map msr_schema_bch = {
    {"ia32_misc_package_ctls", {
        {"energy_filtering_enable", 0, 0},
        {"reserved", 1, 63}
    }}
};

inline msr_schema_map msr_schema_bdh = {
    {"ia32_xapic_disable_status", {
        {"legacy_xapic_disabled", 0, 0},
        {"reserved", 1, 63}
    }}
};

inline msr_schema_map msr_schema_c1h = {
    {"ia32_pmc0", {
        {"general_performance_counter_0", 0, 63}
    }}
};

inline msr_schema_map msr_schema_c2h = {
    {"ia32_pmc1", {
        {"general_performance_counter_1", 0, 63}
    }}
};

inline msr_schema_map msr_schema_c3h = {
    {"ia32_pmc2", {
        {"general_performance_counter_2", 0, 63}
    }}
};

inline msr_schema_map msr_schema_c4h = {
    {"ia32_pmc3", {
        {"general_performance_counter_3", 0, 63}
    }}
};

inline msr_schema_map msr_schema_c5h = {
    {"ia32_pmc4", {
        {"general_performance_counter_4", 0, 63}
    }}
};

inline msr_schema_map msr_schema_c6h = {
    {"ia32_pmc5", {
        {"general_performance_counter_5", 0, 63}
    }}
};

inline msr_schema_map msr_schema_c7h = {
    {"ia32_pmc6", {
        {"general_performance_counter_6", 0, 63}
    }}
};

inline msr_schema_map msr_schema_c8h = {
    {"ia32_pmc7", {
        {"general_performance_counter_7", 0, 63}
    }}
};

inline msr_schema_map msr_schema_cfh = {
    {"ia32_core_capabilities", {
        {"reserved", 0, 63}
    }}
};

inline msr_schema_map msr_schema_e1h = {
    {"ia32_umwait_control", {
        {"c0_2_is_not_allowed_by_the_os", 0, 0},
        {"reserved", 1, 1},
        {"determines_the_maximum_time_in_tsc_quanta", 2, 31}
    }}
};

inline msr_schema_map msr_schema_e7h = {
    {"ia32_mperf", {
        {"c0_mcnt_c0_tsc_frequency_clock_count", 0, 63}
    }}
};

inline msr_schema_map msr_schema_e8h = {
    {"ia32_aperf", {
        {"c0_acnt_c0_actual_frequency_clock_count", 0, 63}
    }}
};

inline msr_schema_map msr_schema_feh = {
    {"ia32_mtrrcap", {
        {"vcnt_the_number_of_variable_memory_type_ranges_in_the_processor", 0, 7},
        {"fixed_range_mtrrs_are_supported_when_set", 8, 8},
        {"reserved1", 9, 9},
        {"wc_supported_when_set", 10, 10},
        {"smrr_supported_when_set", 11, 11},
        {"prmrr_supported_when_set", 12, 12},
        {"reserved2", 13, 63}
    }}
};


inline msr_schema_map msr_schema_10bh = {
    {"ia32_flush_cmd", {
        {"l1d_flush", 0, 0},
        {"reserved", 1, 63}
    }}
};

inline msr_schema_map msr_schema_10fh = {
    {"ia32_tsx_force_abort", {
        {"rtm_force_abort", 0, 0},
        {"tsx_cpuid_clear", 1, 1},
        {"sdv_enable_rtm", 2, 2},
        {"reserved", 3, 63}
    }}
};

inline msr_schema_map msr_schema_122h = {
    {"ia32_tsx_ctrl", {
        {"rtm_disable", 0, 0},
        {"tsx_cpuid_clear", 1, 1},
        {"reserved", 2, 63}
    }}
};

inline msr_schema_map msr_schema_123h = {
    {"ia32_mcu_opt_ctrl", {
        {"rngds_mitg_dis", 0, 0},
        {"rtm_allow", 1, 1},
        {"rtm_locked", 2, 2},
        {"fb_clear_dis", 3, 3},
        {"reserved", 4, 63}
    }}
};

inline msr_schema_map msr_schema_174h = {
    {"ia32_sysenter_cs", {
        {"cs_selector", 0, 15},
        {"not_used", 16, 31},
        {"reserved", 32, 63}
    }}
};

inline msr_schema_map msr_schema_175h = {
    {"ia32_sysenter_esp", {
        {"esp", 0, 63}
    }}
};

inline msr_schema_map msr_schema_176h = {
    {"ia32_sysenter_eip", {
        {"eip", 0, 63}
    }}
};

inline msr_schema_map msr_schema_179h = {
    {"ia32_mcg_cap", {
        {"count", 0, 7},
        {"mcg_ctl_p", 8, 8},
        {"mcg_ext_p", 9, 9},
        {"mcp_cmci_p", 10, 10},
        {"mcg_tes_p", 11, 11},
        {"reserved_1", 12, 15},
        {"mcg_ext_cnt", 16, 23},
        {"mcg_ser_p", 24, 24},
        {"reserved_2", 25, 25},
        {"mcg_elog_p", 26, 26},
        {"mcg_lmce_p", 27, 27},
        {"reserved_3", 28, 63}
    }}
};

inline msr_schema_map msr_schema_17ah = {
    {"ia32_mcg_status", {
        {"ripv", 0, 0},
        {"eipv", 1, 1},
        {"mcip", 2, 2},
        {"lmce_s", 3, 3},
        {"reserved", 4, 63}
    }}
};

inline msr_schema_map msr_schema_17bh = {
    {"ia32_mcg_ctl", {
        {"mcg_ctl", 0, 63} // The contents are not architecturally defined.
    }}
};

inline msr_schema_map msr_schema_186h = {
    {"ia32_perfevtsel0", {
        {"event_select", 0, 7},
        {"umask", 8, 15},
        {"usr", 16, 16},
        {"os", 17, 17},
        {"edge", 18, 18},
        {"pc", 19, 19},
        {"int", 20, 20},
        {"any_thread", 21, 21},
        {"en", 22, 22},
        {"inv", 23, 23},
        {"cmask", 24, 31},
        {"reserved", 32, 63}
    }}
};

// Schema for 187h, 188h, 189h, 18ah, 18bh, 18ch, and 18dh are identical to 186h
inline msr_schema_map msr_schema_187h = msr_schema_186h;
inline msr_schema_map msr_schema_188h = msr_schema_186h;
inline msr_schema_map msr_schema_189h = msr_schema_186h;
inline msr_schema_map msr_schema_18ah = msr_schema_186h;
inline msr_schema_map msr_schema_18bh = msr_schema_186h;
inline msr_schema_map msr_schema_18ch = msr_schema_186h;
inline msr_schema_map msr_schema_18dh = msr_schema_186h;

inline msr_schema_map msr_schema_195h = {
    {"ia32_overclocking_status", {
        {"overclocking_utilized", 0, 0},
        {"undervolt_protection", 1, 1},
        {"overclocking_secure_status", 2, 2},
        {"reserved", 3, 63}
    }}
};

inline msr_schema_map msr_schema_198h = {
    {"ia32_perf_status", {
        {"current_performance_state_value", 0, 15},
        {"reserved", 16, 63}
    }}
};

inline msr_schema_map msr_schema_199h = {
    {"ia32_perf_ctl", {
        {"target_performance_state_value", 0, 15},
        {"reserved1", 16, 31},
        {"ida_engage", 32, 32},
        {"reserved2", 33, 63}
    }}
};

inline msr_schema_map msr_schema_19ah = {
    {"ia32_clock_modulation", {
        {"extended_on_demand_clock_modulation_duty_cycle", 0, 0},
        {"on_demand_clock_modulation_duty_cycle", 1, 3},
        {"on_demand_clock_modulation_enable", 4, 4},
        {"reserved", 5, 63}
    }}
};

inline msr_schema_map msr_schema_19bh = {
    {"ia32_therm_interrupt", {
        {"high_temp_interrupt_enable", 0, 0},
        {"low_temp_interrupt_enable", 1, 1},
        {"prochot_interrupt_enable", 2, 2},
        {"forcepr_interrupt_enable", 3, 3},
        {"critical_temp_interrupt_enable", 4, 4},
        {"reserved1", 5, 7},
        {"threshold1_value", 8, 14},
        {"threshold1_interrupt_enable", 15, 15},
        {"threshold2_value", 16, 22},
        {"threshold2_interrupt_enable", 23, 23},
        {"power_limit_notification_enable", 24, 24},
        {"hardware_feedback_notification_enable", 25, 25},
        {"reserved2", 26, 63}
    }}
};

inline msr_schema_map msr_schema_19ch = {
    {"ia32_therm_status", {
        {"thermal_status", 0, 0},
        {"thermal_status_log", 1, 1},
        {"prochot_or_forcepr_event", 2, 2},
        {"prochot_or_forcepr_log", 3, 3},
        {"critical_temp_status", 4, 4},
        {"critical_temp_status_log", 5, 5},
        {"thermal_threshold1_status", 6, 6},
        {"thermal_threshold1_log", 7, 7},
        {"thermal_threshold2_status", 8, 8},
        {"thermal_threshold2_log", 9, 9},
        {"power_limitation_status", 10, 10},
        {"power_limitation_log", 11, 11},
        {"current_limit_status", 12, 12},
        {"current_limit_log", 13, 13},
        {"cross_domain_limit_status", 14, 14},
        {"cross_domain_limit_log", 15, 15},
        {"digital_readout", 16, 22},
        {"reserved1", 23, 26},
        {"resolution_in_degrees_celsius", 27, 30},
        {"reading_valid", 31, 31},
        {"reserved2", 32, 63}
    }}
};

inline msr_schema_map msr_schema_1a0h = {
    {"ia32_misc_enable", {
        {"fast_strings_enable", 0, 0},
        {"automatic_thermal_control_circuit_enable", 3, 3},
        {"performance_monitoring_available", 7, 7},
        {"branch_trace_storage_unavailable", 11, 11},
        {"processor_event_based_sampling_unavailable", 12, 12},
        {"enhanced_intel_speedstep_technology_enable", 16, 16},
        {"enable_monitor_fsm", 18, 18},
        {"limit_cpuid_maxval", 22, 22},
        {"xtpr_message_disable", 23, 23},
        {"xd_bit_disable", 34, 34},
        {"reserved", 35, 63}
    }}
};

inline msr_schema_map msr_schema_1b0h = {
    {"ia32_energy_perf_bias", {
        {"power_policy_preference", 0, 3},
        {"reserved", 4, 63}
    }}
};

inline msr_schema_map msr_schema_1b1h = {
    {"ia32_package_therm_status", {
        {"pkg_thermal_status", 0, 0},
        {"pkg_thermal_status_log", 1, 1},
        {"pkg_prochot_event", 2, 2},
        {"pkg_prochot_log", 3, 3},
        {"pkg_critical_temperature_status", 4, 4},
        {"pkg_critical_temperature_status_log", 5, 5},
        {"pkg_thermal_threshold_1_status", 6, 6},
        {"pkg_thermal_threshold_1_log", 7, 7},
        {"pkg_thermal_threshold_2_status", 8, 8},
        {"pkg_thermal_threshold_1_log", 9, 9},
        {"pkg_power_limitation_status", 10, 10},
        {"pkg_power_limitation_log", 11, 11},
        {"pkg_digital_readout", 16, 22},
        {"hardware_feedback_interface_structure_change_status", 26, 26},
        {"reserved", 12, 15},
        {"reserved", 23, 25},
        {"reserved", 27, 63}
    }}
};

inline msr_schema_map msr_schema_1b2h = {
    {"ia32_package_therm_interrupt", {
        {"pkg_high_temperature_interrupt_enable", 0, 0},
        {"pkg_low_temperature_interrupt_enable", 1, 1},
        {"pkg_prochot_interrupt_enable", 2, 2},
        {"pkg_overheat_interrupt_enable", 4, 4},
        {"pkg_threshold_1_value", 8, 14},
        {"pkg_threshold_1_interrupt_enable", 15, 15},
        {"pkg_threshold_2_value", 16, 22},
        {"pkg_threshold_2_interrupt_enable", 23, 23},
        {"pkg_power_limit_notification_enable", 24, 24},
        {"hardware_feedback_interrupt_enable", 25, 25},
        {"reserved", 3, 3},
        {"reserved", 5, 7},
        {"reserved", 26, 63}
    }}
};

inline msr_schema_map msr_schema_1c4h = {
    {"ia32_xfd", {
        {"state_component_bitmap", 0, 63}
    }}
};

inline msr_schema_map msr_schema_1c5h = {
    {"ia32_xfd_err", {
        {"state_component_bitmap", 0, 63}
    }}
};

inline msr_schema_map msr_schema_1d9h = {
    {"ia32_debugctl", {
        {"lbr", 0, 0},
        {"btf", 1, 1},
        {"bld", 2, 2},
        {"reserved", 3, 5},
        {"tr", 6, 6},
        {"bts", 7, 7},
        {"btint", 8, 8},
        {"bts_off_os", 9, 9},
        {"bts_off_usr", 10, 10},
        {"freeze_lbrs_on_pmi", 11, 11},
        {"freeze_perfmon_on_pmi", 12, 12},
        {"enable_uncore_pmi", 13, 13},
        {"freeze_while_smm", 14, 14},
        {"rtm_debug", 15, 15},
        {"reserved", 16, 63}
    }}
};

inline msr_schema_map msr_schema_1ddh = {
    {"ia32_ler_from_ip", {
        {"from_ip", 0, 63}
    }}
};

inline msr_schema_map msr_schema_1deh = {
    {"ia32_ler_to_ip", {
        {"to_ip", 0, 63}
    }}
};

inline msr_schema_map msr_schema_1e0h = {
    {"ia32_ler_info", {
        {"undefined_1", 0, 55},
        {"br_type", 56, 59},
        {"undefined_2", 60, 60},
        {"tsx_abort", 61, 61},
        {"in_tsx", 62, 62},
        {"mispred", 63, 63}
    }}
};
inline msr_schema_map msr_schema_1f2h = {
    {"ia32_smrr_physbase", {
        {"type", 0, 7},
        {"reserved_1", 8, 11},
        {"physbase", 12, 31},
        {"reserved_2", 32, 63}
    }}
};

inline msr_schema_map msr_schema_1f3h = {
    {"ia32_smrr_physmask", {
        {"reserved_1", 0, 10},
        {"valid", 11, 11},
        {"physmask", 12, 31},
        {"reserved_2", 32, 63}
    }}
};

inline msr_schema_map msr_schema_1f8h = {
    {"ia32_platform_dca_cap", {
        {"platform_dca_cap", 0, 63},
    }}
};

inline msr_schema_map msr_schema_1f9h = {
    {"ia32_cpu_dca_cap", {
        {"cpu_dca_cap", 0, 63},
    }}
};

inline msr_schema_map msr_schema_1fah = {
    {"ia32_dca_0_cap", {
        {"dca_active", 0, 0},
        {"transaction", 1, 2},
        {"dca_type", 3, 6},
        {"dca_queue_size", 7, 10},
        {"reserved_1", 11, 12},
        {"dca_delay", 13, 16},
        {"reserved_2", 17, 23},
        {"sw_block", 24, 24},
        {"reserved_3", 25, 25},
        {"hw_block", 26, 26},
        {"reserved_4", 27, 31}
    }}
};

inline msr_schema_map msr_schema_277h = {
    {"ia32_pat", {
        {"pa0", 0, 2},
        {"reserved1", 3, 7},
        {"pa1", 8, 10},
        {"reserved2", 11, 15},
        {"pa2", 16, 18},
        {"reserved3", 19, 23},
        {"pa3", 24, 26},
        {"reserved4", 27, 31},
        {"pa4", 32, 34},
        {"reserved5", 35, 39},
        {"pa5", 40, 42},
        {"reserved6", 43, 47},
        {"pa6", 48, 50},
        {"reserved7", 51, 55},
        {"pa7", 56, 58},
        {"reserved8", 59, 63}
    }}
};

inline msr_schema_map msr_schema_280h = {
    {"ia32_mc0_ctl2", {
        {"corrected_error_count_threshold", 0, 14},
        {"reserved1", 15, 29},
        {"cmci_en", 30, 30},
        {"reserved2", 31, 63}
    }}
};

inline msr_schema_map msr_schema_2ffh = {
    {"ia32_mtrr_def_type", {
        {"default_memory_type", 0, 2},
        {"reserved1", 3, 9},
        {"fixed_range_mtrr_enable", 10, 10},
        {"mtrr_enable", 11, 11},
        {"reserved2", 12, 63}
    }}
};

inline msr_schema_map msr_schema_309h = {
    {"ia32_fixed_ctr0", {
        {"counts_instr_retired_any", 0, 63} // Entire MSR used for counter
    }}
};

inline msr_schema_map msr_schema_30ah = {
    {"ia32_fixed_ctr1", {
        {"counts_cpu_clk_unhalted_core", 0, 63} // Entire MSR used for counter
    }}
};

inline msr_schema_map msr_schema_30bh = {
    {"ia32_fixed_ctr2", {
        {"counts_cpu_clk_unhalted_ref", 0, 63} // Entire MSR used for counter
    }}
};

inline msr_schema_map msr_schema_345h = {
    {"ia32_perf_capabilities", {
        {"lbr_format", 0, 5},
        {"pebs_trap", 6, 6},
        {"pebs_save_arch_regs", 7, 7},
        {"pebs_record_format", 8, 11},
        {"freeze_while_smm_supported", 12, 12},
        {"full_width_counter_writable", 13, 13},
        {"pebs_baseline", 14, 14},
        {"performance_metrics_available", 15, 15},
        {"pebs_output_in_pt_trace_stream", 16, 16},
        {"reserved", 17, 63}
    }}
};

inline msr_schema_map msr_schema_38dh = {
    {"ia32_fixed_ctr_ctrl", {
        {"en0_os", 0, 0},
        {"en0_usr", 1, 1},
        {"anythr0", 2, 2},
        {"en0_pmi", 3, 3},
        {"en1_os", 4, 4},
        {"en1_usr", 5, 5},
        {"anythr1", 6, 6},
        {"en1_pmi", 7, 7},
        {"en2_os", 8, 8},
        {"en2_usr", 9, 9},
        {"anythr2", 10, 10},
        {"en2_pmi", 11, 11},
        {"en3_os", 12, 12},
        {"en3_usr", 13, 13},
        {"reserved1", 14, 14},
        {"en3_pmi", 15, 15},
        {"reserved2", 16, 63}
    }}
};

inline msr_schema_map msr_schema_38eh = {
    {"ia32_perf_global_status", {
        {"ovf_pmc0", 0, 0},
        {"ovf_pmc1", 1, 1},
        {"ovf_pmc2", 2, 2},
        {"ovf_pmc3", 3, 3},
        {"ovf_pmcn", 4, 31}, // assuming up to n=31, needs dynamic handling
        {"ovf_fixedctr0", 32, 32},
        {"ovf_fixedctr1", 33, 33},
        {"ovf_fixedctr2", 34, 34},
        {"reserved1", 35, 47},
        {"ovf_perf_metrics", 48, 48},
        {"reserved2", 49, 54},
        {"trace_topa_pmi", 55, 55},
        {"reserved3", 56, 57},
        {"lbr_frz", 58, 58},
        {"ctr_frz", 59, 59},
        {"asci", 60, 60},
        {"ovf_uncore", 61, 61},
        {"ovfbuf", 62, 62},
        {"condchgd", 63, 63}
    }}
};

inline msr_schema_map msr_schema_38fh = {
    {"ia32_perf_global_ctrl", {
        {"en_pmc0", 0, 0},
        {"en_pmc1", 1, 1},
        {"en_pmc2", 2, 2},
        {"en_pmcn", 3, 31}, // assuming up to n=31, needs dynamic handling
        {"en_fixed_ctr0", 32, 32},
        {"en_fixed_ctr1", 33, 33},
        {"en_fixed_ctr2", 34, 34},
        {"reserved1", 35, 47},
        {"en_perf_metrics", 48, 48}
    }}
};

inline msr_schema_map msr_schema_390h_ovf = {
    {"ia32_perf_global_ovf_ctrl", {
        {"clear_ovf_pmc0", 0, 0},
        {"clear_ovf_pmc1", 1, 1},
        {"clear_ovf_pmc2", 2, 2},
        {"clear_ovf_pmcn", 3, 31}, // assuming up to n=31, needs dynamic handling
        {"clear_ovf_fixed_ctr0", 32, 32},
        {"clear_ovf_fixed_ctr1", 33, 33},
        {"clear_ovf_fixed_ctr2", 34, 34},
        {"reserved1", 35, 54},
        {"clear_trace_topa_pmi", 55, 55},
        {"reserved2", 56, 60},
        {"clear_ovf_uncore", 61, 61},
        {"clear_ovfbuf", 62, 62},
        {"clear_condchgd", 63, 63}
    }}
};

inline msr_schema_map msr_schema_390h_reset = {
    {"ia32_perf_global_status_reset", {
        {"reset_ovf_pmc0", 0, 0},
        {"reset_ovf_pmc1", 1, 1},
        {"reset_ovf_pmc2", 2, 2},
        {"reset_ovf_pmcn", 3, 31}, // assuming up to n=31, needs dynamic handling
        {"reset_ovf_fixed_ctr0", 32, 32},
        {"reset_ovf_fixed_ctr1", 33, 33},
        {"reset_ovf_fixed_ctr2", 34, 34},
        {"reserved1", 35, 47},
        {"reset_ovf_perf_metrics", 48, 48},
        {"reserved2", 49, 54},
        {"reset_trace_topa_pmi", 55, 55},
        {"reserved3", 56, 57},
        {"reset_lbr_frz", 58, 58},
        {"reset_ctr_frz", 59, 59},
        {"reset_ovf_uncore", 61, 61},
        {"reset_ovfbuf", 62, 62},
        {"reset_condchgd", 63, 63}
    }}
};

inline msr_schema_map msr_schema_391h = {
    {"ia32_perf_global_status_set", {
        {"cause_ovf_pmc0", 0, 0},
        {"cause_ovf_pmc1", 1, 1},
        {"cause_ovf_pmc2", 2, 2},
        {"cause_ovf_pmcn", 3, 31}, // assuming up to n=31, needs dynamic handling
        {"cause_ovf_fixed_ctr0", 32, 32},
        {"cause_ovf_fixed_ctr1", 33, 33},
        {"cause_ovf_fixed_ctr2", 34, 34},
        {"reserved1", 35, 47},
        {"set_ovf_perf_metrics", 48, 48},
        {"reserved2", 49, 54},
        {"cause_trace_topa_pmi", 55, 55},
        {"reserved3", 56, 57},
        {"cause_lbr_frz", 58, 58},
        {"cause_ctr_frz", 59, 59},
        {"cause_asci", 60, 60},
        {"cause_ovf_uncore", 61, 61},
        {"cause_ovfbuf", 62, 62},
        {"reserved4", 63, 63}
    }}
};

inline msr_schema_map msr_schema_392h = {
    {"ia32_perf_global_inuse", {
        {"perfevtsel0_in_use", 0, 0},
        {"perfevtsel1_in_use", 1, 1},
        {"perfevtsel2_in_use", 2, 2},
        {"perfevtseln_in_use", 3, 31}, // assuming up to n=31, needs dynamic handling
        {"fixed_ctr0_in_use", 32, 32},
        {"fixed_ctr1_in_use", 33, 33},
        {"fixed_ctr2_in_use", 34, 34},
        {"reserved1", 35, 62},
        {"pmi_in_use", 63, 63}
    }}
};

inline msr_schema_map msr_schema_3f1h = {
    {"ia32_pebs_enable", {
        {"enable_pebs", 0, 0},
        {"reserved1", 1, 3},
        {"reserved2", 4, 31},
        {"reserved3", 32, 34},
        {"reserved4", 35, 63}
    }}
};

inline msr_schema_map msr_schema_6a0h = {
    {"ia32_u_cet", {
        {"sh_stk_en", 0, 0},
        {"wr_shstk_en", 1, 1},
        {"endbr_en", 2, 2},
        {"leg_iw_en", 3, 3},
        {"no_track_en", 4, 4},
        {"suppress_dis", 5, 5},
        {"reserved1", 6, 9},
        {"suppress", 10, 10},
        {"tracker", 11, 11},
        {"eb_leg_bitmap_base", 12, 63}
    }}
};

inline msr_schema_map msr_schema_efer = {
    {"ia32_efer", {
        {"sce", 0, 0},
        {"reserved1", 1, 7},
        {"lme", 8, 8},
        {"reserved2", 9, 9},
        {"lma", 10, 10},
        {"nxe", 11, 11},
        {"reserved3", 12, 63}
    }}
};

inline msr_schema_map msr_schema_1b01h = {
    {"ia32_uarch_misc_ctl", {
        {"doitm", 0, 0},
        {"reserved", 1, 63}
    }}
};


inline std::unordered_map<int32_t, msr_schema_map> msr_schema_list = {
    {0x00000000, msr_schema_invalid},
    {0x00000017, msr_schema_17h},
    {0x0000001b, msr_schema_1bh},
    {0x0000003a, msr_schema_3ah},
    {0x00000010a, msr_schema_10ah},
    {0x0000001a0, msr_schema_1a0h},
    {0x00000048, msr_schema_48h},
    {0x0000008c, msr_schema_8ch},
    {0x0000008d, msr_schema_8dh},
    {0x0000008e, msr_schema_8eh},
    {0x0000008f, msr_schema_8fh},
    {0x0000009b, msr_schema_9bh},
    {0x0000009e, msr_schema_9eh},
    {0x000000bc, msr_schema_bch},
    {0x000000bd, msr_schema_bdh},
    {0x000000c1, msr_schema_c1h},
    {0x000000c2, msr_schema_c2h},
    {0x000000c3, msr_schema_c3h},
    {0x000000c4, msr_schema_c4h},
    {0x000000c5, msr_schema_c5h},
    {0x000000c6, msr_schema_c6h},
    {0x000000c7, msr_schema_c7h},
    {0x000000c8, msr_schema_c8h},
    {0x000000cf, msr_schema_cfh},
    {0x000000e1, msr_schema_e1h},
    {0x000000e7, msr_schema_e7h},
    {0x000000e8, msr_schema_e8h},
    {0x000000fe, msr_schema_feh},
    {0x0000010b, msr_schema_10bh},
    {0x0000010f, msr_schema_10fh},
    {0x00000122, msr_schema_122h},
    {0x00000123, msr_schema_123h},
    {0x00000174, msr_schema_174h},
    {0x00000175, msr_schema_175h},
    {0x00000176, msr_schema_176h},
    {0x00000179, msr_schema_179h},
    {0x0000017a, msr_schema_17ah},
    {0x0000017b, msr_schema_17bh},
    {0x00000186, msr_schema_186h},
    {0x00000187, msr_schema_187h},
    {0x00000188, msr_schema_188h},
    {0x00000189, msr_schema_189h},
    {0x0000018a, msr_schema_18ah},
    {0x0000018b, msr_schema_18bh},
    {0x0000018c, msr_schema_18ch},
    {0x0000018d, msr_schema_18dh},
    {0x00000195, msr_schema_195h},
    {0x00000198, msr_schema_198h},
    {0x00000199, msr_schema_199h},
    {0x0000019a, msr_schema_19ah},
    {0x0000019b, msr_schema_19bh},
    {0x0000019c, msr_schema_19ch},
    {0x000001a0, msr_schema_1a0h},
    {0x000001b0, msr_schema_1b0h},
    {0x000001b1, msr_schema_1b1h},
    {0x000001b2, msr_schema_1b2h},
    {0x000001c4, msr_schema_1c4h},
    {0x000001c5, msr_schema_1c5h},
    {0x000001d9, msr_schema_1d9h},
    {0x000001dd, msr_schema_1ddh},
    {0x000001de, msr_schema_1deh},
    {0x000001e0, msr_schema_1e0h},
    {0x000001f2, msr_schema_1f2h},
    {0x000001f3, msr_schema_1f3h},
    {0x000001f8, msr_schema_1f8h},
    {0x000001f9, msr_schema_1f9h},
    {0x000001fa, msr_schema_1fah},
    {0x00000277, msr_schema_277h},
    {0x00000280, msr_schema_280h},
    {0x000002ff, msr_schema_2ffh},
    {0x00000309, msr_schema_309h},
    {0x0000030a, msr_schema_30ah},
    {0x0000030b, msr_schema_30bh},
    {0x00000345, msr_schema_345h},
    {0x0000038d, msr_schema_38dh},
    {0x0000038e, msr_schema_38eh},
    {0x0000038f, msr_schema_38fh},
    {0x00000390, msr_schema_390h_ovf},
    {0x00000390, msr_schema_390h_reset},
    {0x00000391, msr_schema_391h},
    {0x00000392, msr_schema_392h},
    {0x000003F1, msr_schema_3f1h},
    {0x000006A0, msr_schema_6a0h},
    {0x00001b01, msr_schema_1b01h},
    {0xc0000080, msr_schema_efer},

    // add more schema maps here...
    //
};