#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "common_certificate.h"

std::string Certificate::get_serial_number_with_format() {
	std::string serial_num = std::to_string(this->serial_number);
	char hex_serial_num[15];
	snprintf(hex_serial_num, 15, " (0x%08x)\n", this->serial_number);
	serial_num.insert(0, "\tserial number: ");
	serial_num.append(hex_serial_num);
	return serial_num;
}

std::string Certificate::get_subject_with_format() {
	std::string aux = this->subject;
	aux.insert(0, "\tsubject: ");
	aux += '\n';
	return aux;
}

std::string Certificate::get_issuer_with_format() {
	std::string aux = this->issuer;
	aux.insert(0, "\tissuer: ");
	aux += '\n';
	return aux;
}


std::string Certificate::get_start_date_with_format() {
	std::string aux = this->start_date;
	aux.insert(0, "\t\tnot before: ");
	aux+='\n';
	return aux;
}

std::string Certificate::get_end_date_with_format() {
	std::string aux = this->end_date;
	aux.insert(0, "\t\tnot after: ");
	aux+='\n';
	return aux;
}

std::string Certificate::get_modulus_with_format() {
	std::string mod = std::to_string(this->client_modulus);
	mod.insert(0, "\t\tmodulus: ");
	char hex_mod_num[11];
	snprintf(hex_mod_num, 11, " (0x%04x)\n", this->client_modulus);
	mod.append(hex_mod_num);
	return mod;
}

std::string Certificate::get_exponent_with_format() {
	std::string exp = std::to_string(this->client_exponent);
	exp.insert(0, "\t\texponent: ");
	char hex_exp_num[9];
	snprintf(hex_exp_num, 9, " (0x%02x)\n", this->client_exponent);
	exp.append(hex_exp_num);
	return exp;
}

std::string Certificate::get_file_name() {
	std::string file_name = this->subject;
	return file_name += ".cert";
}

void Certificate::save() {
	std::string file_name = this->get_file_name();
	std::ofstream output(file_name);
	output << this->operator()();
	output.close();
}


std::string Certificate::operator()() {
	std::string certificate = "certificate:\n"; 	
	std::string serial_num = this->get_serial_number_with_format();
	certificate.append(serial_num);
	std::string subject = this->get_subject_with_format();
	certificate.append(subject);
	std::string issuer = this->get_issuer_with_format();
	certificate.append(issuer);
	certificate.append("\tvalidity:\n"); 
	std::string start_date = this->get_start_date_with_format();
	certificate.append(start_date);
	std::string end_date = this->get_end_date_with_format();
	certificate.append(end_date);
	certificate.append("\tsubject public key info:\n");
	std::string mod = this->get_modulus_with_format();
	certificate.append(mod);
	std::string exp = this->get_exponent_with_format();
	certificate.append(exp);
	return certificate;
}

Certificate::Certificate(){}


uint32_t Certificate::get_number(std::string &num) {
	std::string number;
	std::istringstream string_num(num);
	string_num >> number;
	return std::stoi(number);
}


Certificate::Certificate(std::string &file_name) {
	std::ifstream file(file_name);
	std::string key_side, value_side;
	std::getline(file, key_side);
	std::getline(file, key_side, ':');
	std::getline(file, value_side);
	this->serial_number = this->get_number(value_side);
	std::getline(file, key_side, ':');
	std::getline(file.ignore( 1, ' '), value_side);
	this->subject = value_side;
	std::getline(file, key_side); //ISSUER
	std::getline(file, key_side); //VALIDITY

	std::getline(file, key_side, ':');
	std::getline(file.ignore( 1, ' '), value_side);
	this->start_date = value_side;

	std::getline(file, key_side, ':');
	std::getline(file.ignore( 1, ' '), value_side);
	this->end_date = value_side;

	std::getline(file, key_side);

	std::getline(file, key_side, ':');
	std::getline(file, value_side);
	this->client_exponent = (uint8_t) this->get_number(value_side);

	std::getline(file, key_side, ':');
	std::getline(file, value_side);
	this->client_modulus = (uint16_t) this->get_number(value_side);
}
