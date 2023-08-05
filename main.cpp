
#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <termios.h> 
#include <sys/ioctl.h>
#include <unistd.h> 
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <cstdio>
using namespace std;



bool CheckPath(string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}
int minim(int a,int b){
    if(a<b){
        return a;
    }
    else{
        return b;
    }
}
void gotoxy(int x, int y){
    printf("%c[%d;%df",0x1B,y,x);
}


void titlebar(int wincols,int limit){
    int x=10,y=1;
    int xtemp = wincols/7;
    if(limit == 5){
        gotoxy(x,0);
        cout<<"Name"<<endl;
        gotoxy(x+xtemp,y);
        cout<<"Size"<<endl;
        gotoxy(x+(xtemp*2),y);
        cout<<"Owner";
        gotoxy(x+(xtemp*3),y);
        cout<<"Group";
        gotoxy(x+(xtemp*4),y);
        cout<<"Permissions";
        gotoxy(x+(xtemp*5),y);
        cout<<"Last Modified";
    }
    else if(limit==4){
        gotoxy(x,0);
    cout<<"Name"<<endl;
    gotoxy(x+xtemp,y);
    cout<<"Size"<<endl;
    gotoxy(x+(xtemp*2),y);
    cout<<"Owner";
    gotoxy(x+(xtemp*3),y);
    cout<<"Group";
    gotoxy(x+(xtemp*4),y);
    cout<<"Permissions";
    }
}
int getch(void) 
{ 
    struct termios oldattr, newattr; 
    int ch; 
    tcgetattr(STDIN_FILENO, &oldattr); 
    newattr = oldattr; 
    newattr.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); 
    ch = getchar(); 
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); 
    return ch; 
} 

void removespace(string s, vector<string> &words)
{
    string x = "";
    for (int i=0;i<s.size();i++)
    {
        if (s[i] != ' ')
        {
            x = x + s[i];
        }
        else {
            words.push_back(x);
            x = "";
        }
    }
    words.push_back(x);
    return;
}

int createdirectory(vector<string>&words){
    string filepath = words[2];
    filepath = filepath + '/';
    filepath = filepath + words[1];
    char *path = new char[filepath.length() + 1];
    strcpy(path, filepath.c_str());
    return mkdir(path, 0777); 
}

int createfile(vector<string>&words){
    ofstream file;
    int f;
    string filepath = words[2];
    filepath = filepath + '/';
    filepath = filepath + words[1];
    char *path = new char[filepath.length() + 1];
    strcpy(path, filepath.c_str());
    file.open(path);
    if(!file.is_open())
    {
        f = 0;
    } 
    else{
        f = 1;    
    }
    file.close();
    return f;
}

int make_vector(string s,vector<vector<string>>&deets,vector<string>&buff){ //return -1 if path is wrong
        char *path = new char[s.length() + 1];
        strcpy(path, s.c_str());
        DIR* dir = opendir(path);
        if(dir == NULL){
            return -1;
        }
        string temp="";
        char mtime[100];
        char atime[100];
        struct stat sb;
        struct dirent *entity;
        entity=readdir(dir);
        buff.clear();
        deets.clear();
        while(entity!=NULL){
            //cout<<path;
            temp = path;
            temp = temp + '/';
            temp = temp + entity->d_name;
            char *temp2 = new char[temp.length() + 1];
            strcpy(temp2, temp.c_str());
            stat(temp2,&sb);

            strcpy(mtime, ctime(&sb.st_mtime));
            buff.push_back(entity->d_name); //file or directory name
            buff.push_back(to_string(sb.st_size)); //file or directory size

            struct passwd *p = getpwuid(sb.st_uid);
            struct group *g = getgrgid(sb.st_gid);
            buff.push_back(p->pw_name);
            buff.push_back(g->gr_name);

            string per="";
            if(S_ISDIR(sb.st_mode)){ //checking if directory or not and pushing in string
                per = per + 'd';
            }
            else{
                per = per+ '-';
            }
            sb.st_mode & S_IRUSR ? per=per+'r' : per=per+'-';
            sb.st_mode & S_IWUSR ? per=per+'w' : per=per+'-';
            sb.st_mode & S_IXUSR ? per=per+'x' : per=per+'-';
            sb.st_mode & S_IRGRP ? per=per+'r' : per=per+'-';
            sb.st_mode & S_IWGRP ? per=per+'w' : per=per+'-';
            sb.st_mode & S_IXGRP ? per=per+'x' : per=per+'-';
            sb.st_mode & S_IROTH ? per=per+'r' : per=per+'-';
            sb.st_mode & S_IWOTH ? per=per+'w' : per=per+'-';
            sb.st_mode & S_IXOTH ? per=per+'x' : per=per+'-';
            buff.push_back(per); //pushing permission string
            buff.push_back(mtime); //pushing last modified time
            deets.push_back(buff);
            buff.clear();
            entity=readdir(dir);
        }
        closedir(dir);
        return 0; //vector successful
}

