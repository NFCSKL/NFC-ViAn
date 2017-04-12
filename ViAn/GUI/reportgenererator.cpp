#include "reportgenererator.h"



ReportGenererator::ReportGenererator()
{
    picPath = "C:\\Users\\georg\\Documents\\bild.png";
    QAxObject*  word = new QAxObject("Word.Application");

    //2.OPEN THE DOCUMENT
    QAxObject* doc = word->querySubObject("Documents");
    doc->dynamicCall("Add()");
    doc->setProperty("Visible",true); // second bool to hide application when opened.

    //3.GET TO THE CONTENTS
    QAxObject* activeDocument=word->querySubObject("ActiveDocument");

    QAxObject* activeWindow = activeDocument->querySubObject( "ActiveWindow" );
    QAxObject* selection = activeWindow->querySubObject( "Selection" );

    QAxObject* shapes = selection->querySubObject( "InlineShapes" );
    QAxObject* inlineShape = shapes->querySubObject(
                "AddPicture(const QString&,bool,bool,QVariant)",
                 picPath, true, true );

    //4. Save and close file
    activeDocument->dynamicCall("SaveAs (const QString&)", QString("tempp.docx"));
    doc->dynamicCall("Close()");
    word->dynamicCall("Quit()");
}
