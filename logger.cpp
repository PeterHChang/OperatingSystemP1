#include <iostream>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <string>

//make sure to add dup2, as well as change file to 'making a new file'


using namespace std;

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        cout <<"Error: Text File Missing";
        return 1;
    }


    fstream myfile(argv[1], ios::out | ios::app);


    if(!myfile.is_open())
    {
        cout << "Error in opening file";
        return 0;
    }

    time_t rawtime;
    tm* timeinfo;
    char buffer [80];

    string temp;

    string delim = " ";

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y-%m-%d-%H:%M",timeinfo);

  	string templastWord;
  	string lastWord;

  	//output beginning
    myfile <<  buffer << "[START] " << "Logging Started" << endl;

    char mode;
    char mode1;

    cin.read(&mode,1);


	while(mode != 4)
    {
        switch(mode)
        {
            case 0: //passkey
                {
                cin.read((char*)&templastWord,sizeof(lastWord));
                lastWord = templastWord;

                myfile <<  buffer << "[PASSKEY] " << lastWord << endl;

                break;
                }
            case 1: //encrypr
                {
                cin.read((char*)&templastWord,sizeof(lastWord));
                lastWord = templastWord;

                myfile <<  buffer << "[ENCRYPT] " << lastWord << endl;

                break;
                }
            case 2: //decrypt
                {
                cin.read((char*)&templastWord,sizeof(lastWord));
                lastWord = templastWord;

                myfile <<  buffer << "[DECRYPT] " << lastWord << endl;

                break;
                }

            case 3: //history
                {
                    string temp;
                    myfile.seekg(0, ios::beg);

                    int i = 0;

                    while(getline(myfile, temp))
                    {
                        i+=1;
                    }

                    cout.write((char*)&i,sizeof(i));

                    //initialize string array for separation
                    string history[i];
                    string pastpasskeys[i];
                    string pastencryptions[i];
                    string pastdecrypts[i];

                    string delimier1 = "[";
                    string delimier2 = "]";

                    i = 0;
                    while(getline(myfile, temp))
                    {
                        history[i] = temp;

                        unsigned first = temp.find(delimier1);
                        unsigned last = temp.find(delimier2);
                        string firstpart = temp.substr (first,last-first);

                        if(firstpart == "PASSKEY")
                        {
                            string templast = temp.substr(last + 1);
                            pastpasskeys[i] = templast;
                        }
                        if(firstpart == "ENCRYPT")
                        {
                            string templast = temp.substr(last + 1);
                            pastencryptions[i] = templast;
                        }
                        if(firstpart == "DECRYPT")
                        {
                            string templast = temp.substr(last + 1);
                            pastdecrypts[i] = templast;
                        }

                        i+=1;
                    }


                    cin.read(&mode1,1);
                        switch(mode1)
                        {
                            case 0: // output previous passkeys
                                {
                                    cout.write((char*)&pastpasskeys,sizeof(pastpasskeys));
                                    break;
                                }
                            case 1: // output previous encryptions
                                {
                                    cout.write((char*)&pastencryptions,sizeof(pastencryptions));
                                    break;
                                }
                            case 2: // output previous decrypts
                                {
                                    cout.write((char*)&pastdecrypts,sizeof(pastdecrypts));
                                    break;
                                }
                            case 3: //output all history
                                {
                                    cout.write((char*)&history,sizeof(history));
                                    break;

                                }

                        }
                }


        }

        cin.read(&mode,1);
    }

    myfile <<  buffer << " [QUIT] " << "STOP Logging Stopped." << endl;

    exit(0);

}
