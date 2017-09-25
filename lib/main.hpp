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

/*
	TagTypes are labeled in this way (i.e. 1, 2, 4, 8, ...) so that different tag types can
	be easily excluded using integer codes. In binary the we have

	Event 			      : 00000001 => 1
	TimeMarkerDeadTime    : 00000010 => 2
	TimeMarkerElapsed     : 00000100 => 4
	GantryMotion          : 00001000 => 8
	GatingExpandedRespOn  : 00010000 => 16
	GatingExpandedRespOff : 00100000 => 32
	GatingOther           : 01000000 => 64
	ControlAcq            : 10000000 => 128

	thus, the skipevents flag can be set to include different unions of these tag types
	by using the bitwise and operator, e.g.

	if ( (lm_packet.tagtype & (Event | GantryMotion | ControlAcq)) > 0 ){
		// skip processing this packet
	}  

*/
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
        unsigned long long int filesize_in_bytes;
        unsigned long long int num_packets;
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









