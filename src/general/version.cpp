/* LD */

/*
File: version.C
Date: 26.10.1998
*/ 

/**
@name LiSA version management
@author Lars Dornheim
@version 2.2a0
**/ 

#include "version.hpp"


int Lisa_GetMajorVersion(void)

{
return LISA_MAJOR_VERSION;
}


int Lisa_GetMinorVersion(void)

{
return LISA_MINOR_VERSION;
}


int Lisa_GetReleaseLevel(void)

{
return LISA_RELEASE_LEVEL;
}


char Lisa_GetReleaseLevelLetter(void)

{
return LISA_RELEASE_LEVEL_LETTER;
}


int Lisa_GetReleaseSerial(void)

{
return LISA_RELEASE_SERIAL;
}


const char* Lisa_GetVersion(void)

{
return LISA_VERSION;
}


const char* Lisa_GetPatchLevel(void)

{
return LISA_PATCH_LEVEL;
}



