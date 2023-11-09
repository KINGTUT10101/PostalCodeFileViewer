#ifndef PostalCodeBuffer_
#define PostalCodeBuffer_

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Used as a file buffer for zip code objects
// Implemented as a delimited record, delimited field buffer
// '\n' and ',' are used as delimiters. Thus, they should not be used within any fields
// This class should only be used with CSV files generated by MS Excel
// It assumes that each record is stored in the following format:
// ZipCode,PlaceName,State,County,Lat,Long\n
class PostalCodeBuffer {
	public:
			// CONSTRUCTORS
		/** Constructor with default parameters
		 * @param mb: the maximum number of bytes the buffer will be able to hold
		 * @post: creates a zip code buffer object with a maximum size and initialized the buffer on the heap */
		PostalCodeBuffer (int mb = 1000);
		
		/** Copy constructor
		 * @param buff: the zip code buffer whose data will be copied into this zip code buffer
		 * @post: creates a zip code buffer whose data has been copied from buff */
		PostalCodeBuffer (const PostalCodeBuffer& buff);
		
		/** Destructor
		 * @post: destroys the character array on the heap and assigns NULL to the buffer attribute */
		virtual ~PostalCodeBuffer ();
		
			// MODIFICATION METHODS
		/** Copies the buffer data from one zip code buffer into this zip code buffer
		 * @param buff: the zip code buffer whose data will be copied into this zip code buffer
		 * @post: modifies this buffer's data and recreates the buffer if it's not large enough to hold the copied value */
		virtual PostalCodeBuffer& operator = (const PostalCodeBuffer& buff);
		
		/** Reads the header and determines if it matches the one defined within the class
		 * @param file: the file to read data from
		 * @param indexFilename: Currently unused. Added for compatibility reasons
		 * @param indexSchema: Currently unused. Added for compatibility reasons
		 * @pre: assumes the file follows the correct data format
		 * @post: sets the read pointer to the first character after the end of the header
		 * @return: returns the size of the header or -1 if an error occured*/
		virtual int readHeader (istream& file, const string& indexFilename, const string& indexSchema);

		/** Skips past the header in the file and prepares the buffer to write the first record
		 * @param file: the file to write data to
		 * @pre: the file is formatted correctly
		 * @post: sets the put pointer to the beginning of the stream
		 * @return: returns the size of the header or -1 if error occured */
		virtual int writeHeader(ostream& file) const;
		
		/** Reads a record from the file
		 * @param file: the file to read data from
		 * @pre: assumes the file follows the correct data format
		 * @post: sets the read pointer to the first character after the end of the header and packs the buffer
		 * @return: returns the first character in the record or -1 if the end of the file was reached before the end of the record */
		virtual int read (istream& file);

		/** Writes a record to the file
		 * @param file: the file to write data to
		 * @pre: the file is formatted correctly
		 * @post: sets the put pointer to the beginning of the stream and unpacks the buffer
		 * @return: returns the first character in the record or -1 if an error occured */
		virtual int write(ostream& file) const;
		
		/** Reads a record from the file
		 * @param file: the file to read data from
		 * @param fileIndex: the position within the file to start reading from
		 * @pre: assumes that fileIndex starts at the first character of a valid record within the file and that the file follows the correct data format
		 * @post: sets the read pointer to the first character after the end of the header and packs the buffer
		 * @return: returns the first character in the record or -1 if the end of the file was reached before thre end of the record */
		virtual int dRead (istream& file, int fileIndex);

		/** Writes a record to the file
		 * @param file: the file to write data to
		 * @param fileIndex: the position within the file to start writing to
		 * @pre: the file is formatted correctly and fileIndex starts at the first character of a valid record within the file
		 * @post: sets the put pointer to the beginning of the stream
		 * @return: returns the first character in the record or -1 if the end of the file was reached before the end of the record */
		virtual int dWrite(ostream& file, int fileIndex) const;
		
		/** Set the value of the next field of the buffer
		 * @param field: the character array to be set in buffer
		 * @param size: the maximum size of field
		 * @post if successful, the number of bytes packed will be returned
		 * @return returns the number of bytes packed into the buffer or -1 if there is an error */
		virtual int pack (const char* field, int size = -1);
		
		/** Cuts a field from a buffer and pastes it into a character array
		 * @param field: the character array that the data will be pasted into
		 * @param strLen: the maximum size of field
		 * @post: if successful, next byte will be moved to the next field and it will copy a field of data into the provided character array. 
		 * @return: returns the number of bytes extracted from the buffer or -1 if an error occured */
		virtual int unpack (char* field, int strLen = -1);
		
		/** Erases all data from the buffer
		 * @post: sets the next byte and length to 0, effectively resetting the buffer */
		virtual void clear ();
	
	private:
		static const char recordDelim = '\n'; //!< The character that indicates the end of a record
		static const char fieldDelim = ','; //!< The character that indicates the end of a field
		const char* headerStr = "\"Zip\nCode\",\"Place\nName\",State,County,Lat,Long\n"; //!< The header that is used in all postal code CSV files
		const int headerSize = strlen(headerStr); //!< The length of the file header

	protected:
		int maxBytes; //!< The maximum number of characters the buffer can hold
		char* buffer; //!< The buffer that will temporarily hold data. If it equals NULL then it hasn't been initialized yet
		int nextByte; //!< The index of the next byte to unpack from the buffer
		int length; //!< The size of the buffer
};

#include "PostalCodeBuffer.cpp"
#endif