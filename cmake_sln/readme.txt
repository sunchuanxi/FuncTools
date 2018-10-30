file structure
                 --------------- ------------- --------------- --------------- ------------------ ----------
                 |              |             |               |               |                  |
                 |              |             |               |               |                  |
                build        include         lib             src         CmakeLists.txt      readme.txt




generate & build 
1. cmake -G "Visual Studio 12 2013 Win64" -Thost=x64 ..
2. open ./build/hello.sln and build with VS2013 