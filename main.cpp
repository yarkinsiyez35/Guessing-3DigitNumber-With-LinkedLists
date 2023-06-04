#include <iostream>
#include "feedback.h"
#include <sstream>

using namespace std;

struct node{
    int number;
    node * next;
    node()                      //default constructor
    {
        number = -1;
        next = NULL;
    }
    node(int n, node * p)    //alternative constructor
    {
        number = n;
        next = p;
    }
};

bool IsNumber(string &userGuess_string)             //takes the guess and checks if there are non integer characters
{
    if (userGuess_string.length() != 3)             //returns false if the string does not have exactly 3 characters, prevents other operations
    {
        return false;
    }
    for (unsigned int i =0; i<3; i++)           //for each character in the string
    {
        if (int(userGuess_string[i])<48 || int(userGuess_string[i]> 57))        //checks the ascii values of characters
        {
            return false;                                                   //returns false if there is a character different than [0,9]
        }
    }
    return true;                                                        //returns true if every character is an integer
}

bool IsDistinct(string &userGuess_string)       //takes the guess and checks if there are any recurrent integers
{
    for (unsigned int i =0; i<2; i++)                   //starts by the beginning of the string
    {
        for (unsigned int j =i+1; j<3;j++)      //checks the characters to the right of the i'th character
        {
            if (userGuess_string[i] == userGuess_string[j])     //returns false if they are the same
            {
                return false;
            }
        }

    }
    return true;                        //returns true if every character is distinct
}

int CharToInt(char &ch)     //finds the integer to add to the desired linked list
{
    return (int) ch - '0';
}

bool CheckNode(node *head, const int&value)              //checks if the node exists in the linked list, returns false if it does not exist
{

    while (head != NULL)                    //while the list is not over
    {
        if (head->number == value)          //if there is a match
        {
            return true;
        }
        head = head->next;                  //goes to the next node
    }
    return false;                           //returns false if there is not a match
}

bool FullList(node*head)            //returns true if the list is full
{
    int count = 0;          //this will hold the number of total elements in the list
    head = head->next;      //skips the node with the value -1
    while (head != NULL)            //while there is an element
    {
        count++;                //increases the count because there exists an element
        head = head->next;      //goes to the next element
    }
    return count == 9;             //list is full if count is 10 because there exists ten elements between 1 to 9 but one should be in the possibles list
}

void CheckAndAddNode(node* head, const int &n)          //adds the value if it doesn't exist, does nothing if it exists, use only for possible linked lists
{
    if(!CheckNode(head, n))         //if the number does not exist
    {

        while(head->next != NULL && head->next->number <n)      //while there exists another item and that items value is less than n
        {
            head = head->next;              //goes to the next element
        }
        node* addedNode = new node(n,head->next);       //creates a node with the value n and connects its end with the next element
        head->next = addedNode;                             //connects the end of the previous node with the created node
    }
}

void FillTheList(node*head)         //fills the linked list with each number from 1 to 9, only happens when we know the correct digit for the slot
{
    if (!FullList(head))            //if the list is not full
    {
        for (int i =0; i<10; i++)       //for each number from 1 to 9
        {
            CheckAndAddNode(head,i);        //adds the number to the linked list if it doesn't exist
        }
    }

}

void RemoveNode(node*head, const int &n)        //removes the node if it exists with the given value, works without a pre-check
{
    while (head->next !=NULL)                       //while there exists another element
    {
        if (head->next->number ==n)                 //if the next node is the target
        {
            node * erased = head->next;             //creates a pointer and assigns it to the node with the target value
            head->next = head->next->next;          //connects the current node with the 2nd next node
            delete erased;                          //deletes the node with the target value
            erased = nullptr;
            break;                                  //stops the loop
        }
        head = head->next;                      //goes to the next element
    }
}

void DeleteList(node*head)          //deletes each node of the list
{
    node *temp;
    while(head != NULL)         //while there are elements in the list
    {
        temp = head;            //creates a temp pointer to delete afterwards
        head = head->next;      //goes to the next element
        delete temp;            //deletes the previous element
        temp = NULL;            //sets temp to NULL to avoid complications
    }
}

