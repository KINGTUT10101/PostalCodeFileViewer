#include "PostalCodeBuffer.h"

	// CONSTRUCTORS
PostalCodeBuffer::PostalCodeBuffer (int mb) : maxBytes (mb), buffer (new char[maxBytes]), nextByte (0), length (0) {}

PostalCodeBuffer::PostalCodeBuffer (const PostalCodeBuffer& buff) {
	maxBytes = buff.maxBytes;
	nextByte = buff.nextByte;
	length = buff.length;
	buffer = new char[maxBytes]; // Allocates a new buffer
	memcpy (buffer, buff.buffer, length); // Copies the old buffer
}

PostalCodeBuffer::~PostalCodeBuffer () {
	// Deletes the character array from the heap
	delete[] buffer;
	buffer = NULL;
}


	// MODIFICATION METHODS
PostalCodeBuffer& PostalCodeBuffer::operator = (const PostalCodeBuffer& buff) {
	// Checks if the buffer if big enough to hold the other buffer's contents
	if (maxBytes < buff.length) {
		// Creates a new array big enough to hold the new value
		maxBytes = buff.maxBytes;
		delete[] buffer;
		buffer = new char[maxBytes];
	}
	
	nextByte = buff.nextByte;
	length = buff.length;
	memcpy (buffer, buff.buffer, length); // Copies the old buffer
	
	return *this;
}

int PostalCodeBuffer::readHeader (istream& file, const string& indexFilename, const string& indexSchema) {
	int result = -1;
	bool error = file.eof () or !file.good (); // Determines if the end of file was reached or if an error occured
	char ch = file.get (); // Stores the last character read from the file
	bool quotes = ch == '"'; // Used to ignore extra new line characters within sets of quotation marks
	
	// Reads until it encounters a new line character
	while ((ch != recordDelim or quotes == true) and error == false) {
		//cout << "State: " << error << ", " << ch << ", " << quotes << endl;
		
		error = file.eof () or !file.good (); // Checks if the end of the file was reached
		file.get (ch); // Reads a single character
		
		// Checks for sets of quotation marks
		if (ch == '"')
			quotes = !quotes;
	}
	
	if (error == true) {
		//cout << "Error: " << file.eof () << ", " << file.good () << endl;
		file.clear ();
		result = -1;
	}
	else
		result = file.tellg ();
	
	return result;
}

int PostalCodeBuffer::writeHeader (ostream& file) const
{
  bool error = file.eof() or !file.good(); // Determines if end of file was reached or if an error occurred

  file.seekp(0, ios::beg); // Set pointer to the beginning of stream
  file.write(headerStr, headerSize);

  // Checks for error
  if (error == true)
    return -1;

  else
    return file.tellp();
}

int PostalCodeBuffer::read (istream& file) {
	int result = file.tellg ();

	// Clears the buffer
	clear ();

	bool error = length >= maxBytes or !file.good (); // Checks if the buffer is full or if the file has an error
	bool eof = file.eof (); // Checks if the read pointer has reached the end of the file
	char ch = file.get (); // Stores the last character read from the file
    
    // Reads until a new line, end of file, or error is encountered
    while (ch != recordDelim and eof == false and error == false){
        // Appends the character to the buffer
        buffer[length] = ch;
        length += 1;
        
        // Reads the next character
        error = length >= maxBytes or !file.good ();
        ch = file.get ();
        eof = file.eof ();
    }
    
    // Checks if an error occured
    if (error == true) {
        result = -1;
        file.clear ();
    }
    
    // Appends a delimiter to the end of the buffer
    buffer[length] = fieldDelim;
    length += 1;

	/*cout << "READ: ";
	for (int i = 0; i < maxBytes; ++i)
		cout << buffer[i];
	cout << endl;*/
    
    return result;
}

int PostalCodeBuffer::write(ostream& file) const
{
  // Write specified record
  const char delim[] = {recordDelim, 0};
  int result = file.tellp();

  // Checks for error with file
  if (!file)
    result = -1;
  file.write(buffer, length); // Writes buffer
  file.write(delim, 1); // Write delimiter

  // Checks whether state of stream is good
  if (!file.good())
    result = -1;

  return result;
}

int PostalCodeBuffer::dRead (istream& file, int fileIndex) {
	int result;
	
	// Sets the position of the get pointer
	file.seekg (fileIndex, ios::beg);
	
	// Checks if the get pointer was placed at the intended position
	if (file.tellg () != fileIndex)
		result = -1;
	else
		result = read (file);
	
    return result;
}

int PostalCodeBuffer::dWrite(ostream& file, int fileIndex) const
{
  // Sets the position of the put pointer
  file.seekp(fileIndex, ios::beg);
  
  // Checks if the put pointer was placed at the intended position
  if (file.tellp() != fileIndex)
	  return -1;
  
  return write(file);
}

int PostalCodeBuffer::pack (const char* field, int size)
{
	// Set the value of the next field of the buffer;
	// If size = -1 (default) use strlen(field) as delim of field
	// Return number of bytes packed, -1 if error
	int len; // Length of the string to be packed

	if (size >= 0) len = size;
	else len = strlen((char*)field);

	if (len > (int)strlen((char*)field))
		return -1; // Field is too short

	int start = nextByte; // First character to be packed
	nextByte += len + 1;
	if (nextByte > maxBytes)
		return -1;
	memcpy(&buffer[start], field, len); // Copies the field to the buffer
	buffer[start + len] = fieldDelim; // Adds the field delimeter
	length = nextByte;
	return len;
}

int PostalCodeBuffer::unpack (char* field, int strLen) {
	int fieldLen = 0; // The length of the unpacked field
	int start = nextByte; // The next character to read from the buffer

	/*cout << "BUFFER: ";
	for (int i = 0; i < maxBytes; ++i)
		cout << buffer[i];
	cout << endl;*/
	
	// Reads characters and appends them to field until a delimiter is encountered
	while (start < length and buffer[start] != fieldDelim) {
		fieldLen += 1;
		start += 1;
	}
	
	// Checks if a delimiter was found and whether the field will fit into the provided character array
	if ((fieldLen < strLen or strLen == -1) and start >= nextByte and buffer[start] == fieldDelim) {
		memcpy (field, &buffer[nextByte], fieldLen); // Copies the field into the provided character array
		field[fieldLen] = 0; // Zero termination
		nextByte = start + 1; // Updates nextByte
	}
	else
		fieldLen = -1;
		
	return fieldLen;
}

void PostalCodeBuffer::clear () {
	// This effectly clears the character array from the program's perspective
	nextByte = 0;
	length = 0;
}
