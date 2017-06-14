#include "filehandler.h"

/**
 * @brief FileHandler::FileHandler
 * Initialize filehandler and set
 * default value for workspace
 */
FileHandler::FileHandler() {
    this->file_id = 0;
    this->dir_id = 0;
    this->last_error = false;     
    #ifdef _WIN32
        this->work_space = create_directory("C:/");
    #elif __APPLE__
        this->work_space =  create_directory("/Applications/");
    #elif __unix__
        this->work_space =  create_directory("~/");
    #endif
    load();
}

/**
 * @brief FileHandler::~FileHandler
 * Save filehandler state when destroyed.
 * Deletes projects.
 */
FileHandler::~FileHandler(){
    // Delete projects
    for(auto it = projects.begin(); it != projects.end(); it++){
        delete it->second;
    }
    save();
}

/**
 * @brief FileHandler::save
 * Save filehandler.
 */
void FileHandler::save(){
    QDir dir;
    this->save_name = dir.absoluteFilePath("state").toStdString();
    save_saveable(this,add_dir(dir), this->save_format);
}

/**
 * @brief FileHandler::load
 * Load filehandler.
 */
void FileHandler::load(){
    QDir dir;
    dir.cd(".");
    if(this->save_format == BINARY )this->save_name = dir.absoluteFilePath("state.dat").toStdString();
    else this->save_name = dir.absoluteFilePath("state.json").toStdString();
    load_saveable(this, this->save_name, this->save_format);
}

/**
 * @todo save workspace to file
 * @brief FileHandler::set_workspace
 * @param new_work_space
 * Change default work_space, ie where project files are saved.
 */
void FileHandler::set_work_space(std::string new_work_space){
    this->work_space = create_directory(QString::fromStdString(new_work_space));
}

/**
 * @brief FileHandler::get_work_space
 * @return default work_space
 */
QDir FileHandler::get_work_space(){
    return this->get_dir(this->work_space);
}

/**
 * @brief FileHandler::read
 * @param json
 * Reads filehandler from a json object.
 */
void FileHandler::read(const QJsonObject &json){
//    QJsonArray json_projs = json["open"].toArray();
//    for(int i = 0; i != json_projs.size(); i++){
//        QJsonObject json_path = json_projs[i].toObject();
//        std::string path = json_path["path"].toString().toStdString();
//        open_project(load_project(path)->id);
//    }
}
/**
 * @brief FileHandler::write
 * @param json
 * Writes filehandler to a json object
 */
void FileHandler::write(QJsonObject &json){
//    QJsonArray json_projs;
//    for (auto it = open_projects.begin();  it != open_projects.end(); it++) {
//        QJsonObject json_path;
//        ID id = *it;
//        Project* proj = get_project(id);
//        QDir dir = get_dir(proj->dir);
//        QString path;
//        if(this->save_format == BINARY ) path = dir.absoluteFilePath((proj->name + ".dat").c_str());
//        else path = dir.absoluteFilePath((proj->name + ".json").c_str());

//        json_path["path"] = path;
//        json_projs.append(json_path);
//    }
//    json["open"] = json_projs;
}


/**
 * @brief FileHandler::create_directory
 * @param dir_path
 * @return id
 * Creates a directory and all directories needed for that directory, ie
 * "C:/THIS/IS/A/PATH/" will create THIS, IS, A and PATH directories if these do not exist.
 */
ID FileHandler::create_directory(QString dir_path){
    QDir dir (QDir::root());
    last_error = !dir.mkpath(dir_path);
    if(last_error){
        qWarning("Could not create directory %s",dir_path.toStdString().c_str());
    }

    dir.setPath(dir_path);
    ID id = this->add_dir(dir);
    return id;
}

/**
 * @brief FileHandler::delete_directory
 * @param id
 * @return bool, success.
 * Deletes a given directory
 */
bool FileHandler::delete_directory(ID id){
    QDir temp = this->get_dir(id);
    this->dir_map_lock.lock(); // Order important, locked in getter
    if(temp.rmdir(temp.absolutePath())){
        this->dir_map.erase(id);
        this->dir_map_lock.unlock();
        return true;
    }
    this->dir_map_lock.unlock();
    return false;

}

