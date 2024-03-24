/******************************************************************************

Sohaib Rehman
sxr220163

CORE IMPLEMENTATION DUE 3/23/2024
FINAL PROJECT DUE 3/31/2024

This code is a repeat of the first project and instead of using our older more 
primitive methods of data manipulation we will be using pointers and pointer 
arithmetic along with linked lists to store and manipulate our data. This code 
uses the same few function from the previous project along with a few new ones 
namely these being, a sort record function that sorts records based on a criteria 
set by the user whether it be the names(alpha order) or by the plays(low to high).
Furthermore all the data manipulation is occuring in the memory instead of the file.
All the previous functions maintain the same functionality and work with around the same 
logic as used in project 1.
*******************************************************************************/


#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>


using namespace std;

struct record
{
    // This struct stores all of our variables for each database entry along with our method of moving through the database
    string name;
    int highscore;
    string initials;
    int plays;
    double revenue = 0.00;
    record* next = nullptr;
};

void searchRecord(record*, string);
void addRecord(record*, record*);
void printList(record*, ofstream&);
void editRecord(record*, string, int, string);

int main()
{
    // Head variable for reference point, database batchfile and the finaldatabase with all our revisions 
    record *head = nullptr;
    string database;
    string batchfile;
    string finaldatabase;
    // Made this a constant to make declaration across the code easier 
    finaldatabase = "freeplay.dat";
    
    // User input
    cout << "What is the name of the database file? ";
    
    cin >> database;
    
    cout << "What is the name of the batch file? ";
    
    cin >> batchfile;
    cout << endl;
    
    // Open the file with all our data 
    ifstream input;
    input.open(database, ios::binary);
    // Case if the file doesnt open 
    if(!input.is_open())
    {
        cout << "Could not open database file." << endl;
        cout << "Exiting program" << endl;
        return 1;
    }
    
    
    // Variables to store all of our values to create the linked list 
    int checkhighscore;
    string checkinitials;
    intcheckplays;
    string dataname;
    double datarevenue;
    string line;
    
    while(getline(input, line) ) // Creating a linked list here 
    {
        record* newNode = new record;
        
        // We are using the commas as a flag for where we are in the entry 
        unsigned long int comma1 = line.find(',');
        
        unsigned long int comma2 = line.find(',', comma1 + 1);
        
        unsigned long int comma3 = line.find(',', comma2 + 1);
        
        unsigned long int comma4 = line.find(',', comma3 + 1);
        
        unsigned long int dollarsign = line.find('$');
        
        // Set the values equal to the values in the current line substringed by the comma that will go after the value 
        //Name 
        dataname = line.substr(0, comma1);
        //High Score 
        datahighscore = stoi( line.substr(comma1 + 2, comma2 - comma1 - 2) );
        //Initials 
        datainitials = line.substr(comma2 + 2, comma3 - comma2 - 2);
        //Plays
        dataplays = stoi( line.substr(comma3 + 2, comma4 - comma3 - 2) );
        //Revenue 
        datarevenue = stod( line.substr(dollarsign + 1) );
        
        //DeRef all the values and set them equal 
        newNode->name = dataname;
        newNode->highscore = datahighscore;
        newNode->initials = datainitials;
        newNode->plays = dataplays;
        newNode->revenue = datarevenue;
        
        newNode->next = nullptr;
        if(head == nullptr)
        {
            head = newNode;
        }
        
        else
        {
            record* temp = head;
            while(temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        
        
    }
    
    input.close();
    
    
    
    line = "";
    input.open(batchfile);
    
    if(!input.is_open())
    {
        cout << "Could not open batch file." << endl;
        cout << "Exiting program" << endl;
        return 1;
    }
    
    int function_choice;
    while( getline(input, line) )
    {
        unsigned long int function_choice_pos = line.find(' ');
        function_choice = stoi( line.substr(0, function_choice_pos) );
        line.erase(0, function_choice_pos + 1);
        
        if(function_choice == 1)
        {
            record* addNode = new record;
            
            unsigned long int name_start = line.find('"');
            unsigned long int name_end = line.find('"', name_start + 1);
            dataname = line.substr(name_start + 1, name_end - name_start - 1);
            
            // Find the space after the closing quotation mark for name
            unsigned long int space_after_name = line.find(' ', name_end);
            line = line.substr(space_after_name + 1); // Move line pointer to highscore
            
            // Extract highscore
            unsigned long int highscore_end = line.find(' ', 0);
            datahighscore = stoi(line.substr(0, highscore_end));
            line = line.substr(highscore_end + 1); // Move line pointer to initials
            
            // Extract initials
            unsigned long int initials_end = line.find(' ', 0);
            datainitials = line.substr(0, initials_end);
            line = line.substr(initials_end + 1); // Move line pointer to plays
            
            // Extract plays
            unsigned long int plays_end = line.find(' ', 0);
            dataplays = stoi(line.substr(0, plays_end));
            line = line.substr(plays_end + 1); // Move line pointer to revenue
            
            // Extract revenue
            unsigned long int revenue_end = line.find(' ', 0);
            datarevenue = stod(line.substr(1, revenue_end)); // Exclude dollar sign
            
            addNode->name = dataname;
            addNode->highscore = datahighscore;
            addNode->initials = datainitials;
            addNode->plays = dataplays;
            addNode->revenue = datarevenue;
            
            addRecord(addNode, head);
        }
        else if(function_choice == 2)
        {
            unsigned long int term_pos = line.find(' ');
            
            string term = line.substr(term_pos + 1);
            
            searchRecord(head, term);
        }
        else if(function_choice == 3)
        {
            unsigned long int name_start = line.find('"');
            unsigned long int name_end = line.find('"', name_start + 1);
            dataname = line.substr(name_start + 1, name_end - name_start - 1);
            
            // Find the space after the closing quotation mark for name
            unsigned long int space_after_name = line.find(' ', name_end);
            line = line.substr(space_after_name + 1); // Move line pointer to highscore
        
            unsigned long int editchoice_end = line.find(' ', 0);
            int editchoice;
            editchoice = stoi(line.substr(0, editchoice_end));
            line = line.substr(editchoice_end + 1); // Move line pointer to initials
        
            unsigned long int changedvalue_end = line.find(' ', 0);
            string changedvalue;
            changedvalue = line.substr(0, changedvalue_end);
            
            editRecord(head, dataname, editchoice, changedvalue);
            
        }
    }
    
    input.close();
    
    ofstream output;
    output.open(finaldatabase);

    if (!output.is_open()) 
    {
        cout << "Failed to open freeplay.dat" << endl;
        return 1;
    }
    
    printList(head, output);
    
    output.close();

    return 0;
}

void editRecord(record* head, string nameToEdit, int editchoice, string changedValue) 
{
    record* current = head;

    while (current != nullptr) 
    {
        if (current->name == nameToEdit) 
        {
            // Update record fields
            cout << current->name << " UPDATED" << endl;
            
            if(editchoice == 1)
            {
                cout << "UPDATE TO high score - VALUE " << changedValue << endl;
                current->highscore = stoi(changedValue);
            }
            else if(editchoice == 2)
            {
                cout << "UPDATE TO initials - VALUE " << changedValue << endl;
                current->initials = changedValue;
            }
            else if(editchoice == 3)
            {
                cout << "UPDATE TO plays - VALUE " << changedValue << endl;
                current->plays = stoi(changedValue);
                current->revenue = 0.25 * current->plays;
            }

            cout << "Name: " << current->name << endl;
            cout << "High Score: " << current->highscore << endl;
            cout << "Initials: " << current->initials << endl;
            cout << "Plays: " << current->plays << endl;
            cout << "Revenue: $" << fixed << setprecision(2) << current->revenue << endl;
            cout << endl;
            return;
        }
        current = current->next;
    }

}

void addRecord(record* node, record* head)
{
    record* newNode = new record;
    newNode->name = node->name;
    newNode->highscore = node->highscore;
    newNode->initials = node->initials;
    newNode->plays = node->plays;
    newNode->revenue = node->revenue;
    newNode->next = nullptr; // Ensure the new node points to nullptr

    // If the list is empty, make the new node the head
    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        // Find the last node
        record* temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        // Insert the new node after the last node
        temp->next = newNode;
    }
    
    cout << "RECORD ADDED" << endl;
    cout << "Name: " << node->name << endl;
    cout << "High Score: " << node->highscore << endl;
    cout << "Initials: " << node->initials << endl;
    cout << "Plays: " << node->plays << endl;
    cout << "Revenue: $" << fixed << setprecision(2) << node->revenue << endl;
    cout << endl;
}

void printList(record* head, ofstream& os) 
{
    if (head == nullptr) 
    {
        os.close(); // Close the file stream when the end of the list is reached
        return;
    }
    
    os << head->name << ", ";
    os << head->highscore << ", ";
    os << head->initials << ", ";
    os << head->plays << ", $";
    os << fixed << setprecision(2) << head->revenue << endl;
    
    printList(head->next, os); // Recursive call for the next node
}

void searchRecord(record* head, string searchTerm) 
{
    record* current = head;
    record* searchResultHead = nullptr; // Head of the search result linked list

    while (current != nullptr) 
    {
        // Check if the current record's name contains the search term
        if (current->name.find(searchTerm) != string::npos) 
        {
            // Create a new node and copy data from the current node
            record* newNode = new record;
            newNode->name = current->name;
            newNode->highscore = current->highscore;
            newNode->initials = current->initials;
            newNode->plays = current->plays;
            newNode->revenue = current->revenue;
            newNode->next = nullptr;
            
            // If the search result linked list is empty, set the new node as the head
            if (searchResultHead == nullptr) 
            {
                searchResultHead = newNode;
            } 
            else 
            {
                // Otherwise, add the new node to the end of the search result linked list
                record* temp = searchResultHead;
                while (temp->next != nullptr) 
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }
        current = current->next; // Move to the next record in the original linked list
    }

    // Print search results
    if (searchResultHead == nullptr) 
    {
        cout << searchTerm << " NOT FOUND" << endl;
    } 
    else 
    {
        current = searchResultHead;
        while (current != nullptr) 
        {
            cout << current->name << " FOUND" << endl;
            cout << "High Score: " << current->highscore << endl;
            cout << "Initials: " << current->initials << endl;
            cout << "Plays: " << current->plays << endl;
            cout << "Revenue: $" << fixed << setprecision(2) << current->revenue << endl;
            cout << endl;
            current = current->next;
        }
        
    }

    // Free memory allocated for the search result linked list
    while (searchResultHead != nullptr) 
    {
        record* temp = searchResultHead;
        searchResultHead = searchResultHead->next;
        delete temp;
    }
}
