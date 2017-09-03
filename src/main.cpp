#include "main.hpp"

using namespace std;

size_t GetFileSizeInBytes(string filename){

    struct stat fileattr;
    int errorflag;

    errorflag = stat(filename.c_str(), &fileattr);

    if (errorflag != 0) // failed read
        return 0;

    return fileattr.st_size;
}

unsigned int GetNibble(uint32_t num, unsigned int ind){
    // ind is from least significant to most significant 
    // ind = 1 --> least significant nibble 
    // ind = 8 --> most significant nibble
    // e.g. first nibble below is 0001, last nibble is 1111
    // 1111 0000 0000 1111 0000 0000 1011 0001 

    unsigned int nib;
    unsigned int bitshift_amount = (ind-1)*4;

    // Shift 1111 mask left to desired nibble location
    nib = (0xF << bitshift_amount) & num;

    // Shift resulting number back to least significant nibble location
    nib = (nib >> bitshift_amount);
    
    return nib;
}

bool CheckBit(uint32_t num, unsigned int bit_index){
    // bit_index counts from 0 to (N-1)

    return (( 1 <<  bit_index ) & num) > 0;

}


TagType GetTagType(uint32_t packetval){

    unsigned int most_significant_nibble;
    most_significant_nibble = GetNibble(packetval, 8);
    
    //cout << "<" << most_significant_nibble << ">";

    if (most_significant_nibble < 0x8) // 0XXX
        return Event;

    if (most_significant_nibble < 0xC){ // 10XX
        if (CheckBit(packetval, 29)){
            return TimeMarkerDeadTime;}  // 101X
        else{
            return TimeMarkerElapsed;}   // 100X
    } 
    
    if (most_significant_nibble < 0xE) // 110X
        return GantryMotion;
    
    // Determine what kind of Gating Tag
    if (most_significant_nibble < 0xF){ // 1110
        unsigned int nib7 = GetNibble(packetval, 7); // second most signif. nibble
        if (nib7 == 0x1){
            unsigned int nib4 = GetNibble(packetval, 4);
            if (nib4 == 0x1){
                unsigned int nib1 = GetNibble(packetval, 1);
                if (nib1 == 0x0){
                    return GatingExpandedRespOn;}
                else if (nib1 == 0x1){
                    return GatingExpandedRespOff;}
            }
        } 
        return GatingOther;
    } 
    return ControlAcq;                 // 1111
}

long int GetElapsedTime(LMPacket lm_packet){

    if (lm_packet.tagtype == TimeMarkerElapsed){
        uint32_t elapsed;
        elapsed = ( lm_packet.value & 0x1FFFFFFF );
        return elapsed;
    }

    return -1;

}

LMStream::LMStream(string filename){
    
    this->filename     = filename;
    filesize_in_bytes  = GetFileSizeInBytes(filename);
    num_packets        = filesize_in_bytes / BYTES_PER_LM_PACKET;

    lm_file.open( filename.c_str(), ios::in | ios::binary );
}

LMPacket LMStream::ReadPacket(){

    struct LMPacket lm_packet;
    uint32_t value;

    lm_file.read(reinterpret_cast<char *>(&value), 
                 BYTES_PER_LM_PACKET);

    lm_packet.value   = value;
    lm_packet.tagtype = GetTagType(value);

    return lm_packet;
}

size_t LMStream::GetBytesPerLMPacket(){
    
    return BYTES_PER_LM_PACKET;        
}

vector<LMPacket> ReadEntireListmodeFile(string filename){

    // Open file for reading
    LMStream lm_file(filename);
    
    vector<LMPacket> lm_packets(lm_file.num_packets);

    for ( unsigned int i=0; i<lm_file.num_packets ; i++ ){
        lm_packets[i] = lm_file.ReadPacket();
    }

    return lm_packets;

}

string DumpAllLMTags2String(string filename, unsigned int skiptypes){

    LMStream lm_file(filename);
    
    unsigned long int last_timetag   = 0;
    struct LMPacket lm_packet;

    stringstream buffer;

    buffer << left << setw(15) << "BytePosition" 
                   << setw(15) << "TagContent" 
                   << setw(15) << "TagType" 
                   << setw(15) << "LastTimeTag(ms)" <<
                   "\n";
    
    for ( unsigned int i=0; i<lm_file.num_packets ; i++ ){ 

        lm_packet = lm_file.ReadPacket();
        
        if (lm_packet.tagtype == TimeMarkerElapsed){
            last_timetag = GetElapsedTime(lm_packet); }

        if ( (lm_packet.tagtype & skiptypes) > 0 ){
            continue; }

        buffer << "\n" << left << setw(15) << lm_file.GetBytesPerLMPacket()*i 
                       << setw(15) << hex << uppercase << lm_packet.value << dec
                       << setw(15) << lm_packet.tagtype
                       << setw(15) << last_timetag; 
                       
    }

    return buffer.str();

}

int WriteString2File(string s, string filename){
    
    ofstream out(filename.c_str());
    out << s;
    out.close();
    
    return 0;
}


int DumpAllLMTags2File(string infilename, unsigned int skiptypes, string outfilename){

    string alltags = DumpAllLMTags2String(infilename, skiptypes);
    WriteString2File(alltags, outfilename);
    return 0;
}


int main(int argc, char* argv[]){
    
    if (argc < 4){
        cout <<"\n";
        cerr << "Usage: " << "dumptags infile.bf outfile.txt skipevents";
        cout << "\n\n" <<
             "skipevents: " << "\n" << 
             "0: Skip Event Packets Only" << "\n" << 
             "7: Skip Events and Time Markers";
        cout << "\n\n";

        return 1;
    }

    string infilename       = argv[1];
    string outfilename      = argv[2];
    unsigned int skipevents;

    if (argv[3]){
        skipevents = atoi(argv[3]); }
    else{
        skipevents = (Event | TimeMarkerDeadTime | TimeMarkerElapsed);
    }
    
    DumpAllLMTags2File(infilename, skipevents, outfilename);
    
}

