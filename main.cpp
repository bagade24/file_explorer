#include <ctype.h>
#include<iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include<sys/wait.h>
#include<pwd.h>
#include<grp.h>
// includes
//include file 1
#include<string.h>
#include<bits/stdc++.h>
#include<experimental/filesystem>
#include<dirent.h>
#include<unistd.h>
#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
#include <iomanip>
#include <sys/ioctl.h>

using namespace std;
#define CTRL_KEY(k) ((k) & 0x1f)
//defines
int get_ls();
void leftispushed();
void rightispushed();
void setcursor();
void editorRefreshScreen();

// file 1 program

int MAX_SIZE=100;
vector<dirent> directory_list;
vector<char*> details;
vector<string> list_name;
vector<string> owner;
int kxx=0;
vector<int> list_type;
vector<string> visited,right_list;
string home;
void openfile(string file_path,string name);
int listfilesbydirectory(const char* directory);
class details_list{
public:
  string name;
  string permissions;
  string ownership;
  string sizee;
  string lastmodified;
};

vector<details_list> display_list;
class filedetails{
public:
string name;
details_list details;
int type;
filedetails()
{

}
};
vector<filedetails> fd;


string convertToString(char* a, int size)
{
    string s = a;
    return s;
}

void getpresentdirectory(char* cwd)
{
    if (getcwd(cwd, MAX_SIZE) != NULL) {
      return;
    } else {
       perror("getcwd() error");
       return ;
  }
}



void display_vector(vector<dirent> &inp){
  //std::stringstream sstream;
  
  for(auto x:inp)
  {
    
      char cwd[MAX_SIZE];
      getpresentdirectory(cwd);

      int file_size = sizeof(x.d_type) / sizeof(char);
      string fileinput=convertToString(cwd, MAX_SIZE)+"/"+convertToString(x.d_name, file_size);
      openfile(fileinput,convertToString(x.d_name, file_size));
      list_name.push_back(x.d_name);
      list_type.push_back((int)x.d_type);
      
    
  }
  
}

void adding()
{
    fd.clear();
   vector<filedetails> temp1; 
   for(int i=0;i<list_name.size()-1;i++)
   {
    filedetails temp;
    temp.name=list_name[i];
    temp.details=display_list[i];
    
    temp.type=list_type[i];
    temp1.push_back(temp);
   }
   list_name.clear();
   owner.clear();
   display_list.clear();
   list_type.clear();
   fd=temp1;
}

bool fd_comparator(filedetails &i1, filedetails &i2)
{
    return (i1.name < i2.name);
}

int listfilesbydirectory(const char* directory)
{
  details.clear();
  DIR* dir=opendir(directory);
  if(dir==NULL)
  {
    //cout<<"here2"<<endl;
    return 1;
  }
  struct dirent* entity;
  
  entity=readdir(dir);
  while(entity !=NULL)
  {
    directory_list.push_back(*entity);
    entity=readdir(dir);
  }
  //sort(directory_list.begin(),directory_list.end(),directory_comparator);
  display_vector(directory_list);
  directory_list.clear();
  closedir(dir);
  adding();
  sort(fd.begin(),fd.end(),fd_comparator);
  return 0;
}
void printFileProperties(string filename,struct stat stats,string name)
{
    struct tm dt;

    // File permissions
    //cout<<filename<<" <<<here "<<endl;
    details_list temp;
   string s="";
   temp.name = name;
    if (stats.st_mode & S_IRUSR){
      s+="r";
    }
    else{
      s+="-";
    }
    if (stats.st_mode &S_IWUSR){
      s+="w";
    }
    else{
      s+="-";
    }
    if(stats.st_mode & S_IXUSR)
    {
      s+="x";
    }
    else
    {
        s+="-";
    }

    if (stats.st_mode & S_IRGRP){
      s+="r";
    }
    else{
      s+="-";
    }
    if (stats.st_mode &S_IWGRP){
      s+="w";
    }
    else{
      s+="-";
    }
    if(stats.st_mode & S_IXGRP)
    {
      s+="x";
    }
    else
    {
        s+="-";
    }
    if (stats.st_mode & S_IROTH){
      s+="r";
    }
    else{
      s+="-";
    }
    if (stats.st_mode &S_IWOTH){
      s+="w";
    }
    else{
      s+="-";
    }
    if(stats.st_mode & S_IXOTH)
    {
      s+="x";
    }
    else
    {
        s+="-";
    }
    struct passwd *pw = getpwuid(stats.st_uid);
    struct group  *gr = getgrgid(stats.st_gid);
    string a="";
    if(pw != 0){

        a +=convertToString(pw->pw_name,MAX_SIZE);
    }
    a+=" ";
    if(gr!=0){
        a+=convertToString(gr->gr_name,MAX_SIZE);
    }

    temp.ownership=a;
    temp.permissions=s;
    s="";
    unsigned int sizee=(stats.st_size)/2048;
    s=to_string(sizee)+"kB";
    temp.sizee=s;

    dt = *(gmtime(&stats.st_mtime));
    s="";

    s+=to_string(dt.tm_mday)+"/";
    s+=to_string(dt.tm_mon)+"/";
    s+=to_string(dt.tm_year+1900)+" ";
    s+=to_string(dt.tm_hour)+":";
    s+=to_string(dt.tm_min)+":";
    s+=to_string(dt.tm_sec);
    temp.lastmodified=s;

    display_list.push_back(temp);
   

}
void openfile(string file_path,string name)
{

std::ifstream input(file_path);

    if(input)
    {
      struct stat stats;
   char path[file_path.length()+1];
   strcpy(path, file_path.c_str());
   if (stat(path, &stats) == 0)
    {
      //cout<<name<<" ";
        printFileProperties(file_path,stats,name);
        //cout<<endl;
    }
    }
   
   //path=file_path;
}

