Please, compile and run the code in Linux machine.
Java source code is compiled and run by Java Virtual Machine (JVM) 1.6.0.

Please follow the instruction below:

(1) Compile the classes:

    >javac Contig.java
    >javac Position.java
    >javac Utilities.java
    >javac Assembly.java
    >javac Utils.java
    >javac Driver.java
	>chmod 777 *

(2) Run the tool:

    >java Driver <reference_file_name> <target_file_name> <compressed_file_name>	
    This command will produce <compressed_file_name>.7z archive file
	Example: java Driver chr_ko224_21.fa chr_hg18_21.fa chr21.nrgc
	The above command will produce chr21.nrgc.7z

(3) 7zip executable files should be in the running directory.
    For convenience we have already provided the executables.

(4) properties.prop file is an interface. Users can change some variables according to their needs. 
    We have used default values. If the tool exits outputing "System is exiting..." users should 
	increase the "division length" to get compress the file.
	
(5) properties.prop contains:

    #division length - segments containing the bases
    500000
    #pattern - fingerprint/barcode
    ACTAC
    #compute pattern - if "on" the tool will generate the fingerprint
    off
    #pattern length - length of the fingerprint
    5