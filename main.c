#include <stdio.h>
#include <zconf.h>
#include "ios.h"

int main() {
    printf("iPhone rootFS Tool - DuffyAPP_IT\nEnsure All Prereqs Installed & Device Is Ready (ReadMe!)..\n");
    printf("Sleeping for 5 seconds before dump..\n");
    //sleep for 5 secs for user to stop exec if device isnt ready.
    sleep(5);
    printf("Checking for device access..");
    //call fetch access function to ensure iphone tunnel is running properly.
    if (ios_fetch_access("127.0.0.1","2222")==0){
        printf("Connected!\n");
        sleep(1);
        //attempt remount. either result is fine.
        printf("Remounting RootFS as r/w if it isnt already...\n");
        if(ios_run_comm("mount -o rw,union,update / >/dev/null 2>/dev/null \; echo $?","alpine")==0) {
            printf("Disk remounted succesfully!\n");
        } else {
            printf("Disk was already remounted!\n");
        }

        printf("Compressing rootFS!..\n");
        printf("This can take a while, enough time to make a coffee or something!\n");
        if(ios_run_comm("tar -zcf  /rootFS.tar.gz / >/dev/null 2>/dev/null \; echo $?","alpine")==0){
            printf("Compression Complete. Receiving File To Mac.\n");
            if(ios_rec_f("/rootFS.tar.gz","rootFS.tar.gz")==0){
                printf("rootFS exported to rootFS.tar.gz\n");
                ios_run_comm("rm /rootFS.tar.gz","alpine");
            } else {
                printf("Something went wrong.. Check your disk space on the mac and try again?\n");
                ios_run_comm("rm /rootFS.tar.gz","alpine");
            }
        } else{
            return 1;
        }


    } else {
        printf("Is iPhone Tunnel running?\n");
        return 1;
    }

    return 0;
}