//////////////////////////////////////// Normal Mode screen handler /////////////////////////////////////////
enum editorKey {
  BACKSPACE = 127,
   ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
};



//
//bufffer
/*** append buffer ***/


//
//
//data//
struct editorConfig {
  int cx, cy;
   int screenrows;
  int screencols;
  struct termios orig_termios;
};
struct editorConfig E;
//terminal

void die(const char *s) {

   write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);
  exit(1);
}

void moveCursor(int x,int y) {
    cout<<"\033["<<x<<";"<<y<<"H";
    fflush(stdout);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);
    struct termios raw = E.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
//
int scrollymin=0,scrollxmin=0;

void editorDrawRows() {
   int y;
   
  for (y = scrollymin;y < E.screenrows + scrollymin - 2 && y < fd.size(); y++) {
      string buf="";
      if(fd[y].details.name.size()>=17)
      {
        buf="..";
      }
      cout<<std::left<<std::setw(20-kxx) <<fd[y].details.name.substr(0,17-kxx)+buf<<std::left<<std::setw(20-kxx) <<fd[y].details.permissions<<std::left<<std::setw(20-kxx)<<fd[y].details.ownership <<std::left<<std::setw(20-kxx) <<fd[y].details.sizee<<std::left<<std::setw(20-kxx) << fd[y].details.lastmodified <<"\r\n";
  }
}

void editorRefreshScreen() {
  cout<<"\033[H\033[2J\033[3J";
  editorDrawRows();
  moveCursor(0,0);
}

//output

int editorReadKey() {
    int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
    if (seq[0] == '[') {
      switch (seq[1]) {
        case 'A': return ARROW_UP;
        case 'B': return ARROW_DOWN;
        case 'C': return ARROW_RIGHT;
        case 'D': return ARROW_LEFT;
      }
    }
    return '\x1b';
  } else {
    return c;
  
  }
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

////

//input//
int t1=0,t2=0;
void windowsizer()
{
getWindowSize(&t1, &t2);
if(t2 <= 100)
{
  kxx= 20 -t2 /5; 
}else if(t2 < 70)
{
  kxx = 7;
}
else{
  kxx=0;
}
if(t1 == E.screenrows && t2 == E.screencols)
{
  return;
}
E.screenrows =t1;
E.screencols =t2;
editorRefreshScreen();
}



void editorMoveCursor(int key) {
   windowsizer();
  switch (key) {
    case ARROW_LEFT:
      if (E.cx >= 0) {
         //E.cx--;
        leftispushed();
      }
      break;
    case ARROW_RIGHT:
      if (E.cx != E.screencols - 1) {
        //E.cx++;
        rightispushed();
      }
      break;
    case ARROW_UP:
      if (E.cy != 0 && scrollymin == 0) {
        E.cy--;
      }
      if(scrollymin != 0)
      {
        scrollymin--;
        editorRefreshScreen();
      }
      break;
    case ARROW_DOWN:
      if (E.cy != E.screenrows - 2 && E.cy < fd.size()) {
        E.cy++;
      }
      if(E.cy == E.screenrows -2)
      {
        if(scrollymin + E.screenrows-2 <= fd.size())
        {
          scrollymin++;
          editorRefreshScreen();
        }
      }
      break;
      


  }
  moveCursor(E.cy,E.cx);
}

