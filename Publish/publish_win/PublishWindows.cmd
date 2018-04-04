rmdir /s /q trse
set dir=C:\Users\leuat\OneDrive\Documents\GitHub\build-Fluff64-Desktop_Qt_5_10_1_MSVC2017_64bit-Release\
mkdir trse
xcopy /s %dir%\release\*.exe trse\
xcopy /s %dir%\release\iconengines trse\
xcopy /s %dir%\release\platforms trse\
xcopy /s %dir%\release\*.dll trse\
xcopy /s ..\source\* trse
xcopy 7z.exe trse\
move trse\fluff64.exe trse\trse.exe
move trse bin
mkdir trse
move bin trse

xcopy /e  ..\tutorials\RasLib trse\RasLib\
xcopy /e  ..\tutorials\Tutorials1-10\* trse\Tutorials1-10\
xcopy /e  ..\tutorials\TutorialGame_RogueBurgerOne\* trse\TutorialGame_RogueBurgerOne\
copy C:\Users\leuat\OneDrive\Documents\GitHub\Fluff64\Strapper\build-strapper-Desktop_Qt_5_10_1_MSVC2017_64bit-Release\release\strapper.exe trse\trse.exe
xcopy /e dll\* trse\
