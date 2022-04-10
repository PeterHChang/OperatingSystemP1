#include <iostream>
#include <string>

using namespace std;

//delete while loop, as well as replace break  with returns in the switch.
//the while loop would be implemented in the main driver program.

//https://www.tutorialspoint.com/cplusplus-program-to-implement-the-vigenere-cypher

class Vig {
   public:
   string k;
   Vig(string k)
   {
      for (int i = 0; i < k.size(); ++i)
        {
         if (k[i] >= 'A' && k[i] <= 'Z')
            this->k += k[i];
         else if (k[i] >= 'a' && k[i] <= 'z')
            this->k += k[i] + 'A' - 'a';
        }
   }
   string encryption(string t)
   {
      string output;
      for (int i = 0, j = 0; i < t.length(); ++i)
        {
         char c = t[i];
         if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
         else if (c < 'A' || c > 'Z')
            continue;
         output += (c + k[j] - 2 * 'A') % 26 + 'A'; //added 'A' to bring it in range of ASCII alphabet [ 65-90 | A-Z ]
         j = (j + 1) % k.length();
        }
      return output;
   }

   string decryption(string t)
   {
        string output;
        for (int i = 0, j = 0; i < t.length(); ++i)
        {
         char c = t[i];
         if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
         else if (c < 'A' || c > 'Z')
            continue;
         output += (c - k[j] + 26) % 26 + 'A';//added 'A' to bring it in range of ASCII alphabet [ 65-90 | A-Z ]
         j = (j + 1) % k.length();
      }
      return output;
   }
};

// by returning the index number of the array, this counter can be used for switch function.
int findindex(string array[], int size, string target)
{
    for (int i = 0; i < size; i++)
    {
        if(array[i] == target)
            return i;
    }
}


int main(int argc, const char** argv)
{
    //char passkey[50];
    //string password(passkey);
    string temppasskey;
    string passkey;
    string tempstringtoencrypt;
    string stringtoencrypt;

    string stringtodecrypt;
    string tempstringtodecrypt;

    bool ispasskeyset = false;

    char mode;

    cin.read(&mode,1);

	while(mode != 5)
    {
        switch(mode)
        {
            case 0: //retrieve passkey
                {
                cout.write((char*)&temppasskey,sizeof(string));
                passkey = temppasskey;
                break;
                }
            case 1: //enter passkey
                {
                ispasskeyset = true;
                cin.read((char*)&temppasskey,sizeof(string));
                passkey = temppasskey;

                break;
                }
            case 2: //encrypt set
                {
                cin.read((char*)&tempstringtoencrypt,sizeof(string));
                stringtoencrypt = tempstringtoencrypt;
                break;
                }
            case 3: //encrypt result
                {
                    if(ispasskeyset ==false)
                    {
                        string error = "ERROR Password not set";
                        cout.write((char*)&error,sizeof(string));
                        break;
                    }

                Vig v(passkey);
                    string encrypt = v.encryption(stringtoencrypt);
                    cout.write((char*)&encrypt,sizeof(string));

                    break;
                }
            case 4: //decrypt set
                {
                cin.read((char*)&tempstringtodecrypt,sizeof(string));
                stringtodecrypt = tempstringtodecrypt;
                break;
                }
            case 5:
                {
                    if(ispasskeyset ==false)
                    {
                        string error = "ERROR Password not set";
                        cout.write((char*)&error,sizeof(string));
                        break;
                    }

                Vig v(passkey);
                    string decrypt = v.decryption(stringtodecrypt);

                    cout.write((char*)&decrypt,sizeof(string));

                    break;
                }
        }


    cin.read(&mode,1);
    }

    return 0;
}

        /*

        firstWord = temp.substr(0, temp.find(" "));
        int pos = temp.find(" ");
        string last = temp.substr(pos + 1);


        switch(findindex(array, 4, firstWord))
        {
            case 0 :
                passkey = last;
                ispasskeyset = true;
                cout << "RESULT" << endl;
                break;
            case 1 :
                {
                    if(ispasskeyset == false)
                    {
                        cout << "ERROR Password not set" << endl;
                        break;
                    }
                    Vig v(passkey);
                    string encrypt = v.encryption(last);
                    cout << "\nRESULT " << encrypt << endl;
                    break;
                }
            case 2 :
                {
                    if(ispasskeyset == false)
                    {
                        cout << "ERROR Password not set" << endl;
                        break;
                    }
                    Vig v(passkey);
                    string decrypt = v.decryption(last);
                    cout << "\nRESULT " << decrypt << endl;
                    break;
                }
            case 3 :
                cout << "Exiting Encrypt" << endl;
                break;
            default:
                break;

    }
*/
