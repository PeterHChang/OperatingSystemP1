#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>               /* UNIX and POSIX constants and functions (fork, pipe) */

using namespace std;



//1 child =logger.cpp (read) P1[0]
//2 child = encryption.cpp (read and write) P2[1], P2[2]
//parent = driver program (write to encryption, read from encryption, write to logger)

/*
GET HISTORY
    int i = 0;
    while (getline in log.txt ) //read pipe from log.
    {
                    parse the string to find [passkey].
                        if found
                        store in an string array[MAX_INT] at i
                        i++;wait

    }


    while(array[i] != NULL)
    {

    if(array[i] = a not a string)
        break;
    i++;
    }

    for(int j = 0; j < i; j++)
        cout << j + 1 << " " << array[j] << endl;

*/
string password;

int main(int argc, const char** argv)
{

    int fd[3][2];

    int i;
    for(i = 0; i < 3; i++)
    {
        if(pipe(fd[i]) < 0)
        {
            cout << "piping was unsuccessful" << endl;
            return 1;
        }
    }




    //------------------------------------------------------------------------------------------\\



    //------------------------------------------------------------------------------------------\\

                                            //logger.cpp

    int pid1 = fork();

    if(pid1 < 0)
    {
        cout << "Error Forking" << endl;
        return 2;
    }

	if(pid1 == 0)
	{
	    //child process for logger
	    //only using fd[0][0]read from driver. dup these two processes

	    close(fd[1][0]); //read driver from encryption
	    close(fd[1][1]); // write to encryption from driver
	    close(fd[2][0]); // read encryption from driver
	    close(fd[2][1]); //write to diver from encryption
	    close(fd[0][1]);
	    close(fd[3][0]);

        dup2(fd[0][0],0);
        dup2(fd[3][2],1); // output to driver

		char *args[] = {"./logger","logger.txt",NULL};


		execvp(args[0],args); //replace this program with the mem program

		close(fd[0][0]);
		close(fd[0][1]);

		return 0;
	}

    //------------------------------------------------------------------------------------------\\
                                        //Encryption\\

    int pid2 = fork();

	if(pid2 < 0)
    {
        cout << "Error Forking" << endl;
        return 5;
    }
    if(pid2 == 0)
    {
        //child process for Encryption
	    //only using fd[1][0] and fd[2][1]

	    close(fd[0][0]);
	    close(fd[0][1]);
	    close(fd[1][1]);
	    close(fd[2][0]);
	    close(fd[3][0]);
	    close(fd[3][1]);

        dup2(fd[1][0] , 0); // map for stdin
        dup2(fd[2][1] , 1); // map for stdout

		char *args[] = {"./encryption",NULL};


		execvp(args[0],args); //replace this program with the mem program

    }
    else
    {



	//------------------------------------------------------------------------------------------\\

    //------------------------------------------------------------------------------------------\\
                                        //Parent\\


    //------------------------------------------------------------------------------------------\\
                                        //menu\\


    //encryption process
    //dup2(fd[1][0], 0);
    //dup2(fd[2][1], 1);

	close(fd[0][0]);
    close(fd[1][0]);
    close(fd[2][1]);
    close(fd[3][1]);

    int choice;
    char tmp;

    string encrypt;
    string decrypt;


        cout << "Pick 1 to set a Password" << endl;
        cout << "Pick 2 to encrypt with current password" << endl;
        cout << "Pick 3 to decrypt with current password" << endl;
        cout << "Pick 4 to view history" << endl;
        cout << "Pick 5 to quit" << endl;

        cin >> choice;

    while(choice != 5)
    {
        switch(choice)
        {
        case 1:
            {
            cout << "You have chosen to set a new password" << endl;
            cout << "Press 1 if you would like to set up one. Press 2 to use a previous used password" << endl;

            int passwordchoice;

            cin >> passwordchoice;

            switch(passwordchoice)
            {
                case 1:
                    {
                        cout << "Enter your password" << endl;
                        string temppassword;
                        string password;


                        cin >> temppassword;

                        password = temppassword;

                        tmp = 1;
                        write(fd[1][1], &tmp, 1);
                        write(fd[1][1], (char*)&password, sizeof(string));

                        tmp = 0;
                        write(fd[1][1], &tmp, 1);

                        string temp;
                        string temporary;

                        read(fd[2][0], (char*)&temp, sizeof(string));
                        temporary = temp;

                        cout << temporary;


                        //output to logger
                        tmp = 0;
                        write(fd[0][1], &tmp, 1);
                        write(fd[0][1], (char*)&password, sizeof(string));
                        break;
                    }
                case 2:
                    {
                        //from logger
                        int i;

                        tmp = 3;
                        write(fd[0][1], &tmp, 1);
                        read(fd[3][0], (char*)&i, sizeof(int)); //reading size

                        if(i <=0)
                        {
                            cout << "There is no saved passkeys" << endl;
                            break;
                        }

                        string temppreviouspasskey;
                        string previouspasskey;

                        tmp = 0;
                        write(fd[0][1], &tmp, 1);
                        read(fd[3][0], (char*)&temppreviouspasskey, sizeof(string)); //reading data

                        previouspasskey = temppreviouspasskey;

                        i = 0;
                        while(previouspasskey[i] != NULL)
                        {
                            cout << "i: " << previouspasskey[i] << endl;
                            i++;
                        }
                        int pick;

                        cout << "Pick from one of the saved passwords" << endl;
                        cin >> pick;
                        cout << "You have chosen " << previouspasskey[pick];

                        string chosen;
                        chosen = previouspasskey[pick];


                        string tempo = chosen;
                        tmp = 1;
                        write(fd[1][1], &tmp, 1);
                        write(fd[1][1], (char*)&tempo, sizeof(string));

                        //output to logger
                        tmp = 0;
                        write(fd[0][1], &tmp, 1);
                        write(fd[0][1], (char*)&tempo, sizeof(string));
                        break;

                        break;


                    }

            }

            break;
            }

        case 2:
            {
            cout << "You have previouspasskey[pick]chosen to encrypt" << endl;
            cout << "Press 1 if you would like to set up one. Press 2 to use a previous used one" << endl;

            int passwordchoice;

            cin >> passwordchoice;

            switch(passwordchoice)
            {
                case 1:
                    {
                        cout << "Enter your string to be encrypted" << endl;

                        cin >> encrypt;

                        tmp = 2;
                        write(fd[1][1], &tmp, 1);
                        write(fd[1][1], (char*)&encrypt, sizeof(string));


                        tmp = 3;
                        write(fd[1][1], &tmp, 1);

                        string temp;
                        string temporary;

                        read(fd[2][0], (char*)&temp, sizeof(string));
                        temporary = temp;

                        cout << temporary;

                        //output to logger
                        tmp = 1;
                        write(fd[0][1], &tmp, 1);
                        write(fd[0][1], (char*)&temp, sizeof(string));
                        break;
                    }
                 case 2:
                    {
                        //from logger
                        int i;

                        tmp = 3;
                        write(fd[0][1], &tmp, 1);
                        read(fd[3][0], (char*)&i, sizeof(int)); //reading size

                        if(i <=0)
                        {
                            cout << "There is no saved encrypts" << endl;
                            break;
                        }

                        string temppastencryptions;
                        string pastencryptions;
                        string tempo;

                        tmp = 1;
                        write(fd[0][1], &tmp, 1);
                        read(fd[3][0], (char*)&temppastencryptions, sizeof(string)); //reading data

                        pastencryptions = temppastencryptions;

                        i = 0;
                        while(pastencryptions[i] != NULL)
                        {
                            cout << "i: " << pastencryptions[i] << endl;
                            i++;
                        }
                        int pick;

                        cout << "Pick from one of the saved encryptions" << endl;
                        cin >> pick;
                        cout << "You have chosen " << pastencryptions[pick];

                        tempo = pastencryptions[pick];

                        tmp = 2;
                        write(fd[1][1], &tmp, 1);
                        write(fd[1][1], (char*)&tempo, sizeof(string));

                        //output to logger
                        tmp = 1;
                        write(fd[0][1], &tmp, 1);
                        write(fd[0][1], (char*)&tempo, sizeof(string));
                        break;
                    }

            }

            break;
            }
        case 3:
            {
            cout << "You have chosen to decrypt" << endl;
            cout << "Press 1 if you would like to set up one. Press 2 to use a previous used decrypts" << endl;

            int passwordchoice;

            cin >> passwordchoice;

            switch(passwordchoice)
            {
                case 1:
                    {
                    cout << "Enter your string to be decrypted" << endl;

                    cin >> decrypt;

                    tmp = 4;
                    write(fd[1][1], &tmp, 1);
                    write(fd[1][1], (char*)&decrypt, sizeof(string));


                    tmp = 5;
                    write(fd[1][1], &tmp, 1);

                    string temp;
                    string temporary;

                    read(fd[2][0], (char*)&temp, sizeof(string));
                    temporary = temp;

                    cout << temporary;

                    //output to logger
                    tmp = 2;
                    write(fd[0][1], &tmp, 1);
                    write(fd[0][1], (char*)&temp, sizeof(string));
                    break;
                    }
                case 2:
                    {
                        //from logger
                        int i;

                        tmp = 3;
                        write(fd[0][1], &tmp, 1);
                        read(fd[3][0], (char*)&i, sizeof(int)); //reading size

                        if(i <=0)
                        {
                            cout << "There is no saved decrypts" << endl;
                            break;
                        }

                        string temppastdecrypts;
                        string pastdecrypts;
                        string tempo;

                        tmp = 2;
                        write(fd[0][1], &tmp, 1);
                        read(fd[3][0], (char*)&temppastdecrypts, sizeof(string)); //reading data
                        pastdecrypts= temppastdecrypts;

                        i = 0;
                        while(pastdecrypts[i] != NULL)
                        {
                            cout << "i: " << pastdecrypts[i] << endl;
                            i++;
                        }
                        int pick;

                        cout << "Pick from one of the saved encryptions" << endl;
                        cin >> pick;
                        cout << "You have chosen " << pastdecrypts[pick];

                        tempo = pastdecrypts[pick];

                        tmp = 3;
                        write(fd[1][1], &tmp, 1);
                        write(fd[1][1], (char*)&tempo, sizeof(string));

                        //output to logger
                        tmp = 2;
                        write(fd[0][1], &tmp, 1);
                        write(fd[0][1], (char*)&tempo, sizeof(string));
                        break;
                    }
            }
            break;

            }
        case 4: //history
            {
                int i;

                tmp = 3;
                write(fd[0][1], &tmp, 1);
                read(fd[3][0], (char*)&i, sizeof(int)); //reading size

                if(i <=0)
                {
                cout << "There is no saved history" << endl;
                break;
                }

                string temphistory;
                string history;

                tmp = 2;
                write(fd[0][1], &tmp, 1);

                read(fd[3][0], (char*)&temphistory, sizeof(string)); //reading data

                history = temphistory;

                i = 0;

                while(history[i] != NULL)
                {
                cout << history[i] << endl;
                i++;
                }

            break;
            }
        default:
            cout <<"please choose from 1-4" << endl;
            break;

        }

        cout << "Pick 1 to set a Password" << endl;
        cout << "Pick 2 to encrypt with current password" << endl;
        cout << "Pick 3 to decrypt with current password" << endl;
        cout << "Pick 4 to view history" << endl;
        cout << "Pick 5 to quit" << endl;

        cin >> choice;
    }


    cout << "Goodbye";
    //quit on logger
    tmp = 5;
    write(fd[0][1], &tmp, 1);




//logger process
    //dup2(fd[0][1], 0);


    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);




    }

    //------------------------------------------------------------------------------------------\\

    return 0;
}
