# Benchmark Report
- **Model**: gpt-4o-mini
- **Benchmark**: benchmark_170
- **Timestamp**: 2025-06-10 19:30:01

## Performance Summary
| Version | Mean (ms) | Std (ms) | Speedup vs Original | Speedup vs Baseline Optimized |
|---------|-----------|----------|---------------------|-------------------------------|
| Original | 7877.60 | 394.64 | N/A | N/A |
| Baseline Optimized | 3514.40 | 58.29 | 2.24x | N/A |
| Llm Optimized | nan | nan | N/A | N/A |

## Correctness Check
❌ Failed

## Compiler Optimization Passes
| Optimization Pass | Original | Baseline Optimized | LLM Optimized |
|-------------------|----------|--------------------|---------------|
| Dead code elimination | ❌ | ❌ | ❌ |
| Loop unrolling | ❌ | ❌ | ❌ |
| Loop interchange | ❌ | ❌ | ❌ |
| Common subexpr elimination | ❌ | ❌ | ❌ |
| Loop peeling | ❌ | ❌ | ❌ |
| Constant propagation | ❌ | ❌ | ❌ |
| Tail call optimization | ❌ | ❌ | ❌ |
| Strength reduction | ❌ | ❌ | ❌ |
| Inlining | ✅ | ✅ | ❌ |
| Vectorization | ❌ | ❌ | ❌ |

## Optimization Transformation Counts
| Metric | Original | Baseline Optimized | LLM Optimized | Base-Orig | LLM-Base |
|-------|---------|-------------------|--------------|----------|---------|
| branches_removed | 0 | 0 | 0 | +0 | +0 |
| branches_simplified | 0 | 0 | 0 | +0 | +0 |
| common_subexpr_eliminated | 0 | 0 | 0 | +0 | +0 |
| constants_propagated | 0 | 0 | 0 | +0 | +0 |
| dead_stores_removed | 0 | 0 | 0 | +0 | +0 |
| functions_cloned | 0 | 0 | 0 | +0 | +0 |
| functions_eliminated | 0 | 0 | 0 | +0 | +0 |
| functions_inlined | 467 | 450 | 0 | -17 | -450 |
| instructions_eliminated | 0 | 0 | 0 | +0 | +0 |
| loops_peeled | 0 | 0 | 0 | +0 | +0 |
| loops_unrolled | 0 | 0 | 0 | +0 | +0 |
| loops_vectorized | 0 | 0 | 0 | +0 | +0 |
| memory_accesses_optimized | 0 | 0 | 0 | +0 | +0 |
| redundant_load_store_removed | 0 | 0 | 0 | +0 | +0 |
| strength_reduced | 0 | 0 | 0 | +0 | +0 |
| tail_calls_optimized | 0 | 0 | 0 | +0 | +0 |