//////////////////////////////////////// COMMAND MODE /////////////////////////////////////////
void homeispushed()
{
chdir(home.c_str());
listfilesbydirectory(".");
visited.clear();
visited.push_back(home);
}

void backspace()
{
  
  chdir("..");
  char cwd[MAX_SIZE];
  getpresentdirectory(cwd);
  string entry=string(cwd);
  listfilesbydirectory(".");
  visited.push_back(entry);
}

void enterispushed()
{
  int index=E.cy + scrollymin -1;
  if(index > fd.size())
  {
    return;
  }
  if(fd[index].type != 4) // matlab nahi hai directory
  {
  char *args[3];
  string vim="vi";
  args[0]=(char*)vim.c_str();
  args[1]=(char*)fd[index].name.c_str();
  args[2]=NULL;
  pid_t pid=fork();
  if(pid == 0)
  {
    if(execvp(args[0],args)== -1)
    {
      perror("exec");
    }
    exit(0);
  }
  if(pid>0)
  {
    wait(0);
  }
  return;
}
if(fd[index].type==4)
{
  scrollymin=0;
  scrollxmin=0;
if(fd[index].name==".")
{
  return;
}
if(fd[index].name=="..")
{
  chdir("..");
  char cwd[MAX_SIZE];
  getpresentdirectory(cwd);
  string path= convertToString(cwd,MAX_SIZE);
  visited.push_back(path);
  listfilesbydirectory(".");
  return;
}
char cwd[MAX_SIZE];
getpresentdirectory(cwd);
string path= convertToString(cwd,MAX_SIZE)+"/"+fd[index].name;
visited.push_back(path);
chdir(path.c_str());
listfilesbydirectory(path.c_str());
return;
}
}

void leftispushed()
{
  if(visited.size()<1)
  {
    return;
  }
  string path=visited[visited.size()-1];
  right_list.push_back(path);
  chdir(path.c_str());
  listfilesbydirectory(path.c_str());
  visited.pop_back();
  scrollymin=0;
  editorRefreshScreen();
  //editorDrawRows();
  ////setcursor();
}

void rightispushed()
{
if(right_list.size()==0)
{
  return;
}
  string path=right_list[right_list.size()-1];
  visited.push_back(path);
  chdir(path.c_str());
  listfilesbydirectory(path.c_str());
  right_list.pop_back();
  scrollymin=0;
  editorRefreshScreen();
  //editorDrawRows();
  ///setcursor();
}

void setcursor()
{
  E.cx = 0;
  E.cy = 0;
  moveCursor(0,0);
}
///////////////////////////////////
vector<string> command_component;
string home_dir;
int x=E.cy-1,y=2;
string entered_command="";
string tobesearched="";
/////////////////////////////////////////

