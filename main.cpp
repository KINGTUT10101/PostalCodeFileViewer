#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include "PostalCodeBuffer.h"
#include "NewPostalCodeBuffer.h"
#include "PostalCode.h"

using namespace std;

int unpackPostalCode (PostalCode& pc, PostalCodeBuffer* buff);

bool fillTable (map<string, vector<PostalCode> >& stateMap, const char* filename, PostalCodeBuffer* buff, string fileFormat);

void displayHeader ();

void displayTable (const map<string, vector<PostalCode> >& stateMap);

// argv[1] = input file, argv[2] = file format
int main(int argc, char* argv[]) {
    map<string, vector<PostalCode> > stateMap; // Create a map to store PostalCode objects by state ID
	PostalCodeBuffer* buff;
	
	cout << endl; // CentOS formatting

	// Checks if the number of arguments is correct
	if (argc < 3) {
        cout << "Enter './[program name] [record file name]  [file format]'" << endl;
        cout << "For example, './myProgram zip_codes.csv -old'" << endl;
        return 1;
    }

	string filename = argv[1];
	string fileFormat = argv[2];
	
	// Creates the buffer object that will be used to read the records
    if (fileFormat == "-old") {
        buff = new PostalCodeBuffer (1000);
    }
    else if (fileFormat == "-new") {
        buff = new NewPostalCodeBuffer (1000);
    }
    else {
        cerr << "Invalid file format (Valid arguments are '-old' and '-new')" << endl;
        return 1;
    }

	// Fills the map and displays the records
	fillTable (stateMap, filename.c_str (), buff, fileFormat);
	displayHeader ();
	displayTable (stateMap);

	cout << endl << endl; // CentOS formatting
	
	return 0;
}

// Unpacks the buffer's contents into a postal code object
int unpackPostalCode (PostalCode& pc, PostalCodeBuffer* buff) {
	int result = -1;
	char temp[256];
	
	// Zip code
	result = buff->unpack (temp);
	pc.setZipCode (atoi (temp));
	
	// City
	result = buff->unpack (temp);
	pc.setCity (temp);
	
	// State
	result = buff->unpack (temp);
	pc.setState (temp);
	
	// County
	result = buff->unpack (temp);
	pc.setCounty (temp);
	
	// Lat
	result = buff->unpack (temp);
	pc.setLat (atof (temp));
	
	// Long
	result = buff->unpack (temp);
	pc.setLong (atof (temp));
	
	return result;
}

bool fillTable (map<string, vector<PostalCode> >& stateMap, const char* filename, PostalCodeBuffer* buffer, string fileFormat) {
	// Open the CSV data file
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error: could not open input file" << endl;
        return false;
    }

    // Skip past the header in the file
    buffer->readHeader(infile, "", "");
	
	int records = 0;
	int successes = 0;

    // Read the file and store PostalCode objects in the map
    while (buffer->read(infile) != -1) {
        records += 1;
		
		// Create a new PostalCode object
        PostalCode postalCode;

        // Unpack the data from the buffer and set the attributes of the PostalCode object
		int result = unpackPostalCode (postalCode, buffer);
        string state = postalCode.getState ();
		
		if (result == -1) {
            cout << "Invalid record: " << endl;
			//postalCode.print ();
			continue;
        }
		
        // Add the PostalCode object to the appropriate state vector in the map
        stateMap[state].push_back(postalCode);
		
		successes += 1;
    }
	
	cout << "Number of records read: " << records << endl;
	cout << "Number of valid records read: " << successes << endl;

    // Close the input file
    infile.close();
	
	return true;
}

void displayHeader () {
	// Print the table header
    cout << left << setw(12) << "State ID";
	cout << left << setw(15) << "Easternmost";
	cout << left << setw(15) << "Westernmost";
	cout << left << setw(15) << "Northernmost";
	cout << left << setw(15) << "Southernmost";
	cout << endl;
	
	return;
}

void displayTable (const map<string, vector<PostalCode> >& stateMap) {
	// Iterate over the stateMap and print the data for each state
	// This will display the map in the correct order
    for (auto it = stateMap.begin(); it != stateMap.end(); ++it) {
        // Get the state ID and vector of PostalCode objects for this state
        string stateId = it->first;
        vector<PostalCode> postalCodes = it->second;

        // Sort the PostalCode objects by latitude
        sort(postalCodes.begin(), postalCodes.end(), [](const PostalCode& a, const PostalCode& b) {
            if (a.getLat() == b.getLat()) {
                return a.getZipCode() < b.getZipCode(); // Sort by zip code if latitude is the same
            }
            return a.getLat() > b.getLat(); // Sort by latitude
        });

        PostalCode northernmost = postalCodes.front();
        PostalCode southernmost = postalCodes.back();

		// Sort the PostalCode objects by longitude
        sort(postalCodes.begin(), postalCodes.end(), [](const PostalCode& a, const PostalCode& b) {
            if (a.getLong() == b.getLong()) {
                return a.getZipCode() < b.getZipCode(); // Sort by zip code if longitude is the same
            }
            return a.getLong() < b.getLong(); // Sort by longitude
        });

        PostalCode easternmost = postalCodes.front();
        PostalCode westernmost = postalCodes.back();

        // Print the data for this state
		cout << left << setw(12) << stateId;
		cout << left << setw(15) << easternmost.getZipCode();
		cout << left << setw(15) << westernmost.getZipCode();
		cout << left << setw(15) << northernmost.getZipCode();
		cout << left << setw(15) << southernmost.getZipCode();
		cout << endl;
    }
	
	return;
}
