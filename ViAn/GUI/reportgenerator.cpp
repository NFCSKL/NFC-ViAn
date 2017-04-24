#include "reportgenerator.h"




ReportGenerator::ReportGenerator(Project* proj, FileHandler *file_handler)
{
    this->proj = proj;
    this->file_handler = file_handler;
    //this->bookmark_path = proj_path.append("/bookmarks/");
    //std::cout << "proj_path: " << proj_path << std::endl;
   // std::cout << "bookmark_path: " << bookmark_path << std::endl;
    //1. START APPLICATION
    QAxObject*  word = new QAxObject("Word.Application");

    //2.OPEN THE DOCUMENT
    QAxObject* doc = word->querySubObject("Documents");
    //QAxObject* workbook = doc->querySubObject( "Open(const QString&)", "C:/Users/georg/Documents/Dok3.docx" );
    doc->dynamicCall("Add()");
    word->setProperty("Visible",false); // second bool to hide application when opened.

    //3.GET TO THE CONTENTS
    QAxObject* activeDocument=word->querySubObject("ActiveDocument");
    QAxObject* activeWindow = activeDocument->querySubObject( "ActiveWindow" );
    QAxObject* selection = activeWindow->querySubObject( "Selection" );

    //4. ADD IMAGES FROM BOOKMARK FOLDER
    create_list_of_names();
    add_pictures(selection);

    //5. SAVE AND CLOSE FILE
    save_report(activeDocument);
    close_report(doc, word);
}

void ReportGenerator::create_list_of_names()
{
    std::map<ID, VideoProject*> videos = proj->videos;

    // get all bookmarks for a project by iterating over each videos bookmarks.
    for(std::pair<ID, VideoProject*> video : videos) {
        std::vector<Bookmark *> bookmark_list = video.second->get_bookmarks();
        for(Bookmark* video_bookmark : bookmark_list) {
            all_bookmarks.push_back(video_bookmark->get_file_path().toStdString());
            std::cout << video_bookmark->get_file_path().toStdString() << std::endl;
        }
    }
}

void ReportGenerator::add_pictures(QAxObject* selection)
{

    picPath.clear();
    QAxObject* shapes = selection->querySubObject( "InlineShapes" );
    foreach (std::string bookmark, all_bookmarks) {

       // picPath.append(QString::fromStdString(bookmark_path)).append(QString::fromStdString(name));
       // std::cout << picPath.toStdString() << std::endl;
        QAxObject* inlineShape = shapes->querySubObject(
                    "AddPicture(const QString&,bool,bool,QVariant)",
                     QString::fromStdString(bookmark), true, true );

        // Center image
        selection->querySubObject( "ParagraphFormat" )->setProperty( "Alignment", 1 );

        // Add paragraphs between images
        selection->dynamicCall( "Collapse(int)", 0 );
        for (int i = 0; i < 5; ++i) {
             selection->dynamicCall( "InsertParagraphAfter()" );
        }
        selection->dynamicCall( "Collapse(int)", 0 );

        picPath.clear();
    }
}


std::string ReportGenerator::date_time_generator()
{
    time_t now = time(0);
    std::string dt = ctime(&now);
    std::replace( dt.begin(), dt.end(), ':', '-');
    std::replace( dt.begin(), dt.end(), ' ' , '_');
    dt.erase(std::remove(dt.begin(), dt.end(), '\n'), dt.end());
    return dt;
}

void ReportGenerator::save_report(QAxObject* activeDocument)
{
    std::string dt = date_time_generator();

    std::string proj_path = file_handler->get_dir(proj->dir).absolutePath().toStdString();
    std::string path = proj_path.append("/report_").append(dt).append(".docx");

    activeDocument->dynamicCall("SaveAs (const QString&)", QString::fromStdString(path));
}

void ReportGenerator::close_report(QAxObject* doc, QAxObject*  word)
{
    doc->dynamicCall("Close()");
    word->dynamicCall("Quit()");
}