int fileordir(string path)
{    
  struct stat sb;

  char curr_path[MAX_SIZE];
  strcpy(curr_path, path.c_str());
  //printf("%s\n",curr_path);
  if (stat(curr_path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    switch (sb.st_mode & S_IFMT) {
    case S_IFDIR:  return 1;
                break;
    case S_IFREG:  return 2;
                    break;
    }
}


/////////////////////////////////////////


string getpwd()
{
char newdirr[MAX_SIZE];
getpresentdirectory(newdirr);
string current=convertToString(newdirr,MAX_SIZE);
return current;
}


void gotodir()
{
  string given_dir=command_component[command_component.size()-1];
  if(given_dir==".")
  {
    return;
  }
  if(given_dir=="..")
  {
    chdir("..");
  }
  if("./"==given_dir.substr(0, 2))
  {
    chdir(given_dir.c_str());
    return;
  }
  if(given_dir[0]=='~'&&given_dir[1]=='/')
  {
    string relativedir = "/home"+given_dir.substr(1, given_dir.size());
    chdir(relativedir.c_str());
    //printf("in the given dir \n");
  }
  else{
    string relativedir="./"+given_dir;
    chdir(relativedir.c_str());
  }
  //printf("in the given dir \n");
}

void returntodir()
{
  chdir(home.c_str());
}

string getdestination()
{
  string destination=command_component[command_component.size()-1];
  if(destination==".")
  {
    return getpwd();
  }

  if(destination[0]=='.'&&destination[1]=='/')
  {
    string relativedir = getpwd()+"/"+destination.substr(1, destination.size());
    return relativedir;
  }

  if(destination[0]=='~'&&destination[1]=='/')
  {
    string relativedir = "/home"+destination.substr(1, destination.size());
    return relativedir;
  }
  else{
    return destination;
  }

}
///////////////////////////////////
void updatePermissions(string s, string d){
    mode_t mode = 0;

    char source[MAX_SIZE],destn[MAX_SIZE];
    strcpy(source, s.c_str());
    strcpy(destn, d.c_str());
    struct stat dest, src;
    if (stat(destn, &dest) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if (stat(source, &src) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    
    if( src.st_mode & S_IRUSR )
        mode |= 0400;
    if( src.st_mode & S_IWUSR )
        mode |= 0200;
    if( src.st_mode & S_IXUSR )
        mode |= 0100;
    if( src.st_mode & S_IRGRP )
        mode |= 0040; 
    if( src.st_mode & S_IWGRP )
        mode |= 0020; 
    if( src.st_mode & S_IXGRP )
        mode |= 0010; 
    if( src.st_mode & S_IROTH )
        mode |= 0004; 
    if( src.st_mode & S_IWOTH )
        mode |= 0002;
    if( src.st_mode & S_IXOTH )
        mode |= 0001;
    chmod(destn, mode);
}
///////////////////////////////////




///////////////////////////////////////////////////////////////
bool search(const char* dirname)
{
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        return false;
    }

    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) {
      if(entity->d_name == tobesearched)
      {
        return true;
      }
        //printf("%hhd %s/%s\n", entity->d_type, dirname, entity->d_name);
        if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            char path[100] = { 0 };
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            bool tst= search(path);
            if(tst)
            {
              return true;
            }
        }
        entity = readdir(dir);
    }

    closedir(dir);
    return false;
}




void move()
{
  
  string prev_path=getpwd();
  string next_path=getdestination();
  //cout<<prev_path +"/"+command_component[1]<<" here  "<<next_path +"/"+command_component[1]<<" \r\n";
  //sleep(2);
  int sized=command_component.size()-2;
  for(int i=1;i<=sized;i++)
  {
  string oldname = prev_path+"/"+command_component[i];
  string newname = next_path+"/"+command_component[i];

  if (rename(oldname.c_str(), newname.c_str()) != 0)
    perror("Error moving file");
  
  }

}

void rename_file()
{
  string oldname = command_component[1];
  string newname =command_component[2];

  if (rename(oldname.c_str(), newname.c_str()) != 0)
    perror("Error rename file");
  else
    cout << "File renamed successfully";

}


void makedir()
{
  gotodir();
  int sized=command_component.size()-2;
  if(sized==0)
  {
    return;
  }
  for(int i=1;i<=sized;i++)
  {
    if (mkdir(command_component[i].c_str(), 0777) == -1)
        cerr << "Error :  " << strerror(errno) << endl;
 
  }
   returntodir();
}


void createfile(int index)
{
  if(command_component.size()<=1)
  {
    return;
  }

  
  fstream file;
    file.open(command_component[index].c_str(),ios::out);
   if(!file)
   {
       cout<<"Error in creating file!!!";
       return;
   }
    cout<<"File created successfully.";

}

void create()
{
  gotodir();
  int sized=command_component.size()-2;
  if(sized==0)
  {
    return;
  }
  while(sized>=1)
  {
    createfile(sized);
    sized--;
  }
  returntodir();

}


void copyfile(string prev_path,string next_path)
{
  
  fstream file;
  string line;
  file.open(next_path.c_str(),ios::out);
  ifstream ini_file {prev_path};
    ofstream out_file {next_path};
 
    if(ini_file && out_file){
 
        while(getline(ini_file,line)){
            out_file << line << "\n";
        }
 
        
 
    } 
    ini_file.close();
    out_file.close();
    updatePermissions(prev_path,next_path);

}


void copyDirectory(string src, char SRC[], string dest, char DEST[]){
  int fd=mkdir(DEST, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP |S_IROTH|S_IXUSR);
  if(fd == -1)
   {
    cout<<"error in creating dir.. "<<DEST<<endl;
    exit(0);
   }
  DIR *directory;
  dirent *fileInDir;
  directory=opendir(SRC);
  if(directory){
    while((fileInDir=readdir(directory))!=NULL){
      string fileName(fileInDir->d_name);
      if(fileName[0]!='.'){ 
        string copyTO=dest+"/"+fileName;
        string copyFROM=src+"/"+fileName;
        char newSrc[200], newDest[200];
        strcpy(newSrc, copyFROM.c_str() );
        strcpy(newDest, copyTO.c_str() );
        //printf("%s aa\n",newSrc);
        //printf("%s bb\n",newDest);
        if(fileordir(copyFROM)==2){ //file
          copyfile(copyFROM, copyTO);
        }
        else{
          copyTO+='/';
          copyFROM+='/';
          strcpy(newSrc, copyFROM.c_str() );
          strcpy(newDest, copyTO.c_str() );
          copyDirectory(copyFROM, newSrc, copyTO, newDest);
        }
      }
    }
    updatePermissions(src,dest);
    closedir(directory);
  }
}



void copy()
{
   int sized=command_component.size()-2;
   for(int i=1;i<=sized;i++)
  {

  string prev_path=getpwd()+"/"+command_component[i];
  string next_path=getdestination()+"/"+command_component[i];
  if(fileordir(prev_path)==2)
  {
    copyfile(prev_path,next_path);
  }
  else{
    char SRC[MAX_SIZE], DEST[MAX_SIZE];
    strcpy(SRC, prev_path.c_str());
    strcpy(DEST, next_path.c_str());
    copyDirectory(prev_path, SRC, next_path, DEST);
  }

  }
 
}
///////////////////////////////////////

bool deletedir(const char* dirname)
{
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        return false;
    }

    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) {
      
        //printf("%hhd %s/%s\n", entity->d_type, dirname, entity->d_name);
        if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            char path[100] = { 0 };
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            deletedir(path);
            
        }
        else{
          string s=string(dirname) + "/" +string(entity->d_name);
          remove(s.c_str());
        }
        entity = readdir(dir);
    }

    rmdir(dirname);
    return false;
}





