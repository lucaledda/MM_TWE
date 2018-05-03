#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
using namespace std;

int main(){
	fstream BOM_file;
	fstream modified_BOM_file;
	string component_line, percentage_value, nominal_value;
	string zero="0";
	int index_nominal_value_start = 0;
	int index_nominal_value_end = 0;
	float nominal_value_float = 0.0;
	float percentage_value_float = 0.0;
	float tolerance_value_float = 0.0;
	string line_to_write="";
	string unit;
	ostringstream string_stream;
	int ready = 0;
	int unit_end = 0;
	
	// Opening the BOM.txt file in "INPUT" mode
	BOM_file.open ("BOM.txt");
	
	// Opening the output file in "OUTPUT" mode
	modified_BOM_file.open ("BOM_TWE.txt", ios::out);
	
	 try {
        while (getline(BOM_file,component_line)){
		line_to_write="";
		index_nominal_value_start = 0;
		index_nominal_value_end = 0;

		

		if (component_line.find("Ref") != component_line.npos)
			ready = 1;

		// If the component has a percentage tolerance
		if (component_line.find('%') != component_line.npos && ready == 1){
			// PICK PERCENTAGE VALUE -------------------------------------------------------------------------------------------------------
			// Transform the percentage in value
			percentage_value = component_line.substr (component_line.find('-')+1, component_line.find('%') - component_line.find('-') - 1); 
			// If the percentage present the point, zero is aggregated.

			if (percentage_value.find('.') != percentage_value.npos)
				percentage_value = zero + percentage_value;
			
			// PICK NOMINAL VALUE -----------------------------------------------------------------------------------------------------------
			for (int i=0; i<3; i++)
				index_nominal_value_start = component_line.find('\t', index_nominal_value_start + 1);
			
			// INDEXes FOR NOMINAL_VALUE AND ITS UNIT
			index_nominal_value_end = component_line.find_first_not_of("0123456789", index_nominal_value_start + 1); // it is also unit_start
			unit_end = component_line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", index_nominal_value_end);

			// VALUES 
			nominal_value = component_line.substr (index_nominal_value_start, index_nominal_value_end - index_nominal_value_start); 	
			unit = 	component_line.substr (index_nominal_value_end, unit_end - index_nominal_value_end); 	
			
			//getchar();

			// COMPUTE TOLERANCE VALUE ------------------------------------------------------------------------------------------------------
			nominal_value_float = strtof (nominal_value.c_str(),0);
			percentage_value_float = strtof (percentage_value.c_str(),0);
			tolerance_value_float = nominal_value_float * percentage_value_float / 100.0;
			
			// WRITE THE LINE WITH TOLERANCE ------------------------------------------------------------------------------------------------
			string_stream << tolerance_value_float;
			line_to_write = component_line.substr (0, component_line.find('-')+1) + string_stream.str() + unit + component_line.substr (component_line.find('%')+1, component_line.npos - (component_line.find('%')+1));
			modified_BOM_file << line_to_write << endl;

			string_stream.str("");
		}
		
		// Component line is reported if the tolerance is already expressed in value
		else
			modified_BOM_file << component_line << endl;
			
	}
    } catch(const std::out_of_range& e) {
        std::cout << "pos exceeds string size\n";
    }

	
	
	BOM_file.close();
	modified_BOM_file.close();
	
	return 0;
	}
