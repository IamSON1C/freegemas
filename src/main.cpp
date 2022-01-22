#include "Game.h"

#ifdef __amigaos4__

#include <exec/types.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>
#include <proto/exec.h>
#include <proto/icon.h>

#include "version.h"

static const char* __attribute__((used)) version = VERSTAG;

BOOL getToolTypes(struct WBArg*);
void cleanExit(const char*, LONG);
void cleanup(void);

struct Library* iconBase = NULL;
struct IconIFace* iIcon = NULL;

BOOL fromWb;

bool gVSync = true;
bool gLowCPU = false;

#endif

int main(int argc, char *argv[])
{
#ifdef __amigaos4__

	struct WBStartup* wBenchMessage;
    struct WBArg* wbArg;
    FILE* file;
    LONG wLen;
    SHORT i;

    fromWb = (argc==0) ? true : false;

    if (fromWb) {

        iconBase = IExec->OpenLibrary("icon.library", 54);
        iIcon = (struct IconIFace*)IExec->GetInterface(IconBase, "main", 1, NULL);

        if (iIcon == NULL)
            cleanExit("Can't open icon.library\n", RETURN_FAIL);

        wBenchMessage = (struct WBStartup*) argv;

        for (i=0, wbArg = wBenchMessage->sm_ArgList; i < wBenchMessage->sm_NumArgs; i++, wbArg++) {
            getToolTypes(wbArg);
        }

        cleanup();
    }

#endif

    (void) argc;
    (void) argv;

    Game w;
    w.show();

    return 0;
}

#ifdef __amigaos4__

BOOL getToolTypes(struct WBArg* wbarg) {

    struct DiskObject* dobj;
    char** toolarray;
    char* s;
    bool success = false;

    if ((*wbarg->wa_Name) && (dobj = IIcon->GetDiskObject(wbarg->wa_Name))) {
        
        toolarray = (char**) dobj->do_ToolTypes;

		if (s = (char*) IIcon->FindToolType(toolarray, "VSYNC")) {
			if (strcmp(s, "NO") == 0) {
                gVSync = false;
			}
		}

		if (s = (char*) IIcon->FindToolType(toolarray, "LOWCPU")) {
			if(strcmp(s, "YES") == 0) {
				gLowCPU = true;
			}
		}

        IIcon->FreeDiskObject(dobj);
        success = true;
    }

    return(success);
}

void cleanExit(const char*, LONG n) {

    cleanup();
    exit(n);
}

void cleanup() {

	IExec->DropInterface((struct Interface*)IIcon);
	IExec->CloseLibrary(IconBase);
}

#endif
