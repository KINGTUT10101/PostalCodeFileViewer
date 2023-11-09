#ifndef NewPostalCodeBuffer_
#define NewPostalCodeBuffer_

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "PostalCodeBuffer.h"
#include "PostalCodeHeader.h"

using namespace std;

// Used as a file buffer for zip code objects
// Implemented as a length-indicated record, delimited field buffer
// ',' is used as a field delimiter. Thus, it should not be used within any fields
// The length is indicated by a 2-byte value at the beginning of the record
// It assumes that each record is stored in the following format:
// ZipCode,PlaceName,State,County,Lat,Long

/** Used to read and write new DAT postal code files
 * @author CSCI 331 Group 4
 * @date 2023-11-09
 */
class NewPostalCodeBuffer : public PostalCodeBuffer {
	public:
			// CONSTRUCTORS
		/** Constructor with default parameters
		 * @param mb: the maximum number of bytes the buffer will be able to hold
		 * @post: creates a zip code buffer object with a maximum size and initialized the buffer on the heap */
		NewPostalCodeBuffer (int mb = 1000);
		
		/** Destructor
		 * @post: destroys the character array on the heap and assigns NULL to the buffer attribute */
		virtual ~NewPostalCodeBuffer ();
		
		/** Reads the file header, validates it, and ensures the file uses the correct delimiter
		 * @param file: the file to read data from
		 * @param indexFilename: The name of the index file, which should match the one listed in the header
		 * @param indexSchema: The file storage scheme used by the index, which should match the one listed in the header
		 * @post: sets the read pointer to the first character after the end of the header
		 * @return: returns the size of the header or -1 if an error occured*/
		int readHeader (istream& file, const string& indexFilename, const string& indexSchema);

		/** Write the file header to the file, including the delimiter
		 * @param file: the file to write data to
		 * @param recordCount: the number of records that will be written to the file
		 * @param indexFilename: The name of the associated index file
		 * @param indexSchema: The file storage scheme that will be used by the associated index
		 * @post: sets the put pointer to the character after the buffer
		 * @return: returns the size of the header or -1 if an error occured */
		int writeHeader(ostream& file, unsigned short recordCount, const string& indexFilename, const string& indexSchema);
		
		/** Reads a record from the file
		 * @param file: the file to read data from
		 * @pre: assumes the file follows the correct data format
		 * @post: sets the read pointer to the first character after the end of the header and packs the buffer
		 * @return: returns the first character in the record or -1 if the end of the file was reached before the end of the record */
		int read (istream& file);

		/** Writes a record to the file
		 * @param file: the file to write data to
		 * @pre: the file is formatted correctly
		 * @post: sets the put pointer to the beginning of the stream and unpacks the buffer
		 * @return: returns the first character in the record or -1 if an error occured */
		int write(ostream& file) const;
	
	private:
		static const char fieldDelim = ','; //!< The character that indicates the end of a field
		PostalCodeHeader headerMan; //!< The header manager for the postal code buffer
};

#include "NewPostalCodeBuffer.cpp"
#endif
