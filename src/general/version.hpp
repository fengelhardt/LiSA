/* LD */

/*
File: version.h
Date: 26.10.1998
*/ 

/**
@name LiSA version management
@author Lars Dornheim
@version 2.2a0
**/ 

#ifndef version_h
#define version_h


/*
The release level should be 0 for alpha, 1 for beta, and 2 for final/patch.
The release serial value is the number that follows the "a", "b", or "p" in
the patch level; for example, if the patch level is 1.0b2, RELEASE_SERIAL is
2. It restarts at 1 whenever the release level is changed, except for the
final release which is 0 (the first patch will start at 1).
*/

#define LISA_MAJOR_VERSION 2
#define LISA_MINOR_VERSION 2
#define LISA_RELEASE_LEVEL 2
#define LISA_RELEASE_LEVEL_LETTER 'a'
#define LISA_RELEASE_SERIAL 0

#define LISA_VERSION "2.2"
#define LISA_PATCH_LEVEL "2.2a0"


int Lisa_GetMajorVersion(void);
int Lisa_GetMinorVersion(void);
int Lisa_GetReleaseLevel(void);
char Lisa_GetReleaseLevelLetter(void);
int Lisa_GetReleaseSerial(void);

const char* Lisa_GetVersion(void);
const char* Lisa_GetPatchLevel(void);


#endif



