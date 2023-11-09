#ifndef PostalCode_
#define PostalCode_

#include <iostream>

using namespace std;

class PostalCode {
	public:
			// CONSTRUCTORS
		/** Constructor that sets default values
		 * @post: creates a postal code object with default values that need to be set later */
		PostalCode ();
		
			// MODIFICATION METHODS
		/** Sets the zip code value
		 * @param n: the new zip code value
		 * @post: sets the value of the zip code */
		void setZipCode (int n);
		
		/** Sets the city value
		 * @param s: the new city value
		 * @post: sets the value of the city */
		void setCity (string s);
		
		/** Sets the state value
		 * @param s: the new state value
		 * @post: sets the value of the state */
		void setState (string s);
		
		/** Sets the county value
		 * @param s: the new zip code value
		 * @post: sets the value of the county */
		void setCounty (string s);
		
		/** Sets the latitude value
		 * @param n: the new latitude value
		 * @post: sets the value of the latitude */
		void setLat (double n);
		
		/** Sets the longitude value
		 * @param n: the new longitude value
		 * @post: sets the value of the longitude */
		void setLong (double n);
			
			// CONSTANT METHODS
		/** Gets the value of the zip code
		 * @return: returns the zip code value */
		int getZipCode () const;
		
		/** Gets the value of the city
		 * @return: returns the city value */
		string getCity () const;
		
		/** Gets the value of the state
		 * @return: returns the state value */
		string getState () const;
		
		/** Gets the value of the county
		 * @return: returns the county value */
		string getCounty () const;
		
		/** Gets the value of the latitude
		 * @return: returns the latitude value */
		double getLat () const;
		
		/** Gets the value of the longitude
		 * @return: returns the longitude value */
		double getLong () const;
		
		/** Prints the attributes to the screen
		 * @post: the object's attributes will be printed to the screen via cout */
		void print () const;
	
	private:
		int zipCode; //!< The zip code of the area
		string city; //!< The city of the area
		string state; //!< The state of the area
		string county; //!< The county of the area
		double lat; //!< The latitude of the area
		double lng; //!< The longitude of the area
};

#include "PostalCode.cpp"
#endif