## GCC Pass Count Comparison
| Pass | Original Count | Baseline Count | LLM Count | Base-Orig | LLM-Base |
|------|---------------|---------------|-----------|-----------|----------|
| adjust_alignment | 1 | 1 | 0 | +0 | -1 |
| alias | 1 | 1 | 0 | +0 | -1 |
| alignments | 1 | 1 | 0 | +0 | -1 |
| asmcons | 1 | 1 | 0 | +0 | -1 |
| backprop | 1 | 1 | 0 | +0 | -1 |
| barriers | 1 | 1 | 0 | +0 | -1 |
| bbpart | 1 | 1 | 0 | +0 | -1 |
| bbro | 1 | 1 | 0 | +0 | -1 |
| bswap | 1 | 1 | 0 | +0 | -1 |
| build_ssa_passes | 1 | 1 | 0 | +0 | -1 |
| ccp1 | 1 | 1 | 0 | +0 | -1 |
| ccp2 | 1 | 1 | 0 | +0 | -1 |
| ccp3 | 1 | 1 | 0 | +0 | -1 |
| cdce | 1 | 1 | 0 | +0 | -1 |
| cddce1 | 1 | 1 | 0 | +0 | -1 |
| cddce2 | 1 | 1 | 0 | +0 | -1 |
| cddce3 | 1 | 1 | 0 | +0 | -1 |
| ce1 | 1 | 1 | 0 | +0 | -1 |
| ce2 | 1 | 1 | 0 | +0 | -1 |
| ce3 | 1 | 1 | 0 | +0 | -1 |
| cfg | 1 | 1 | 0 | +0 | -1 |
| cgraph | 1 | 1 | 0 | +0 | -1 |
| ch2 | 1 | 1 | 0 | +0 | -1 |
| ch_vect | 1 | 1 | 0 | +0 | -1 |
| cmpelim | 1 | 1 | 0 | +0 | -1 |
| combine | 1 | 1 | 0 | +0 | -1 |
| comdats | 1 | 1 | 0 | +0 | -1 |
| compgotos | 1 | 1 | 0 | +0 | -1 |
| copyprop1 | 1 | 1 | 0 | +0 | -1 |
| copyprop2 | 1 | 1 | 0 | +0 | -1 |
| copyprop3 | 1 | 1 | 0 | +0 | -1 |
| copyprop5 | 1 | 1 | 0 | +0 | -1 |
| cp | 1 | 1 | 0 | +0 | -1 |
| cplxlower1 | 1 | 1 | 0 | +0 | -1 |
| cprop1 | 1 | 1 | 0 | +0 | -1 |
| cprop2 | 1 | 1 | 0 | +0 | -1 |
| cprop3 | 1 | 1 | 0 | +0 | -1 |
| cprop_hardreg | 1 | 1 | 0 | +0 | -1 |
| crited1 | 1 | 1 | 0 | +0 | -1 |
| csa | 1 | 1 | 0 | +0 | -1 |
| cse1 | 1 | 1 | 0 | +0 | -1 |
| cse2 | 1 | 1 | 0 | +0 | -1 |
| cse_local | 1 | 1 | 0 | +0 | -1 |
| cselim | 1 | 1 | 0 | +0 | -1 |
| cunroll | 1 | 1 | 0 | +0 | -1 |
| cunrolli | 1 | 1 | 0 | +0 | -1 |
| dce2 | 1 | 1 | 0 | +0 | -1 |
| dce3 | 1 | 1 | 0 | +0 | -1 |
| dce4 | 1 | 1 | 0 | +0 | -1 |
| dce6 | 1 | 1 | 0 | +0 | -1 |
| dce7 | 1 | 1 | 0 | +0 | -1 |
| debug | 1 | 1 | 0 | +0 | -1 |
| devirt | 1 | 1 | 0 | +0 | -1 |
| dfinish | 1 | 1 | 0 | +0 | -1 |
| dfinit | 1 | 1 | 0 | +0 | -1 |
| dom2 | 1 | 1 | 0 | +0 | -1 |
| dom3 | 1 | 1 | 0 | +0 | -1 |
| dse1 | 2 | 2 | 0 | +0 | -2 |
| dse2 | 2 | 2 | 0 | +0 | -2 |
| dse4 | 1 | 1 | 0 | +0 | -1 |
| dwarf2 | 1 | 1 | 0 | +0 | -1 |
| ealias | 1 | 1 | 0 | +0 | -1 |
| early_optimizations | 1 | 1 | 0 | +0 | -1 |
| earlydebug | 1 | 1 | 0 | +0 | -1 |
| eh | 1 | 1 | 0 | +0 | -1 |
| eh_ranges | 1 | 1 | 0 | +0 | -1 |
| ehcleanup1 | 1 | 1 | 0 | +0 | -1 |
| ehcleanup2 | 1 | 1 | 0 | +0 | -1 |
| ehdisp | 1 | 1 | 0 | +0 | -1 |
| ehopt | 1 | 1 | 0 | +0 | -1 |
| einline | 1 | 1 | 0 | +0 | -1 |
| endbr_and_patchable_area | 1 | 1 | 0 | +0 | -1 |
| esra | 1 | 1 | 0 | +0 | -1 |
| ethread | 1 | 1 | 0 | +0 | -1 |
| evrp | 1 | 1 | 0 | +0 | -1 |
| expand | 1 | 1 | 0 | +0 | -1 |
| fab1 | 1 | 1 | 0 | +0 | -1 |
| final | 1 | 1 | 0 | +0 | -1 |
| fix_loops | 1 | 1 | 0 | +0 | -1 |
| fixup_cfg1 | 1 | 1 | 0 | +0 | -1 |
| fixup_cfg2 | 1 | 1 | 0 | +0 | -1 |
| fixup_cfg3 | 1 | 1 | 0 | +0 | -1 |
| fnsplit | 1 | 1 | 0 | +0 | -1 |
| fnsummary | 1 | 1 | 0 | +0 | -1 |
| forwprop1 | 1 | 1 | 0 | +0 | -1 |
| forwprop2 | 1 | 1 | 0 | +0 | -1 |
| forwprop3 | 1 | 1 | 0 | +0 | -1 |
| forwprop4 | 1 | 1 | 0 | +0 | -1 |
| fre1 | 1 | 1 | 0 | +0 | -1 |
| fre3 | 1 | 1 | 0 | +0 | -1 |
| fre5 | 1 | 1 | 0 | +0 | -1 |
| free-fnsummary1 | 1 | 1 | 0 | +0 | -1 |
| free-fnsummary2 | 1 | 1 | 0 | +0 | -1 |
| fwprop1 | 1 | 1 | 0 | +0 | -1 |
| fwprop2 | 1 | 1 | 0 | +0 | -1 |
| gcse2 | 1 | 1 | 0 | +0 | -1 |
| gimple | 1 | 1 | 0 | +0 | -1 |
| icf | 1 | 1 | 0 | +0 | -1 |
| ifcombine | 1 | 1 | 0 | +0 | -1 |
| ifcvt | 1 | 1 | 0 | +0 | -1 |
| iftoswitch | 1 | 1 | 0 | +0 | -1 |
| init-regs | 1 | 1 | 0 | +0 | -1 |
| inline | 1 | 1 | 0 | +0 | -1 |
| into_cfglayout | 1 | 1 | 0 | +0 | -1 |
| ipa-clones | 1 | 1 | 0 | +0 | -1 |
| ira | 1 | 1 | 0 | +0 | -1 |
| isel | 1 | 1 | 0 | +0 | -1 |
| isolate-paths | 1 | 1 | 0 | +0 | -1 |
| ivcanon | 1 | 1 | 0 | +0 | -1 |
| ivopts | 1 | 1 | 0 | +0 | -1 |
| jump | 1 | 1 | 0 | +0 | -1 |
| jump2 | 1 | 1 | 0 | +0 | -1 |
| jump_after_combine | 1 | 1 | 0 | +0 | -1 |
| laddress | 1 | 1 | 0 | +0 | -1 |
| ldist | 1 | 1 | 0 | +0 | -1 |
| lim2 | 1 | 1 | 0 | +0 | -1 |
| lim4 | 1 | 1 | 0 | +0 | -1 |
| linterchange | 1 | 1 | 0 | +0 | -1 |
| local-fnsummary1 | 1 | 1 | 0 | +0 | -1 |
| local-fnsummary2 | 1 | 1 | 0 | +0 | -1 |
| local-pure-const1 | 1 | 1 | 0 | +0 | -1 |
| local-pure-const2 | 1 | 1 | 0 | +0 | -1 |
| loop | 1 | 1 | 0 | +0 | -1 |
| loop2 | 1 | 1 | 0 | +0 | -1 |
| loop2_done | 1 | 1 | 0 | +0 | -1 |
| loop2_init | 1 | 1 | 0 | +0 | -1 |
| loop2_invariant | 1 | 1 | 0 | +0 | -1 |
| loopdone | 1 | 1 | 0 | +0 | -1 |
| loopinit | 1 | 1 | 0 | +0 | -1 |
| lower | 1 | 1 | 0 | +0 | -1 |
| lsplit | 1 | 1 | 0 | +0 | -1 |
| lversion | 1 | 1 | 0 | +0 | -1 |
| mach | 1 | 1 | 0 | +0 | -1 |
| mergephi1 | 1 | 1 | 0 | +0 | -1 |
| mergephi2 | 1 | 1 | 0 | +0 | -1 |
| mergephi3 | 1 | 1 | 0 | +0 | -1 |
| mode_sw | 1 | 1 | 0 | +0 | -1 |
| modref | 1 | 1 | 0 | +0 | -1 |
| modref1 | 1 | 1 | 0 | +0 | -1 |
| modref2 | 1 | 1 | 0 | +0 | -1 |
| no_loop | 1 | 1 | 0 | +0 | -1 |
| nothrow | 2 | 2 | 0 | +0 | -2 |
| nrv | 1 | 1 | 0 | +0 | -1 |
| objsz1 | 1 | 1 | 0 | +0 | -1 |
| objsz2 | 1 | 1 | 0 | +0 | -1 |
| ompexp | 1 | 1 | 0 | +0 | -1 |
| omplower | 1 | 1 | 0 | +0 | -1 |
| opt_local_passes | 1 | 1 | 0 | +0 | -1 |
| optimized | 1 | 1 | 0 | +0 | -1 |
| original | 1 | 1 | 0 | +0 | -1 |
| outof_cfglayout | 1 | 1 | 0 | +0 | -1 |
| pcom | 1 | 1 | 0 | +0 | -1 |
| peephole2 | 1 | 1 | 0 | +0 | -1 |
| phiopt1 | 1 | 1 | 0 | +0 | -1 |
| phiopt2 | 1 | 1 | 0 | +0 | -1 |
| phiopt3 | 1 | 1 | 0 | +0 | -1 |
| phiopt4 | 1 | 1 | 0 | +0 | -1 |
| phiprop | 1 | 1 | 0 | +0 | -1 |
| post_ipa_warn1 | 1 | 1 | 0 | +0 | -1 |
| postreload | 1 | 1 | 0 | +0 | -1 |
| pre | 2 | 2 | 0 | +0 | -2 |
| pro_and_epilogue | 1 | 1 | 0 | +0 | -1 |
| profile_estimate | 2 | 2 | 0 | +0 | -2 |
| pure-const | 1 | 1 | 0 | +0 | -1 |
| reassoc1 | 1 | 1 | 0 | +0 | -1 |
| reassoc2 | 1 | 1 | 0 | +0 | -1 |
| ree | 1 | 1 | 0 | +0 | -1 |
| reginfo | 1 | 1 | 0 | +0 | -1 |
| release_ssa | 1 | 1 | 0 | +0 | -1 |
| reload | 1 | 1 | 0 | +0 | -1 |
| remove_symbols | 1 | 1 | 0 | +0 | -1 |
| resx | 1 | 1 | 0 | +0 | -1 |
| retslot | 1 | 1 | 0 | +0 | -1 |
| rtl_dce | 1 | 1 | 0 | +0 | -1 |
| sccp | 1 | 1 | 0 | +0 | -1 |
| sched2 | 1 | 1 | 0 | +0 | -1 |
| shorten | 1 | 1 | 0 | +0 | -1 |
| simdclone | 1 | 1 | 0 | +0 | -1 |
| sincos | 1 | 1 | 0 | +0 | -1 |
| single-use | 1 | 1 | 0 | +0 | -1 |
| sink | 1 | 1 | 0 | +0 | -1 |
| slp1 | 1 | 1 | 0 | +0 | -1 |
| slp2 | 1 | 1 | 0 | +0 | -1 |
| slsr | 1 | 1 | 0 | +0 | -1 |
| split-paths | 1 | 1 | 0 | +0 | -1 |
| split1 | 1 | 1 | 0 | +0 | -1 |
| split2 | 1 | 1 | 0 | +0 | -1 |
| split3 | 1 | 1 | 0 | +0 | -1 |
| sra | 2 | 2 | 0 | +0 | -2 |
| ssa | 1 | 1 | 0 | +0 | -1 |
| stack | 1 | 1 | 0 | +0 | -1 |
| static-var | 1 | 1 | 0 | +0 | -1 |
| statistics | 1 | 1 | 0 | +0 | -1 |
| stdarg | 1 | 1 | 0 | +0 | -1 |
| store-merging | 1 | 1 | 0 | +0 | -1 |
| strlen1 | 1 | 1 | 0 | +0 | -1 |
| stv1 | 1 | 1 | 0 | +0 | -1 |
| stv2 | 1 | 1 | 0 | +0 | -1 |
| subreg1 | 1 | 1 | 0 | +0 | -1 |
| subreg3 | 1 | 1 | 0 | +0 | -1 |
| switchconv | 1 | 1 | 0 | +0 | -1 |
| switchlower1 | 1 | 1 | 0 | +0 | -1 |
| tailc | 1 | 1 | 0 | +0 | -1 |
| tailr1 | 1 | 1 | 0 | +0 | -1 |
| tailr2 | 1 | 1 | 0 | +0 | -1 |
| targetclone | 1 | 1 | 0 | +0 | -1 |
| thread1 | 1 | 1 | 0 | +0 | -1 |
| thread2 | 1 | 1 | 0 | +0 | -1 |
| thread3 | 1 | 1 | 0 | +0 | -1 |
| thread4 | 1 | 1 | 0 | +0 | -1 |
| type-inheritance | 1 | 1 | 0 | +0 | -1 |
| ud_dce | 1 | 1 | 0 | +0 | -1 |
| uncprop1 | 1 | 1 | 0 | +0 | -1 |
| unrolljam | 1 | 1 | 0 | +0 | -1 |
| unswitch | 1 | 1 | 0 | +0 | -1 |
| veclower21 | 1 | 1 | 0 | +0 | -1 |
| vect | 1 | 1 | 0 | +0 | -1 |
| visibility | 1 | 1 | 0 | +0 | -1 |
| vregs | 1 | 1 | 0 | +0 | -1 |
| vrp1 | 1 | 1 | 0 | +0 | -1 |
| vrp2 | 1 | 1 | 0 | +0 | -1 |
| walloca2 | 1 | 1 | 0 | +0 | -1 |
| whole-program | 1 | 1 | 0 | +0 | -1 |
| widening_mul | 1 | 1 | 0 | +0 | -1 |
| wrestrict | 1 | 1 | 0 | +0 | -1 |
| zero_call_used_regs | 1 | 1 | 0 | +0 | -1 |

## Individual Run Timings (ms)
| Run | Original | Baseline Optimized | LLM Optimized |
|-----|----------|--------------------|---------------|
| 1 | 7533.00 | 3536.00 | nan |
| 2 | 7600.00 | 3560.00 | nan |
| 3 | 7460.00 | 3563.00 | nan |
| 4 | 7650.00 | 3475.00 | nan |
| 5 | 7452.00 | 3563.00 | nan |
| 6 | 8675.00 | 3419.00 | nan |
| 7 | 8302.00 | 3591.00 | nan |
| 8 | 8236.00 | 3425.00 | nan |
| 9 | 7892.00 | 3474.00 | nan |
| 10 | 7976.00 | 3538.00 | nan |

## Error
Compilation failed: Command '['g++', 'harness.cpp', '/workspace/llm_outputs/downstream_task_1_optimized_code/gpt-4o-mini/benchmark_170/optimized.cpp', '-o', 'bench_llm', '-fopt-info-all=bench_llm_optimizations.log', '-I', '/workspace/benchmarks/benchmark_170', '-std=c++17', '-O3', '-fdump-tree-all-all', '-fdump-rtl-all-all', '-fdump-ipa-all-all']' returned non-zero exit status 1.
