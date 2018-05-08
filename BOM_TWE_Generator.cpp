// To compile the cpp for windows 64 bits
// i686-w64-mingw32-g++ -static-libgcc -static-libstdc++ -o BOM_TWE_Generator.exe BOM_TWE_Generator.cpp 

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <vector>
using namespace std;

int main(){
	/* --------------------------------------------------- VARIABLES ------------------------------------------------------------- */
	vector<string> array_fields;
	string component_line, percentage_value, nominal_value, zero="0", line_to_write="", tolerance_value, unit;
	int index_nominal_value_start = 0, index_nominal_value_end = 0, ready = 0, unit_end = 0;
	float nominal_value_float = 0.0, percentage_value_float = 0.0, tolerance_value_float = 0.0;
	ostringstream string_stream;
	fstream BOM_file, modified_BOM_file;
	/* --------------------------------------------------------------------------------------------------------------------------- */

	// Opening the BOM.txt file in "INPUT" mode
	BOM_file.open ("BOM.txt");
	
	// Opening the output file in "OUTPUT" mode
	modified_BOM_file.open ("BOM_TWE.txt", ios::out);
	
	 try {
	     while (getline(BOM_file,component_line)){
			line_to_write="";
			index_nominal_value_start = 0;
			index_nominal_value_end = 0;

			// SCANNING THE FILE TILL UP TO THE FIRST COMPONENT
			if (component_line.find("Ref") != component_line.npos && ready==0)
				ready = 1;
			
 
 			// COMPONENTS
			else if (ready == 1) {

				// PERCENTAGE TOLERANCE COMPONENTS
				if (component_line.find('%') != component_line.npos){

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
					
					// COMPUTE TOLERANCE VALUE ------------------------------------------------------------------------------------------------------
					nominal_value_float = strtof (nominal_value.c_str(),0);
					percentage_value_float = strtof (percentage_value.c_str(),0);
					tolerance_value_float = nominal_value_float * percentage_value_float / 100.0;
					
					// WRITE THE LINE WITH TOLERANCE ------------------------------------------------------------------------------------------------
					string_stream << tolerance_value_float;

					// PROPERLY FORMATTING THE BOM ---------------------------------------------------------------------------------------------------
					string to_split = component_line;
					line_to_write = "";
					size_t pos = 0, found;
					while((found = to_split.find_first_of('\t', pos)) != string::npos) {
					    array_fields.push_back(to_split.substr(pos, found - pos));
					    pos = found+1;
					}
					array_fields.push_back(to_split.substr(pos));

					vector<string>::iterator it;
					int count = 0; 
					for (it = array_fields.begin(); it != array_fields.end(); it++) {
						
						if (count != 2 && count != 5){
							if (count != 6)
								line_to_write = line_to_write + *it + '\t';
							else
								line_to_write = line_to_write + *it;
						}
						if (count == 3)
							line_to_write = line_to_write + string_stream.str() + '\t' + string_stream.str() + '\t';

						count++;
					}

					array_fields.clear();

					modified_BOM_file << line_to_write << endl;
					// ---------------------------------------------------------------------------------------------------------------------------------

					string_stream.str("");
				}
				
				// NO PERCENTAGE TOLERANCE COMPONENTS
				else {
					// Pick up the tolerance value if any
					if (component_line.find("+-") != component_line.npos && ready != 0)
						tolerance_value = component_line.substr (component_line.find('-')+1, component_line.find_first_not_of("0123456789,", component_line.find('-')+1) - component_line.find('-') - 1); 
					else
						tolerance_value = "";

					// PROPERLY FORMATTING THE BOM ------------------------------------------------------------------------------------------------------
					string to_split = component_line;
					line_to_write = "";
					size_t pos = 0, found;
					while((found = to_split.find_first_of('\t', pos)) != string::npos) {
					    array_fields.push_back(to_split.substr(pos, found - pos));
					    pos = found + 1;
					}
					array_fields.push_back(to_split.substr(pos));

					vector<string>::iterator it;
					int count = 0; // 0, 1, 3, 4, 
					for (it = array_fields.begin(); it != array_fields.end(); it++) {
						
						if (count != 2 && count != 5){
							if (count != 6)
								line_to_write = line_to_write + *it + '\t';
							else
								line_to_write = line_to_write + *it;
						}
						if (count == 3)
							line_to_write = line_to_write + tolerance_value + '\t' + tolerance_value + '\t';
						
									 
						count++;
					}

					array_fields.clear();

					modified_BOM_file << line_to_write << endl;
					// -------------------------------------------------------------------------------------------------------------------------------------
				}
			}
				
			
			
				
		}
    } catch (const std::out_of_range& e) {
    	
    	std::cout << "pos exceeds string size\n";

    }

	BOM_file.close();
	modified_BOM_file.close();
	
	return 0;
	}
