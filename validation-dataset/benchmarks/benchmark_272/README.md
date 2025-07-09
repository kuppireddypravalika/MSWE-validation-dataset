# Benchmark 272: FiniteDifferenceNewtonSafe vs Brent

This benchmark is derived from a QuantLib commit where the bootstrap routine switched from the Brent solver to the FiniteDifferenceNewtonSafe solver in order to speed up yield curve recalculation.

- **Commit:** [60df951](https://github.com/lballabio/QuantLib/commit/60df95161a08e0a04a91d88e377762f0f733f772)
- **Author:** Ferdinando Ametrano
- **Optimization:** Use the Newton based `FiniteDifferenceNewtonSafe` solver instead of the bracketing `Brent` method during iterative bootstrapping.

## Benchmark Goal

Evaluate the performance advantage of the Newton based solver over Brent when repeatedly solving root finding problems during curve construction.
