PATH=/opt/qt6.4.3/macos/bin:$PATH
for var in "$@"; do
    if ( [ $var"_" = "build_" ]); then
        qmake
        make
    elif ( [ $var"_" = "run_" ]); then
        ./project.app/Contents/MacOS/project
    elif ( [ $var"_" = "clean_" ]); then
        rm -Rf ./moc
        rm -Rf ./obj
        rm -Rf ./project.app
        rm -Rf ./qrc_resources.cpp
        rm -Rf .qmake.stash
        rm -Rf Makefile
    else
        echo "Enter (build) (run) or (clean) please."
    fi
done