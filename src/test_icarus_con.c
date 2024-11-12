//#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "wda.h"

/*
 * Note:
 */

typedef struct {
    unsigned long size,resident,share,text,lib,data,dt;
} statm_t;

# if 0
void read_off_memory_status(statm_t *result)
{
    unsigned long dummy;
    const char* statm_path = "/proc/self/statm";

    FILE *f = fopen(statm_path,"r");
    if (!f) {
        perror(statm_path);
        abort();
    }
    if (7 != fscanf(f,"%ld %ld %ld %ld %ld %ld %ld",
        &result->size,&result->resident,&result->share,&result->text,&result->lib,&result->data,&result->dt))
    {
        perror(statm_path);
        abort();
    }
    fclose(f);
    fprintf(stderr, "********* total=%ldkB, resident=%ldkB, shared=%ldkB, text=%ldkB, (data+stack)=%ldkB *********\n\n",
        result->size*4, result->resident*4, result->share*4, result->text*4, result->data*4);
}
# else

#define read_off_memory_status(x) ;

# endif


int main(void)
{
    int i, j, k, l;
    int err;
    statm_t stat;

    const char *url = "https://dbdata0vm.fnal.gov:9443/icarus_con_prod/app/data?f=tpc_channelstatus_data&t=1633400536";

    Dataset ds = NULL;
    Tuple tu;

    int len;
    int error;
    char *rr;
    size_t rlen;

    char ss[81920];
    double dd[4096];

    time_t t0 = time(NULL);

    //
    fprintf(stderr, "Before getting dataset\n");
    read_off_memory_status(&stat);
    //
    // Loop N times to exercise the API
    for (l = 0; l < 1; l++) {
        ds = getDataWithTimeout(url, "Icarus Condition Test", 3, &error);     // Get the data
        // More complex use case
        // const char *urls[] = {url, url, url, url};
        // ds = muxGetDataWithTimeout(url, urls, 2, "Icarus Condition Test (mux)", 3, &error);    // Get the data
        // ds = muxGetDataWithTimeout(url, NULL, 0, "Icarus Condition Test (mux)", 3, &error);    // Get the data
        fprintf(stderr, "error=%d\n", error);
        //
        fprintf(stderr, "After getting dataset: loop # %d\n", l);
        read_off_memory_status(&stat);
        //


        fprintf(stderr, "\nntuples=%d\n\n", getNtuples(ds));// Get the number of rows in the dataset

        tu = getTuple(ds, 1);                               // Returns NULL if out of range
        fprintf(stderr, "Get first tuple...\n");
        tu = getTuple(ds, 2);                               // Returns NULL if out of range
        fprintf(stderr, "Get second tuple...\n");
        // tu = getFirstTuple(ds);                             // Get the very first row

        // If we get something then print it
        if (tu != NULL) {                                               // If everything is OK
            int nc = getNfields(tu);                                    // Get the number of columns in this row
            fprintf(stderr, "Number of columns=%d\n", nc);

            for (i = 0; i < nc; i++) {                                  // Loop to get column data as a string
                len = getStringValue(tu, i, ss, sizeof (ss), &err);     // Returns string length
                fprintf(stderr, "[%d]: l=%d, s='%s'\n", i, len, ss);    // Print the results
            }
            fprintf(stderr, "e=%s\n\n", strerror(err));                 // Was it OK?
        }



        tu = getNextTuple(ds);                              // Get the next row
        tu = getNextTuple(ds);                              // Get the next row
        fprintf(stderr, "Next tuple as strings\n");

        if (tu != NULL) {                                               // If everything is OK
            int nc = getNfields(tu);                                    // Get the number of columns in this row
            fprintf(stderr, "Number of columns=%d\n", nc);

            for (i = 0; i < nc; i++) {                                  // Loop to get column data as a string
                len = getStringValue(tu, i, ss, sizeof (ss), &err);     // Returns string length
                fprintf(stderr, "[%d]: l=%d, s='%s'\n", i, len, ss);    // Print the results
            }
            fprintf(stderr, "e=%s\n\n", strerror(err));                 // Was it OK?
        } else {
            fprintf(stderr, "No such tuple\n");
        }
# if 1
        // Let's access all tuples
        fprintf(stderr, "Access all tuples...\n");
        int nrow, ncol;
        nrow = ncol = 0;
        while (tu = getNextTuple(ds)) {
            if (nrow % 10000 == 0) {
                //
                fprintf(stderr, "Accessing tuple [%d]\n", nrow);
                read_off_memory_status(&stat);
                //
            }
            //fprintf(stderr, ".");
            int nc = getNfields(tu);                                    // Get the number of columns in this row
            //fprintf(stderr, "ncols=%d\n", nc);

            for (i = 0; i < nc; i++) {                                  // Loop to get column data as a string
                len = getStringValue(tu, i, ss, sizeof (ss), &err);     // Returns string length
                if (nrow % 10000 == 0)
                    fprintf(stderr, "[%d]: l=%d, s='%s'\n", i, len, ss);    // Print the results
                ncol++;
            }
            //fprintf(stderr, "e=%s\n\n", strerror(err));                 // Was it OK?
            nrow++;
        }
        fprintf(stderr, "Nrows=%d, ncols=%d\n", nrow, ncol);

        //
        fprintf(stderr, "After accessing %d tuples\n", nrow);
        read_off_memory_status(&stat);
# endif
        //
        releaseDataset(ds);                                             // Release dataset to prevent memory leak!
        //
        fprintf(stderr, "After release dataset loop # %d\n", l);
        read_off_memory_status(&stat);
        //

    } // end for l loop to repeat test multiple times

//    return 0;

# if 0
/*
 * Even lower level interface
 * ==========================
 */
    fprintf(stderr, "\n\nUsing lower level interface: 'getHTTP'\n");

    rr = getHTTP(url, NULL, 0, &rlen, &err);
    int rrlen = strlen(rr);

    fprintf(stderr, "len=%d\n", rlen);
    fprintf(stderr, "err=%d\n", err);
    fprintf(stderr, "buf='%s'\n", rr);

    //
    fprintf(stderr, "After getHTTP call\n");
    read_off_memory_status(&stat);
    //
# endif
    return 0;
}


