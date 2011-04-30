#define VERSION_MAJOR    0
#define VERSION_MINOR    5
#define VERSION_REVISION 0
#define VERSION_BUILD    109

#define _STR(s) TEXT(#s)
#define STR(s) _STR(s)

#define MY_PRODUCT_VERSION STR(VERSION_MAJOR) TEXT(".") STR(VERSION_MINOR) TEXT(".") STR(VERSION_REVISION) TEXT(".") STR(VERSION_BUILD)
#define MY_PRODUCT_VERSION_NUM VERSION_MAJOR,VERSION_MINOR,VERSION_REVISION,VERSION_BUILD
