#ifndef PostalCodeHeader_
#define PostalCodeHeader_

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

/* Reads, validates, and writes the header record for a postal code data file
Instead of saving the header to a character array, the header is stored in attributes so it's more user-friendly
The file format is as follows. All fields after the header record size are delimited by '|':
	00 - Header record size (unsigned short)
	"TYPE/TYPE" - Record/field file structure type (ex: LENGTH/DELIM = Length-incated records, delimited fields)
	00 - File structure version (unsigned short)
	00 - Record size for fixed length records, 0 if not a fixed length record (unsigned short)
	0 - Size format, 0 = ASCII and 1 = Binary (int)
	"index_filename.ind" - Name of the index file
	"key/TYPE/VALUE/pos/TYPE/VALUE" - Index file schema for reading/writing the key and position (ex: key/DELIM/,/pos/FIXED/8)
	00 - Record count (unsigned short)
	00 - Number of fields (unsigned short)
	"field/TYPE/VALUE" - Field file schema, repeated for the number of fields (ex: height/DELIM/, = height field is delimited with a comma)
	"field" - The field used as the primary key
*/
class PostalCodeHeader {
	public:
			// CONSTRUCTORS
		/**
		 * @brief Default constructor
		 * @post creates an object with default values
		*/
		PostalCodeHeader ();
		

			// BUFFER OPERATIONS
		/**
		 * @brief Reads the header into memory by seting the attributes
		 * @param file the file to read data from
		 * @post sets the read pointer to the first character after the end of the header and sets the attributes
		 * @return returns the size of the header or -1 if an error occured
		*/
		int readHeader (istream& file);

		/**
		 * @brief Reads the header and checks if it matches the stored attributes
		 * @param file the file to read data from
		 * @post sets the read pointer to the first character after the end of the header
		 * @return returns the size of the header or -1 if an error occured
		*/
		int validateHeader (istream& file) const;

		/**
		 * @brief Writes the header using the stored attirbutes
		 * @param file: the file to read data from
		 * @post sets the put pointer to the beginning of the stream and writes to the file
		 * @return returns the size of the header or -1 if error occured
		*/
		int writeHeader(ostream& file) const;

		/**
		 * @brief Erases all data from the buffer
		 * @post sets all attributes to their default values
		*/
		void clear ();


			// GETTERS
		/**
		 * @brief Returns the structure of the postal code header.
		 * @return A string representing the structure of the postal code header.
		*/
		string getStructure() const;

		/**
		 * @brief Returns the version of the postal code header.
		 * @return An unsigned short representing the version of the postal code header.
		*/
		unsigned short getVersion() const;

		/**
		 * @brief Returns the record size of the postal code header.
		 * @return An unsigned short representing the record size of the postal code header.
		*/
		unsigned short getRecordSize() const;

		/**
		 * @brief Returns the size format of the postal code header.
		 * @return An integer representing the size format of the postal code header.
		*/
		int getSizeFormat() const;

		/**
		 * @brief Returns the index filename of the postal code header.
		 * @return A string representing the index filename of the postal code header.
		*/
		string getIndexFilename() const;

		/**
		 * @brief Returns the index schema of the postal code header.
		 * @return A string representing the index schema of the postal code header.
		*/
		string getIndexSchema() const;

		/**
		 * @brief Returns the number of records in the postal code header.
		 * @return An unsigned short representing the number of records in the postal code header.
		*/
		unsigned short getRecordCount() const;

		/**
		 * @brief Returns the number of fields in the postal code header.
		 * @return An unsigned short representing the number of fields in the postal code header.
		*/
		unsigned short getFieldCount() const;

		/**
		 * @brief Returns the field information of the postal code header.
		 * @return A vector of strings representing the field information of the postal code header.
		*/
		vector<string> getFieldInfo() const;
		
		/**
		 * @brief Returns the primary key of the postal code header.
		 * @return A string representing the primary key of the postal code header.
		*/
		string getPrimaryKey() const;

			// SETTERS
		/**
		 * @brief Sets the structure of the postal code header.
		 * @param structure A string representing the structure of the postal code header.
		*/
		void setStructure(const string& structure);

		/**
		 * @brief Sets the version of the postal code header.
		 * @param version An unsigned short representing the version of the postal code header.
		*/
		void setVersion(unsigned short version);

		/**
		 * @brief Sets the record size of the postal code header.
		 * @param recordSize An unsigned short representing the record size of the postal code header.
		*/
		void setRecordSize(unsigned short recordSize);

		/**
		 * @brief Sets the size format of the postal code header.
		 * @param sizeFormat An integer representing the size format of the postal code header.
		*/
		void setSizeFormat(int sizeFormat);

		/**
		 * @brief Sets the index filename of the postal code header.
		 * @param indexFilename A string representing the index filename of the postal code header.
		*/
		void setIndexFilename(const string& indexFilename);

		/**
		 * @brief Sets the index schema of the postal code header.
		 * @param indexSchema A string representing the index schema of the postal code header.
		*/
		void setIndexSchema(const string& indexSchema);

		/**
		 * @brief Sets the number of records in the postal code header.
		 * @param recordCount An unsigned short representing the number of records in the postal code header.
		*/
		void setRecordCount(unsigned short recordCount);

		/**
		 * @brief Sets the number of fields in the postal code header.
		 * @param fieldCount An unsigned short representing the number of fields in the postal code header.
		*/
		void setFieldCount(unsigned short fieldCount);

		/**
		 * @brief Sets the field information of the postal code header.
		 * @param fieldInfo A vector of strings representing the field information of the postal code header.
		*/
		void setFieldInfo(const vector<string>& fieldInfo);

		/**
		 * @brief Sets the primary key of the postal code header.
		 * @param primaryKey A string representing the primary key of the postal code header.
		*/
		void setPrimaryKey(const string& primaryKey);

	
	private:
		/**
		 * @brief Takes a string, finds the next token (delimited by "|") and extracts it from the string
		 * @param str The delimited string that the token resides within
		 * @post The string will have the token removed from it
		 * @return The token will be returned, or the entire string will be returned if an error occured
		*/
		string readHeaderHelper (string& str) const;
		
		string structure; //!< Overall structure of the file
		unsigned short version; //!< File version
		unsigned short recordSize; //!< Size of the records or 0 if not fixed-length
		int sizeFormat; //!< The format used to store numbers in the file. 0 = ASCII and 1 = binary
		string indexFilename; //!< The name of the index file associated with this header
		string indexSchema; //!< The file storage scheme used by the index
		unsigned short recordCount; //!< The number of records in the data file
		unsigned short fieldCount; //!< The number of fields per record
		vector<string> fieldInfo; //!< The file storage scheme for each field
		string primaryKey; //!< The field that's used as the primary key
};

#include "PostalCodeHeader.cpp"
#endif