void deletefunc()
{
   int sized=command_component.size()-2;
    
  string prev_path=command_component[1];
  //cout<<prev_path<<"\r\n";
  if(fileordir(prev_path)==2)
  {
    remove(prev_path.c_str());
    return;
  }
  else{
    deletedir(prev_path.c_str());
  }
 
}








///////////////////////////
void stripincomponents()
{
  command_component.clear();
  int prev=0;
  for(int i=0;i<entered_command.size();i++)
  {
    if(entered_command[i]==' '||i==entered_command.size()-1)
    {
      if(entered_command[i]!=' ')
      {
        string s=entered_command.substr(prev,i-prev+1);
        prev=i+1;
        command_component.push_back(s);
      }
      else{
        string s=entered_command.substr(prev,i-prev);
        prev=i+1;
        command_component.push_back(s);
      }
      
    }
  }
}

void clearscreen()
{
  listfilesbydirectory(".");
  editorRefreshScreen();
  moveCursor(E.screenrows-1,0);
  entered_command.clear();
  command_component.clear();
  x=E.screenrows-1;
  y=2;
  string str=":";
  write(STDOUT_FILENO, str.c_str(), 1);
}

void commandisentered()
{

  home=getpwd();
  sleep(3);
  stripincomponents();
  if(command_component[0]=="goto")
  {
    gotodir();
    home=getpwd();
  }
  if(command_component[0]=="search")
  {
    tobesearched=command_component[1];
    string dirr=getpwd();
    bool tst=search(".");
    int n = tst ? printf("True") : printf("False");
  }
  if(command_component[0]=="create_file")
  {
    create();
  }
  if(command_component[0]=="create_dir")
  {
    makedir();
  }
  if(command_component[0]=="copy")
  {
    copy();
  }
  if(command_component[0]=="move")
  {
    move();
  }
  if(command_component[0]=="delete")
  {
    deletefunc();
  }
  entered_command.clear();
  tobesearched.clear();
  sleep(1);
  clearscreen();
}
////////////////////////////