/**
 * @brief FileHandler::create_file
 * @param std::string file name, ID directory id
 * create a file by given name in already existing
 * application tracked directory.
 * Note that the file is not actually created
 * in the file system until the file is written to.
 */
ID FileHandler::create_file(QString file_name, QDir dir){
    return this->add_file(dir.absoluteFilePath(file_name)); // File created
  }

/**
 * @brief FileHandler::delete_file
 * delete application tracked file
 * @param ID file id
 */
 bool FileHandler::delete_file(ID id){
    QFile file(this->get_file(id));
    file.remove();
    return this->file_map.erase(id);
 }

 /**
  * @todo make threadsafe
  * @brief FileHandler::write_file
  *  Write given text to an application tracked file
  * @param ID file id, std::string text
  * @return void
  * Write given text with OPTION opt, supported OPTIONs are append and overwrite.
  */
 void FileHandler::write_file(ID id, QString text, WRITE_OPTION opt){
    QFile file;
    file.setFileName(this->get_file(id));

    switch(opt){
    case OVERWRITE:
        if(!file.open(QIODevice::ReadWrite | QIODevice::QIODevice::Truncate |QIODevice::Text))return; // Empty file
        break;
    case APPEND:
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) return; // File can not be written
        break;
    default:
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) return; // File can not be written
        break;
    }
    QTextStream out(&file);
    out << text;
    file.close();
 }

 /**
  * @brief FileHandler::read_file
  * @param ID file id, std::string text
  * @return void
  * Read given length of lines to buffer from application
  * tracked file. OBS! If number of lines exceeds lines in file,
  * reads to end of file (EOF)
  */
 void FileHandler::read_file(ID id,  QString& buf, int lines_to_read){
    QFile file (this->get_file(id));
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        qWarning("File %s could not be read", file.fileName().toStdString().c_str());
        return; // File can not be read
    }
    QTextStream in(&file);
    QString line;
    while(lines_to_read-- && !in.atEnd()){
        line = in.readLine();
        buf.append(line);
    };
    file.close();
 }

 /**
  * @brief FileHandler::get_file
  * Getter
  * @param ID project file id
  * @return std::string file_path
  * Get file by ID, locks in doing so.
  */
 QString FileHandler::get_file(ID id){
    this->file_map_lock.lock();
    QString file = this->file_map.at(id);
    this->file_map_lock.unlock();
    return file;
 }

 /**
  * @brief FileHandler::get_dir
  * @param ID directory id
  * @return directory path
  * Gets directory by id, locks in doing so.
  */
 QDir FileHandler::get_dir(ID id){
    this->dir_map_lock.lock();
    QDir dir = this->dir_map.at(id);
    this->dir_map_lock.unlock();
    return dir;
 }


 /**
  * @brief FileHandler::add_file
  * @param std::string file_path
  * @return unique file identifier
  * Adds file to filesystem.
  */
ID FileHandler::add_file(QString file){
    add_file(this->file_id, file);
    return this->file_id++;
 }

/**
 * @brief FileHandler::add_file
 * @param id
 * @param file_path
 * Adds file to file_map, locks in doing so.
 */
void FileHandler::add_file(ID id ,QString file){
    std::pair<ID,QString> pair = std::make_pair(id, file);
    this->file_map_lock.lock();
    this->file_map.insert(pair);
    this->file_map_lock.unlock();
}

 /**
  * @brief FileHandler::add_dir
  * @param std::string dir_path
  * @return unique directory identifier
  * Adds directory to directories, locks in doing so.
  */
ID FileHandler::add_dir(QDir dir){
    add_dir(this->dir_id, dir);
    return this->dir_id++;
 }

/**
 * @brief FileHandler::add_dir
 * @param dir
 * Inserts directory by id in directories.
 */
void FileHandler::add_dir(ID dir_id, QDir dir){
    std::pair<ID,QDir> pair = std::make_pair(dir_id, dir);
    this->dir_map_lock.lock();
    this->dir_map.insert(pair);
    this->dir_map_lock.unlock();
}

