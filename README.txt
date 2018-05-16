Beta version of a remote TestWay environment

*** Abstract ***
When using TestWay, the user locally interacts with the tool saving and retrieving local data. A remote environment is provided in order for each member of the team to operate with updated data. The implemented mechanism is based on PUSH/PULL information to/from the shared server folder, which is considered a golden database. Data are pulled in local when the user intends to operate with them. Data are pushed to the server when the user intends to update them.

*** Conditions ***
Precondition : TestWay installed in the user PC.
Postcondition : TestWay environment interacting with MM remote data.

*** How To ***
To use this environment :
1) Unzip MARELLI.zip into C:\TestWay\Data\ folder.
2) Move "BoardView.ini" and "visalib.ini" into C:\TestWay\ folder.
3) Launch the script.exe (you can launch from any folder within your PC, it works with absolute paths).

*** Notes ***
MARELLI folder contains :
- PROJECTS : Local TWE projects. An example on SMU3 board is provided in the zip.
- LIB1 : MasterLib files.
- MODELS : Component model files.
- CUSTOM : Customized configuration files for the MM purposes.

