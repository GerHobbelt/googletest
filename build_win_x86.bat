
md build
cd build
cmake -A Win32  -DCMAKE_CXX_COMPILER="c++" ../

set devenv_dir="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv"
echo "start build Release"
%devenv_dir% googletest-distribution.sln /Build "Release|Win32"


md ../infistd/lib/win/x86/
rem cp -v build/lib/libgtest.a ../infistd/lib/osx/x86/