///////////////////////////

char readkey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}



void keypress(int &flag) {
  char c = readkey();
 
  switch (c) {
    case 'q':
      if(entered_command.size()==0)
      exit(0);
      break;
      break;
    case '\r':
        commandisentered();
      break;
    case 27:
      flag=1;
      break;  
  }
  if(c==127 && entered_command.size()>0)
  {
    string temp="\b\033[J";
    printf("%s\n", temp.c_str());
    y=y-1;
    entered_command.pop_back();
    moveCursor(x,y);
  }
   if (c == '\x1b') {

   flag=0;
   while(entered_command.size() != 0)
   {
    string temp="\b\033[J";
    printf("%s\n", temp.c_str());
    y=y-1;
    entered_command.pop_back();
    moveCursor(x,y);
   }
   string temp="\b\033[J";
    printf("%s\n", temp.c_str());
    y=y-1;
   moveCursor(0,0);
   return; 
  }
  if(c != 127 && c != '\r')
  {
  printf("%c\n",c);
  moveCursor(x,++y);
  entered_command.push_back(c);
  //cout<<c<<"is pushed \r\n";
  }
}





void commandmode()
{

moveCursor(E.screenrows-1,0);
x=E.screenrows-1;
string str=":";
y=2;
write(STDOUT_FILENO, str.c_str(), 1);
int flag=1;
 while (flag) {
    keypress(flag);
  }

}

//////////////////////////////////////// key press /////////////////////////////////////////
void editorProcessKeypress() {
   windowsizer();
   int c = editorReadKey();
  switch (c) {
    case 'q':
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
      case '\r':
      cout<<E.cy+scrollymin<<endl;
      enterispushed(); 
      editorRefreshScreen();
      setcursor();
      break;
      case 'h':
      homeispushed();
      editorRefreshScreen();
      setcursor();
      break;
      case BACKSPACE:
      backspace();
      editorRefreshScreen();
      setcursor();
      break;
    case ARROW_UP:
              editorMoveCursor(c);
               break;
    case ARROW_DOWN:
              editorMoveCursor(c);
               break;
    case ARROW_LEFT:
              //editorMoveCursor(c);
               leftispushed();
               break;
    case ARROW_RIGHT:
              //editorMoveCursor(c);
              rightispushed();
               break;
    case ':':
            commandmode();
            break;
    case 's':
              moveCursor(0,0);
              break;
     case 'k':
            if(scrollymin != 0)
            {
             scrollymin=scrollymin-E.screenrows;
             if(scrollymin <=0)
             {
               scrollymin = 0;
             }
              editorRefreshScreen();
             }
              break;

      case 'l':
            if(scrollymin != fd.size()-E.screenrows)
              {
                scrollymin=scrollymin+E.screenrows;
                if(scrollymin >= fd.size()-E.screenrows)
                {
                scrollymin=scrollymin+E.screenrows;
                }
              editorRefreshScreen();
            }
              break;         

}

}
//////////////////////////////////////// Init functions /////////////////////////////////////////
void sethomedirectory()
{
  char cwd[MAX_SIZE];
  getpresentdirectory(cwd);
  home=convertToString(cwd,MAX_SIZE);
  visited.push_back(home);
  visited.push_back(home);
}

void initEditor() {
  E.cx = 0;
  E.cy = 0;
  listfilesbydirectory(".");
  moveCursor(0,0);
  int wsx,wsy;
  getWindowSize(&wsx,&wsy);
  sethomedirectory();
  E.screenrows=wsx;
  E.screencols=wsy;
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}


int get_ls(){
   char cwd[MAX_SIZE];
 // cout << "\033[2J\033[1;1H";
  getpresentdirectory(cwd);
  listfilesbydirectory(cwd);
  return 0;
}
//////////////////////////////////////// SIG HANDLER /////////////////////////////////////////

static void sig_handler(int sig)
{

  if (SIGWINCH == sig) {
    cout <<"\033[2J\033[1;1H";
    windowsizer();
  }

} 


//////////////////////////////////////// MAIN /////////////////////////////////////////
int main() {
 enableRawMode();
 initEditor();
 signal(SIGWINCH, sig_handler);
 editorRefreshScreen();
  while (1) {
      //windowsizer();
     editorProcessKeypress();
  }
  return 0;
}