int FindSum(node*head)
{
    int sum = 0;
    head = head->next;
    while (head != NULL)
    {
        sum += head->number;
        head = head->next;
    }
    return sum;
}

string IntToString(const int n)
{
    stringstream stream;
    stream <<n;
    string s;
    stream >> s;
    return s;
}

int main() {

    /* creating the head pointers for the linked lists */
    //for each of these pointers, values are set at -1, thus they will be always the head of their respective linked lists because the numbers range between 1 and 9, this will prevent checking for the head in linked list operations
    node * possible1 = new node();
    node * possible2 = new node();
    node * possible3 = new node();
    node * impossible1 = new node();
    node * impossible2 = new node();
    node * impossible3 = new node();

    /* creating the pointer arrays */
    //built-in arrays are used to store linked lists' heads because the size is already known, there is no need to dynamically allocate this array
    node *POSSIBLES[3];
    node *IMPOSSIBLES[3];

    /* filling the pointer arrays with the previously created heads */
    POSSIBLES[0] = possible1;
    POSSIBLES[1] = possible2;
    POSSIBLES[2] = possible3;
    IMPOSSIBLES[0] = impossible1;
    IMPOSSIBLES[1] = impossible2;
    IMPOSSIBLES[2] = impossible3;

    /* getting game ID */
    cout << "Please enter a game ID.\n";
    int gameId;
    cin >> gameId;

    while(!FullList(IMPOSSIBLES[0]) || !FullList(IMPOSSIBLES[1]) || !FullList(IMPOSSIBLES[2]))   //while 3 of the lists are not full, meaning the number is not guessed
    {
        /* getting the guess */
        cout << "Enter your guess.\n";
        string userGuess_string;
        cin >> userGuess_string;

        /* checking the guess */
        while (!IsNumber(userGuess_string) || !IsDistinct(userGuess_string))            //gets in a loop until a valid guess is entered
        {
            cout << "Invalid guess. Enter your guess.\n";
            cin >> userGuess_string;
        }
        /* getting the feedback */
        string feedback = get_the_feedback(userGuess_string, gameId);

        /* modifying the linked lists according to the feedback */
        char feedback_letter; char feedback_number_char; int feedback_number;               //these will be used in every slot, explanations are given in each slot

        //Modifying the first slot
        feedback_letter = feedback[0];                 //finds the corresponding feedback for the slot 'G', 'R', 'Y'
        feedback_number_char = userGuess_string[0];        //finds the user digit for the first slot
        feedback_number = CharToInt(feedback_number_char);  //converts the digit to int data type

        if (feedback_letter == 'G')         //if the number is in the correct slot
        {
            //add the digit to other IMPOSSIBLES linked lists
            CheckAndAddNode(IMPOSSIBLES[1],feedback_number);
            CheckAndAddNode(IMPOSSIBLES[2],feedback_number);
            //fill the list of IMPOSSIBLES[0] and remove the number from that list
            FillTheList(IMPOSSIBLES[0]);
            RemoveNode(IMPOSSIBLES[0],feedback_number);
            //add the number to the POSSIBLES[0] LIST
            CheckAndAddNode(POSSIBLES[0],feedback_number);
            //remove the other numbers from the POSSIBLES[0] list
            for (int i =0; i<10; i++)       //for each number between 0 and 9
            {
                if (i != feedback_number)       //if the number is not equal to the feedback_number
                {
                    RemoveNode(POSSIBLES[0],i);     //removes the number if it exists in the list
                }
            }
            //remove the number from other POSSIBLES linked lists
            RemoveNode(POSSIBLES[1],feedback_number);
            RemoveNode(POSSIBLES[2],feedback_number);
        }

        else if (feedback_letter == 'Y')     //if the number exists in another slot
        {
            //adds the number to IMPOSSIBLES[0]
            CheckAndAddNode(IMPOSSIBLES[0],feedback_number);

            //adds the number to other POSSIBLES but checks if that digit is in the IMPOSSIBLES of those lists
            if(!CheckNode(IMPOSSIBLES[1],feedback_number))          //if that number is not in the IMPOSSIBLES[1] list
            {
                CheckAndAddNode(POSSIBLES[1],feedback_number);        //adds the number to POSSIBLES[1] list
            }
            if(!CheckNode(IMPOSSIBLES[2],feedback_number))          //if that number is not in the IMPOSSIBLES[2] list
            {
                CheckAndAddNode(POSSIBLES[2],feedback_number);        //adds the number to the POSSIBLES[2] list
            }
            //removes the number from POSSIBLES[0]
            RemoveNode(POSSIBLES[0],feedback_number);
        }
        else                                //if the number does not exist anywhere
        {
            //remove number from every POSSIBLES list
            RemoveNode(POSSIBLES[0],feedback_number);
            RemoveNode(POSSIBLES[1],feedback_number);
            RemoveNode(POSSIBLES[2], feedback_number);
            //add the number to every IMPOSSIBLES list
            CheckAndAddNode(IMPOSSIBLES[0], feedback_number);
            CheckAndAddNode(IMPOSSIBLES[1], feedback_number);
            CheckAndAddNode(IMPOSSIBLES[2],feedback_number);
        }

        //modifying the second slot
        feedback_letter = feedback[1];                 //finds the corresponding feedback for the slot 'G', 'R', 'Y'
        feedback_number_char = userGuess_string[1];        //finds the user digit for the first slot
        feedback_number = CharToInt(feedback_number_char);  //converts the digit to int data type

        if (feedback_letter == 'G')         //if the number is in the correct slot
        {
            //add the digit to other IMPOSSIBLES linked lists
            CheckAndAddNode(IMPOSSIBLES[0],feedback_number);
            CheckAndAddNode(IMPOSSIBLES[2],feedback_number);
            //fill the list of IMPOSSIBLES[1] and remove the number from that list
            FillTheList(IMPOSSIBLES[1]);
            RemoveNode(IMPOSSIBLES[1],feedback_number);
            //add the number to the POSSIBLES[1] LIST
            CheckAndAddNode(POSSIBLES[1],feedback_number);
            //remove the other numbers from the POSSIBLES[1] list
            for (int i =0; i<10; i++)       //for each number between 0 and 9
            {
                if (i != feedback_number)       //if the number is not equal to the feedback_number
                {
                    RemoveNode(POSSIBLES[1],i);     //removes the number if it exists in the list
                }
            }
            //remove the number from other POSSIBLES linked lists
            RemoveNode(POSSIBLES[0],feedback_number);
            RemoveNode(POSSIBLES[2],feedback_number);
        }
        else if (feedback_letter == 'Y')        //if the number is in another slot
        {
            //adds the number to IMPOSSIBLES[1]
            CheckAndAddNode(IMPOSSIBLES[1],feedback_number);

            //adds the number to other POSSIBLES but checks if that digit is in the IMPOSSIBLES of those lists
            if(!CheckNode(IMPOSSIBLES[0],feedback_number))          //if that number is not in the IMPOSSIBLES[1] list
            {
                CheckAndAddNode(POSSIBLES[0],feedback_number);        //adds the number to POSSIBLES[1] list
            }
            if(!CheckNode(IMPOSSIBLES[2],feedback_number))          //if that number is not in the IMPOSSIBLES[2] list
            {
                CheckAndAddNode(POSSIBLES[2],feedback_number);        //adds the number to the POSSIBLES[2] list
            }
            //removes the number from POSSIBLES[1]
            RemoveNode(POSSIBLES[1],feedback_number);
        }
        else                        //if the number does not exist anywhere
        {
            //remove number from every POSSIBLES list
            RemoveNode(POSSIBLES[0],feedback_number);
            RemoveNode(POSSIBLES[1],feedback_number);
            RemoveNode(POSSIBLES[2], feedback_number);
            //add the number to every IMPOSSIBLES list
            CheckAndAddNode(IMPOSSIBLES[0], feedback_number);
            CheckAndAddNode(IMPOSSIBLES[1], feedback_number);
            CheckAndAddNode(IMPOSSIBLES[2],feedback_number);
        }

        //modifying the third slot
        feedback_letter = feedback[2];                 //finds the corresponding feedback for the slot 'G', 'R', 'Y'
        feedback_number_char = userGuess_string[2];        //finds the user digit for the first slot
        feedback_number = CharToInt(feedback_number_char);  //converts the digit to int data type

        if (feedback_letter == 'G')         //if the number is in the correct slot
        {
            //add the digit to other IMPOSSIBLES linked lists
            CheckAndAddNode(IMPOSSIBLES[0],feedback_number);
            CheckAndAddNode(IMPOSSIBLES[1],feedback_number);
            //fill the list of IMPOSSIBLES[2] and remove the number from that list
            FillTheList(IMPOSSIBLES[2]);
            RemoveNode(IMPOSSIBLES[2],feedback_number);
            //add the number to the POSSIBLES[2] LIST
            CheckAndAddNode(POSSIBLES[2],feedback_number);
            //remove the other numbers from the POSSIBLES[2] list
            for (int i =0; i<10; i++)       //for each number between 0 and 9
            {
                if (i != feedback_number)       //if the number is not equal to the feedback_number
                {
                    RemoveNode(POSSIBLES[2],i);     //removes the number if it exists in the list
                }
            }
            //remove the number from other POSSIBLES linked lists
            RemoveNode(POSSIBLES[0],feedback_number);
            RemoveNode(POSSIBLES[1],feedback_number);
        }
        else if (feedback_letter == 'Y')        //if the number is in another slot
        {
            //adds the number to IMPOSSIBLES[2]
            CheckAndAddNode(IMPOSSIBLES[2],feedback_number);

            //adds the number to other POSSIBLES but checks if that digit is in the IMPOSSIBLES of those lists
            if(!CheckNode(IMPOSSIBLES[0],feedback_number))          //if that number is not in the IMPOSSIBLES[1] list
            {
                CheckAndAddNode(POSSIBLES[0],feedback_number);        //adds the number to POSSIBLES[1] list
            }
            if(!CheckNode(IMPOSSIBLES[1],feedback_number))          //if that number is not in the IMPOSSIBLES[2] list
            {
                CheckAndAddNode(POSSIBLES[1],feedback_number);        //adds the number to the POSSIBLES[2] list
            }
            //removes the number from POSSIBLES[2]
            RemoveNode(POSSIBLES[2],feedback_number);
        }
        else                        //if the number does not exist anywhere
        {
            //remove number from every POSSIBLES list
            RemoveNode(POSSIBLES[0],feedback_number);
            RemoveNode(POSSIBLES[1],feedback_number);
            RemoveNode(POSSIBLES[2], feedback_number);
            //add the number to every IMPOSSIBLES list
            CheckAndAddNode(IMPOSSIBLES[0], feedback_number);
            CheckAndAddNode(IMPOSSIBLES[1], feedback_number);
            CheckAndAddNode(IMPOSSIBLES[2],feedback_number);
        }




        /* printing the results */
        cout << "Linked lists:\n";
        for (int i =0; i<3; i++)                        //for each slot
        {
            cout << "Slot: " << i+1 << endl;
            node * temp = IMPOSSIBLES[i];                   //sets temp as the head of the impossibles linked list
            cout << "Impossibles: ";
            temp = temp->next;      //skips to the second element because the first one is -1
            while(temp !=NULL)      //while the list is not empty
            {
                cout << temp->number << " ";            //prints the value
                temp = temp->next;
            }
            cout << endl;
            temp = POSSIBLES[i];
            temp = temp->next;
            cout << "Possibles: ";
            while (temp != NULL)
            {
                cout << temp->number << " ";
                temp = temp->next;
            }
            cout << endl << endl;
        }
    }
    /* finding the secret number */
   int hundreds = 45 - FindSum(IMPOSSIBLES[0]);
   int tens = 45 - FindSum(IMPOSSIBLES[1]);
   int ones = 45 - FindSum(IMPOSSIBLES[2]);

   string secretNumber;
   secretNumber = IntToString(hundreds) + IntToString(tens) + IntToString(ones);
   cout << "The secret number is: " << secretNumber << endl;
   cout << "Congrats! Now, deleting the lists...\n";

    /* deleting the lists */
    DeleteList(IMPOSSIBLES[0]);
    DeleteList(IMPOSSIBLES[1]);
    DeleteList(IMPOSSIBLES[2]);
    DeleteList(POSSIBLES[0]);
    DeleteList(POSSIBLES[1]);
    DeleteList(POSSIBLES[2]);

    return 0;
}