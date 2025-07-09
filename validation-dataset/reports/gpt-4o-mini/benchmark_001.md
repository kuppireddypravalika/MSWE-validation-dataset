# Benchmark Report
- **Model**: gpt-4o-mini
- **Benchmark**: benchmark_001
- **Timestamp**: 2025-06-10 19:32:31

## Performance Summary
| Version | Mean (ms) | Std (ms) | Speedup vs Original | Speedup vs Baseline Optimized |
|---------|-----------|----------|---------------------|-------------------------------|
| Original | 9360.90 | 578.17 | N/A | N/A |
| Baseline Optimized | 71.60 | 0.80 | 130.74x | N/A |
| Llm Optimized | 10965.70 | 3556.87 | 0.85x | 0.01x |

## Correctness Check
✅ Passed

## Compiler Optimization Passes
| Optimization Pass | Original | Baseline Optimized | LLM Optimized |
|-------------------|----------|--------------------|---------------|
| Strength reduction | ❌ | ❌ | ❌ |
| Dead code elimination | ❌ | ❌ | ❌ |
| Constant propagation | ❌ | ❌ | ❌ |
| Vectorization | ❌ | ❌ | ❌ |
| Tail call optimization | ❌ | ❌ | ❌ |
| Loop peeling | ❌ | ❌ | ❌ |
| Inlining | ✅ | ✅ | ✅ |
| Loop unrolling | ❌ | ❌ | ❌ |
| Loop interchange | ❌ | ❌ | ❌ |
| Common subexpr elimination | ❌ | ❌ | ❌ |

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
| functions_inlined | 931 | 975 | 931 | +44 | -44 |
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
| adjust_alignment | 1 | 1 | 1 | +0 | +0 |
| alias | 1 | 1 | 1 | +0 | +0 |
| alignments | 1 | 1 | 1 | +0 | +0 |
| asmcons | 1 | 1 | 1 | +0 | +0 |
| backprop | 1 | 1 | 1 | +0 | +0 |
| barriers | 1 | 1 | 1 | +0 | +0 |
| bbro | 1 | 1 | 1 | +0 | +0 |
| bswap | 1 | 1 | 1 | +0 | +0 |
| build_ssa_passes | 1 | 1 | 1 | +0 | +0 |
| ccp1 | 1 | 1 | 1 | +0 | +0 |
| ccp2 | 1 | 1 | 1 | +0 | +0 |
| ccp3 | 1 | 1 | 1 | +0 | +0 |
| cdce | 1 | 1 | 1 | +0 | +0 |
| cddce1 | 1 | 1 | 1 | +0 | +0 |
| cddce2 | 1 | 1 | 1 | +0 | +0 |
| cddce3 | 1 | 1 | 1 | +0 | +0 |
| ce1 | 1 | 1 | 1 | +0 | +0 |
| ce2 | 1 | 1 | 1 | +0 | +0 |
| ce3 | 1 | 1 | 1 | +0 | +0 |
| cfg | 1 | 1 | 1 | +0 | +0 |
| cgraph | 1 | 1 | 1 | +0 | +0 |
| ch2 | 1 | 1 | 1 | +0 | +0 |
| ch_vect | 1 | 1 | 1 | +0 | +0 |
| cmpelim | 1 | 1 | 1 | +0 | +0 |
| combine | 1 | 1 | 1 | +0 | +0 |
| comdats | 1 | 1 | 1 | +0 | +0 |
| compgotos | 1 | 1 | 1 | +0 | +0 |
| copyprop1 | 1 | 1 | 1 | +0 | +0 |
| copyprop2 | 1 | 1 | 1 | +0 | +0 |
| copyprop3 | 1 | 1 | 1 | +0 | +0 |
| copyprop5 | 1 | 1 | 1 | +0 | +0 |
| cp | 1 | 1 | 1 | +0 | +0 |
| cplxlower1 | 1 | 1 | 1 | +0 | +0 |
| cprop1 | 1 | 1 | 1 | +0 | +0 |
| cprop2 | 1 | 1 | 1 | +0 | +0 |
| cprop3 | 1 | 1 | 1 | +0 | +0 |
| cprop_hardreg | 1 | 1 | 1 | +0 | +0 |
| crited1 | 1 | 1 | 1 | +0 | +0 |
| csa | 1 | 1 | 1 | +0 | +0 |
| cse1 | 1 | 1 | 1 | +0 | +0 |
| cse2 | 1 | 1 | 1 | +0 | +0 |
| cse_local | 1 | 1 | 1 | +0 | +0 |
| cselim | 1 | 1 | 1 | +0 | +0 |
| cunroll | 1 | 1 | 1 | +0 | +0 |
| cunrolli | 1 | 1 | 1 | +0 | +0 |
| dce2 | 1 | 1 | 1 | +0 | +0 |
| dce3 | 1 | 1 | 1 | +0 | +0 |
| dce4 | 1 | 1 | 1 | +0 | +0 |
| dce6 | 1 | 1 | 1 | +0 | +0 |
| dce7 | 1 | 1 | 1 | +0 | +0 |
| debug | 1 | 1 | 1 | +0 | +0 |
| devirt | 1 | 1 | 1 | +0 | +0 |
| dfinish | 1 | 1 | 1 | +0 | +0 |
| dfinit | 1 | 1 | 1 | +0 | +0 |
| dom2 | 1 | 1 | 1 | +0 | +0 |
| dom3 | 1 | 1 | 1 | +0 | +0 |
| dse1 | 2 | 2 | 2 | +0 | +0 |
| dse2 | 2 | 2 | 2 | +0 | +0 |
| dse4 | 1 | 1 | 1 | +0 | +0 |
| dwarf2 | 1 | 1 | 1 | +0 | +0 |
| ealias | 1 | 1 | 1 | +0 | +0 |
| early_optimizations | 1 | 1 | 1 | +0 | +0 |
| earlydebug | 1 | 1 | 1 | +0 | +0 |
| eh | 1 | 1 | 1 | +0 | +0 |
| ehcleanup1 | 1 | 1 | 1 | +0 | +0 |
| ehopt | 1 | 1 | 1 | +0 | +0 |
| einline | 1 | 1 | 1 | +0 | +0 |
| endbr_and_patchable_area | 1 | 1 | 1 | +0 | +0 |
| esra | 1 | 1 | 1 | +0 | +0 |
| ethread | 1 | 1 | 1 | +0 | +0 |
| evrp | 1 | 1 | 1 | +0 | +0 |
| expand | 1 | 1 | 1 | +0 | +0 |
| fab1 | 1 | 1 | 1 | +0 | +0 |
| final | 1 | 1 | 1 | +0 | +0 |
| fix_loops | 1 | 1 | 1 | +0 | +0 |
| fixup_cfg1 | 1 | 1 | 1 | +0 | +0 |
| fixup_cfg2 | 1 | 1 | 1 | +0 | +0 |
| fixup_cfg3 | 1 | 1 | 1 | +0 | +0 |
| fnsplit | 1 | 1 | 1 | +0 | +0 |
| fnsummary | 1 | 1 | 1 | +0 | +0 |
| forwprop1 | 1 | 1 | 1 | +0 | +0 |
| forwprop2 | 1 | 1 | 1 | +0 | +0 |
| forwprop3 | 1 | 1 | 1 | +0 | +0 |
| forwprop4 | 1 | 1 | 1 | +0 | +0 |
| fre1 | 1 | 1 | 1 | +0 | +0 |
| fre3 | 1 | 1 | 1 | +0 | +0 |
| fre5 | 1 | 1 | 1 | +0 | +0 |
| free-fnsummary1 | 1 | 1 | 1 | +0 | +0 |
| free-fnsummary2 | 1 | 1 | 1 | +0 | +0 |
| fwprop1 | 1 | 1 | 1 | +0 | +0 |
| fwprop2 | 1 | 1 | 1 | +0 | +0 |
| gcse2 | 1 | 1 | 1 | +0 | +0 |
| gimple | 1 | 1 | 1 | +0 | +0 |
| hoist | 1 | 1 | 1 | +0 | +0 |
| icf | 1 | 1 | 1 | +0 | +0 |
| ifcombine | 1 | 1 | 1 | +0 | +0 |
| ifcvt | 1 | 1 | 1 | +0 | +0 |
| iftoswitch | 1 | 1 | 1 | +0 | +0 |
| init-regs | 1 | 1 | 1 | +0 | +0 |
| inline | 1 | 1 | 1 | +0 | +0 |
| into_cfglayout | 1 | 1 | 1 | +0 | +0 |
| ipa-clones | 1 | 1 | 1 | +0 | +0 |
| ira | 1 | 1 | 1 | +0 | +0 |
| isel | 1 | 1 | 1 | +0 | +0 |
| isolate-paths | 1 | 1 | 1 | +0 | +0 |
| ivcanon | 1 | 1 | 1 | +0 | +0 |
| ivopts | 1 | 1 | 1 | +0 | +0 |
| jump | 1 | 1 | 1 | +0 | +0 |
| jump2 | 1 | 1 | 1 | +0 | +0 |
| jump_after_combine | 1 | 1 | 1 | +0 | +0 |
| laddress | 1 | 1 | 1 | +0 | +0 |
| ldist | 1 | 1 | 1 | +0 | +0 |
| lim2 | 1 | 1 | 1 | +0 | +0 |
| lim4 | 1 | 1 | 1 | +0 | +0 |
| linterchange | 1 | 1 | 1 | +0 | +0 |
| local-fnsummary1 | 1 | 1 | 1 | +0 | +0 |
| local-fnsummary2 | 1 | 1 | 1 | +0 | +0 |
| local-pure-const1 | 1 | 1 | 1 | +0 | +0 |
| local-pure-const2 | 1 | 1 | 1 | +0 | +0 |
| loop | 1 | 1 | 1 | +0 | +0 |
| loop2 | 1 | 1 | 1 | +0 | +0 |
| loop2_done | 1 | 1 | 1 | +0 | +0 |
| loop2_init | 1 | 1 | 1 | +0 | +0 |
| loop2_invariant | 1 | 1 | 1 | +0 | +0 |
| loopdone | 1 | 1 | 1 | +0 | +0 |
| loopinit | 1 | 1 | 1 | +0 | +0 |
| lower | 1 | 1 | 1 | +0 | +0 |
| lsplit | 1 | 1 | 1 | +0 | +0 |
| lversion | 1 | 1 | 1 | +0 | +0 |
| mach | 1 | 1 | 1 | +0 | +0 |
| mergephi1 | 1 | 1 | 1 | +0 | +0 |
| mergephi2 | 1 | 1 | 1 | +0 | +0 |
| mergephi3 | 1 | 1 | 1 | +0 | +0 |
| mode_sw | 1 | 1 | 1 | +0 | +0 |
| modref | 1 | 1 | 1 | +0 | +0 |
| modref1 | 1 | 1 | 1 | +0 | +0 |
| modref2 | 1 | 1 | 1 | +0 | +0 |
| no_loop | 1 | 1 | 1 | +0 | +0 |
| nothrow | 2 | 2 | 2 | +0 | +0 |
| nrv | 1 | 1 | 1 | +0 | +0 |
| objsz1 | 1 | 1 | 1 | +0 | +0 |
| objsz2 | 1 | 1 | 1 | +0 | +0 |
| ompexp | 1 | 1 | 1 | +0 | +0 |
| omplower | 1 | 1 | 1 | +0 | +0 |
| opt_local_passes | 1 | 1 | 1 | +0 | +0 |
| optimized | 1 | 1 | 1 | +0 | +0 |
| original | 1 | 1 | 1 | +0 | +0 |
| outof_cfglayout | 1 | 1 | 1 | +0 | +0 |
| pcom | 1 | 1 | 1 | +0 | +0 |
| peephole2 | 1 | 1 | 1 | +0 | +0 |
| phiopt1 | 1 | 1 | 1 | +0 | +0 |
| phiopt2 | 1 | 1 | 1 | +0 | +0 |
| phiopt3 | 1 | 1 | 1 | +0 | +0 |
| phiopt4 | 1 | 1 | 1 | +0 | +0 |
| phiprop | 1 | 1 | 1 | +0 | +0 |
| post_ipa_warn1 | 1 | 1 | 1 | +0 | +0 |
| postreload | 1 | 1 | 1 | +0 | +0 |
| pre | 2 | 2 | 2 | +0 | +0 |
| pro_and_epilogue | 1 | 1 | 1 | +0 | +0 |
| profile_estimate | 2 | 2 | 2 | +0 | +0 |
| pure-const | 1 | 1 | 1 | +0 | +0 |
| reassoc1 | 1 | 1 | 1 | +0 | +0 |
| reassoc2 | 1 | 1 | 1 | +0 | +0 |
| ree | 1 | 1 | 1 | +0 | +0 |
| reginfo | 1 | 1 | 1 | +0 | +0 |
| release_ssa | 1 | 1 | 1 | +0 | +0 |
| reload | 1 | 1 | 1 | +0 | +0 |
| remove_symbols | 1 | 1 | 1 | +0 | +0 |
| resx | 1 | 1 | 1 | +0 | +0 |
| retslot | 1 | 1 | 1 | +0 | +0 |
| rtl_dce | 1 | 1 | 1 | +0 | +0 |
| sccp | 1 | 1 | 1 | +0 | +0 |
| sched2 | 1 | 1 | 1 | +0 | +0 |
| shorten | 1 | 1 | 1 | +0 | +0 |
| simdclone | 1 | 1 | 1 | +0 | +0 |
| sincos | 1 | 1 | 1 | +0 | +0 |
| single-use | 1 | 1 | 1 | +0 | +0 |
| sink | 1 | 1 | 1 | +0 | +0 |
| slp1 | 1 | 1 | 1 | +0 | +0 |
| slp2 | 1 | 1 | 1 | +0 | +0 |
| slsr | 1 | 1 | 1 | +0 | +0 |
| split-paths | 1 | 1 | 1 | +0 | +0 |
| split1 | 1 | 1 | 1 | +0 | +0 |
| split2 | 1 | 1 | 1 | +0 | +0 |
| split3 | 1 | 1 | 1 | +0 | +0 |
| sra | 2 | 2 | 2 | +0 | +0 |
| ssa | 1 | 1 | 1 | +0 | +0 |
| stack | 1 | 1 | 1 | +0 | +0 |
| static-var | 1 | 1 | 1 | +0 | +0 |
| statistics | 1 | 1 | 1 | +0 | +0 |
| stdarg | 1 | 1 | 1 | +0 | +0 |
| store-merging | 1 | 1 | 1 | +0 | +0 |
| strlen1 | 1 | 1 | 1 | +0 | +0 |
| stv1 | 1 | 1 | 1 | +0 | +0 |
| stv2 | 1 | 1 | 1 | +0 | +0 |
| subreg1 | 1 | 1 | 1 | +0 | +0 |
| subreg3 | 1 | 1 | 1 | +0 | +0 |
| switchconv | 1 | 1 | 1 | +0 | +0 |
| switchlower1 | 1 | 1 | 1 | +0 | +0 |
| tailc | 1 | 1 | 1 | +0 | +0 |
| tailr1 | 1 | 1 | 1 | +0 | +0 |
| tailr2 | 1 | 1 | 1 | +0 | +0 |
| targetclone | 1 | 1 | 1 | +0 | +0 |
| thread1 | 1 | 1 | 1 | +0 | +0 |
| thread2 | 1 | 1 | 1 | +0 | +0 |
| thread3 | 1 | 1 | 1 | +0 | +0 |
| thread4 | 1 | 1 | 1 | +0 | +0 |
| type-inheritance | 1 | 1 | 1 | +0 | +0 |
| ud_dce | 1 | 1 | 1 | +0 | +0 |
| uncprop1 | 1 | 1 | 1 | +0 | +0 |
| unrolljam | 1 | 1 | 1 | +0 | +0 |
| unswitch | 1 | 1 | 1 | +0 | +0 |
| veclower21 | 1 | 1 | 1 | +0 | +0 |
| vect | 1 | 1 | 1 | +0 | +0 |
| visibility | 1 | 1 | 1 | +0 | +0 |
| vregs | 1 | 1 | 1 | +0 | +0 |
| vrp1 | 1 | 1 | 1 | +0 | +0 |
| vrp2 | 1 | 1 | 1 | +0 | +0 |
| walloca2 | 1 | 1 | 1 | +0 | +0 |
| whole-program | 1 | 1 | 1 | +0 | +0 |
| widening_mul | 1 | 1 | 1 | +0 | +0 |
| wrestrict | 1 | 1 | 1 | +0 | +0 |
| zero_call_used_regs | 1 | 1 | 1 | +0 | +0 |

## Individual Run Timings (ms)
| Run | Original | Baseline Optimized | LLM Optimized |
|-----|----------|--------------------|---------------|
| 1 | 10043.00 | 73.00 | 13752.00 |
| 2 | 10063.00 | 71.00 | 20821.00 |
| 3 | 10065.00 | 71.00 | 9118.00 |
| 4 | 10103.00 | 71.00 | 9014.00 |
| 5 | 8887.00 | 71.00 | 8910.00 |
| 6 | 8862.00 | 71.00 | 9070.00 |
| 7 | 8868.00 | 72.00 | 9073.00 |
| 8 | 8930.00 | 72.00 | 9994.00 |
| 9 | 8890.00 | 71.00 | 9938.00 |
| 10 | 8898.00 | 73.00 | 9967.00 |
