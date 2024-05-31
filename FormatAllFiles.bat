@echo off

echo "formatting chip8"

for /r "chip8" %%i in (*.c *.h *.cpp *.hpp) do (
    echo "%%i"
    clang-format -i "%%i"
)
echo "formatting app"

for /r "app" %%i in (*.c *.h *.cpp *.hpp) do (
    echo "%%i"
    clang-format -i "%%i"
)
