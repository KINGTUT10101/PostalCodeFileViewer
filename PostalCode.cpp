#include "PostalCode.h"

PostalCode::PostalCode () : zipCode (-1), city ("City"), state ("NA"), county ("County"), lat (0), lng (0) {}


	// MODIFICATION METHODS
void PostalCode::setZipCode (int n) {
	zipCode = n;
}

void PostalCode::setCity (string s) {
	city = s;
}

void PostalCode::setState (string s) {
	state = s;
}

void PostalCode::setCounty (string s) {
	county = s;
}

void PostalCode::setLat (double n) {
	lat = n;
}

void PostalCode::setLong (double n) {
	lng = n;
}
	
	
	// CONSTANT METHODS
int PostalCode::getZipCode () const {
	return zipCode;
}

string PostalCode::getCity () const {
	return city;
}

string PostalCode::getState () const {
	return state;
}

string PostalCode::getCounty () const {
	return county;
}

double PostalCode::getLat () const {
	return lat;
}

double PostalCode::getLong () const {
	return lng;
}

void PostalCode::print () const {
	cout << zipCode << ", " << city << ", " << state << ", " << county << ", " << lat << ", " << lng << endl;
	
	return;
}