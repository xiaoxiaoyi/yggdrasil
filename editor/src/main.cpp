#include <iostream>
#include "MapEditorWindow.hpp"
#include "sprite/SpriteEditorWindow.hpp"
#include "common_editor.hpp"

#include <QApplication>

int main(int argc, char ** argv) {
    QApplication a(argc, argv);
    setlocale(LC_NUMERIC, "C");

    //MainWindow mapEditor;
    SpriteEditorWindow spriteEditor;

    int  runWindow(2);

    while(runWindow != 0){
        //mapEditor.hide();
        spriteEditor.hide();
/*
        if(runWindow == ygg::MAP_EDITOR_ID)
            mapEditor.show();

        else */if(runWindow == ygg::SPRITE_EDITOR_ID)
            spriteEditor.show();

        runWindow = a.exec();
    }

    return 0;
}
