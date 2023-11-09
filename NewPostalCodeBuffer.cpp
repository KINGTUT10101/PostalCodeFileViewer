#include "NewPostalCodeBuffer.h"

	// CONSTRUCTORS
NewPostalCodeBuffer::NewPostalCodeBuffer (int mb) {
	// Sets default values for the postal code header
	headerMan.setStructure ("LENGTH/DELIM");
	headerMan.setVersion (1);
	headerMan.setRecordSize (0);
	headerMan.setSizeFormat (1);
	headerMan.setFieldCount (6);
	vector<string> fieldInfo = {
		"zipCode/DELIM/,",
		"city/DELIM/,",
		"state/DELIM/,",
		"county/DELIM/,",
		"lat/DELIM/,",
		"lng/DELIM/,"
	};
	headerMan.setFieldInfo (fieldInfo);
	headerMan.setPrimaryKey ("zipCode");
}


NewPostalCodeBuffer::~NewPostalCodeBuffer () {
	// Deletes the character array from the heap
	delete[] buffer;
	buffer = NULL;
}


int NewPostalCodeBuffer::readHeader (istream& file, const string& indexFilename, const string& indexSchema) {
	// Prepares the header manager for validating the header
	headerMan.setIndexFilename (indexFilename);
	headerMan.setIndexSchema (indexSchema);
		
	return headerMan.validateHeader (file);
}

int NewPostalCodeBuffer::writeHeader (ostream& file, unsigned short recordCount, const string& indexFilename, const string& indexSchema) {
	// Prepares the header manager for writing to the file
	headerMan.setRecordCount (recordCount);
	headerMan.setIndexFilename (indexFilename);
	headerMan.setIndexSchema (indexSchema);

	return headerMan.writeHeader (file);
}

int NewPostalCodeBuffer::read (istream& file) {
	int result = -1;

	// Checks if the end of the file was reached
	if (file.eof () == false) {
		int recaddr = file.tellg (); // Position of the next record
		char* temp = new char[2];
		clear (); // Makes room in the buffer for the next record

		// Reads the record size
		unsigned short recordSize; // Unsigned shorts are 16-bit positive ints
		file.read (temp, 2);
		recordSize = (temp[1] << 8) | temp[0];
		delete temp;
		temp = NULL;

		// Checks for file problems or buffer overflow before writing to the buffer
		if (file.good () == false or maxBytes < recordSize)
			clear ();
		else {
			file.read (buffer, recordSize);

			if (file.good () == true) {
				result = recaddr;
				length = recordSize;
			}
			else
				clear ();
		}
	}
    
    return result;
}

int NewPostalCodeBuffer::write (ostream& file) const {
	int result = -1;
	int recaddr = file.tellp ();
	unsigned short recordSize = length; // Gets the length of the record to write

	// Writes the record size to the file
	file.write ((char*)&recordSize, sizeof (recordSize));

	if (file.good () == true) {
		file.write (buffer, length); // Writes the buffer contents

		if (file.good () == true)
			result = recaddr;
	}

	return result;
}