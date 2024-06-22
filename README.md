Assembly Project Report: Memory Hierarchy Simulator

Farida Islam, Khaled Nana, Janna Osama, and Zeina Elsawy
CSCE2303 Computer Organization and Assembly Language Programming (2024 Spring)
Department of Computer Science and Engineering, AUC


A brief description of your implementation including any bonus features included
The project simulates a cache memory system in C++, processing memory access patterns to evaluate cache performance metrics like hit ratio, miss ratio, and average memory access time (AMAT). 
Main Components
Hexadecimal to Binary Conversion:
Function: hexaToBin(const string& hexStr)
Purpose: Converts a hexadecimal string to a 32-bit binary string.
Address Class:
Attributes: hex, tag, index, offset
Purpose: Represents a memory address, splitting it into tag, index, and offset based on given sizes.
CacheLine Class:
Attributes: valid, tag, data, index
Purpose: Represents a line in the cache, storing metadata and the data itself.
Functions
create_addresses:
Parameters: indexSize, offsetSize, filePath
Purpose: Reads memory addresses from a file, converts them to binary, and creates Address objects.
Implementation: Reads from CSV files and processes each hexadecimal address.
process_cache_accesses:
Parameters: addresses, cache, hitPenalty, missPenalty
Purpose: Simulates accessing cache lines with the given addresses and updates the cache.
Implementation: Iterates through addresses, checks for hits/misses, updates cache lines, and calculates performance metrics.
Main Function
Initialization:
Defines cache and line sizes (S and L), cache access time (cache_access), and file paths for instruction and data addresses.
Calculates index size and displacement size based on cache and line sizes.
Creating Addresses:
Uses create_addresses to read and process instruction and data addresses from specified files.
Cache Simulation:
Initializes instruction and data caches.
Calls process_cache_accesses for both instruction and data addresses to simulate cache operations and print performance metrics.
Key Metrics Calculated
Hits and Misses: Counts of cache hits and misses during the simulation.
Hit Ratio: The proportion of accesses that were hits.
Miss Ratio: The proportion of accesses that were misses.
AMAT (Average Memory Access Time): Calculated using hit and miss penalties.

Any design decisions and/or assumptions you made
There are three main design decisions we made for our simulation, two of which were requirements for the assessment. The first two implementations that were required and actually implemented were direct mapping and constant memory access. The simulation assumes a direct mapping structure due to the fact that it is one of the simplest kinds of cache mapping. By providing a quick and straightforward way to demonstrate how caches work, we get to verify tags given, handle hits and misses, and understand cache indexing. The second design decision the assignment assumes is that accessing the memory will take 100 clock cycles. This simplifies the simulation by keeping a constant time for memory accesses and helps us calculate the AMAT. Lastly,  we implemented a way to convert the addresses to hexadecimal. This allowed us to physically separate the address into the tag component and the index component. 

Any known bugs or issues in your simulator
Throughout our process of developing this code, we naturally ran into a few problems. First of which was fully understanding the requirements of the assignment itself, unlike our first project, this seemed to be more complicated but we separated it into:
Read-only one-level caching 
Direct mapping
Cache Information
Access Sequence
Valid bits and tags of all entries
Total number of accesses
The hit and miss ratios
The Average Memory Access Time (AMAT) of the memory hierarchy (in cycles)
After narrowing down and understanding what each of these meant, we started thinking about how we would implement it as code. One of the first issues we ran into was cache initialization. The cache's initial state was not clearly mentioned, which led to errors during the first few cache visits. We decided to fix it by initializing the state of the cache such that all lines are set to invalid. This is done implicitly by constructing a class called “CacheLine” and setting the boolean “valid” as false. Another major issue we encountered was assuring that all the addresses would be correctly set up, meaning that we would get the correct index and tag. We decided to convert all addresses to hexadecimal, then to read we take the last three significant bits as our index then the rest is our tag. For example, we have address ‘0X070’ then turn it to 16 bits in hexadecimal which is ‘0000000001110000’. Index would be ‘000’ and the tag would equal to ‘0000000001110’. Lastly, we were afraid our simulation would overwrite the data when handling cache misses. To fix it we added this code snippet:
if (!cache[n].valid || cache[n].tag != address.tag) 
{
            cache[n].valid = true;
            cache[n].tag = address.tag;
            cache[n].data = address.offset;
            cache[n].index = address.index;
            misses++;
}
This code checks if the cache line is empty or has old data. If either is true, it updates the cache line with new data and marks it as valid, then increases the count of cache misses.

A list of programs (and associated data if any) you simulated
We developed two types of memory access patterns: instruction access and data access (as per bonus 4 criteria). In the 'Instruction Access Simulation', the program reads a series of instruction addresses from the data file. It then analyzes these addresses using the cache simulator, keeping track of things such as the hits, misses, and average memory access time (AMAT). Similarly, the 'Data Access Simulation' reads a series of data addresses from the data file. These addresses are treated in the same way as instruction addresses, and the simulator monitors and reports on cache performance.

A user guide showing how to compile and run your simulator including a full simulation example step-by-step with screenshots.
Create two text files named instrAddresses.txt and Data_Addresses.txt in the same directory as main.cpp.
Populate these files with hexadecimal addresses, each prefixed with '0x' and separated by commas.0x1A2B3C,0x4D5E6F,0x7A8B9C
Open Terminal or Command Prompt:
Open the terminal (Linux/macOS) or command prompt (Windows).
Navigate to the Directory:
Use the cd command to navigate to the directory where main.cpp, instrAddresses.txt, and Data_Addresses.txt are located.ath/to/your/directory
Compile the Code:
Compile the C++ code using the C++ compiler. If you're using g++, the command will be:
g++ main.cpp -o cache_simulator
This command compiles the main.cpp file and creates an executable named cache_simulator.
Run the Executable:./cache_simulator
This will start the simulation, and you will see the output printed in the terminal.






