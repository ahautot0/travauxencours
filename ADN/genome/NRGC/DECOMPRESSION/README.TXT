Please, compile and run the code in Linux machine.
Java source code is compiled and run by Java Virtual Machine (JVM) 1.6.0.

Please follow the instruction below:

(1) Compile the classes:
    >javac DecompressCG.java
	>chmod 777 *
	
(2) Run the tool:
    >java DecompressCG <reference_file_name> <archive_name> <out_file_name>
	This command will produce <out_file_name> 
	Example: java Driver chr_ko224_21.fa chr21.nrgc.7z chr21.fa
	The above command will produce chr21.fa
	
(3) 7zip executable files should be in the running directory.
    For convenience we have already provided the executables.