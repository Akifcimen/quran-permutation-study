#!/usr/bin/env bash
# Regenerate the complete fixed-budget experiment. Run from the unpacked folder.
set -euo pipefail
study_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$study_root"
mkdir -p results figures review .cache/matplotlib
export MPLCONFIGDIR="$study_root/.cache/matplotlib"
g++ -O3 -std=c++17 -march=native -fopenmp -Wall -Wextra src/study_engine.cpp -o study_engine
study_models=(uniform block6 inversion_exact)
for study_i in 0 1 2; do
  study_model="${study_models[$study_i]}"
  study_train_seed=$((202610250101+study_i))
  study_test_seed=$((202610250201+study_i))
  ./study_engine --phase train --model "$study_model" --n 200000 --threads 8 --seed "$study_train_seed" --out "results/train_$study_model"
  ./study_engine --phase observed --model "$study_model" --train "results/train_$study_model.bin" --out "results/observed_$study_model"
  ./study_engine --phase test --model "$study_model" --n 1000000 --threads 8 --seed "$study_test_seed" --train "results/train_$study_model.bin" --out "results/test_$study_model"
done
python observed_structure.py
python analyze.py
python review/observed_decomposition.py
python semantic/analyze_results_independent.py
python build_notebook.py
