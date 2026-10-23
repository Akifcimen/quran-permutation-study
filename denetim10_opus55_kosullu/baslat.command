#!/bin/zsh
cd "$HOME/opus5.5/kosullu"
exec "$HOME/.local/bin/claude" --model claude-opus-5-5 --effort max "Bu klasördeki GOREV.md dosyasını baştan sona oku ve görevi uygula. ~/Desktop/hrf2 salt okunurdur; bütün çalışma bu klasörde. Ön-kaydı hiçbir koşullu puana bakmadan yaz ve dondur."