void display(vector<vector<string>>&deets,int wincols){
        int x=10,y=2,xtemp=wincols/7;
        titlebar(wincols,5);
        sort(deets.begin(), deets.end());
        int show = minim(10,deets.size());
        for(int i=0;i<show;i++){
                gotoxy(x,y);
                if(deets[i][0].size() <=20){
                    cout<<deets[i][0]; //file name
                }
                else{
                    string nametemp = deets[i][0].substr(0, 20);
                    nametemp = nametemp + "...";
                    cout<<nametemp; 
                }
                gotoxy(x+xtemp,y);
                cout<<deets[i][1]; //file size
                gotoxy(x+(xtemp*2),y);
                cout<<deets[i][2]; //user
                gotoxy(x+(xtemp*3),y);
                cout<<deets[i][3]; //group
                gotoxy(x+(xtemp*4),y);
                cout<<deets[i][4]; //permission string
                gotoxy(x+(xtemp*5),y);
                cout<<deets[i][5]; //last modified
                y+=1;
            }
        if(deets.size()>5){
            gotoxy(x,y);
            cout<<".";
            gotoxy(x,y+1);
            cout<<".";
            gotoxy(x,y+2);
            cout<<".";
            gotoxy(x,y+3);
            cout<<"Total records : "<<deets.size();
        }
}
//static int iop = 1;
int searchfile(string s, string gotos){
    //iop++;
    //gotoxy(20,iop);
    //cout<<gotos<<endl;
    vector<vector<string>>deets;
    vector<string>buff;
    string temp = "";
    int x;
    int vecstatus = make_vector(gotos,deets,buff);
    if(vecstatus == -1){
        return -1; //path was invalid
    }
    for(int i=0;i<deets.size();i++){
        if(deets[i][0]=="." || deets[i][0] == ".."){
            continue;
        }
        if(deets[i][0]==s){
            if(deets[i][4][0]=='d')
                return 1; //directory found
            else
                return 2; //file found
        }
        if(deets[i][4][0]=='d'){
            temp = gotos;
            temp = temp + '/';
            temp = temp + deets[i][0];
            //iop++;
            //gotoxy(20,iop);
            //cout<<temp<<endl;
            x = searchfile(s,temp);
            if(x==1 || x==2){
                return x;
            }
        }
    }
    return 0; //file not found
}

int renamefile(string oldn1, string newn1, string gotos){
    char *oldn = new char[oldn1.length() + 1];
    strcpy(oldn, oldn1.c_str());
    char *newn = new char[newn1.length() + 1];
    strcpy(newn, newn1.c_str());
    char *newpath = new char[gotos.length() + 1];
    strcpy(newpath, gotos.c_str());
    vector<vector<string>>deets;
    vector<string>buff;
    int x=1;
    int vecstatus = make_vector(gotos,deets,buff);
    if(vecstatus == -1){
        return -1; //path was invalid
    }
    for(int i=0;i<deets.size();i++){
        if(deets[i][0]==oldn1){
            chdir(newpath);
            x = rename(oldn, newn);
            break;
        }
    }
    if (x == 0)
		        return 1;
	else
		return 0; //file not found
}

