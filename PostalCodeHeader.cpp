#include "PostalCodeHeader.h"

    // CONSTRUCTORS
PostalCodeHeader::PostalCodeHeader () {
    structure = "NULL/NULL";
    version = 0;
    recordSize = 0;
    sizeFormat = 0;
    indexFilename = "NULL";
    indexSchema = "key/NULL/0/pos/NULL/0";
    recordCount = 0;
    fieldCount = 0;
    primaryKey = "NULL";
}


    // HELPER FUNCTIONS
string PostalCodeHeader::readHeaderHelper (string& str) const {
    string token;
    int pos = str.find('|');
    if (pos != string::npos) {
        token = str.substr(0, pos);
        str.erase(0, pos + 1);
    }
    else {
        token = str;
        str.clear();
    }

    //cout << token << " - " << str << endl;

    return token;
}
	

    // BUFFER OPERATIONS
int PostalCodeHeader::readHeader (istream& file) {
    int result = -1;
    unsigned short size; // Header record size
    char* temp = new char[4]; // Used for reading sizes from the header
    string header;

    // Sets the get pointer to the beginning of the file
    file.seekg (0, ios::beg);

    // Reads the header record size
    file.read (temp, 2);
    size = (temp[1] << 8) | temp[0];

    // Reads the rest of the header
    char ch;
    for (int i = 0; i < size && file.get(ch); i++)
        header += ch;

    if (file.good () == true) {
        string field; // Holds the current field taken from the header

        // File structure type
        structure = readHeaderHelper (header);

        // File version
        field = readHeaderHelper (header);
        version = (field[1] << 8) | field[0];

        // Record size
        field = readHeaderHelper (header);
        recordSize = (field[1] << 8) | field[0];

        // Size format
        field = readHeaderHelper (header);
        sizeFormat = int(field[0]) - 48;

        // Index filename
        indexFilename = readHeaderHelper (header);

        // Index file schema
        indexSchema = readHeaderHelper (header);

        // Record count
        field = readHeaderHelper (header);
        recordCount = (field[1] << 8) | field[0];

        // Field count
        field = readHeaderHelper (header);
        fieldCount = (field[1] << 8) | field[0];

        // Field file schemas
        for (int i = 0; i < fieldCount; ++i)
            fieldInfo[i] = readHeaderHelper (header);

        // Primary key
        primaryKey = readHeaderHelper (header);

        // The header string should be empty by this point
        if (header == "")
            result = size + 2;
    }

    return result;
}

int PostalCodeHeader::validateHeader (istream& file) const {
    int result = -1;
    unsigned short size; // Header record size
    string header;

    // Sets the get pointer to the beginning of the file
    file.seekg (0, ios::beg);

    // Reads the header record size
    file.read ((char*)&size, sizeof (size));

    // Reads the rest of the header
    char ch;
    for (int i = 0; i < size && file.get(ch); i++)
        header += ch;

    //cout << "SIZE: " << size << endl;
    //cout << "START: " << header << endl << endl;

    if (file.good () == true) {
        string field; // Holds the current field taken from the header
        unsigned short tempVal; // Holds the current field if it has been translated to a number
        int invalid = false;

        // File structure type
        field = readHeaderHelper (header);
        invalid = invalid or field != structure;

        //cout << invalid << " (" << field << "), " << endl;

        // File version
        field = readHeaderHelper (header);
        tempVal = (field[1] << 8) | field[0];
        invalid = invalid or tempVal != version;

        //cout << invalid << "(" << tempVal << "), " << endl;

        // Record size
        field = readHeaderHelper (header);
        tempVal = (field[1] << 8) | field[0];
        invalid = invalid or tempVal != recordSize;

        //cout << invalid << "(" << tempVal << "), " << endl;

        // Size format
        field = readHeaderHelper (header);
        tempVal = int(field[0]) - 48;
        invalid = invalid or tempVal != sizeFormat;

        //cout << invalid << "(" << tempVal << "), " << endl;

        // Index filename
        field = readHeaderHelper (header);
        invalid = invalid or field != indexFilename;

        //cout << invalid << "(" << field << "), " << endl;

        // Index file schema
        field = readHeaderHelper (header);
        invalid = invalid or field != indexSchema;

        //cout << invalid << "(" << field << "), " << endl;

        // Record count (not checked)
        field = readHeaderHelper (header);

        //cout << invalid << "(" << field << "), " << endl;

        // Field count
        field = readHeaderHelper (header);
        tempVal = (field[1] << 8) | field[0];
        invalid = invalid or tempVal != fieldCount;

        //cout << invalid << "(" << tempVal << "), " << endl;

        // Field file schemas
        for (int i = 0; i < fieldCount; ++i) {
            field = readHeaderHelper (header);
            invalid = invalid or field != fieldInfo[i];

            //cout << invalid << "(" << field << "), " << endl;
        }

        // Primary key
        field = readHeaderHelper (header);
        invalid = invalid or field != primaryKey;

        //cout << invalid << "(" << field << "), " << endl;

        // The header string should be empty by this point
        if (header == "" and invalid == false)
            result = size + 2;
    }

    return result;
}

