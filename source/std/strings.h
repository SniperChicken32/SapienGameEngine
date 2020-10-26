//
// String functions

/** Find a strings within a given string.*/
bool   StringFind(std::string SubString, std::string String) {
    if (strstr ( String.c_str(), SubString.c_str() ) != NULL) {return 1;}
    return 0;
}

/** Returns the number of characters in the string.*/
size_t StringLen(std::string String) {
    size_t StrSz = strlen( String.c_str() );
    return StrSz;
}

/** Explodes the string by the given delimiter.*/
std::vector<std::string> StringExplode(std::string String, const char Char) {
	
	std::vector<std::string> result;
    std::istringstream iss(String);
    
    for (std::string token; std::getline(iss, token, Char); ) {
        
        if (std::move(token) != "") {
            result.push_back(std::move(token));
        }
        
    }
    
    return result;
}

/** Returns the file name from a path string.*/
std::string GetFileName(std::string FilePath) {
    
    char seperator = '/';
    
    // Get last dot position
    //std::size_t dotPos = FilePath.rfind('.');
    std::size_t sepPos = FilePath.rfind(seperator);
    if(sepPos != std::string::npos) {
        return FilePath.substr(sepPos + 1, FilePath.size());
    }
    
    return "";
}