int movefile(string dest1, string gotos, vector<string>&files,vector<string> &notfoundfiles){
    string d; 
    char *newpath = new char[gotos.length() + 1];
    strcpy(newpath, gotos.c_str());
    vector<vector<string>>deets;
    vector<string>buff;
    int x=1;
    int vecstatus = make_vector(gotos,deets,buff);
    if(vecstatus == -1){
        return -1; //path was invalid
    }
    int flag=0;
    for(int j=0;j<files.size();j++){
        for(int i=0;i<deets.size();i++){
        if(deets[i][0]==files[j]){
            chdir(newpath);
            char *oldn = new char[files[j].length() + 1];
            strcpy(oldn, files[j].c_str());
            d = dest1;
            d = d + '/';
            d = d + files[j];
            char *dest = new char[d.length() + 1];
            strcpy(dest, d.c_str());
            x = rename(oldn, dest);
            flag=1;
            break;
            }
        }
        if(flag==0){
            notfoundfiles.push_back(files[j]);
        }
        flag=0;
    }
    return 1;
} 

void copy_file(string src, string des)
{
	int inp, outp, n, status;
	char blk[1024];
	struct stat srcstat, desstat;
	inp = open(src.c_str(), O_RDONLY);
	outp = open(des.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

	while ((n = read(inp, blk, sizeof(blk))) > 0)
	{
		write(outp, blk, n);
	}
}

void copy_dir(string src, string des)
{
    mkdir(des.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	DIR *direc;
	struct dirent *dir;
	direc = opendir(src.c_str());
	if (direc)
	{
		while ((dir = readdir(direc)) != NULL)
		{
			if ((string(dir->d_name) == "..") || (string(dir->d_name) == "."))
			{
                continue;
			}
			else
			{
				string f = src + "/" + string(dir->d_name);
				string d = des + "/" + string(dir->d_name);

				struct stat sb;
				if (stat(f.c_str(), &sb) == -1)
				{
					perror("error in file");
				}
				else
				{

					if ((S_ISDIR(sb.st_mode)))
					{
						copy_dir(f, d);
					}
					else
					{
						copy_file(f, d);
					}
				}
			}
		}
	}
}

int deletefile(string dest){
    int status;

    char *path = new char[dest.length() + 1];
    strcpy(path, dest.c_str());

    status = remove(path);
    //status = 1;
    //gotoxy(20,20);
    //cout<<"deleting "<<dest;
    if(status!=0){
        return 0;
    }
    else return 1;
}

void deletedir(string des)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(des.c_str());
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if ((string(dir->d_name) == "..") || (string(dir->d_name) == "."))
			{
                continue;
			}
			else
			{
				string filepath = des;
                filepath = filepath + "/";
                filepath = filepath + string(dir->d_name);

				struct stat sb;
				if (stat(filepath.c_str(), &sb) == -1)
				{
					perror("lstat");
				}
				else
				{

					if ((S_ISDIR(sb.st_mode)))
					{
						deletedir(filepath);
					}
					else
					{
						deletefile(filepath);
					}
				}
			}
		}
		closedir(d);
		int status = rmdir(des.c_str());
		if (status == -1)
		{
			gotoxy(10,20);
            cout<<"Error in deleting Directory"<<endl;// showError("Error in removing the Directory with path ::::: " + string(path));
            sleep(2);
		}
        else{
            gotoxy(10,20);
            cout<<"Deleted Directory"<<endl;
            sleep(2);
        }
	}
}

void copystuff(string dest, string gotos, vector<string>&files)
{
    string filepath1;
    string newdirpath1="";
    if(CheckPath(dest)==0){
        gotoxy(25,19);
        cout<<dest<<" Does not Exist"<<endl;
        sleep(2);
        return;
    }
		for (int i = 0; i < files.size(); i++)
		{
            filepath1 = gotos;
            filepath1 = filepath1 + '/';
            filepath1 = filepath1 + files[i];
            char *filepath = new char[filepath1.length() + 1];
            strcpy(filepath, filepath1.c_str());

            newdirpath1 = dest + '/';
            newdirpath1 = newdirpath1 + files[i];
            char *newdirpath = new char[newdirpath1.length() + 1];
            strcpy(newdirpath, newdirpath1.c_str());
            if(CheckPath(filepath1)==0){
                gotoxy(25,i+20);
                cout<<filepath1<<" Does not Exist"<<endl;
                sleep(2);
                continue;
            }
			struct stat sbnew;
            stat(filepath, &sbnew);
				if ((S_ISDIR(sbnew.st_mode)))
				{
                    mkdir(newdirpath, 0777); 
                    copy_dir(filepath1, newdirpath1);
                    gotoxy(25,i+20);
                    cout<<"Directory "<<files[i]<<" Copied Successfully"<<endl;
                    sleep(2);
                }
				else
				{
                    copy_file(filepath1, newdirpath1);
                    gotoxy(25,i+20);
                    cout<<"File "<<files[i]<<" Copied Successfully"<<endl;
                    sleep(2);
				}
			//}
		}
	//}
}
void getcoord(int &x, int &y){
    #ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    x = ts.ts_cols;
    y = ts.ts_lines;
    #elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts); 
    #endif /* TIOCGSIZE */
    x = ts.ws_col;
    y = ts.ws_row;
}