int PostalCodeHeader::writeHeader (ostream& file) const {
    int result = -1;
    string temp; // Holds the entire header string so the method can get its size later
    stringstream ss; // Allows the program to manipulate the string like a file stream
    unsigned short size;
	
	// Sets the put pointer to the beginning of the file
	file.seekp (0, ios::beg);

    ss << structure << "|"; // File structure type
    ss.write ((char*)&version, sizeof (version)); // File version
    ss << "|";
    ss.write ((char*)&recordSize, sizeof (recordSize)); // Record size
    ss << "|";
    ss << sizeFormat << "|"; // Size storage format
    ss << indexFilename << "|"; // Name of the index file
    ss << indexSchema << "|"; // Index storage scheme
    ss.write ((char*)&recordCount, sizeof (recordCount)); // Number of records
    ss << "|";
    ss.write ((char*)&fieldCount, sizeof (fieldCount)); // Number of fields per record
    ss << "|";
    // Name and schema for each field
    for (int i = 0; i < fieldInfo.size (); ++i)
        ss << fieldInfo[i] << "|";
    ss << primaryKey << "|"; // The field used as the primary key

    temp = ss.str (); // Copy string to temp
    size = temp.size (); // Gets the size of the header

    // Writes the entire header with its size at the start
    file.write ((char*)&size, sizeof (size));
    file << temp;

    if (file.good () == true)
        result = size;

    return result;
}

void PostalCodeHeader::clear () {
    // Sets all the attributes to default values
    structure = "NULL|NULL";
    version = 0;
    recordSize = 0;
    sizeFormat = 0;
    indexFilename = "NULL";
    indexSchema = "key|NULL|0|pos|NULL|0";
    fieldCount = 0;
    fieldInfo.clear ();
    primaryKey = "NULL";
}


    // GETTERS
string PostalCodeHeader::getStructure() const {
    return structure;
}

unsigned short PostalCodeHeader::getVersion() const {
    return version;
}

unsigned short PostalCodeHeader::getRecordSize() const {
    return recordSize;
}

int PostalCodeHeader::getSizeFormat() const {
    return sizeFormat;
}

string PostalCodeHeader::getIndexFilename() const {
    return indexFilename;
}

string PostalCodeHeader::getIndexSchema() const {
    return indexSchema;
}

unsigned short PostalCodeHeader::getRecordCount() const {
    return recordCount;
}

unsigned short PostalCodeHeader::getFieldCount() const {
    return fieldCount;
}

vector<string> PostalCodeHeader::getFieldInfo() const {
    return fieldInfo;
}

string PostalCodeHeader::getPrimaryKey() const {
    return primaryKey;
}


    // SETTERS
void PostalCodeHeader::setStructure(const string& structure) {
    this->structure = structure;
}

void PostalCodeHeader::setVersion(unsigned short version) {
    this->version = version;
}

void PostalCodeHeader::setRecordSize(unsigned short recordSize) {
    this->recordSize = recordSize;
}

void PostalCodeHeader::setSizeFormat(int sizeFormat) {
    this->sizeFormat = sizeFormat;
}

void PostalCodeHeader::setIndexFilename(const string& indexFilename) {
    this->indexFilename = indexFilename;
}

void PostalCodeHeader::setIndexSchema(const string& indexSchema) {
    this->indexSchema = indexSchema;
}

void PostalCodeHeader::setRecordCount(unsigned short recordCount) {
    this->recordCount = recordCount;
}

void PostalCodeHeader::setFieldCount(unsigned short fieldCount) {
    this->fieldCount = fieldCount;
}

void PostalCodeHeader::setFieldInfo(const vector<string>& fieldInfo) {
    this->fieldInfo = fieldInfo;
}

void PostalCodeHeader::setPrimaryKey(const string& primaryKey) {
    this->primaryKey = primaryKey;
}