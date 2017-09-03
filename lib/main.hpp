#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <stdint.h>
#include <iomanip>  
#include <vector>
#include <sstream>


using namespace std;

enum TagType { Event                 = 1 << 0 , 
               TimeMarkerDeadTime    = 1 << 1 ,
               TimeMarkerElapsed     = 1 << 2 ,
               GantryMotion          = 1 << 3 , 
               GatingExpandedRespOn  = 1 << 4 ,
               GatingExpandedRespOff = 1 << 5 ,
               GatingOther           = 1 << 6 ,          
               ControlAcq            = 1 << 7  
            };

struct LMPacket{
    
        uint32_t value;
        TagType tagtype; 
        
    };

class LMStream {
    
    public:
        LMStream(string filename);
        LMPacket ReadPacket();
        string   filename;
        size_t   filesize_in_bytes;
        size_t   num_packets;
        size_t GetBytesPerLMPacket();
    private:
        const static size_t BYTES_PER_LM_PACKET = 4;
        ifstream lm_file;


};


size_t GetFileSizeInBytes(string filename);
unsigned int GetNibble(uint32_t num, unsigned int ind);

TagType GetTagType(uint32_t packetval);
string DumpAllLMTags2String(string filename, unsigned int skiptypes = Event);
int WriteString2File(string s, string filename);
int DumpAllLMTags2File(string infilename, unsigned int skiptypes, string outfilename);