int main(){

    system("clear");
    int command = 0;
    //struct winsize w;
    //ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int cols;
    int rows;

    getcoord(cols,rows);
    //printf ("lines %d\n", w.ws_row);
    //printf ("columns %d\n", w.ws_col);
    vector<string>st;
    vector<string>stright;
    string str = "/home";
    st.push_back(str);
    str  = getenv("PWD");
    string homedir = getenv("HOME");
    //str = "/home/anurag";
    st.push_back(str);
    int flag = 0;
    string gotos = homedir; //changed str to homedir
    while(1){
        //int winrows = w.ws_row;
        //int wincols = w.ws_col;
        getcoord(cols, rows);
        int winrows = rows;
        int wincols = cols;
        //gotoxy(20,30);
        //cout<<"First loop wr:"<<winrows<<" wc:"<<wincols<<endl;
        if(command == 0){
            //winrows = w.ws_row;
            //wincols = w.ws_col;
            getcoord(cols, rows);
            winrows = rows;
            wincols = cols;
        if(st.size()==0){
            str = "/home";
            st.push_back(str);
        }
        str = st.back();
        gotos = str;
        char *path = new char[str.length() + 1];
        strcpy(path, str.c_str());
        DIR* dir = opendir(path);
        if(dir == NULL){
            return -1;
        }
        string temp="";
        char mtime[100];
        char atime[100];
        struct stat sb;
        struct dirent *entity;
        entity=readdir(dir);
        vector<vector<string>>deets;
        vector<string>buff;
    
        buff.clear();
        deets.clear();
        while(entity!=NULL){
            //cout<<path;
            temp = path;
            temp = temp + '/';
            temp = temp + entity->d_name;
            char *temp2 = new char[temp.length() + 1];
            strcpy(temp2, temp.c_str());
            stat(temp2,&sb);

            strcpy(mtime, ctime(&sb.st_mtime));
            buff.push_back(entity->d_name); //file or directory name
            int sz = sb.st_size;
            string unit = "B";
            if(sz>=1024){ //B to KB
                sz=sz/1024;
                unit = "KB";
            }
            if(sz>=1024){ //KB to MB
                sz=sz/1024;
                unit = "MB";
            }
            if(sz>=1024){ //MB to GB
                sz=sz/1024;
                unit = "GB";
            }
            string finalsz = to_string(sz) + " " + unit;
            buff.push_back(finalsz); //file or directory size

            struct passwd *p = getpwuid(sb.st_uid);
            struct group *g = getgrgid(sb.st_gid);
            buff.push_back(p->pw_name);
            buff.push_back(g->gr_name);

            string per="";
            if(S_ISDIR(sb.st_mode)){ //checking if directory or not and pushing in string
                per = per + 'd';
            }
            else{
                per = per+ '-';
            }
            sb.st_mode & S_IRUSR ? per=per+'r' : per=per+'-';
            sb.st_mode & S_IWUSR ? per=per+'w' : per=per+'-';
            sb.st_mode & S_IXUSR ? per=per+'x' : per=per+'-';
            sb.st_mode & S_IRGRP ? per=per+'r' : per=per+'-';
            sb.st_mode & S_IWGRP ? per=per+'w' : per=per+'-';
            sb.st_mode & S_IXGRP ? per=per+'x' : per=per+'-';
            sb.st_mode & S_IROTH ? per=per+'r' : per=per+'-';
            sb.st_mode & S_IWOTH ? per=per+'w' : per=per+'-';
            sb.st_mode & S_IXOTH ? per=per+'x' : per=per+'-';
            buff.push_back(per); //pushing permission string
            buff.push_back(mtime); //pushing last modified time
            deets.push_back(buff);
            buff.clear();
            entity=readdir(dir);
        }
        closedir(dir);
        int x=10,y=2;
        char a;
        int k=0;
        int start=0,end= minim(deets.size()-1,10);
    /*
    do  //identify ascii of character
    {
        a = getch();
        printf("a=%d\n", a);
        cout<<"\n";
    }
    while (1);
    */
        
        int xtemp,ytemp;
        while(1){ //while loop for printing stuff on screen
            //winrows = w.ws_row;
            //wincols = w.ws_col;
            getcoord(cols, rows);
            winrows = rows;
            wincols = cols;
            if(wincols<108){
                titlebar(wincols,4);
            }
            else{
                titlebar(wincols,5);
            }
            //titlebar(wincols);
            sort(deets.begin(), deets.end());
            if(k==start-1){
                    start--;
                    end--;
                }
                if(k==end+1){
                        start++;
                        end++;
                }
            for(int i=start;i<=end;i++){
                if(wincols<108){
                    if(k==i){
                    gotoxy(0,y);
                    cout<<"---->";    
                }
                gotoxy(x,y);
                if(deets[i][0].size() <=20){
                    cout<<deets[i][0]; //file name
                }
                else{
                    string nametemp = deets[i][0].substr(0, 20);
                    nametemp = nametemp + "...";
                    cout<<nametemp; 
                }
                xtemp = wincols/7;
                gotoxy(x+xtemp,y);
                cout<<deets[i][1]; //file size
                gotoxy(x+(xtemp*2),y);
                cout<<deets[i][2]; //user
                gotoxy(x+(xtemp*3),y);
                cout<<deets[i][3]; //group
                gotoxy(x+(xtemp*4),y);
                cout<<deets[i][4]; //permission string
                y+=1;
                gotoxy(x,14);
                cout<<"Total files/directories : "<<k+1<<"/"<<deets.size();    
                }
                else{

                if(k==i){
                    gotoxy(0,y);
                    cout<<"---->";    
                }
                gotoxy(x,y);
                if(deets[i][0].size() <=20){
                    cout<<deets[i][0]; //file name
                }
                else{
                    string nametemp = deets[i][0].substr(0, 20);
                    nametemp = nametemp + "...";
                    cout<<nametemp; 
                }
                xtemp = wincols/7;
                gotoxy(x+xtemp,y);
                cout<<deets[i][1]; //file size
                gotoxy(x+(xtemp*2),y);
                cout<<deets[i][2]; //user
                gotoxy(x+(xtemp*3),y);
                cout<<deets[i][3]; //group
                gotoxy(x+(xtemp*4),y);
                cout<<deets[i][4]; //permission string
                gotoxy(x+(xtemp*5),y);
                cout<<deets[i][5]; //last modified
                y+=1;
                gotoxy(x,14);
                cout<<"Total files/directories : "<<k+1<<"/"<<deets.size();
                }
            } 
            //display path below
            gotoxy(5,winrows-5);
            cout<<"PATH - "<<str<<"   (NORMAL MODE) Press ':' to enter command mode";
            a = getch();
            if ((a == 'q')) // stop if EOF or 'q'.
                {
                    flag=1;
                    break;
                }
            if ( a =='h' ){ //press h for /home/user_name 
                
                //string strh = "/home/anurag";
                //st.push_back(strh);
                st.push_back(homedir);
                cout << "\033[2J\033[1;1H";
                break;
            } 
            if (a==27){
                //system("clear");
                cout << "\033[2J\033[1;1H"; //clear screen
                continue;
            }    
            if (a==91){
                //system("clear");
                cout << "\033[2J\033[1;1H"; //clear screen
                continue;
            }
            if (a==127){ //Backspace key
                //system("clear");
                cout << "\033[2J\033[1;1H"; //clear screen
                stright.push_back(str);
                if(str == "/home"){
                    st.push_back("/");
                    cout << "\033[2J\033[1;1H";
                    break;    
                }
                if(str == "/"){
                    cout << "\033[2J\033[1;1H";
                    y=2;
                    continue;
                }
                size_t pos = str.find_last_of('/');
                string strback = str.substr(0,pos);
                st.push_back(strback);
                break;
            }     
            if (a==65){ //up arrow
                //system("clear");
                cout << "\033[2J\033[1;1H"; //clear screen
                if(k==0){
                    k=0;
                }
                else
                    k--;
                y=2;
                continue;
            }   
            if (a==66){ //down arrow
                //system("clear");
                cout << "\033[2J\033[1;1H"; //clear screen
                if(k!=deets.size()-1){
                    k++;
                    
                }
                y=2;
                continue;
            }  
            if (a==67){ //right arrow
                //system("clear");
                if(stright.size()==0){
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                cout << "\033[2J\033[1;1H"; //clear screen
                string rightst = stright.back();
                stright.pop_back();
                st.push_back(rightst);
                break;
                
            }
            if (a==10){  //enter key (goes inside the directory)
                //system("clear");
                cout << "\033[2J\033[1;1H"; //clear screen
                if(deets[k][0]=="."){
                    break;
                }
                else if(deets[k][0]==".."){
                    cout << "\033[2J\033[1;1H"; //clear screen
                    stright.push_back(str);
                    size_t pos = str.find_last_of('/');
                    string strback = str.substr(0,pos);
                    st.push_back(strback);
                    break;
                }
                else if(deets[k][4][0] == 'd'){
                    if(str != "/"){
                        str = str + '/';
                    }
                    str = str + deets[k][0];
                    st.push_back(str);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                else{
                    //system("clear");
                    cout << "\033[2J\033[1;1H"; //clear screen
                    if(fork()==0){
                        string tempstr = str + '/';
                        tempstr = tempstr + deets[k][0];
                        char filepath[tempstr.length()+1];
                        snprintf(filepath,sizeof(filepath),"%s",tempstr.c_str());
                        execl("/usr/bin/xdg-open","xdg-open",filepath,NULL);
                        exit(1);
                    }
                }
        }   
            if (a==58){ //command mode flag by pressing :
                command = 1;
                cout << "\033[2J\033[1;1H";
                break;
            } 
            if (a==68){  //left key (goes to previous directory)
                //system("clear");
                cout << "\033[2J\033[1;1H"; //clear screen
                stright.push_back(str);
                st.pop_back();
                break;
            }
            else{
                //system("clear");
                cout << "\033[2J\033[1;1H"; //clear screen
                y=2;
                continue;
            }
            
        } 
        if(flag==1){
        break;
        }
    }//endif for command = 0
    else{ //command mode
        while(1){

        while(1){ 
            // cout << "\033[2J\033[1;1H";
            getcoord(cols, rows);
            winrows = rows;
            wincols = cols;
            string comstr="";
            vector<string> words;
            char a;
            int inc = 7;
            int pathflag = 0;
            //string check = "";
            while(1){ //taking text input
                gotoxy(5,winrows-5); //printing the current path
                cout<<"PATH - "<<gotos<<endl;
                gotoxy(5,winrows-4);
                cout<<"(COMMAND MODE) Press 'Esc' to enter Normal mode";
                gotoxy(5,winrows-10);
                cout<<"Enter Commands : (Press 'Esc' for NORMAL MODE OR Enter 'quit' to exit)";
                gotoxy(5,winrows-8);
                cout<<"$ ";
                a = getch();
                //if (a == 27) // Esc to go to normal mode.
                //check.push_back(a);
                if (a == 27) // Esc to go to normal mode.
                {
                    command = 0;
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                if (a==127) // Backspace to Erase.
                {
                    if(comstr == ""){
                        continue;
                    }
                    comstr.pop_back();
                    cout << "\033[2J\033[1;1H";
                    inc--;
                    gotoxy(7,winrows-8);
                    cout<<comstr;
                    continue;
                }
                if (a=='\n'){
                    break;
                }
                gotoxy(inc,winrows-8);
                cout<<a;
                comstr = comstr+a;
                inc++;
            }
            if(command==0){
                break;
            }
            //getline(cin,comstr);
            //cout<<"\n"<<comstr;
            if(comstr == "quit"){
                flag=1;
                break;
            }
            removespace(comstr,words);
            if(words[words.size()-1][0] == '~'){ //relative path tild
                words[words.size()-1] = homedir + words[words.size()-1].substr(1,words[words.size()-1].size()-1); 
            }

            if(words[words.size()-1][0] == '.' && words[words.size()-1][1] == '.'){ //relative path ..
                string path = words[words.size()-1];
                string append = gotos;
                size_t posi;
                while(path[0] == '.' && path[1] == '.'){
                    if(append == ""){
                        pathflag = 1;
                        break;    
                    }
                    posi = append.find_last_of('/');
                    append = append.substr(0,posi);
                    path = path.substr(3,words[words.size()-1].size()-1);
                }
                if(pathflag==1){
                    break;
                }
                string ch = append + '/' + path;
                if(CheckPath(ch)==0){
                    gotoxy(15,5);
                    cout << "Path Entered is invalid"<<endl;
                    sleep(2);
                    pathflag=1;
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                words[words.size()-1] = append + '/' + path; 
            }
            if(pathflag==1){ 
                gotoxy(10,20);
                cout<<"Invalid path"<<endl;
                cout << "\033[2J\033[1;1H";
                break;
            }
            if(words[words.size()-1][0] == '.' && words[words.size()-1][1] != '.'){ //relative path . 
                string path = words[words.size()-1];
                string append = gotos;
                size_t posi;
                while(path[0] == '.'){
                    path = path.substr(2,words[words.size()-1].size()-1);
                }
                words[words.size()-1] = append + '/' + path; 
            }

            //gotoxy(15,30);
            //cout<<words[words.size()-1]<<endl;
            //sleep(3);
            if(words[0]=="create_dir"){ //creating a directory
                
                if(words.size()<3){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                if(CheckPath(words[words.size()-1])==0){
                    gotoxy(15,5);
                    cout << "Path Entered is invalid"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                int res = createdirectory(words);
                if (res != -1){
                    gotoxy(15,5);
                    cout << "Directory created"<<endl;
                    sleep(2);
                }
                else{
                    gotoxy(15,5);
                    cerr << "Error :  " << strerror(errno) << endl;
                    sleep(2);
                }
                cout << "\033[2J\033[1;1H";
                break;
                }

            if(words[0]=="create_file"){ //creating a file
                
                if(words.size()<3){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                if(CheckPath(words[words.size()-1])==0){
                    gotoxy(15,5);
                    cout << "Path Entered is invalid"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                int f = createfile(words); 
                if(f==0)
                {
                    gotoxy(15,5);   
                    cout << "Could not create the File" <<endl;
                    sleep(2);
                } 
                else{
                    gotoxy(15,5);
                    cout << "File created successfully"<<endl;
                    sleep(2);    
                }
                cout << "\033[2J\033[1;1H";
                break;
            }

            if(words[0] == "goto"){ //goto functionality
                if(words.size()<2){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                string s = words[1];
                gotos = s;
                vector<vector<string>>deets;
                vector<string>buff;
                int vecstatus = make_vector(s,deets,buff);
                cout << "\033[2J\033[1;1H";
                if(vecstatus == -1){
                    gotoxy(15,5);
                    cout << "Failed to go to "<<s<<endl;
                    sleep(2);    
                    cout << "\033[2J\033[1;1H";
                }
                else{
                    //gotoxy(5,0);
                    display(deets,wincols);
                }
                break;
            }
            if(words[0] == "search"){ //search functionality

                if(words.size()<2){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                string s = words[1];
                int ser = searchfile(s,gotos);
                if(ser==-1){
                    gotoxy(5,winrows-7);
                    cout << "Failed to search "<<endl;
                    sleep(2); 
                }
                else if (ser == 0 ){
                    gotoxy(5,winrows-7);
                    cout << "File/Directory Not Found in Path : "<<gotos<<endl;
                    sleep(2); 
                }
                else if (ser == 1){
                    gotoxy(5,winrows-7);
                    cout << "Directory "<<s<<" Found"<<endl;
                    sleep(2); 
                }
                else{
                    gotoxy(5,winrows-7);
                    cout << "File "<<s<<" Found"<<endl;
                    sleep(2);
                }
                cout << "\033[2J\033[1;1H";
                break;
            }
            if(words[0] == "rename"){ //rename functionality

                if(words.size()<3){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                string oldn = words[1];
                string newn = words[2];
                int ren = renamefile(oldn,newn,gotos);
                if(ren==-1){
                    gotoxy(5,winrows-7);
                    cout << "Failed to search "<<endl;
                    sleep(2); 
                }
                else if (ren == 0 ){
                    gotoxy(5,winrows-7);
                    cout << "FileNot Found in Path : "<<gotos<<"  Rename Failed"<<endl;
                    sleep(2); 
                }
                else if (ren == 1){
                    gotoxy(5,winrows-7);
                    cout << "Rename Successful"<<endl;
                    sleep(2); 
                }
                cout << "\033[2J\033[1;1H";
                break;
            }

            if(words[0] == "copy"){ //copy functionality

                if(words.size()<3){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                string dest;
                vector<string> files;
                //vector<string> notfoundfiles;
                for(int i=1;i<words.size()-1;i++){
                    files.push_back(words[i]);
                }
                dest = words[words.size()-1]; 
                copystuff(dest,gotos,files);
                cout << "\033[2J\033[1;1H";
                break;
                
            }

            if(words[0] == "delete_file"){ //delete file functionality

                if(words.size()<2){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                if(CheckPath(words[words.size()-1])==0){
                    gotoxy(15,5);
                    cout << "Path Entered is invalid"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                string dest;
                dest = words[words.size()-1]; 
                int delstat = deletefile(dest);
                if(delstat==0){
                    gotoxy(5,winrows-7);
                    cout << "Failed to delete "<<endl;
                    sleep(2); 
                }
                else{
                    gotoxy(5,winrows-7);
                    cout << "Delete file successful"<<endl;
                    sleep(2);
                }
                cout << "\033[2J\033[1;1H";
                break;
                
            }
            if(words[0] == "delete_dir"){ //delete dir functionality

                if(words.size()<2){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                if(CheckPath(words[words.size()-1])==0){
                    gotoxy(15,5);
                    cout << "Path Entered is invalid"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                string dest;
                dest = words[words.size()-1]; 
                deletedir(dest);
                cout << "\033[2J\033[1;1H";
                break;
                
            }

            if(words[0] == "move"){ //move functionality

                if(words.size()<3){
                    gotoxy(15,5);
                    cout << "Wrong Command"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                if(CheckPath(words[words.size()-1])==0){
                    gotoxy(15,5);
                    cout << "Path Entered is invalid"<<endl;
                    sleep(2);
                    cout << "\033[2J\033[1;1H";
                    break;
                }
                string dest;
                vector<string> files;
                vector<string> notfoundfiles;
                for(int i=1;i<words.size()-1;i++){
                    files.push_back(words[i]);
                }
                dest = words[words.size()-1];
                int mv = movefile(dest,gotos,files,notfoundfiles);
                if(mv == -1){
                    gotoxy(5,winrows-7);
                    cout << "Failed to move due to error in path "<<endl;
                    sleep(2); 
                }
                else{

                if(notfoundfiles.size()==0){
                    gotoxy(5,winrows-7);
                    cout << "Files/Directories are successfully moved"<<endl;
                    sleep(2); 
                }
                else{
                    string nff = "";
                    for(int i=0;i<notfoundfiles.size();i++){
                        nff = notfoundfiles[i];
                        nff = nff + ' ';
                    }
                    gotoxy(5,winrows-7);
                    cout << "Files/Directories that failed to move are : "<<nff<<endl;
                    sleep(2); 
                    }
                }
                cout << "\033[2J\033[1;1H";
                break;
            }
            gotoxy(15,5);  //incase no match
            cout << "Wrong Command"<<endl;
            sleep(2);
            cout << "\033[2J\033[1;1H";
            break;
        }
        if(command==0){
            break;
        }
        if(flag==1)
            break;
        }
        }
    if(flag==1)
        break;
    }
    //}
    return 0;
    }





